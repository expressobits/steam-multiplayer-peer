#include "register_types.h"

#include <gdextension_interface.h>

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

static Steam *SteamPtr;

void initialize_godotsteampeer_module(ModuleInitializationLevel level){
	if(level == MODULE_INITIALIZATION_LEVEL_SCENE){
	}
}

void uninitialize_godotsteampeer_module(ModuleInitializationLevel level){
	if(level == MODULE_INITIALIZATION_LEVEL_SCENE){
	}
}

extern "C" {
	GDExtensionBool GDE_EXPORT godotsteam_init(GDExtensionInterfaceGetProcAddress p_interface, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization){
		godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

		init_obj.register_initializer(initialize_godotsteampeer);
		init_obj.register_terminator(uninitialize_godotsteampeer);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}