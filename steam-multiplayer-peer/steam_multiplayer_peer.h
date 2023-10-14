#ifndef STEAM_MULTIPLAYER_PEER_H
#define STEAM_MULTIPLAYER_PEER_H

#include <godot_cpp/classes/multiplayer_peer_extension.hpp>
#include <godot_cpp/templates/hash_map.hpp>

// Include Steamworks API headers
#include "map"
#include "steam/steam_api.h"
#include "steam/steamnetworkingfakeip.h"
#include "steam_connection.h"

using namespace godot;

#define MAX_PLAYERS_PER_SERVER 16

class SteamMultiplayerPeer : public MultiplayerPeerExtension {
	GDCLASS(SteamMultiplayerPeer, MultiplayerPeerExtension)

private:
	enum Mode {
		MODE_NONE,
		MODE_SERVER,
		MODE_CLIENT
	};

	Mode active_mode = MODE_NONE;
	uint32_t unique_id = 0;
	_FORCE_INLINE_ bool _is_active() const { return active_mode != MODE_NONE; }
	int32_t target_peer = -1;
	TransferMode transfer_mode = TRANSFER_MODE_RELIABLE;

protected:
	static void _bind_methods();

public:
	enum SocketConnectionType {
		NET_SOCKET_CONNECTION_TYPE_NOT_CONNECTED = k_ESNetSocketConnectionTypeNotConnected,
		NET_SOCKET_CONNECTION_TYPE_UDP = k_ESNetSocketConnectionTypeUDP,
		NET_SOCKET_CONNECTION_TYPE_UDP_RELAY = k_ESNetSocketConnectionTypeUDPRelay
	};
	enum SocketState {
		NET_SOCKET_STATE_INVALID = k_ESNetSocketStateInvalid,
		NET_SOCKET_STATE_CONNECTED = k_ESNetSocketStateConnected,
		NET_SOCKET_STATE_INITIATED = k_ESNetSocketStateInitiated,
		NET_SOCKET_STATE_LOCAL_CANDIDATE_FOUND = k_ESNetSocketStateLocalCandidatesFound,
		NET_SOCKET_STATE_RECEIVED_REMOTE_CANDIDATES = k_ESNetSocketStateReceivedRemoteCandidates,
		NET_SOCKET_STATE_CHALLENGE_HANDSHAKE = k_ESNetSocketStateChallengeHandshake,
		NET_SOCKET_STATE_DISCONNECTING = k_ESNetSocketStateDisconnecting,
		NET_SOCKET_STATE_LOCAL_DISCONNECT = k_ESNetSocketStateLocalDisconnect,
		NET_SOCKET_STATE_TIMEOUT_DURING_CONNECT = k_ESNetSocketStateTimeoutDuringConnect,
		NET_SOCKET_STATE_REMOTE_END_DISCONNECTED = k_ESNetSocketStateRemoteEndDisconnected,
		NET_SOCKET_STATE_BROKEN = k_ESNetSocketStateConnectionBroken
	};

	enum NetworkingConnectionState {
		CONNECTION_STATE_NONE = k_ESteamNetworkingConnectionState_None,
		CONNECTION_STATE_CONNECTING = k_ESteamNetworkingConnectionState_Connecting,
		CONNECTION_STATE_FINDING_ROUTE = k_ESteamNetworkingConnectionState_FindingRoute,
		CONNECTION_STATE_CONNECTED = k_ESteamNetworkingConnectionState_Connected,
		CONNECTION_STATE_CLOSED_BY_PEER = k_ESteamNetworkingConnectionState_ClosedByPeer,
		CONNECTION_STATE_PROBLEM_DETECTED_LOCALLY = k_ESteamNetworkingConnectionState_ProblemDetectedLocally,
		CONNECTION_STATE_FIN_WAIT = k_ESteamNetworkingConnectionState_FinWait,
		CONNECTION_STATE_LINGER = k_ESteamNetworkingConnectionState_Linger,
		CONNECTION_STATE_DEAD = k_ESteamNetworkingConnectionState_Dead,
		CONNECTION_STATE_FORCE_32BIT = k_ESteamNetworkingConnectionState__Force32Bit
	};
	SteamMultiplayerPeer();
	~SteamMultiplayerPeer();
	Error _get_packet(const uint8_t **r_buffer, int32_t *r_buffer_size) override;
	Error _put_packet(const uint8_t *p_buffer, int32_t p_buffer_size) override;
	int32_t _get_available_packet_count() const override;
	int32_t _get_max_packet_size() const override;
	// PackedByteArray _get_packet_script();
	// Error _put_packet_script(const PackedByteArray &p_buffer);
	int32_t _get_packet_channel() const;
	MultiplayerPeer::TransferMode _get_packet_mode() const;
	void _set_transfer_channel(int32_t p_channel) override;
	int32_t _get_transfer_channel() const;
	void _set_transfer_mode(MultiplayerPeer::TransferMode p_mode) override;
	MultiplayerPeer::TransferMode _get_transfer_mode() const override;
	void _set_target_peer(int32_t p_peer) override;
	int32_t _get_packet_peer() const override;
	bool _is_server() const override;
	void _poll() override;
	void _close() override;
	void _disconnect_peer(int32_t p_peer, bool p_force) override;
	int32_t _get_unique_id() const override;
	// void _set_refuse_new_connections(bool p_enable) override;
	// bool _is_refusing_new_connections() const override;
	bool _is_server_relay_supported() const override;
	MultiplayerPeer::ConnectionStatus _get_connection_status() const override;

	bool close_listen_socket();
	Error create_host(int n_local_virtual_port, Array options);
	Error create_client(uint64_t identity_remote, int n_remote_virtual_port, Array options);
	bool get_identity(SteamNetworkingIdentity *p_identity);
	const SteamNetworkingConfigValue_t *convert_options_array(Array options);
	Ref<SteamConnection> get_connection_by_peer(int peer_id);
	void add_connection(const uint64_t steam_id, HSteamNetConnection connection);

	void _process_message(const SteamNetworkingMessage_t *msg);
	void _process_ping(const SteamNetworkingMessage_t *msg);

	uint64_t get_steam64_from_peer_id(const uint32_t peer) const; //Steam64 is a Steam ID
	int get_peer_id_from_steam64(uint64_t steamid);
	void set_steam_id_peer(uint64_t steam_id, int peer_id);

	void set_listen_socket(const int listen_socket);
	int get_listen_socket() const;

	Dictionary get_peer_map();

	// Nagle's Algorithm: Inhibit the sending of new TCP segments, when new outgoing data arrives from the user,
	// if any previously transmitted data on the connection remains unacknowledged
	//
	// Exists to reduce small packets and avoid counterproductive overhead
	bool no_nagle = false;
	void set_no_nagle(bool value) {
		no_nagle = value;
	}
	bool get_no_nagle() {
		return no_nagle;
	}

	bool no_delay = false; //What?
	void set_no_delay(bool value) {
		no_delay = value;
	}
	bool get_no_delay() {
		return no_delay;
	}

	bool as_relay = false; //Again, what?
	void set_as_relay(bool value) {
		as_relay = value;
	}
	bool get_as_relay() {
		return as_relay;
	}

private:
	HashMap<uint64_t, Ref<SteamConnection>> connections_by_steamId64;
	HashMap<int, Ref<SteamConnection>> peerId_to_steamId;
	HSteamListenSocket listen_socket;
	HSteamNetConnection connection;

	Ref<SteamPacketPeer> next_received_packet; // gets deleted at the very first get_packet request
	List<Ref<SteamPacketPeer>> incoming_packets;
	const int _get_steam_transfer_flag();
	ConnectionStatus connection_status = ConnectionStatus::CONNECTION_DISCONNECTED;

	// Networking Sockets callbacks /////////
	STEAM_CALLBACK(SteamMultiplayerPeer, network_connection_status_changed, SteamNetConnectionStatusChangedCallback_t, callback_network_connection_status_changed);
};

#endif // STEAM_MULTIPLAYER_PEER_H