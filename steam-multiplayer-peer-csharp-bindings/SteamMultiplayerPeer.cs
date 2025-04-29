using System;
using Godot;
using Dictionary = Godot.Collections.Dictionary;

namespace SteamMultiplayerPeerCSharp;

public sealed partial class SteamMultiplayerPeer : RefCounted
{
    public MultiplayerPeerExtension MultiplayerPeer => (MultiplayerPeerExtension)_classReference;

    public delegate void NetworkConnectionStatusChangedEventHandler(long connect_handle, Dictionary connection, long old_state);

    public event NetworkConnectionStatusChangedEventHandler NetworkConnectionStatusChanged;

    public int ListenSocket
    {
        get => _classReference.Call(Methods.GetListenSocket).As<int>();
        set => _classReference.Call(Methods.SetListenSocket, value);
    }

    public bool NoNagle
    {
        get => _classReference.Call(Methods.GetNoNagle).As<bool>();
        set => _classReference.Call(Methods.SetNoNagle, value);
    }

    public bool NoDelay
    {
        get => _classReference.Call(Methods.GetNoDelay).As<bool>();
        set => _classReference.Call(Methods.SetNoDelay, value);
    }

    public SteamPeerConfig Configs
    {
        get => GetConfigs();
        set => SetConfigs(value);
    }

    private readonly GodotObject _classReference;
    private readonly Callable _signalTrampoline;
    private bool _disposed;

    public SteamMultiplayerPeer()
    {
        var sn = new StringName(nameof(SteamMultiplayerPeer));
        if (!ClassDB.ClassExists(sn)) throw new NotSupportedException($"{sn} class doesn't exist");
        if (!ClassDB.CanInstantiate(sn)) throw new Exception($"{sn} cannot be instantiated");

        _classReference = ClassDB.Instantiate(sn).AsGodotObject();

        _signalTrampoline = Callable.From(
            (long h, Dictionary d, long o) => NetworkConnectionStatusChanged?.Invoke(h, d, o));
        _classReference.Connect(Signals.NetworkConnectionStatusChanged, _signalTrampoline);
    }

    protected override void Dispose(bool disposing)
    {
        if (_disposed) return;
        if (_classReference.IsConnected(Signals.NetworkConnectionStatusChanged, _signalTrampoline))
            _classReference.Disconnect(Signals.NetworkConnectionStatusChanged, _signalTrampoline);
        _disposed = true;
        base.Dispose(disposing);
    }

    public Error CreateHost(ushort port) => _classReference.Call(Methods.CreateHost, port).As<Error>();
    public Error CreateClient(ulong steamId, ushort port) => _classReference.Call(Methods.CreateClient, steamId, port).As<Error>();
    public ulong GetSteam64FromPeerId(int peerId) => _classReference.Call(Methods.GetSteam64FromPeerId, peerId).As<ulong>();
    public int GetPeerIdFromSteam64(ulong steamId) => _classReference.Call(Methods.GetPeerIdFromSteam64, steamId).As<int>();
    public bool CloseListenSocket() => _classReference.Call(Methods.CloseListenSocket).As<bool>();
    public void SetListenSocket(int s) => _classReference.Call(Methods.SetListenSocket, s);
    public int GetListenSocket() => _classReference.Call(Methods.GetListenSocket).As<int>();
    public void SetNoNagle(bool e) => _classReference.Call(Methods.SetNoNagle, e);
    public bool GetNoNagle() => _classReference.Call(Methods.GetNoNagle).As<bool>();
    public void SetNoDelay(bool e) => _classReference.Call(Methods.SetNoDelay, e);
    public bool GetNoDelay() => _classReference.Call(Methods.GetNoDelay).As<bool>();
    public Dictionary GetPeerMap() => _classReference.Call(Methods.GetPeerMap).As<Dictionary>();
    public void SetConfigs(SteamPeerConfig c) => _classReference.Call(Methods.SetConfigs, c?.ClassReference);
    public SteamPeerConfig GetConfigs() => Configs;
    public void SetConfig(SteamNetworkingConfig c, Variant v) => Configs.SetConfig(c, v);
    public void ClearConfig(SteamNetworkingConfig c) => Configs.ClearConfig(c);
    public void ClearAllConfigs() => Configs.ClearAllConfigs();

    private static class Methods
    {
        public static readonly StringName CreateHost = new("create_host");
        public static readonly StringName CreateClient = new("create_client");
        public static readonly StringName GetSteam64FromPeerId = new("get_steam64_from_peer_id");
        public static readonly StringName GetPeerIdFromSteam64 = new("get_peer_id_from_steam64");
        public static readonly StringName CloseListenSocket = new("close_listen_socket");
        public static readonly StringName SetListenSocket = new("set_listen_socket");
        public static readonly StringName GetListenSocket = new("get_listen_socket");
        public static readonly StringName SetNoNagle = new("set_no_nagle");
        public static readonly StringName GetNoNagle = new("get_no_nagle");
        public static readonly StringName SetNoDelay = new("set_no_delay");
        public static readonly StringName GetNoDelay = new("get_no_delay");
        public static readonly StringName GetPeerMap = new("get_peer_map");
        public static readonly StringName SetConfigs = new("set_configs");
        public static readonly StringName GetConfigs = new("get_configs");
    }

    private static class Signals
    {
        public static readonly StringName NetworkConnectionStatusChanged = new("network_connection_status_changed");
    }
}
