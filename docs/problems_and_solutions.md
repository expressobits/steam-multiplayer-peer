## Problems and Solutions
This document list problems and solutions founded

### Unique id != steam_id
Use enet get_unique_id, dont use steam_id because return is 32bits and steam_id has 64bits
This problem cause implementation of maps for relation unique_id with steam_id

### pool vs network_connection_status_changed
On enet example uses pool event for connect clients and steamworks works with network_connection_status_changed callback.