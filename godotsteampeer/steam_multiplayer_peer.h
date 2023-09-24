#ifndef STEAM_MULTIPLAYER_PEER_H
#define STEAM_MULTIPLAYER_PEER_H

#include <godot_cpp/classes/multiplayer_peer_extension.hpp>

namespace godot {

class SteamMultiplayerPeer : public MultiplayerPeerExtension {
  GDCLASS(SteamMultiplayerPeer, MultiplayerPeerExtension)

public:
  SteamMultiplayerPeer();
  ~SteamMultiplayerPeer();
  Error _get_packet(const uint8_t **r_buffer, int32_t *r_buffer_size) override;
  Error _put_packet(const uint8_t *p_buffer, int32_t p_buffer_size) override;
  int32_t _get_available_packet_count() const override;
  int32_t _get_max_packet_size() const override;
  // PackedByteArray _get_packet_script();
  // Error _put_packet_script(const PackedByteArray &p_buffer);
  int32_t _get_packet_channel() const;
  MultiplayerPeer::TransferMode _get_packet_mode() const;
  // void _set_transfer_channel(int32_t p_channel) override;
  // int32_t _get_transfer_channel() const;
  // void _set_transfer_mode(MultiplayerPeer::TransferMode p_mode) override;
  // MultiplayerPeer::TransferMode _get_transfer_mode() const override;
  void _set_target_peer(int32_t p_peer) override;
  int32_t _get_packet_peer() const override;
  bool _is_server() const override;
  void _poll() override;
  void _close() override;
  void _disconnect_peer(int32_t p_peer, bool p_force) override;
  int32_t _get_unique_id() const override;
  // void _set_refuse_new_connections(bool p_enable) override;
  // bool _is_refusing_new_connections() const override;
  bool _is_server_relay_supported() const override;
  MultiplayerPeer::ConnectionStatus _get_connection_status() const override;

protected:
  static void _bind_methods();
};

} // namespace godot

#endif // STEAM_MULTIPLAYER_PEER_H