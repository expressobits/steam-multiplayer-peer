#include "steam_connection.h"

void SteamConnection::_bind_methods() {
}

EResult SteamConnection::raw_send(Packet* packet) {
    // if(packet->channel == ChannelManagement::PING_CHANNEL) {
    //     if(packet->size != sizeof(PingPayload)) {
    //         Steam::get_singleton()->steamworksError("Error: This ping is the wrong size, rejecting");
    //         return k_EResultFail;
    //     }
    // }
    int64 *pOutMessageNumber;
    return SteamNetworkingSockets()->SendMessageToConnection(connection, packet->data, packet->size, packet->transfer_mode, pOutMessageNumber);
    // return SteamNetworkingMessages()->SendMessageToUser(networkIdentity, packet->data, packet->size, packet->transfer_mode, packet->channel);

}

bool SteamConnection::operator==(const SteamConnection &other) {
	return steam_id == other.steam_id;
}

SteamConnection::SteamConnection(SteamID steam_id) {
	this->peer_id = -1;
	this->steam_id.set_from_CSteamID(steam_id.data);
	this->last_msg_timestamp = 0;
	networkIdentity = SteamNetworkingIdentity();
	networkIdentity.SetSteamID(steam_id.data);
}

SteamConnection::~SteamConnection() {
	SteamNetworkingSockets()->CloseConnection(this->connection, ESteamNetConnectionEnd::k_ESteamNetConnectionEnd_App_Generic,"Disconnect Default!", true);
    // while(pending_retry_packets.size()) {   //zero is falsy in C++
    //     delete pending_retry_packets.front()->get();
    //     pending_retry_packets.pop_front();
    // }
}
