using System;
using Godot;

namespace SteamMultiplayerPeerCSharp;

public sealed partial class SteamConnection : RefCounted
{
    private readonly GodotObject _classReference;

    public SteamConnection()
    {
        var name = new StringName(nameof(SteamConnection));
        
        if (!ClassDB.ClassExists(name))
            throw new NotSupportedException($"{name} class doesn't exist");
            
        if (!ClassDB.CanInstantiate(name))
            throw new Exception($"{name} cannot be instantiated");
            
        _classReference = ClassDB.Instantiate(name).AsGodotObject();
    }
}
