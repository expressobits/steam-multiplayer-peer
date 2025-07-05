using System;
using Godot;
using Dictionary = Godot.Collections.Dictionary;

namespace SteamMultiplayerPeerCSharp;

public sealed partial class SteamPeerConfig : RefCounted
{
    public GodotObject ClassReference => _classReference;

    private readonly GodotObject _classReference;

    public SteamPeerConfig()
    {
        var stringName = new StringName(nameof(SteamPeerConfig));

        if (!ClassDB.ClassExists(stringName))
            throw new NotSupportedException($"{stringName} class doesn't exist");

        if (!ClassDB.CanInstantiate(stringName))
            throw new Exception($"{stringName} cannot be instantiated.");

        _classReference = ClassDB.Instantiate(stringName).AsGodotObject();
    }
    public SteamPeerConfig(GodotObject classReference)
    {
        _classReference = classReference ?? throw new ArgumentNullException(nameof(classReference));
    }

    public Dictionary Options
    {
        get => GetOptions();
        set => SetOptions(value);
    }

    public Dictionary GetOptions()
        => _classReference.Call(Methods.GetOptions).As<Dictionary>();

    public void SetOptions(Dictionary options)
        => _classReference.Call(Methods.SetOptions, options);

    public void SetConfig(SteamNetworkingConfig config, Variant value) =>
        _classReference.Call(Methods.SetConfig, (long)config, value);

    public void ClearConfig(SteamNetworkingConfig config) =>
        _classReference.Call(Methods.ClearConfig, (long)config);

    public void ClearAllConfigs() =>
        _classReference.Call(Methods.ClearAllConfigs);

    private static class Methods
    {
        public static readonly StringName GetOptions = new("get_options");
        public static readonly StringName SetOptions = new("set_options");
        public static readonly StringName SetConfig = new("set_config");
        public static readonly StringName ClearConfig = new("clear_config");
        public static readonly StringName ClearAllConfigs = new("clear_all_configs");
    }
}
