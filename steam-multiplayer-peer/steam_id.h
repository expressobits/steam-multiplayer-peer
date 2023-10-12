#ifndef STEAMID_H
#define STEAMID_H

#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include "steam/steam_api.h"

namespace godot {

class SteamID : public RefCounted
{
    GDCLASS(SteamID, RefCounted)

    CSteamID data;

public:
    SteamID();
    SteamID(CSteamID steam_id);
    void setData(CSteamID value);
    uint64_t to_int() const;
    void set_from_int(uint64_t i);
    void set_from_CSteamID(CSteamID i);
    uint32_t get_account_id();
    uint32_t get_un_account_instance();

    enum AccountType {
        ACCOUNT_TYPE_INVALID = k_EAccountTypeInvalid,
        ACCOUNT_TYPE_INDIVIDUAL = k_EAccountTypeIndividual,
        ACCOUNT_TYPE_MULTISEAT = k_EAccountTypeMultiseat,
        ACCOUNT_TYPE_GAME_SERVER = k_EAccountTypeGameServer,
        ACCOUNT_TYPE_ANON_GAME_SERVER = k_EAccountTypeAnonGameServer,
        ACCOUNT_TYPE_PENDING = k_EAccountTypePending,
        ACCOUNT_TYPE_CONTENT_SERVER = k_EAccountTypeContentServer,
        ACCOUNT_TYPE_CLAN = k_EAccountTypeClan,
        ACCOUNT_TYPE_CHAT = k_EAccountTypeChat,
        ACCOUNT_TYPE_CONSOLE_USER = k_EAccountTypeConsoleUser,
        ACCOUNT_TYPE_ANON_USER = k_EAccountTypeAnonUser
    };
    AccountType get_account_type();

    enum Universe {
        UNIVERSE_INVALID = k_EUniverseInvalid,
        UNIVERSE_PUBLIC = k_EUniversePublic,
        UNIVERSE_BETA = k_EUniverseBeta,
        UNIVERSE_INTERNAL = k_EUniverseInternal,
        UNIVERSE_DEV = k_EUniverseDev
    };
    Universe get_universe();

    inline bool operator==(const SteamID &val) const { return data == val.data; }
    inline bool operator!=(const SteamID &val) const { return data != val.data; }

    static void _bind_methods();
};

}
#endif // STEAMID_H
