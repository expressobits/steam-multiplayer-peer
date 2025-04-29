using System;
using Godot;

public sealed partial class SteamPacketPeer : RefCounted
{
    private readonly GodotObject _classReference;

    public SteamPacketPeer()
    {
        var name = new StringName(nameof(SteamPacketPeer));
        
        if (!ClassDB.ClassExists(name))
            throw new NotSupportedException($"{name} class doesn't exist");
            
        if (!ClassDB.CanInstantiate(name))
            throw new Exception($"{name} cannot be instantiated");

        _classReference = ClassDB.Instantiate(name).AsGodotObject();
    }
}
