#include "register_types.h"

#include <gdextension_interface.h>

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/godot.hpp>

#include "steam_connection.h"
#include "steam_multiplayer_peer.h"

using namespace godot;

void initialize_godotsteampeer(ModuleInitializationLevel level){
	if(level == MODULE_INITIALIZATION_LEVEL_SCENE){
		ClassDB::register_class<SteamConnection>();
		ClassDB::register_class<SteamMultiplayerPeer>();
	}
}

void uninitialize_godotsteampeer(ModuleInitializationLevel level){
	if(level == MODULE_INITIALIZATION_LEVEL_SCENE){
	}
}

extern "C" {
	GDExtensionBool GDE_EXPORT godotsteampeer_init(GDExtensionInterfaceGetProcAddress p_interface, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization){
		godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

		init_obj.register_initializer(initialize_godotsteampeer);
		init_obj.register_terminator(uninitialize_godotsteampeer);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}