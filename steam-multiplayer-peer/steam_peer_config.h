#ifndef STEAM_PEER_CONFIG
#define STEAM_PEER_CONFIG

#include "steam/steam_api.h"
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {

class SteamPeerConfig : public RefCounted {
	GDCLASS(SteamPeerConfig, RefCounted)

private:
	Dictionary options;
	SteamNetworkingConfigValue_t *option_array;

public:
	SteamPeerConfig() {}
	Dictionary get_options() const;
	int size() const;
	SteamNetworkingConfigValue_t *get_convert_options() const;
	void set_options(const Dictionary new_options);
	void add(const int config, Variant value);
	void remove(const int config);

protected:
	static void _bind_methods();
};

} //namespace godot

#endif // STEAM_PEER_CONFIG