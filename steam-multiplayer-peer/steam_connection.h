#ifndef STEAM_CONNECTION_H
#define STEAM_CONNECTION_H

#include <godot_cpp/classes/multiplayer_peer_extension.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/time.hpp>
#include <memory>

#include "steam_packet_peer.h"

#define MAX_STEAM_PACKET_SIZE k_cbMaxSteamNetworkingSocketsMessageSizeSend

using namespace godot;

class SteamConnection : public RefCounted {
	GDCLASS(SteamConnection, RefCounted)
public:
	struct SetupPeerPayload {
		uint32_t peer_id = -1;
	};
	bool m_bActive; // Is this slot in use? Or is it available for new connections?
	uint64_t steam_id; // What is the steamid of the player?
	HSteamNetConnection steam_connection; // The handle for the connection to the player
	uint64 m_ulTickCountLastData; // What was the last time we got data from the player?
	int peer_id;
	uint64_t last_msg_timestamp;
	List<Ref<SteamPacketPeer>> pending_retry_packets;

private:
	EResult _raw_send(Ref<SteamPacketPeer> packet);
	String _convert_eresult_to_string(EResult e);
	Error _send_pending();
	void _add_packet(Ref<SteamPacketPeer> packet);
	Error _send_setup_peer(const SetupPeerPayload payload);

protected:
	static void _bind_methods();

public:
	// REVIEW Enetpeer contains broadcast method (Research for method in steam sockets)
	// void broadcast(enet_uint8 p_channel, ENetPacket *p_packet);
	bool operator==(const SteamConnection &data);
	Error send(Ref<SteamPacketPeer> packet);
	void flush();
	bool close();
	SteamConnection(uint64_t steam_id);
	SteamConnection() {}
	~SteamConnection();

	Error request_peer();
	Error send_peer(uint32_t peer_id);
};

#endif // STEAM_CONNECTION_H
