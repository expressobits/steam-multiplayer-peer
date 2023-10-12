#include "steam_id.h"

using namespace godot;

VARIANT_ENUM_CAST(SteamID::AccountType);
VARIANT_ENUM_CAST(SteamID::Universe);

SteamID::SteamID() {
    data = CSteamID();
}

SteamID::SteamID(CSteamID steam_id) {
    data = steam_id;
}

void SteamID::setData(CSteamID value) {
    data = value;
}

uint64_t SteamID::to_int() const {
    return data.ConvertToUint64();
}

void SteamID::set_from_int(uint64_t i) {
    data.SetFromUint64(i);
}

void SteamID::set_from_CSteamID(CSteamID i) {
    data = i;
}

uint32_t SteamID::get_account_id() {
    return data.GetAccountID();
}

uint32_t SteamID::get_un_account_instance() {
    return data.GetUnAccountInstance();
}

SteamID::AccountType SteamID::get_account_type() {
    return AccountType(data.GetEAccountType());
}

SteamID::Universe SteamID::get_universe() {
    return Universe(data.GetEUniverse());
}

void SteamID::_bind_methods() {
    ClassDB::bind_method(D_METHOD("to_int"), &SteamID::to_int);
    ClassDB::bind_method(D_METHOD("from_int", "id"), &SteamID::set_from_int);

    ClassDB::bind_method(D_METHOD("get_account_id"), &SteamID::get_account_id);
    ClassDB::bind_method(D_METHOD("get_account_type"), &SteamID::get_account_type);
    ClassDB::bind_method(D_METHOD("get_un_account_instance"), &SteamID::get_un_account_instance);
    ClassDB::bind_method(D_METHOD("get_universe"), &SteamID::get_universe);

    BIND_ENUM_CONSTANT(ACCOUNT_TYPE_INVALID);
    BIND_ENUM_CONSTANT(ACCOUNT_TYPE_INDIVIDUAL);
    BIND_ENUM_CONSTANT(ACCOUNT_TYPE_MULTISEAT);
    BIND_ENUM_CONSTANT(ACCOUNT_TYPE_GAME_SERVER);
    BIND_ENUM_CONSTANT(ACCOUNT_TYPE_ANON_GAME_SERVER);
    BIND_ENUM_CONSTANT(ACCOUNT_TYPE_PENDING);
    BIND_ENUM_CONSTANT(ACCOUNT_TYPE_CONTENT_SERVER);
    BIND_ENUM_CONSTANT(ACCOUNT_TYPE_CLAN);
    BIND_ENUM_CONSTANT(ACCOUNT_TYPE_CHAT);
    BIND_ENUM_CONSTANT(ACCOUNT_TYPE_CONSOLE_USER);
    BIND_ENUM_CONSTANT(ACCOUNT_TYPE_ANON_USER);

    BIND_ENUM_CONSTANT(UNIVERSE_INVALID);
    BIND_ENUM_CONSTANT(UNIVERSE_PUBLIC);
    BIND_ENUM_CONSTANT(UNIVERSE_BETA);
    BIND_ENUM_CONSTANT(UNIVERSE_INTERNAL);
    BIND_ENUM_CONSTANT(UNIVERSE_DEV);

}
