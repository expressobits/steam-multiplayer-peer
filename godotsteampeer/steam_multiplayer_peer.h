#ifndef STEAM_MULTIPLAYER_PEER_H
#define STEAM_MULTIPLAYER_PEER_H

#include <godot_cpp/classes/multiplayer_peer_extension.hpp>

// Include Steamworks API headers
#include "map"
#include "steam/steam_api.h"
#include "steam/steamnetworkingfakeip.h"

using namespace godot;

#define MAX_PLAYERS_PER_SERVER 16

struct ClientConnectionData_t
{
	bool m_bActive;					// Is this slot in use? Or is it available for new connections?
	CSteamID m_SteamIDUser;			// What is the steamid of the player?
	uint64 m_ulTickCountLastData;	// What was the last time we got data from the player?
	HSteamNetConnection m_hConn;	// The handle for the connection to the player

	ClientConnectionData_t() {
		m_bActive = false;
		m_ulTickCountLastData = 0;
		m_hConn = 0;
	}
};

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
	// void _set_transfer_channel(int32_t p_channel) override;
	// int32_t _get_transfer_channel() const;
	// void _set_transfer_mode(MultiplayerPeer::TransferMode p_mode) override;
	// MultiplayerPeer::TransferMode _get_transfer_mode() const override;
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
	Error create_listen_socket_p2p(int n_local_virtual_port, Array options);
	Error connect_p2p(long identity_remote, int n_remote_virtual_port, Array options);
	bool get_identity(SteamNetworkingIdentity *p_identity);
	const SteamNetworkingConfigValue_t *convert_options_array(Array options);

private:
	HSteamListenSocket listen_socket;
	std::map<String, SteamNetworkingIdentity> networking_identities;
	// Vector to keep track of client connections
	ClientConnectionData_t m_rgClientData[MAX_PLAYERS_PER_SERVER];
	// Networking Sockets callbacks /////////
	STEAM_CALLBACK(SteamMultiplayerPeer, network_connection_status_changed, SteamNetConnectionStatusChangedCallback_t, callback_network_connection_status_changed);
	STEAM_CALLBACK(SteamMultiplayerPeer, network_authentication_status, SteamNetAuthenticationStatus_t, callback_network_authentication_status);
	STEAM_CALLBACK(SteamMultiplayerPeer, networking_fake_ip_result, SteamNetworkingFakeIPResult_t, callback_networking_fake_ip_result);

	// Networking Utils callbacks ///////////
	STEAM_CALLBACK(SteamMultiplayerPeer, relay_network_status, SteamRelayNetworkStatus_t, callback_relay_network_status);


};

#endif // STEAM_MULTIPLAYER_PEER_H