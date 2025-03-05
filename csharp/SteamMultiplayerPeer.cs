using System;
using Array = Godot.Collections.Array;

namespace Godot;

public sealed partial class SteamMultiplayerPeer : MultiplayerPeerExtension
{
    /// <summary>
    /// Reference to the Godot class in gdscript.
    /// </summary>
    private readonly GodotObject _classReference;

    public SteamMultiplayerPeer()
    {
        var stringName = new StringName(nameof(SteamMultiplayerPeer));
        
        if (!ClassDB.ClassExists(stringName))
            throw new NotSupportedException($"{stringName} class doesn't exist");
        
        if (!ClassDB.CanInstantiate(stringName))
            throw new Exception($"{stringName} cannot be instantiated.");
        
        _classReference = ClassDB.Instantiate(stringName).AsGodotObject();
    }

    public Error CreateHost(ushort port, Array options)
    {
        return _classReference.Call(Methods.CreateHost, port, options).As<Error>();
    }

    public Error CreateClient(ulong steamId, ushort port, Array options)
    {
        return _classReference.Call(Methods.CreateClient, steamId, port, options).As<Error>();
    }

    public override ConnectionStatus _GetConnectionStatus()
    {
        return _classReference.Call(Methods.GetConnectionStatus).As<ConnectionStatus>();
    }

    public override int _GetUniqueId()
    {
        return _classReference.Call(Methods.GetUniqueId).As<int>();
    }

    public override int _GetAvailablePacketCount()
    {
        return _classReference.Call(Methods.GetAvailablePacketCount).As<int>();
    }

    public override void _Poll()
    {
        _classReference.Call(Methods.Poll);
    }

    public override void _SetTransferChannel(int pChannel)
    {
        _classReference.Call(Methods.SetTransferChannel, pChannel);
    }

    public override void _SetTransferMode(TransferModeEnum pMode)
    {
        _classReference.Call(Methods.SetTransferMode, (long)pMode);
    }

    public override void _Close()
    {
        _classReference.Call(Methods.Close);
    }

    public override void _DisconnectPeer(int pPeer, bool pForce)
    {
        _classReference.Call(Methods.DisconnectPeer, pPeer, pForce);
    }

    public override int _GetMaxPacketSize()
    {
        return _classReference.Call(Methods.GetMaxPacketSize).As<int>();
    }

    public override int _GetPacketChannel()
    {
        return _classReference.Call(Methods.GetPacketChannel).As<int>();
    }

    public override TransferModeEnum _GetPacketMode()
    {
        return _classReference.Call(Methods.GetPacketMode).As<TransferModeEnum>();
    }

    public override int _GetPacketPeer()
    {
        return _classReference.Call(Methods.GetPacketPeer).As<int>();
    }

    public override byte[] _GetPacketScript()
    {
        return _classReference.Call(Methods.GetPacketScript).As<byte[]>();
    }

    public override int _GetTransferChannel()
    {
        return _classReference.Call(Methods.GetTransferChannel).As<int>();
    }

    public override TransferModeEnum _GetTransferMode()
    {
        return _classReference.Call(Methods.GetTransferMode).As<TransferModeEnum>();
    }

    public override bool _IsRefusingNewConnections()
    {
        return _classReference.Call(Methods.IsRefusingNewConnections).As<bool>();
    }

    public override bool _IsServer()
    {
        return _classReference.Call(Methods.IsServer).As<bool>();
    }

    public override bool _IsServerRelaySupported()
    {
        return _classReference.Call(Methods.IsServerRelaySupported).As<bool>();
    }

    public override Error _PutPacketScript(byte[] pBuffer)
    {
        return _classReference.Call(Methods.PutPacketScript, pBuffer).As<Error>();
    }

    public override void _SetRefuseNewConnections(bool pEnable)
    {
        _classReference.Call(Methods.SetRefuseNewConnections, pEnable);
    }

    public override void _SetTargetPeer(int pPeer)
    {
        _classReference.Call(Methods.SetTargetPeer, pPeer);
    }

    private static class Methods
    {
        public static readonly StringName CreateHost = new("create_host");
        public static readonly StringName CreateClient = new("create_client");
        public static readonly StringName GetConnectionStatus = new("get_connection_status");
        public static readonly StringName GetUniqueId = new("get_unique_id");
        public static readonly StringName GetAvailablePacketCount = new("get_available_packet_count");
        public static readonly StringName Poll = new("poll");
        public static readonly StringName SetTransferChannel = new("set_transfer_channel");
        public static readonly StringName SetTransferMode = new("set_transfer_mode");
        public static readonly StringName Close = new("close");
        public static readonly StringName DisconnectPeer = new("disconnect_peer");
        public static readonly StringName GetMaxPacketSize = new("get_max_packet_size");
        public static readonly StringName GetPacketChannel = new("get_packet_channel");
        public static readonly StringName GetPacketMode = new("get_packet_mode");
        public static readonly StringName GetPacketPeer = new("get_packet_peer");
        public static readonly StringName GetPacketScript = new("get_packet_script");
        public static readonly StringName GetTransferChannel = new("get_transfer_channel");
        public static readonly StringName GetTransferMode = new("get_transfer_mode");
        public static readonly StringName IsRefusingNewConnections = new("is_refusing_new_connections");
        public static readonly StringName IsServer = new("is_server");
        public static readonly StringName IsServerRelaySupported = new("is_server_relay_supported");
        public static readonly StringName PutPacketScript = new("put_packet_script");
        public static readonly StringName SetRefuseNewConnections = new("set_refuse_new_connections");
        public static readonly StringName SetTargetPeer = new("set_target_peer");
    }
}
