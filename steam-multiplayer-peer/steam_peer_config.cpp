#include "steam_peer_config.h"

using namespace godot;

Array SteamPeerConfig::get_options() const {
	return options;
}

void SteamPeerConfig::set_options(const Array new_options) {
    options = new_options;
}

void SteamPeerConfig::add(const SteamNetworkingConfig config, Variant value) {
    Array new_option = Array();
    new_option[0] = config;
    new_option[1] = value.get_type();
    new_option[2] = value;
    options.append(new_option);
}

void SteamPeerConfig::remove(const SteamNetworkingConfig config) {
	// TODO make this
}

void SteamPeerConfig::_bind_methods() {

    ClassDB::bind_method(D_METHOD("set_options", "options"), &SteamPeerConfig::set_options);
    ClassDB::bind_method(D_METHOD("get_options"), &SteamPeerConfig::get_options);
	// ClassDB::bind_method(D_METHOD("set_as_relay", "as_relay"), &SteamMultiplayerPeer::set_as_relay);
	// ClassDB::bind_method(D_METHOD("get_as_relay"), &SteamMultiplayerPeer::get_as_relay);

	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "options"), "set_options", "get_options");
}

// Helper function to turn an array of options into an array of
// SteamNetworkingConfigValue_t structs
SteamNetworkingConfigValue_t *SteamPeerConfig::convert_options_array(const Array options) const{
	// Get the number of option arrays in the array.
	int options_size = options.size();
	// Create the array for options.
	SteamNetworkingConfigValue_t *option_array = new SteamNetworkingConfigValue_t[options_size];
	// If there are options
	if (options_size > 0) {
		// Populate the options
		for (int i = 0; i < options_size; i++) {
			SteamNetworkingConfigValue_t this_option;
			Array sent_option = options[i];
			// Get the configuration value.
			// This is a convoluted way of doing it but can't seem to cast the value
			// as an enum so here we are.
			ESteamNetworkingConfigValue this_value = ESteamNetworkingConfigValue((int)sent_option[0]);
			if ((int)sent_option[1] == 1) {
				this_option.SetInt32(this_value, sent_option[2]);
			} else if ((int)sent_option[1] == 2) {
				this_option.SetInt64(this_value, sent_option[2]);
			} else if ((int)sent_option[1] == 3) {
				this_option.SetFloat(this_value, sent_option[2]);
			} else if ((int)sent_option[1] == 4) {
				char *this_string = { 0 };
				String passed_string = sent_option[2];
				strcpy(this_string, passed_string.utf8().get_data());
				this_option.SetString(this_value, this_string);
			} else {
				Object *this_pointer;
				this_pointer = sent_option[2];
				this_option.SetPtr(this_value, this_pointer);
			}
			option_array[i] = this_option;
		}
	}
	return option_array;
}