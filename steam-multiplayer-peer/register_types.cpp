#include "register_types.h"

#include <gdextension_interface.h>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "steam_connection.h"
#include "steam_multiplayer_peer.h"
#include "steam_packet_peer.h"

using namespace godot;

void initialize_steam_multiplayer_peer(ModuleInitializationLevel level) {
	if (level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		ClassDB::register_class<SteamPacketPeer>();
		ClassDB::register_class<SteamConnection>();
		ClassDB::register_class<SteamMultiplayerPeer>();
	}
}

void uninitialize_steam_multiplayer_peer(ModuleInitializationLevel level) {
	if (level == MODULE_INITIALIZATION_LEVEL_SCENE) {
	}
}

extern "C" {
GDExtensionBool GDE_EXPORT steam_multiplayer_peer_init(GDExtensionInterfaceGetProcAddress p_interface, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

	init_obj.register_initializer(initialize_steam_multiplayer_peer);
	init_obj.register_terminator(uninitialize_steam_multiplayer_peer);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}