#ifndef STEAM_CONNECTION_H
#define STEAM_CONNECTION_H

#include <godot_cpp/classes/multiplayer_peer_extension.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/time.hpp>
#include <memory>


#define MAX_STEAM_PACKET_SIZE k_cbMaxSteamNetworkingSocketsMessageSizeSend

using namespace godot;

class SteamConnection : public RefCounted {
	GDCLASS(SteamConnection, RefCounted)

public:
	struct Packet {
        uint8_t data[MAX_STEAM_PACKET_SIZE];
        uint32_t size = 0;
        SteamID sender = SteamID();
        int transfer_mode = k_nSteamNetworkingSend_Reliable;    //Looks like a spot that might be served by an enum, eventually.
        Packet() {}
        Packet(const void *p_buffer, uint32_t p_buffer_size, int transferMode) {
            ERR_FAIL_COND_MSG(p_buffer_size > MAX_STEAM_PACKET_SIZE, "Error: Tried to send a packet larger than MAX_STEAM_PACKET_SIZE");
            memcpy(this->data, p_buffer, p_buffer_size);
            this->size = p_buffer_size;
            this->sender.set_from_CSteamID(CSteamID());
            this->transfer_mode = transferMode;
        }
    };

    struct PingPayload {
        uint32_t peer_id = -1;
    };

	bool m_bActive;					// Is this slot in use? Or is it available for new connections?
	uint64_t steam_id;				// What is the steamid of the player?
	HSteamNetConnection steam_connection;    // The handle for the connection to the player
	uint64 m_ulTickCountLastData;	// What was the last time we got data from the player?
    int peer_id;
    uint64_t last_msg_timestamp;

    //How is this best used with smart pointers?
    //
    //Looks like it can be done, but we'll need to look at where the packets come from, and how they're used, to determine the right kind.
    List<Packet *> pending_retry_packets;

private:
    EResult _raw_send(Packet* packet);
    String _convert_eresult_to_string(EResult e);
    Error _send_pending();
    void _add_packet(Packet* packet);

protected:
	static void _bind_methods();

public:
    // REVIEW Enetpeer contains broadcast method (Research for method in steam sockets)
    // void broadcast(enet_uint8 p_channel, ENetPacket *p_packet);
	bool operator==(const SteamConnection &data);
    Error send(Packet* packet);
    void flush();
	SteamConnection(uint64_t steam_id);
	SteamConnection() {}
	~SteamConnection();

    Error ping();
    Error ping(const PingPayload &p);
};

#endif // STEAM_CONNECTION_H
