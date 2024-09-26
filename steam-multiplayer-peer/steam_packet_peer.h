#ifndef STEAM_PACKET_PEER_H
#define STEAM_PACKET_PEER_H


#include "steam/steam_api.h"
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/ref_counted.hpp>


#define MAX_STEAM_PACKET_SIZE k_cbMaxSteamNetworkingSocketsMessageSizeSend


using namespace godot;

class SteamPacketPeer : public RefCounted {
	GDCLASS(SteamPacketPeer, RefCounted)


public:
	enum SteamNetworkingSend {
		SEND_UNRELIABLE = k_nSteamNetworkingSend_Unreliable,
		SEND_NO_NAGLE = k_nSteamNetworkingSend_NoNagle,
		SEND_URELIABLE_NO_NAGLE = k_nSteamNetworkingSend_UnreliableNoNagle,
		SEND_NO_DELAY = k_nSteamNetworkingSend_NoDelay,
		SEND_UNRELIABLE_NO_DELAY = k_nSteamNetworkingSend_UnreliableNoDelay,
		SEND_RELIABLE = k_nSteamNetworkingSend_Reliable,
		SEND_RELIABLE_NO_NAGLE = k_nSteamNetworkingSend_ReliableNoNagle,
		SEND_USE_CURRENT_THREAD = k_nSteamNetworkingSend_UseCurrentThread,
		SEND_AUTORESTART_BROKEN_SESSION = k_nSteamNetworkingSend_AutoRestartBrokenSession
	}

	uint8_t data[MAX_STEAM_PACKET_SIZE];
	uint32_t size = 0;
	uint64_t sender;
	int transfer_mode = SteamNetworkingSend.SEND_RELIABLE;
	SteamPacketPeer() {}
	SteamPacketPeer(const void *p_buffer, uint32_t p_buffer_size, int transferMode);


#endif // STEAM_PACKET_PEER_H