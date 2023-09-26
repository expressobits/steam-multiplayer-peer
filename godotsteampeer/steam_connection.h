#ifndef STEAM_CONNECTION_H
#define STEAM_CONNECTION_H

#include <godot_cpp/classes/multiplayer_peer_extension.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/time.hpp>
#include <memory>

#include "steam_id.h"

#define MAX_STEAM_PACKET_SIZE k_cbMaxSteamNetworkingSocketsMessageSizeSend

using namespace godot;

class SteamConnection : public RefCounted {
	GDCLASS(SteamConnection, RefCounted)

public:
	SteamID steam_id;

protected:
	static void _bind_methods();

public:
	bool operator==(const SteamConnection &data);
	SteamConnection();
	~SteamConnection();
};

#endif // STEAM_CONNECTION_H
