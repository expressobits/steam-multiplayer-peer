#ifndef STEAM_MULTIPLAYER_PEER_H
#define STEAM_MULTIPLAYER_PEER_H

#include <godot_cpp/classes/multiplayer_peer_extension.hpp>

namespace godot {

class SteamMultiplayerPeer : MultiplayerPeerExtension {
  GDCLASS(SteamMultiplayerPeer, MultiplayerPeerExtension);

  public:
    Error _get_packet(const uint8_t * *r_buffer, int32_t *r_buffer_size);

  protected:
    static void _bind_methods();
    
};

} // namespace godot

#endif // STEAM_MULTIPLAYER_PEER_H