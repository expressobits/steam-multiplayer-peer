# <img src="https://raw.githubusercontent.com/expressobits/steam-multiplayer-peer/main/icon.png" alt= “icon” width="32" height="32"> Welcome to Expresso Steam Multiplayer Peer 👋
![Version](https://img.shields.io/badge/version-0.0.6-blue.svg?cacheSeconds=2592000)
[![Documentation](https://img.shields.io/badge/documentation-no-red.svg)](todo-doc)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](MIT)

See demos in:
Github: [Branch demos](https://github.com/expressobits/steam-multiplayer-peer/tree/demos)
Godot Asset Lib: https://godotengine.org/asset-library/asset/2258


## Features

✔️ Change easy Enet peer to Steam Peer

✔️ Use Steam Sockets (Low level like enet).

✔️ GDExtension (Easy to add your project)

  - [[WIKI] How to test multiplayer demo](https://github.com/expressobits/inventory-system/wiki/How-to-test-multiplayer-demo)
 
✔️ No dependency with GodotSteam, but demo use GodotSteam to handle connections with lobbies (See lobbies tutorial in Godot Steam [here](https://godotsteam.com/tutorials/lobbies/)).


## GodotSteam SteamMultiplayerPeer Differences

| Differences | This SteamMultiplayerPeer | GodotSteam SteamMultiplayerPeer |
|---|---|---|
| Lib Type | GDExtension, add on your project libs to use easy. | C++ module, you need to use the precompiled ones from godotsteam or compile it yourself |
| Steam Connection | Steam Sockets (https://partner.steamgames.com/doc/api/ISteamNetworkingSockets) Steam Sockets, Steam's lowest connection level, manages a connection  (It's very close to Enet, that's why I chose this approach for the plugin) | Steam Messages (https://partner.steamgames.com/doc/api/ISteamNetworkingMessages) Without a connection idea, the connection is managed by the lobby, determining the need to have steam lobbies. |
| TODO  |  |  |

## Known issues

⚠️ Features No channel support currently
At some point I intend to integrate channels to be used in rpcs commands, but currently it is only necessary to use channel 0 or the default rpcs.

## In Progress

🔨 Bugs fixes

## Planneds

📅 No planned features.

<!-- ## Install
See in [Wiki](https://github.com/ExpressoBits/inventory-system/wiki) -->

## Authors

👤 **Rafael Correa**
* Twitter: [@ScriptsEngineer](https://twitter.com/ScriptsEngineer)
* Github: [@scriptsengineer](https://github.com/scriptsengineer)

👤 **Zennyth**
* Github: [@Zennyth](https://github.com/Zennyth)

👤 **greenfox1505**
* Github: [@greenfox1505](https://github.com/greenfox1505)

👤 **MichaelMacha**
* Github: [@MichaelMacha](https://github.com/MichaelMacha)


## 🤝 Contributing

Contributions, issues and feature requests are welcome!

Feel free to check [issues page](https://github.com/ExpressoBits/steam-multiplayer-peer/issues).

To suggest or discuss some project structure, feel free here [discussions page](https://github.com/expressobits/steam-multiplayer-peer/discussions)


## Show your support

Give a ⭐️ if this project helped you!


## 📝 License

This project is [MIT](MIT) licensed.