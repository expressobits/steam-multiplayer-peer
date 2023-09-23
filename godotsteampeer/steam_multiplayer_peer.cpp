#include <godot_cpp/core/class_db.hpp>

#include "steam_multiplayer_peer.h"
#include "godotsteam/public/godotsteam.h"

using namespace godot;

godot::SteamMultiplayerPeer::SteamMultiplayerPeer() {

    if(SteamUser() != NULL)
        ERR_PRINT("MEU ERRO");

}

godot::SteamMultiplayerPeer::~SteamMultiplayerPeer() {}

Error SteamMultiplayerPeer::_get_packet(const uint8_t **r_buffer,
                                        int32_t *r_buffer_size) {
  return Error();
}

Error SteamMultiplayerPeer::_put_packet(const uint8_t *p_buffer, int32_t p_buffer_size)
{
    return Error();
}

int32_t SteamMultiplayerPeer::_get_available_packet_count() const
{
    return 0;
}

int32_t SteamMultiplayerPeer::_get_max_packet_size() const
{
    return 0;
}

// PackedByteArray SteamMultiplayerPeer::_get_packet_script()
// {
//     return PackedByteArray();
// }

// Error SteamMultiplayerPeer::_put_packet_script(const PackedByteArray &p_buffer)
// {
//     return Error();
// }

int32_t SteamMultiplayerPeer::_get_packet_channel() const
{
    return 0;
}

MultiplayerPeer::TransferMode SteamMultiplayerPeer::_get_packet_mode() const
{
    return MultiplayerPeer::TransferMode();
}

// void SteamMultiplayerPeer::_set_transfer_channel(int32_t p_channel) {}

// int32_t SteamMultiplayerPeer::_get_transfer_channel() const
// {
//     return 0;
// }

// void SteamMultiplayerPeer::_set_transfer_mode(
//     MultiplayerPeer::TransferMode p_mode) {}

void SteamMultiplayerPeer::_set_target_peer(int32_t p_peer) {}

int32_t SteamMultiplayerPeer::_get_packet_peer() const
{
    return 0;
}

bool SteamMultiplayerPeer::_is_server() const
{
    return false;
}

void SteamMultiplayerPeer::_poll() {}

void SteamMultiplayerPeer::_close() {}

void SteamMultiplayerPeer::_disconnect_peer(int32_t p_peer,
                                                   bool p_force) {}

int32_t SteamMultiplayerPeer::_get_unique_id() const
{
    return 0;
}

// void SteamMultiplayerPeer::_set_refuse_new_connections(bool p_enable) {}

// bool SteamMultiplayerPeer::_is_refusing_new_connections() const
// {
//     return false;
// }

bool SteamMultiplayerPeer::_is_server_relay_supported() const
{
    return false;
}

MultiplayerPeer::ConnectionStatus SteamMultiplayerPeer::_get_connection_status() const
{
    return MultiplayerPeer::ConnectionStatus();
}

void SteamMultiplayerPeer::_bind_methods()
{
}