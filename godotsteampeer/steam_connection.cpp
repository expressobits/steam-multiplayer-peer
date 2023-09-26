#include "steam_connection.h"

void SteamConnection::_bind_methods() {
}

bool SteamConnection::operator==(const SteamConnection &other) {
	return steam_id == other.steam_id;
}

SteamConnection::SteamConnection(SteamID steam_id) {
	// this->peer_id = -1;
	this->steam_id.set_from_CSteamID(steam_id.data);
	// this->last_msg_timestamp = 0;
	// networkIdentity = SteamNetworkingIdentity();
	// networkIdentity.SetSteamID(steam_id.data);
}

SteamConnection::~SteamConnection() {
}
