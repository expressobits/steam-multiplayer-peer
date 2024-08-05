#ifndef STEAM_PEER_CONFIG
#define STEAM_PEER_CONFIG

#include "steam/steam_api.h"
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {

class SteamPeerConfig : public RefCounted {
	GDCLASS(SteamPeerConfig, RefCounted)


private:
	Array options;

public:
	enum SteamNetworkingConfig {
		CONFIG_INVALID = k_ESteamNetworkingConfig_Invalid,
		// Connection options
		// CONFIG_TIMEOUT_INITIAL = k_ESteamNetworkingConfig_TimeoutInitial,
		// CONFIG_TIMEOUT_CONNECTED = k_ESteamNetworkingConfig_TimeoutConnected,
		// CONFIG_SEND_BUFFER_SIZE = k_ESteamNetworkingConfig_SendBufferSize,
		// CONFIG_SEND_BUFFER_SIZE = k_ESteamNetworkingConfig_SendBufferSize,
		// // Simulating network conditions
		// CONFIG_FAKE_PACKET_LOSS_SEND = k_ESteamNetworkingConfig_FakePacketLoss_Send,
		// CONFIG_FAKE_PACKET_LOSS_RECV = k_ESteamNetworkingConfig_FakePacketLoss_Recv,
		// CONFIG_FAKE_PACKET_LAG_SEND = k_ESteamNetworkingConfig_FakePacketLag_Send,
		// CONFIG_FAKE_PACKET_REORDER_RECV = k_ESteamNetworkingConfig_FakePacketReorder_Recv,
		// CONFIG_FAKE_PACKET_REORDER_TIME = k_ESteamNetworkingConfig_FakePacketReorder_Time,
		// CONFIG_FAKE_PACKET_DUP_SEND = k_ESteamNetworkingConfig_FakePacketDup_Send,
		// CONFIG_FAKE_PACKET_DUP_RECV = k_ESteamNetworkingConfig_FakePacketDup_Recv,
		// CONFIG_FAKE_PACKET_DUP_TIME_MAX = k_ESteamNetworkingConfig_FakePacketDup_TimeMax,
		// CONFIG_SEND_RATE_MIN = k_ESteamNetworkingConfig_SendRateMin,
		// CONFIG_SEND_RATE_MAX = k_ESteamNetworkingConfig_SendRateMax,
		// CONFIG_SEND_NAGLE_TIME = k_ESteamNetworkingConfig_NagleTime,
		// CONFIG_IP_ALLOW_WITHOUT_AUTH = k_ESteamNetworkingConfig_IP_AllowWithoutAuth,
		// CONFIG_SDR_CLIENT_CONSECUTITIVE_PING_TIMEOUTS_FAIL_INITIAL = k_ESteamNetworkingConfig_SDRClient_ConsecutitivePingTimeoutsFailInitial,
		// CONFIG_SDR_CLIENT_CONSECUTITIVE_Ping_TIMEOUTS_FAIL = k_ESteamNetworkingConfig_SDRClient_ConsecutitivePingTimeoutsFail,
		// CONFIG_SDR_CLIENT_MIN_PINGS_BEFORE_PING_ACCURATE = k_ESteamNetworkingConfig_SDRClient_MinPingsBeforePingAccurate,
		// CONFIG_SDR_CLIENT_SINGLE_SOCKET = k_ESteamNetworkingConfig_SDRClient_SingleSocket,
		// CONFIG_SDR_CLIENT_FORCE_RELAY_CLUSTER = k_ESteamNetworkingConfig_SDRClient_ForceRelayCluster,
		// CONFIG_SDR_CLIENT_DEV_TICKET = k_ESteamNetworkingConfig_SDRClient_DevTicket,
		// CONFIG_SDR_CLIENT_FORCE_PROXY_ADDR = k_ESteamNetworkingConfig_SDRClient_ForceProxyAddr,
		// CONFIG_SDR_CLIENT_FORCE_PROXY_ADDR = k_ESteamNetworkingConfig_SDRClient_FakeClusterPing,
		// Callbacks
		// P2P connection settings
		// Settings for SDR relayed connections
		// Log levels for debugging information of various subsystems.

	};
	SteamPeerConfig() {}
	Array get_options() const;
	int get_size() const;
	void set_options(const Array new_options);
	void add(const SteamNetworkingConfig config, Variant value);
	void remove(const SteamNetworkingConfig config);
	SteamNetworkingConfigValue_t *convert_options_array(const Array options) const;

protected:
	static void _bind_methods();
};

} //namespace godot
#endif // STEAM_PEER_CONFIG