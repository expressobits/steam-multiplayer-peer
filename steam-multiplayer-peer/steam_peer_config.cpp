#include "steam_peer_config.h"
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

Dictionary SteamPeerConfig::get_options() const {
	return options;
}

int SteamPeerConfig::size() const {
	return options.size();
}

SteamNetworkingConfigValue_t *SteamPeerConfig::get_convert_options() const {
	int options_size = options.size();
	SteamNetworkingConfigValue_t *option_array = new SteamNetworkingConfigValue_t[options_size];
	if (options_size > 0) {
		for (int i = 0; i < options_size; i++) {
			SteamNetworkingConfigValue_t this_option;
			int sent_option = (int)options.keys()[i];
			UtilityFunctions::print(sent_option);
			ESteamNetworkingConfigValue this_value = ESteamNetworkingConfigValue((int)sent_option);
			Variant::Type type = options[sent_option].get_type();
			if (type == Variant::INT) {
				if (sent_option == k_ESteamNetworkingConfig_ConnectionUserData) {
					this_option.SetInt64(this_value, options[sent_option]);
				} else {
					this_option.SetInt32(this_value, options[sent_option]);
				}
			} else if (type == Variant::FLOAT) {
				this_option.SetFloat(this_value, options[sent_option]);
			} else if (type == Variant::STRING) {
				char *this_string = { 0 };
				String passed_string = options[sent_option];
				strcpy(this_string, passed_string.utf8().get_data());
				this_option.SetString(this_value, this_string);
			} else {
				Object *this_pointer;
				this_pointer = options[sent_option];
				this_option.SetPtr(this_value, this_pointer);
			}
			option_array[i] = this_option;
		}
	}
	return option_array;
}

void SteamPeerConfig::set_options(const Dictionary new_options) {
	options = new_options;
}

void SteamPeerConfig::add(const int config, Variant value) {
	options[config] = value;
}

void SteamPeerConfig::remove(const int config) {
	options.erase(config);
}

void SteamPeerConfig::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_options", "options"), &SteamPeerConfig::set_options);
	ClassDB::bind_method(D_METHOD("get_options"), &SteamPeerConfig::get_options);
	ClassDB::bind_method(D_METHOD("add", "steam_networking_config", "value"), &SteamPeerConfig::add);
	// ClassDB::bind_method(D_METHOD("remove", "steam_networking_config"), &SteamPeerConfig::remove);
	// ClassDB::bind_method(D_METHOD("set_as_relay", "as_relay"), &SteamMultiplayerPeer::set_as_relay);
	// ClassDB::bind_method(D_METHOD("get_as_relay"), &SteamMultiplayerPeer::get_as_relay);

	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "options"), "set_options", "get_options");
}