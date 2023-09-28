#include <godot_cpp/core/class_db.hpp>

#include "steam_multiplayer_peer.h"

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

/////////////////////////////////////////////////
///// DEFINING CONSTANTS
/////////////////////////////////////////////////
//
// Define Steam API constants
#define STEAM_BUFFER_SIZE 255

SteamMultiplayerPeer::SteamMultiplayerPeer() :
		// Networking Sockets callbacks /////////////
		callback_network_connection_status_changed(this, &SteamMultiplayerPeer::network_connection_status_changed),
		callback_network_authentication_status(this, &SteamMultiplayerPeer::network_authentication_status),
		callback_networking_fake_ip_result(this, &SteamMultiplayerPeer::networking_fake_ip_result),

		// Networking Utils callbacks ///////////////
		callback_relay_network_status(this, &SteamMultiplayerPeer::relay_network_status) {
}

SteamMultiplayerPeer::~SteamMultiplayerPeer() {
	// TODO in SteamMessages is disable peer with leave lobby, check if need Close connection here
}

Error SteamMultiplayerPeer::_get_packet(const uint8_t **r_buffer, int32_t *r_buffer_size) {
	ERR_FAIL_COND_V_MSG(incoming_packets.size() == 0, ERR_UNAVAILABLE, "No incoming packets available.");

	delete next_received_packet;
	next_received_packet = incoming_packets.front()->get();
	*r_buffer_size = next_received_packet->size;
	*r_buffer = (const uint8_t *)(&next_received_packet->data); //REVIEW A pointer to a reference? I feel like this is worthy of more consideration.
	incoming_packets.pop_front();

	return OK;
}

// REVIEW many differences for EnetPeer
Error SteamMultiplayerPeer::_put_packet(const uint8_t *p_buffer, int32_t p_buffer_size) {
	ERR_FAIL_COND_V_MSG(!_is_active(), ERR_UNCONFIGURED, "The multiplayer instance isn't currently active.");
	ERR_FAIL_COND_V_MSG(connection_status != CONNECTION_CONNECTED, ERR_UNCONFIGURED, "The multiplayer instance isn't currently connected to any server or client.");
	ERR_FAIL_COND_V_MSG(target_peer != 0 && !peerId_to_steamId.has(ABS(target_peer)), ERR_INVALID_PARAMETER, vformat("Invalid target peer: %d", target_peer));
	ERR_FAIL_COND_V(active_mode == MODE_CLIENT && !peerId_to_steamId.has(1), ERR_BUG);
	int transferMode = _get_steam_transfer_flag();
	int32_t channel = get_transfer_channel() + SteamConnection::ChannelManagement::SIZE;

	if (target_peer == 0) {
		Error returnValue = OK;
		for (HashMap<int64_t, Ref<SteamConnection>>::Iterator E = connections_by_steamId64.begin(); E; ++E) {
			SteamConnection::Packet *packet = new SteamConnection::Packet(p_buffer, p_buffer_size, transferMode, channel);
			Error errorCode = E->value->send(packet);
			if (errorCode != OK) {
				// DEBUG_DATA_SIGNAL_V("put_packet failed.", errorCode);
				returnValue = errorCode;
			}
		}
		return returnValue;
	} else {
		SteamConnection::Packet *packet = new SteamConnection::Packet(p_buffer, p_buffer_size, transferMode, channel);
		return get_connection_by_peer(target_peer)->send(packet);
	}
}

int32_t SteamMultiplayerPeer::_get_available_packet_count() const {
	return incoming_packets.size();
}

int32_t SteamMultiplayerPeer::_get_max_packet_size() const {
	return k_cbMaxSteamNetworkingSocketsMessageSizeSend;
}

// PackedByteArray SteamMultiplayerPeer::_get_packet_script()
// {
//     return PackedByteArray();
// }

// Error SteamMultiplayerPeer::_put_packet_script(const PackedByteArray
// &p_buffer)
// {
//     return Error();
// }

int32_t SteamMultiplayerPeer::_get_packet_channel() const {
	ERR_FAIL_COND_V_MSG(!_is_active(), TRANSFER_MODE_RELIABLE, "The multiplayer instance isn't currently active.");
	ERR_FAIL_COND_V_MSG(incoming_packets.size() == 0, TRANSFER_MODE_RELIABLE, "No pending packets, cannot get channel.");
	return incoming_packets.front()->get()->channel;

	// REVIEW SYSCH_MAX Use on EnetPeer
	// int ch = incoming_packets.front()->get()->channel;
	// if (ch >= SYSCH_MAX) { // First 2 channels are reserved.
	// 	return ch - SYSCH_MAX + 1;
	// }
	// return 0;
}

MultiplayerPeer::TransferMode SteamMultiplayerPeer::_get_packet_mode() const {
	ERR_FAIL_COND_V_MSG(!_is_active(), TRANSFER_MODE_RELIABLE, "The multiplayer instance isn't currently active.");
	ERR_FAIL_COND_V_MSG(incoming_packets.size() == 0, TRANSFER_MODE_RELIABLE, "No pending packets, cannot get transfer mode.");

	if (incoming_packets.front()->get()->transfer_mode & k_nSteamNetworkingSend_Reliable) {
		return TRANSFER_MODE_RELIABLE;
	} else {
		return TRANSFER_MODE_UNRELIABLE;
	}
}

// void SteamMultiplayerPeer::_set_transfer_channel(int32_t p_channel) {}

// int32_t SteamMultiplayerPeer::_get_transfer_channel() const
// {
//     return 0;
// }

// void SteamMultiplayerPeer::_set_transfer_mode(
//     MultiplayerPeer::TransferMode p_mode) {}

void SteamMultiplayerPeer::_set_target_peer(int32_t p_peer) {
	target_peer = p_peer;
}

int32_t SteamMultiplayerPeer::_get_packet_peer() const {
	ERR_FAIL_COND_V_MSG(!_is_active(), 1, "The multiplayer instance isn't currently active.");
	ERR_FAIL_COND_V_MSG(incoming_packets.size() == 0, 1, "No packets to receive.");

	return connections_by_steamId64[incoming_packets.front()->get()->sender.to_int()]->peer_id;
}

bool SteamMultiplayerPeer::_is_server() const {
	return unique_id == 1;
}

void SteamMultiplayerPeer::_poll() {
	// TODO Implement this method
	ERR_FAIL_COND_MSG(!_is_active(), "The multiplayer instance isn't currently active.");
	// UtilityFunctions::printerr("_poll not implemented!");
}

void SteamMultiplayerPeer::_close() {
	if (!_is_active()) {
		return;
	}
	if (connection_status != CONNECTION_CONNECTED) {
		return;
	}

	for (HashMap<int64_t, Ref<SteamConnection>>::ConstIterator E = connections_by_steamId64.begin(); E; ++E) {
		const Ref<SteamConnection> connection = E->value;
		// TODO On Enet disconnect all peers with
		// peer_disconnect_now(0);
		close_connection(connection);
	}

	if (_is_server()) {
		close_listen_socket();
	}

	peerId_to_steamId.clear();
	connections_by_steamId64.clear();
	active_mode = MODE_NONE;
	unique_id = 0;
	connection_status = CONNECTION_DISCONNECTED;
	steam_id.set_from_CSteamID(CSteamID()); // = SteamID();
}

void SteamMultiplayerPeer::_disconnect_peer(int32_t p_peer, bool p_force) {
	ERR_FAIL_COND(!_is_active() || !peerId_to_steamId.has(p_peer));
	Ref<SteamConnection> connection = get_connection_by_peer(p_peer);
	close_connection(connection);

	connection->flush();
	// peerId_to_steamId[p_peer]->peer_disconnect(0); // Will be removed during next poll.
	if (active_mode == MODE_CLIENT || active_mode == MODE_SERVER) {
		get_connection_by_peer(0)->flush();
	} else {
		// ERR_FAIL_COND(!hosts.has(p_peer));
		// hosts[p_peer]->flush();
	}
	if (p_force) {
		// peers.erase(p_peer);
		// if (hosts.has(p_peer)) {
		// 	hosts.erase(p_peer);
		// }
		if (active_mode == MODE_CLIENT) {
			connections_by_steamId64.clear(); // Avoid flushing again.
			close();
		}
	}
	//TODO - Implement this method
	UtilityFunctions::printerr("_disconnect_peer not implemented p_peer=", p_peer, " p_force=", p_force);
}

int32_t SteamMultiplayerPeer::_get_unique_id() const {
	ERR_FAIL_COND_V_MSG(!_is_active(), 0, "The multiplayer instance isn't currently active.");
	return unique_id;
}

// void SteamMultiplayerPeer::_set_refuse_new_connections(bool p_enable) {}

// bool SteamMultiplayerPeer::_is_refusing_new_connections() const
// {
//     return false;
// }

bool SteamMultiplayerPeer::_is_server_relay_supported() const {
	return active_mode == MODE_SERVER || active_mode == MODE_CLIENT;
}

MultiplayerPeer::ConnectionStatus SteamMultiplayerPeer::_get_connection_status() const {
	return connection_status;
}

bool SteamMultiplayerPeer::close_listen_socket() {
	if (SteamNetworkingSockets() == NULL) {
		UtilityFunctions::printerr("SteamNetworkingSockets is null!");
		return false;
	}
	if (!SteamNetworkingSockets()->CloseListenSocket(listen_socket)) {
		UtilityFunctions::printerr("Fail to close listen socket ", listen_socket);
		return false;
	}
	UtilityFunctions::print("Success for close listen socket ", listen_socket);
	return true;
}

bool SteamMultiplayerPeer::close_connection(const Ref<SteamConnection> connection) {
	if (SteamNetworkingSockets() == NULL) {
		UtilityFunctions::printerr("SteamNetworkingSockets is null!");
		return false;
	}
	if (connection->steam_connection == k_HSteamNetConnection_Invalid) {
		UtilityFunctions::printerr("Steam Connection is invalid!");
		return false;
	}
	if (!SteamNetworkingSockets()->CloseConnection(connection->steam_connection, ESteamNetConnectionEnd::k_ESteamNetConnectionEnd_App_Generic, "Failed to accept connection", false)) {
		UtilityFunctions::printerr("Fail to close connection ", connection);
		return false;
	}
	UtilityFunctions::print("Success for close connection socket ", connection);
	return true;
}

// TODO Rename to create_server? for following enet pattern?
Error SteamMultiplayerPeer::create_listen_socket_p2p(int n_local_virtual_port, Array options) {
	ERR_FAIL_COND_V_MSG(_is_active(), ERR_ALREADY_IN_USE, "The multiplayer instance is already active.");
	if (SteamNetworkingSockets() == NULL) {
		return Error::ERR_CANT_CREATE;
	}
	const SteamNetworkingConfigValue_t *these_options = convert_options_array(options);
	listen_socket = SteamNetworkingSockets()->CreateListenSocketP2P(n_local_virtual_port, 0, nullptr);
	delete[] these_options;
	UtilityFunctions::print("create_listen_socket_p2p with ", listen_socket);
	active_mode = MODE_SERVER;
	unique_id = 1;
	connection_status = ConnectionStatus::CONNECTION_CONNECTED;
	return Error::OK;
}

// TODO Rename to create_client? for following enet pattern?
Error SteamMultiplayerPeer::connect_p2p(long identity_remote, int n_remote_virtual_port, Array options) {
	ERR_FAIL_COND_V_MSG(_is_active(), ERR_ALREADY_IN_USE, "The multiplayer instance is already active.");
	if (SteamNetworkingSockets() == NULL) {
		return Error::ERR_CANT_CONNECT;
	}
	unique_id = generate_unique_id();
	// TODO Add peer_id to options for connection
	const SteamNetworkingConfigValue_t *these_options = convert_options_array(options);
	SteamNetworkingIdentity p_remote_id;
	p_remote_id.SetSteamID64(identity_remote);
	active_mode = MODE_CLIENT;
	
	listen_socket = SteamNetworkingSockets()->ConnectP2P(p_remote_id, n_remote_virtual_port, options.size(), these_options);
	delete[] these_options;
	connection_status = ConnectionStatus::CONNECTION_CONNECTING;
	return Error::OK;
}

bool SteamMultiplayerPeer::get_identity(SteamNetworkingIdentity *p_identity) {
	return SteamNetworkingSockets()->GetIdentity(p_identity);
}

void SteamMultiplayerPeer::_bind_methods() {
	// ClassDB::bind_method(D_METHOD("close_listen_socket"), &SteamMultiplayerPeer::close_listen_socket);
	ClassDB::bind_method(D_METHOD("create_listen_socket_p2p", "n_local_virtual_port", "options"), &SteamMultiplayerPeer::create_listen_socket_p2p);
	ClassDB::bind_method(D_METHOD("connect_p2p", "identity_remote", "n_local_virtual_port", "options"), &SteamMultiplayerPeer::connect_p2p);

	// NETWORKING SOCKETS SIGNALS ///////////////
	ADD_SIGNAL(MethodInfo("network_connection_status_changed", PropertyInfo(Variant::INT, "connect_handle"), PropertyInfo(Variant::DICTIONARY, "connection"), PropertyInfo(Variant::INT, "old_state")));
	ADD_SIGNAL(MethodInfo("network_authentication_status", PropertyInfo(Variant::INT, "available"), PropertyInfo(Variant::STRING, "debug_message")));
	ADD_SIGNAL(MethodInfo("networking_fake_ip_result", PropertyInfo(Variant::INT, "result"), PropertyInfo(Variant::STRING, "identity"), PropertyInfo(Variant::STRING, "fake_ip"), PropertyInfo(Variant::ARRAY, "port_list")));

	// NETWORKING UTILS SIGNALS /////////////////
	ADD_SIGNAL(MethodInfo("relay_network_status", PropertyInfo(Variant::INT, "available"), PropertyInfo(Variant::INT, "ping_measurement"), PropertyInfo(Variant::INT, "available_config"), PropertyInfo(Variant::INT, "available_relay"), PropertyInfo(Variant::STRING, "debug_message")));
}

const int SteamMultiplayerPeer::_get_steam_transfer_flag() {
	MultiplayerPeer::TransferMode transfer_mode = get_transfer_mode();

	int32_t flags = (k_nSteamNetworkingSend_NoNagle * no_nagle) | (k_nSteamNetworkingSend_NoDelay * no_delay); //interesting use

	switch (transfer_mode) {
		case TransferMode::TRANSFER_MODE_RELIABLE:
			return k_nSteamNetworkingSend_Reliable | flags;
			break;
		case TransferMode::TRANSFER_MODE_UNRELIABLE:
			return k_nSteamNetworkingSend_Unreliable | flags;
			break;
		case TransferMode::TRANSFER_MODE_UNRELIABLE_ORDERED:
			//Unreliable order not supported here!
			return k_nSteamNetworkingSend_Reliable | flags;
			break;
	}

	ERR_FAIL_V_MSG(-1, "Flags error. Switch fallthrough in _get_steam_transfer_flag");
}

// NETWORKING SOCKETS CALLBACKS /////////////////
//
//! This callback is posted whenever a connection is created, destroyed, or
//! changes state. The m_info field will contain a complete description of the
//! connection at the time the change occurred and the callback was posted. In
//! particular, m_info.m_eState will have the new connection state.
void SteamMultiplayerPeer::network_connection_status_changed(SteamNetConnectionStatusChangedCallback_t *call_data) {
	// Connection handle.
	uint64_t connect_handle = call_data->m_hConn;
	// Full connection info.
	SteamNetConnectionInfo_t connection_info = call_data->m_info;

	if (connection_info.m_hListenSocket) {
		UtilityFunctions::print("m_hListenSocket is =", connection_info.m_hListenSocket);
	} else {
		UtilityFunctions::print("m_hListenSocket is null!");
	}

	// Move connection info into a dictionary
	Dictionary connection;
	char identity[STEAM_BUFFER_SIZE];
	connection_info.m_identityRemote.ToString(identity, STEAM_BUFFER_SIZE);
	connection["identity"] = identity;
	connection["user_data"] = (uint64_t)connection_info.m_nUserData;
	connection["listen_socket"] = connection_info.m_hListenSocket;
	char ip_address[STEAM_BUFFER_SIZE];
	connection_info.m_addrRemote.ToString(ip_address, STEAM_BUFFER_SIZE, true);
	connection["remote_address"] = ip_address;
	connection["remote_pop"] = connection_info.m_idPOPRemote;
	connection["pop_relay"] = connection_info.m_idPOPRelay;
	connection["connection_state"] = connection_info.m_eState;
	connection["end_reason"] = connection_info.m_eEndReason;
	connection["end_debug"] = connection_info.m_szEndDebug;
	connection["debug_description"] = connection_info.m_szConnectionDescription;
	// Previous state (current state is in m_info.m_eState).
	int old_state = call_data->m_eOldState;
	// // Send the data back via signal
	emit_signal("network_connection_status_changed", connect_handle, connection, old_state);

	// Check if a client has connected
	if (connection_info.m_hListenSocket && call_data->m_eOldState == ESteamNetworkingConnectionState::k_ESteamNetworkingConnectionState_None && call_data->m_info.m_eState == ESteamNetworkingConnectionState::k_ESteamNetworkingConnectionState_Connecting) {
		EResult res = SteamGameServerNetworkingSockets()->AcceptConnection(call_data->m_hConn);
		if (res != k_EResultOK) {
			UtilityFunctions::print("AcceptConnection returned", res);
			SteamGameServerNetworkingSockets()->CloseConnection(call_data->m_hConn, k_ESteamNetConnectionEnd_AppException_Generic, "Failed to accept connection", false);
			return;
		}
		add_pending_peer(call_data->m_info.m_identityRemote.GetSteamID(), call_data->m_hConn);

		// No empty slots.  Server full!
		UtilityFunctions::print("Rejecting connection; server full");
		SteamGameServerNetworkingSockets()->CloseConnection(call_data->m_hConn, k_ESteamNetConnectionEnd_AppException_Generic, "Server full!", false);
	}
}

//! This callback is posted whenever the state of our readiness changes.
void SteamMultiplayerPeer::network_authentication_status(SteamNetAuthenticationStatus_t *call_data) {
	// Status.
	int available = call_data->m_eAvail;
	// Non-localized English language status. For diagnostic / debugging purposes
	// only.
	char *debug_message = new char[256];
	sprintf(debug_message, "%s", call_data->m_debugMsg);
	// Send the data back via signal
	emit_signal("network_authentication_status", available, debug_message);
	delete[] debug_message;
}

// A struct used to describe a "fake IP" we have been assigned to use as an
// identifier. This callback is posted when
// ISteamNetworkingSoockets::BeginAsyncRequestFakeIP completes.
void SteamMultiplayerPeer::networking_fake_ip_result(SteamNetworkingFakeIPResult_t *call_data) {
	int result = call_data->m_eResult;
	// Pass this new networking identity to the map
	// TODO networking_identities removed?
	// networking_identities["fake_ip_identity"] = call_data->m_identity;
	// uint32 ip = call_data->m_unIP;
	// // Convert the IP address back to a string
	// const int NBYTES = 4;
	// uint8 octet[NBYTES];
	// char fake_ip[16];
	// for (int i = 0; i < NBYTES; i++) {
	// 	octet[i] = ip >> (i * 8);
	// }
	// sprintf(fake_ip, "%d.%d.%d.%d", octet[0], octet[1], octet[2], octet[3]);
	// // Get the ports as an array
	// Array port_list;
	// uint16 *ports = call_data->m_unPorts;
	// for (uint16 i = 0; i < sizeof(ports); i++) {
	// 	port_list.append(ports[i]);
	// }
	// emit_signal("networking_fake_ip_result", result, "fake_ip_identity", fake_ip, port_list);
	UtilityFunctions::print("networking_fake_ip_result not implemented! Because networking_identities not found!");
}

// NETWORKING UTILS CALLBACKS ///////////////////
//
//! A struct used to describe our readiness to use the relay network.
void SteamMultiplayerPeer::relay_network_status(SteamRelayNetworkStatus_t *call_data) {
	int available = call_data->m_eAvail;
	int ping_measurement = call_data->m_bPingMeasurementInProgress;
	int available_config = call_data->m_eAvailNetworkConfig;
	int available_relay = call_data->m_eAvailAnyRelay;
	char *debug_message = new char[256];
	sprintf(debug_message, "%s", call_data->m_debugMsg);
	//	debug_message = call_data->m_debugMsg;
	emit_signal("relay_network_status", available, ping_measurement,
			available_config, available_relay, debug_message);
	delete[] debug_message;
}

// GODOT MULTIPLAYER PEER UTILS  ///////////////////
//
// Helper function to turn an array of options into an array of
// SteamNetworkingConfigValue_t structs
const SteamNetworkingConfigValue_t *SteamMultiplayerPeer::convert_options_array(Array options) {
	// Get the number of option arrays in the array.
	int options_size = options.size();
	// Create the array for options.
	SteamNetworkingConfigValue_t *option_array = new SteamNetworkingConfigValue_t[options_size];
	// If there are options
	if (options_size > 0) {
		// Populate the options
		for (int i = 0; i < options_size; i++) {
			SteamNetworkingConfigValue_t this_option;
			Array sent_option = options[i];
			// Get the configuration value.
			// This is a convoluted way of doing it but can't seem to cast the value
			// as an enum so here we are.
			ESteamNetworkingConfigValue this_value = ESteamNetworkingConfigValue((int)sent_option[0]);
			if ((int)sent_option[1] == 1) {
				this_option.SetInt32(this_value, sent_option[2]);
			} else if ((int)sent_option[1] == 2) {
				this_option.SetInt64(this_value, sent_option[2]);
			} else if ((int)sent_option[1] == 3) {
				this_option.SetFloat(this_value, sent_option[2]);
			} else if ((int)sent_option[1] == 4) {
				char *this_string = { 0 };
				String passed_string = sent_option[2];
				strcpy(this_string, passed_string.utf8().get_data());
				this_option.SetString(this_value, this_string);
			} else {
				Object *this_pointer;
				this_pointer = sent_option[2];
				this_option.SetPtr(this_value, this_pointer);
			}
			option_array[i] = this_option;
		}
	}
	return option_array;
}

Ref<SteamConnection> SteamMultiplayerPeer::get_connection_by_peer(int peer_id) {
	if (peerId_to_steamId.has(peer_id))
		return peerId_to_steamId[peer_id];

	return nullptr;
}

void SteamMultiplayerPeer::add_connection_peer(const SteamID &steam_id, HSteamNetConnection connection, int peer_id) {
	ERR_FAIL_COND_MSG(steam_id == SteamUser()->GetSteamID(), "Cannot add self as a new peer.");

	Ref<SteamConnection> connection_data = Ref<SteamConnection>(memnew(SteamConnection(steam_id)));
	connection_data->steam_connection = connection;
	connection_data->peer_id = peer_id;
	connections_by_steamId64[steam_id.to_int()] = connection_data;

	// TODO Remove
	// NO USE PING On sockets for connection
	Error a = connection_data->ping();

	// if(a != OK) {
	//     DEBUG_DATA_SIGNAL_V("add_connection_peer: Error sending ping. ", a);    //shouldn't this be DEBUG_DATA_COND_SIGNAL_V or something like that?
	// }
	ERR_FAIL_COND_MSG(a != OK, "Message failed to join.");
}

void SteamMultiplayerPeer::add_pending_peer(const SteamID &steamId, HSteamNetConnection connection) {
	add_connection_peer(steamId, connection, -1);
}

uint64_t SteamMultiplayerPeer::get_steam64_from_peer_id(int peer) {
	if (peer == this->unique_id) {
		return SteamUser()->GetSteamID().ConvertToUint64();
	} else if (peerId_to_steamId.find(peer) == peerId_to_steamId.end()) {
		return peerId_to_steamId[peer]->steam_id.to_int();
	} else
		return -1;
}

int SteamMultiplayerPeer::get_peer_id_from_steam64(uint64_t steamid) {
	if (steamid == SteamUser()->GetSteamID().ConvertToUint64()) {
		return this->unique_id;
	} else if (connections_by_steamId64.has(steamid)) {
		return connections_by_steamId64[steamid]->peer_id;
	} else
		return -1;
}

//Should this be by reference?
int SteamMultiplayerPeer::get_peer_id_from_steam_id(SteamID &steamid) const {
	if (steamid == SteamID(SteamUser()->GetSteamID())) {
		return this->unique_id;
	} else if (connections_by_steamId64.has(steamid.to_int())) {
		return connections_by_steamId64[steamid.to_int()]->peer_id;
	} else
		return -1;
}

Dictionary SteamMultiplayerPeer::get_peer_map() {
	Dictionary output;
	for (HashMap<int64_t, Ref<SteamConnection>>::ConstIterator E = connections_by_steamId64.begin(); E; ++E) {
		output[E->value->peer_id] = E->value->steam_id.to_int();
	}
	return output;
}