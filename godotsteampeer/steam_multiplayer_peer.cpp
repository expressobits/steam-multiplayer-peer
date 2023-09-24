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
		callback_relay_network_status(this, &SteamMultiplayerPeer::relay_network_status) {}

SteamMultiplayerPeer::~SteamMultiplayerPeer() {}

Error SteamMultiplayerPeer::_get_packet(const uint8_t **r_buffer, int32_t *r_buffer_size) {
	return Error();
}

Error SteamMultiplayerPeer::_put_packet(const uint8_t *p_buffer, int32_t p_buffer_size) {
	return Error();
}

int32_t SteamMultiplayerPeer::_get_available_packet_count() const { return 0; }

int32_t SteamMultiplayerPeer::_get_max_packet_size() const { return 0; }

// PackedByteArray SteamMultiplayerPeer::_get_packet_script()
// {
//     return PackedByteArray();
// }

// Error SteamMultiplayerPeer::_put_packet_script(const PackedByteArray
// &p_buffer)
// {
//     return Error();
// }

int32_t SteamMultiplayerPeer::_get_packet_channel() const { return 0; }

MultiplayerPeer::TransferMode SteamMultiplayerPeer::_get_packet_mode() const {
	// ERR_FAIL_COND_V_MSG(!_is_active(), TRANSFER_MODE_RELIABLE, "The multiplayer
	// instance isn't currently active.");
	// ERR_FAIL_COND_V_MSG(incoming_packets.size() == 0, TRANSFER_MODE_RELIABLE,
	// "No pending packets, cannot get transfer mode.");

	// if(incoming_packets.front()->get()->transfer_mode &
	// k_nSteamNetworkingSend_Reliable) {
	return TRANSFER_MODE_RELIABLE;
	// } else {
	// return TRANSFER_MODE_UNRELIABLE;
	// }
}

// void SteamMultiplayerPeer::_set_transfer_channel(int32_t p_channel) {}

// int32_t SteamMultiplayerPeer::_get_transfer_channel() const
// {
//     return 0;
// }

// void SteamMultiplayerPeer::_set_transfer_mode(
//     MultiplayerPeer::TransferMode p_mode) {}

void SteamMultiplayerPeer::_set_target_peer(int32_t p_peer) {}

int32_t SteamMultiplayerPeer::_get_packet_peer() const { return 0; }

bool SteamMultiplayerPeer::_is_server() const { return listen_socket != 0; }

void SteamMultiplayerPeer::_poll() {}

void SteamMultiplayerPeer::_close() {
	if (_is_server()) {
		close_listen_socket();
	}
}

void SteamMultiplayerPeer::_disconnect_peer(int32_t p_peer, bool p_force) {}

int32_t SteamMultiplayerPeer::_get_unique_id() const { return 0; }

// void SteamMultiplayerPeer::_set_refuse_new_connections(bool p_enable) {}

// bool SteamMultiplayerPeer::_is_refusing_new_connections() const
// {
//     return false;
// }

bool SteamMultiplayerPeer::_is_server_relay_supported() const { return false; }

MultiplayerPeer::ConnectionStatus SteamMultiplayerPeer::_get_connection_status() const {
	return MultiplayerPeer::ConnectionStatus::CONNECTION_CONNECTED;
}

bool SteamMultiplayerPeer::close_listen_socket() {
	if (SteamNetworkingSockets() == NULL) {
		UtilityFunctions::printerr("Fail to close listen socket ", listen_socket);
		return false;
	}
	if (!SteamNetworkingSockets()->CloseListenSocket(listen_socket)) {
		UtilityFunctions::printerr("Fail to close listen socket ", listen_socket);
		return false;
	}
	UtilityFunctions::print("Success for close listen socket ", listen_socket);
	return true;
}

Error SteamMultiplayerPeer::create_listen_socket_p2p(int n_local_virtual_port, Array options) {
	if (SteamNetworkingSockets() == NULL) {
		return Error::ERR_CANT_CREATE;
	}
	SteamNetworkingUtils()->InitRelayNetworkAccess();
	const SteamNetworkingConfigValue_t *these_options = convert_options_array(options);
	listen_socket = SteamNetworkingSockets()->CreateListenSocketP2P(n_local_virtual_port, 0, nullptr);
	delete[] these_options;
	UtilityFunctions::print("create_listen_socket_p2p with ", listen_socket);
	return Error::OK;
}

Error SteamMultiplayerPeer::connect_p2p(int identity_remote, int n_remote_virtual_port, Array options) {
	if (SteamNetworkingSockets() == NULL) {
		return Error::ERR_CANT_CONNECT;
	}
	const SteamNetworkingConfigValue_t *these_options = convert_options_array(options);
	SteamNetworkingIdentity p_remote_id;
	p_remote_id.SetSteamID64(identity_remote);
	listen_socket = SteamNetworkingSockets()->ConnectP2P(p_remote_id, n_remote_virtual_port, options.size(), these_options);
	delete[] these_options;
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
	networking_identities["fake_ip_identity"] = call_data->m_identity;
	uint32 ip = call_data->m_unIP;
	// Convert the IP address back to a string
	const int NBYTES = 4;
	uint8 octet[NBYTES];
	char fake_ip[16];
	for (int i = 0; i < NBYTES; i++) {
		octet[i] = ip >> (i * 8);
	}
	sprintf(fake_ip, "%d.%d.%d.%d", octet[0], octet[1], octet[2], octet[3]);
	// Get the ports as an array
	Array port_list;
	uint16 *ports = call_data->m_unPorts;
	for (uint16 i = 0; i < sizeof(ports); i++) {
		port_list.append(ports[i]);
	}
	emit_signal("networking_fake_ip_result", result, "fake_ip_identity", fake_ip, port_list);
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

// Helper function to turn an array of options into an array of
// SteamNetworkingConfigValue_t structs
const SteamNetworkingConfigValue_t *SteamMultiplayerPeer::convert_options_array(Array options) {
	// Get the number of option arrays in the array.
	int options_size = options.size();
	// Create the array for options.
	SteamNetworkingConfigValue_t *option_array =
			new SteamNetworkingConfigValue_t[options_size];
	// If there are options
	if (options_size > 0) {
		// Populate the options
		for (int i = 0; i < options_size; i++) {
			SteamNetworkingConfigValue_t this_option;
			Array sent_option = options[i];
			// Get the configuration value.
			// This is a convoluted way of doing it but can't seem to cast the value
			// as an enum so here we are.
			ESteamNetworkingConfigValue this_value =
					ESteamNetworkingConfigValue((int)sent_option[0]);
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