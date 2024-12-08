#include "steam_packet_peer.h"

void SteamPacketPeer::_bind_methods() {
}

SteamPacketPeer::SteamPacketPeer() {
}

SteamPacketPeer::SteamPacketPeer(const void *p_buffer, uint32_t p_buffer_size, int transferMode) {
	ERR_FAIL_COND_MSG(p_buffer_size > MAX_STEAM_PACKET_SIZE, vformat("Error: Tried to send a packet larger than MAX_STEAM_PACKET_SIZE: %d", p_buffer_size));
	memcpy(this->data, p_buffer, p_buffer_size);
	this->size = p_buffer_size;
	this->transfer_mode = transferMode;
}