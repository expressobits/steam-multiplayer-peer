#ifndef STEAM_MULTIPLAYER_PEER_H
#define STEAM_MULTIPLAYER_PEER_H

#include <godot_cpp/classes/multiplayer_peer_extension.hpp>
#include <godot_cpp/templates/hash_map.hpp>

// Include Steamworks API headers
#include "map"
#include "steam/steam_api_flat.h"
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
	bool no_nagle = false;
	bool no_delay = false;
	// bool as_relay = false;
	Dictionary options;
	SteamNetworkingConfigValue_t *option_array;

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

	enum SteamNetworkingConfig {
		NETWORKING_CONFIG_INVALID = k_ESteamNetworkingConfig_Invalid,
		NETWORKING_CONFIG_FAKE_PACKET_LOSS_SEND = k_ESteamNetworkingConfig_FakePacketLoss_Send,
		NETWORKING_CONFIG_FAKE_PACKET_LOSS_RECV = k_ESteamNetworkingConfig_FakePacketLoss_Recv,
		NETWORKING_CONFIG_FAKE_PACKET_LAG_SEND = k_ESteamNetworkingConfig_FakePacketLag_Send,
		NETWORKING_CONFIG_FAKE_PACKET_LAG_RECV = k_ESteamNetworkingConfig_FakePacketLag_Recv,
		NETWORKING_CONFIG_FAKE_PACKET_REORDER_SEND = k_ESteamNetworkingConfig_FakePacketReorder_Send,
		NETWORKING_CONFIG_FAKE_PACKET_REORDER_RECV = k_ESteamNetworkingConfig_FakePacketReorder_Recv,
		NETWORKING_CONFIG_FAKE_PACKET_REORDER_TIME = k_ESteamNetworkingConfig_FakePacketReorder_Time,
		NETWORKING_CONFIG_FAKE_PACKET_DUP_SEND = k_ESteamNetworkingConfig_FakePacketDup_Send,
		NETWORKING_CONFIG_FAKE_PACKET_DUP_REVC = k_ESteamNetworkingConfig_FakePacketDup_Recv,
		NETWORKING_CONFIG_FAKE_PACKET_DUP_TIME_MAX = k_ESteamNetworkingConfig_FakePacketDup_TimeMax,
		NETWORKING_CONFIG_PACKET_TRACE_MAX_BYTES = k_ESteamNetworkingConfig_PacketTraceMaxBytes,
		NETWORKING_CONFIG_FAKE_RATE_LIMIT_SEND_RATE = k_ESteamNetworkingConfig_FakeRateLimit_Send_Rate,
		NETWORKING_CONFIG_FAKE_RATE_LIMIT_SEND_BURST = k_ESteamNetworkingConfig_FakeRateLimit_Send_Burst,
		NETWORKING_CONFIG_FAKE_RATE_LIMIT_RECV_RATE = k_ESteamNetworkingConfig_FakeRateLimit_Recv_Rate,
		NETWORKING_CONFIG_FAKE_RATE_LIMIT_RECV_BURST = k_ESteamNetworkingConfig_FakeRateLimit_Recv_Burst,
		NETWORKING_CONFIG_OUT_OF_ORDER_CORRECTION_WINDOW_MICROSECONDS = k_ESteamNetworkingConfig_OutOfOrderCorrectionWindowMicroseconds,
		NETWORKING_CONFIG_CONNECTION_USER_DATA = k_ESteamNetworkingConfig_ConnectionUserData,
		NETWORKING_CONFIG_TIMEOUT_INITIAL = k_ESteamNetworkingConfig_TimeoutInitial,
		NETWORKING_CONFIG_TIMEOUT_CONNECTED = k_ESteamNetworkingConfig_TimeoutConnected,
		NETWORKING_CONFIG_SEND_BUFFER_SIZE = k_ESteamNetworkingConfig_SendBufferSize,
		NETWORKING_CONFIG_RECV_BUFFER_SIZE = k_ESteamNetworkingConfig_RecvBufferSize,
		NETWORKING_CONFIG_RECV_BUFFER_MESSAGES = k_ESteamNetworkingConfig_RecvBufferMessages,
		NETWORKING_CONFIG_RECV_MAX_MESSAGE_SIZE = k_ESteamNetworkingConfig_RecvMaxMessageSize,
		NETWORKING_CONFIG_RECV_MAX_SEGMENTS_PER_PACKET = k_ESteamNetworkingConfig_RecvMaxSegmentsPerPacket,
		NETWORKING_CONFIG_SEND_RATE_MIN = k_ESteamNetworkingConfig_SendRateMin,
		NETWORKING_CONFIG_SEND_RATE_MAX = k_ESteamNetworkingConfig_SendRateMax,
		NETWORKING_CONFIG_NAGLE_TIME = k_ESteamNetworkingConfig_NagleTime,
		NETWORKING_CONFIG_IP_ALLOW_WITHOUT_AUTH = k_ESteamNetworkingConfig_IP_AllowWithoutAuth,
		NETWORKING_CONFIG_IP_LOCAL_HOST_ALLOW_WITHOUT_AUTH = k_ESteamNetworkingConfig_IPLocalHost_AllowWithoutAuth,
		NETWORKING_CONFIG_MTU_PACKET_SIZE = k_ESteamNetworkingConfig_MTU_PacketSize,
		NETWORKING_CONFIG_MTU_DATA_SIZE = k_ESteamNetworkingConfig_MTU_DataSize,
		NETWORKING_CONFIG_UNENCRYPTED = k_ESteamNetworkingConfig_Unencrypted,
		NETWORKING_CONFIG_SYMMETRIC_CONNECT = k_ESteamNetworkingConfig_SymmetricConnect,
		NETWORKING_CONFIG_LOCAL_VIRTUAL_PORT = k_ESteamNetworkingConfig_LocalVirtualPort,
		NETWORKING_CONFIG_DUAL_WIFI_ENABLE = k_ESteamNetworkingConfig_DualWifi_Enable,
		NETWORKING_CONFIG_ENABLE_DIAGNOSTICS_UI = k_ESteamNetworkingConfig_EnableDiagnosticsUI,
		NETWORKING_CONFIG_SDR_CLIENT_CONSEC_PING_TIMEOUT_FAIL_INITIAL = k_ESteamNetworkingConfig_SDRClient_ConsecutitivePingTimeoutsFailInitial,
		NETWORKING_CONFIG_SDR_CLIENT_CONSEC_PING_TIMEOUT_FAIL = k_ESteamNetworkingConfig_SDRClient_ConsecutitivePingTimeoutsFail,
		NETWORKING_CONFIG_SDR_CLIENT_MIN_PINGS_BEFORE_PING_ACCURATE = k_ESteamNetworkingConfig_SDRClient_MinPingsBeforePingAccurate,
		NETWORKING_CONFIG_SDR_CLIENT_SINGLE_SOCKET = k_ESteamNetworkingConfig_SDRClient_SingleSocket,
		NETWORKING_CONFIG_SDR_CLIENT_FORCE_RELAY_CLUSTER = k_ESteamNetworkingConfig_SDRClient_ForceRelayCluster,
		NETWORKING_CONFIG_SDR_CLIENT_DEV_TICKET = k_ESteamNetworkingConfig_SDRClient_DevTicket,
		NETWORKING_CONFIG_SDR_CLIENT_FORCE_PROXY_ADDR = k_ESteamNetworkingConfig_SDRClient_ForceProxyAddr,
		NETWORKING_CONFIG_SDR_CLIENT_FAKE_CLUSTER_PING = k_ESteamNetworkingConfig_SDRClient_FakeClusterPing,
		NETWORKING_CONFIG_SDR_CLIENT_LIMIT_PING_PROBES_TO_NEAREST_N = k_ESteamNetworkingConfig_SDRClient_LimitPingProbesToNearestN,
		NETWORKING_CONFIG_LOG_LEVEL_ACK_RTT = k_ESteamNetworkingConfig_LogLevel_AckRTT,
		NETWORKING_CONFIG_LOG_LEVEL_PACKET_DECODE = k_ESteamNetworkingConfig_LogLevel_PacketDecode,
		NETWORKING_CONFIG_LOG_LEVEL_MESSAGE = k_ESteamNetworkingConfig_LogLevel_Message,
		NETWORKING_CONFIG_LOG_LEVEL_PACKET_GAPS = k_ESteamNetworkingConfig_LogLevel_PacketGaps,
		NETWORKING_CONFIG_LOG_LEVEL_P2P_RENDEZVOUS = k_ESteamNetworkingConfig_LogLevel_P2PRendezvous,
		NETWORKING_CONFIG_LOG_LEVEL_SRD_RELAY_PINGS = k_ESteamNetworkingConfig_LogLevel_SDRRelayPings,
		NETWORKING_CONFIG_CALLBACK_CONNECTION_STATUS_CHANGED = k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged,
		NETWORKING_CONFIG_CALLBACK_AUTH_STATUS_CHANGED = k_ESteamNetworkingConfig_Callback_AuthStatusChanged,
		NETWORKING_CONFIG_CALLBACK_RELAY_NETWORK_STATUS_CHANGED = k_ESteamNetworkingConfig_Callback_RelayNetworkStatusChanged,
		NETWORKING_CONFIG_CALLBACK_MESSAGE_SESSION_REQUEST = k_ESteamNetworkingConfig_Callback_MessagesSessionRequest,
		NETWORKING_CONFIG_CALLBACK_MESSAGES_SESSION_FAILED = k_ESteamNetworkingConfig_Callback_MessagesSessionFailed,
		NETWORKING_CONFIG_CALLBACK_CREATE_CONNECTION_SIGNALING = k_ESteamNetworkingConfig_Callback_CreateConnectionSignaling,
		NETWORKING_CONFIG_CALLBACK_FAKE_IP_RESULT = k_ESteamNetworkingConfig_Callback_FakeIPResult,
		NETWORKING_CONFIG_P2P_STUN_SERVER_LIST = k_ESteamNetworkingConfig_P2P_STUN_ServerList,
		NETWORKING_CONFIG_P2P_TRANSPORT_ICE_ENABLE = k_ESteamNetworkingConfig_P2P_Transport_ICE_Enable,
		NETWORKING_CONFIG_P2P_TRANSPORT_ICE_PENALTY = k_ESteamNetworkingConfig_P2P_Transport_ICE_Penalty,
		NETWORKING_CONFIG_P2P_TRANSPORT_SDR_PENALTY = k_ESteamNetworkingConfig_P2P_Transport_SDR_Penalty,
		NETWORKING_CONFIG_P2P_TURN_SERVER_LIST = k_ESteamNetworkingConfig_P2P_TURN_ServerList,
		NETWORKING_CONFIG_P2P_TURN_USER_LIST = k_ESteamNetworkingConfig_P2P_TURN_UserList,
		NETWORKING_CONFIG_P2P_TURN_PASS_LIST = k_ESteamNetworkingConfig_P2P_TURN_PassList,
		//			NETWORKING_CONFIG_P2P_TRANSPORT_LAN_BEACON_PENALTY = k_ESteamNetworkingConfig_P2P_Transport_LANBeacon_Penalty,
		NETWORKING_CONFIG_P2P_TRANSPORT_ICE_IMPLEMENTATION = k_ESteamNetworkingConfig_P2P_Transport_ICE_Implementation,
		NETWORKING_CONFIG_ECN = k_ESteamNetworkingConfig_ECN,
		NETWORKING_CONFIG_VALUE_FORCE32BIT = k_ESteamNetworkingConfigValue__Force32Bit
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
	void _force_close();
	void _disconnect_peer(int32_t p_peer, bool p_force) override;
	int32_t _get_unique_id() const override;
	// void _set_refuse_new_connections(bool p_enable) override;
	// bool _is_refusing_new_connections() const override;
	bool _is_server_relay_supported() const override;
	MultiplayerPeer::ConnectionStatus _get_connection_status() const override;

	bool close_listen_socket();
	Error create_host(int n_local_virtual_port);
	Error create_client(uint64_t identity_remote, int n_remote_virtual_port);
	bool get_identity(SteamNetworkingIdentity *p_identity);
	const SteamNetworkingConfigValue_t *convert_options_array(Array options);
	Ref<SteamConnection> get_connection_by_peer(int peer_id);
	void add_connection(const uint64_t steam_id, HSteamNetConnection connection);

	void _process_message(const SteamNetworkingMessage_t *msg);
	void _process_ping(const SteamNetworkingMessage_t *msg);

	uint64_t get_steam64_from_peer_id(const uint32_t peer_id) const; //Steam64 is a Steam ID
	uint32_t get_peer_id_from_steam64(const uint64_t steamid) const;
	void set_steam_id_peer(uint64_t steam_id, int peer_id);

	void set_listen_socket(const int listen_socket);
	int get_listen_socket() const;

	Dictionary get_peer_map();
	// Nagle's Algorithm: Inhibit the sending of new TCP segments, when new outgoing data arrives from the user,
	// if any previously transmitted data on the connection remains unacknowledged
	//
	// Exists to reduce small packets and avoid counterproductive overhead
	void set_no_nagle(const bool new_no_nagle);
	bool get_no_nagle() const;
	void set_no_delay(const bool new_no_delay);
	bool get_no_delay() const;
	// void set_as_relay(const bool new_as_relay);
	// bool get_as_relay() const;
	/// Configs
	Dictionary get_options() const;
	SteamNetworkingConfigValue_t *get_convert_options() const;
	void set_options(const Dictionary new_options);
	void set_config(const SteamNetworkingConfig config, Variant value);
	void clear_config(const SteamNetworkingConfig config);
	void clear_all_configs();


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

VARIANT_ENUM_CAST(SteamMultiplayerPeer::SteamNetworkingConfig);

#endif // STEAM_MULTIPLAYER_PEER_H
