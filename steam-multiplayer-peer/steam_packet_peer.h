#ifndef STEAM_PACKET_PEER_H
#define STEAM_PACKET_PEER_H

#include "steam/steam_api.h"
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/ref_counted.hpp>

#define MAX_STEAM_PACKET_SIZE k_cbMaxSteamNetworkingSocketsMessageSizeSend

namespace godot {

class SteamPacketPeer : public RefCounted {
	GDCLASS(SteamPacketPeer, RefCounted)

public:
    uint8_t data[MAX_STEAM_PACKET_SIZE];
    uint32_t size = 0;
    uint64_t sender;
    int transfer_mode = k_nSteamNetworkingSend_Reliable;    //Looks like a spot that might be served by an enum, eventually.
    SteamPacketPeer() {}
    SteamPacketPeer(const void *p_buffer, uint32_t p_buffer_size, int transferMode) {
        ERR_FAIL_COND_MSG(p_buffer_size > MAX_STEAM_PACKET_SIZE, "Error: Tried to send a packet larger than MAX_STEAM_PACKET_SIZE");
        memcpy(this->data, p_buffer, p_buffer_size);
        this->size = p_buffer_size;
        this->transfer_mode = transferMode;
    }

protected:
	static void _bind_methods();
};

} //namespace godot
#endif // STEAM_PACKET_PEER_H