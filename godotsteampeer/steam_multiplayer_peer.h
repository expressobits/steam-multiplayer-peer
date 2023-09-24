#ifndef STEAM_MULTIPLAYER_PEER_H
#define STEAM_MULTIPLAYER_PEER_H

#include <godot_cpp/classes/multiplayer_peer_extension.hpp>

// Include Steamworks API headers
#include "map"
#include "steam/steam_api.h"
#include "steam/steamnetworkingfakeip.h"

using namespace godot;

class SteamMultiplayerPeer : public MultiplayerPeerExtension {
	GDCLASS(SteamMultiplayerPeer, MultiplayerPeerExtension)

public:
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
	Error connect_p2p(const String &identity_remote, int n_remote_virtual_port, Array options);
	bool get_identity(SteamNetworkingIdentity *p_identity);
	const SteamNetworkingConfigValue_t *convert_options_array(Array options);

private:
	HSteamListenSocket listen_socket;
	std::map<String, SteamNetworkingIdentity> networking_identities;
	// Networking Sockets callbacks /////////
	STEAM_CALLBACK(SteamMultiplayerPeer, network_connection_status_changed, SteamNetConnectionStatusChangedCallback_t, callback_network_connection_status_changed);
	STEAM_CALLBACK(SteamMultiplayerPeer, network_authentication_status, SteamNetAuthenticationStatus_t, callback_network_authentication_status);
	STEAM_CALLBACK(SteamMultiplayerPeer, networking_fake_ip_result, SteamNetworkingFakeIPResult_t, callback_networking_fake_ip_result);

	// Networking Utils callbacks ///////////
	STEAM_CALLBACK(SteamMultiplayerPeer, relay_network_status, SteamRelayNetworkStatus_t, callback_relay_network_status);

protected:
	static void _bind_methods();
};

#endif // STEAM_MULTIPLAYER_PEER_H