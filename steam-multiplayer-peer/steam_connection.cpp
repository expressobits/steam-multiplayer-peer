#include "steam_connection.h"
#include <godot_cpp/variant/utility_functions.hpp>

void SteamConnection::_bind_methods() {
}

EResult SteamConnection::_raw_send(Ref<SteamPacketPeer> packet) {
	return SteamNetworkingSockets()->SendMessageToConnection(steam_connection, packet->data, packet->size, packet->transfer_mode, nullptr);
}

// TODO change to return correct error
Error SteamConnection::_send_pending() {
	while (pending_retry_packets.size() > 0) {
		Ref<SteamPacketPeer> packet = pending_retry_packets.front()->get();
		EResult errorCode = _raw_send(packet);
		if (errorCode == k_EResultOK) {
			pending_retry_packets.pop_front();
		} else {
			String errorString = _convert_eresult_to_string(errorCode);
			if (packet->transfer_mode & k_nSteamNetworkingSend_Reliable) { //comparison to ensure inclusion
				WARN_PRINT(String("Send Error (Reliable, will retry): ") + errorString);
				break;
				//break, retry send later
			} else {
				WARN_PRINT(String("Send Error (Unreliable, won't retry): ") + errorString);
				pending_retry_packets.pop_front();
				//toss unreliable packet, move on
			}
		}
	}

	return OK;
}

void SteamConnection::_add_packet(Ref<SteamPacketPeer> packet) {
	pending_retry_packets.push_back(packet);
}

Error SteamConnection::send(Ref<SteamPacketPeer> packet) {
	_add_packet(packet);
	return _send_pending();
}

void SteamConnection::flush() {
	ERR_FAIL_COND_MSG(steam_connection == k_HSteamNetConnection_Invalid, "The Steam Connections is invalid for flush!");
	SteamNetworkingSockets()->FlushMessagesOnConnection(steam_connection);
}

bool SteamConnection::close() {
	if (SteamNetworkingSockets() == NULL) {
		WARN_PRINT(String("SteamNetworkingSockets is null!"));
		return false;
	}
	if (steam_connection == k_HSteamNetConnection_Invalid) {
		WARN_PRINT(String("Steam Connection is invalid!"));
		return false;
	}
	if (!SteamNetworkingSockets()->CloseConnection(steam_connection, ESteamNetConnectionEnd::k_ESteamNetConnectionEnd_App_Generic, "Failed to accept connection", false)) {
		WARN_PRINT(String("Fail to close connection!"));
		return false;
	}
	return true;
}

bool SteamConnection::operator==(const SteamConnection &other) {
	return steam_id == other.steam_id;
}

SteamConnection::SteamConnection(uint64_t steam_id) {
	this->peer_id = -1;
	this->steam_id = steam_id;
	this->last_msg_timestamp = 0;
}

SteamConnection::~SteamConnection() {
	SteamNetworkingSockets()->CloseConnection(this->steam_connection, ESteamNetConnectionEnd::k_ESteamNetConnectionEnd_App_Generic, "Disconnect Default!", true);
	while (pending_retry_packets.size()) {
		Ref<SteamPacketPeer> p = pending_retry_packets.front()->get();
		pending_retry_packets.pop_front();
	}
}

Error SteamConnection::request_peer() {
	SetupPeerPayload payload = SetupPeerPayload();
	return _send_setup_peer(payload);
}

Error SteamConnection::send_peer(uint32_t peer_id) {
	SteamConnection::SetupPeerPayload payload = SteamConnection::SetupPeerPayload();
	payload.peer_id = peer_id;
	return _send_setup_peer(payload);
}

Error SteamConnection::_send_setup_peer(const SetupPeerPayload payload) {
	Ref<SteamPacketPeer> packet = Ref<SteamPacketPeer>(memnew(SteamPacketPeer((void *)&payload, sizeof(SetupPeerPayload), MultiplayerPeer::TRANSFER_MODE_RELIABLE)));
	return send(packet);
}

// Long but simple: just return the type of the EResult as a Godot String
String SteamConnection::_convert_eresult_to_string(EResult e) {
	switch (e) {
		case k_EResultNone:
			return String("k_EResultNone");
		case k_EResultOK:
			return String("k_EResultOK");
		case k_EResultFail:
			return String("k_EResultFail");
		case k_EResultNoConnection:
			return String("k_EResultNoConnection");
		case k_EResultInvalidPassword:
			return String("k_EResultInvalidPassword");
		case k_EResultLoggedInElsewhere:
			return String("k_EResultLoggedInElsewhere");
		case k_EResultInvalidProtocolVer:
			return String("k_EResultInvalidProtocolVer");
		case k_EResultInvalidParam:
			return String("k_EResultInvalidParam");
		case k_EResultFileNotFound:
			return String("k_EResultFileNotFound");
		case k_EResultBusy:
			return String("k_EResultBusy");
		case k_EResultInvalidState:
			return String("k_EResultInvalidState");
		case k_EResultInvalidName:
			return String("k_EResultInvalidName");
		case k_EResultInvalidEmail:
			return String("k_EResultInvalidEmail");
		case k_EResultDuplicateName:
			return String("k_EResultDuplicateName");
		case k_EResultAccessDenied:
			return String("k_EResultAccessDenied");
		case k_EResultTimeout:
			return String("k_EResultTimeout");
		case k_EResultBanned:
			return String("k_EResultBanned");
		case k_EResultAccountNotFound:
			return String("k_EResultAccountNotFound");
		case k_EResultInvalidSteamID:
			return String("k_EResultInvalidSteamID");
		case k_EResultServiceUnavailable:
			return String("k_EResultServiceUnavailable");
		case k_EResultNotLoggedOn:
			return String("k_EResultNotLoggedOn");
		case k_EResultPending:
			return String("k_EResultPending");
		case k_EResultEncryptionFailure:
			return String("k_EResultEncryptionFailure");
		case k_EResultInsufficientPrivilege:
			return String("k_EResultInsufficientPrivilege");
		case k_EResultLimitExceeded:
			return String("k_EResultLimitExceeded");
		case k_EResultRevoked:
			return String("k_EResultRevoked");
		case k_EResultExpired:
			return String("k_EResultExpired");
		case k_EResultAlreadyRedeemed:
			return String("k_EResultAlreadyRedeemed");
		case k_EResultDuplicateRequest:
			return String("k_EResultDuplicateRequest");
		case k_EResultAlreadyOwned:
			return String("k_EResultAlreadyOwned");
		case k_EResultIPNotFound:
			return String("k_EResultIPNotFound");
		case k_EResultPersistFailed:
			return String("k_EResultPersistFailed");
		case k_EResultLockingFailed:
			return String("k_EResultLockingFailed");
		case k_EResultLogonSessionReplaced:
			return String("k_EResultLogonSessionReplaced");
		case k_EResultConnectFailed:
			return String("k_EResultConnectFailed");
		case k_EResultHandshakeFailed:
			return String("k_EResultHandshakeFailed");
		case k_EResultIOFailure:
			return String("k_EResultIOFailure");
		case k_EResultRemoteDisconnect:
			return String("k_EResultRemoteDisconnect");
		case k_EResultShoppingCartNotFound:
			return String("k_EResultShoppingCartNotFound");
		case k_EResultBlocked:
			return String("k_EResultBlocked");
		case k_EResultIgnored:
			return String("k_EResultIgnored");
		case k_EResultNoMatch:
			return String("k_EResultNoMatch");
		case k_EResultAccountDisabled:
			return String("k_EResultAccountDisabled");
		case k_EResultServiceReadOnly:
			return String("k_EResultServiceReadOnly");
		case k_EResultAccountNotFeatured:
			return String("k_EResultAccountNotFeatured");
		case k_EResultAdministratorOK:
			return String("k_EResultAdministratorOK");
		case k_EResultContentVersion:
			return String("k_EResultContentVersion");
		case k_EResultTryAnotherCM:
			return String("k_EResultTryAnotherCM");
		case k_EResultPasswordRequiredToKickSession:
			return String("k_EResultPasswordRequiredToKickSession");
		case k_EResultAlreadyLoggedInElsewhere:
			return String("k_EResultAlreadyLoggedInElsewhere");
		case k_EResultSuspended:
			return String("k_EResultSuspended");
		case k_EResultCancelled:
			return String("k_EResultCancelled");
		case k_EResultDataCorruption:
			return String("k_EResultDataCorruption");
		case k_EResultDiskFull:
			return String("k_EResultDiskFull");
		case k_EResultRemoteCallFailed:
			return String("k_EResultRemoteCallFailed");
		case k_EResultPasswordUnset:
			return String("k_EResultPasswordUnset");
		case k_EResultExternalAccountUnlinked:
			return String("k_EResultExternalAccountUnlinked");
		case k_EResultPSNTicketInvalid:
			return String("k_EResultPSNTicketInvalid");
		case k_EResultExternalAccountAlreadyLinked:
			return String("k_EResultExternalAccountAlreadyLinked");
		case k_EResultRemoteFileConflict:
			return String("k_EResultRemoteFileConflict");
		case k_EResultIllegalPassword:
			return String("k_EResultIllegalPassword");
		case k_EResultSameAsPreviousValue:
			return String("k_EResultSameAsPreviousValue");
		case k_EResultAccountLogonDenied:
			return String("k_EResultAccountLogonDenied");
		case k_EResultCannotUseOldPassword:
			return String("k_EResultCannotUseOldPassword");
		case k_EResultInvalidLoginAuthCode:
			return String("k_EResultInvalidLoginAuthCode");
		case k_EResultAccountLogonDeniedNoMail:
			return String("k_EResultAccountLogonDeniedNoMail");
		case k_EResultHardwareNotCapableOfIPT:
			return String("k_EResultHardwareNotCapableOfIPT");
		case k_EResultIPTInitError:
			return String("k_EResultIPTInitError");
		case k_EResultParentalControlRestricted:
			return String("k_EResultParentalControlRestricted");
		case k_EResultFacebookQueryError:
			return String("k_EResultFacebookQueryError");
		case k_EResultExpiredLoginAuthCode:
			return String("k_EResultExpiredLoginAuthCode");
		case k_EResultIPLoginRestrictionFailed:
			return String("k_EResultIPLoginRestrictionFailed");
		case k_EResultAccountLockedDown:
			return String("k_EResultAccountLockedDown");
		case k_EResultAccountLogonDeniedVerifiedEmailRequired:
			return String("k_EResultAccountLogonDeniedVerifiedEmailRequired");
		case k_EResultNoMatchingURL:
			return String("k_EResultNoMatchingURL");
		case k_EResultBadResponse:
			return String("k_EResultBadResponse");
		case k_EResultRequirePasswordReEntry:
			return String("k_EResultRequirePasswordReEntry");
		case k_EResultValueOutOfRange:
			return String("k_EResultValueOutOfRange");
		case k_EResultUnexpectedError:
			return String("k_EResultUnexpectedError");
		case k_EResultDisabled:
			return String("k_EResultDisabled");
		case k_EResultInvalidCEGSubmission:
			return String("k_EResultInvalidCEGSubmission");
		case k_EResultRestrictedDevice:
			return String("k_EResultRestrictedDevice");
		case k_EResultRegionLocked:
			return String("k_EResultRegionLocked");
		case k_EResultRateLimitExceeded:
			return String("k_EResultRateLimitExceeded");
		case k_EResultAccountLoginDeniedNeedTwoFactor:
			return String("k_EResultAccountLoginDeniedNeedTwoFactor");
		case k_EResultItemDeleted:
			return String("k_EResultItemDeleted");
		case k_EResultAccountLoginDeniedThrottle:
			return String("k_EResultAccountLoginDeniedThrottle");
		case k_EResultTwoFactorCodeMismatch:
			return String("k_EResultTwoFactorCodeMismatch");
		case k_EResultTwoFactorActivationCodeMismatch:
			return String("k_EResultTwoFactorActivationCodeMismatch");
		case k_EResultAccountAssociatedToMultiplePartners:
			return String("k_EResultAccountAssociatedToMultiplePartners");
		case k_EResultNotModified:
			return String("k_EResultNotModified");
		case k_EResultNoMobileDevice:
			return String("k_EResultNoMobileDevice");
		case k_EResultTimeNotSynced:
			return String("k_EResultTimeNotSynced");
		case k_EResultSmsCodeFailed:
			return String("k_EResultSmsCodeFailed");
		case k_EResultAccountLimitExceeded:
			return String("k_EResultAccountLimitExceeded");
		case k_EResultAccountActivityLimitExceeded:
			return String("k_EResultAccountActivityLimitExceeded");
		case k_EResultPhoneActivityLimitExceeded:
			return String("k_EResultPhoneActivityLimitExceeded");
		case k_EResultRefundToWallet:
			return String("k_EResultRefundToWallet");
		case k_EResultEmailSendFailure:
			return String("k_EResultEmailSendFailure");
		case k_EResultNotSettled:
			return String("k_EResultNotSettled");
		case k_EResultNeedCaptcha:
			return String("k_EResultNeedCaptcha");
		case k_EResultGSLTDenied:
			return String("k_EResultGSLTDenied");
		case k_EResultGSOwnerDenied:
			return String("k_EResultGSOwnerDenied");
		case k_EResultInvalidItemType:
			return String("k_EResultInvalidItemType");
		case k_EResultIPBanned:
			return String("k_EResultIPBanned");
		case k_EResultGSLTExpired:
			return String("k_EResultGSLTExpired");
		case k_EResultInsufficientFunds:
			return String("k_EResultInsufficientFunds");
		case k_EResultTooManyPending:
			return String("k_EResultTooManyPending");
		case k_EResultNoSiteLicensesFound:
			return String("k_EResultNoSiteLicensesFound");
		case k_EResultWGNetworkSendExceeded:
			return String("k_EResultWGNetworkSendExceeded");
		case k_EResultAccountNotFriends:
			return String("k_EResultAccountNotFriends");
		case k_EResultLimitedUserAccount:
			return String("k_EResultLimitedUserAccount");
		case k_EResultCantRemoveItem:
			return String("k_EResultCantRemoveItem");
		case k_EResultAccountDeleted:
			return String("k_EResultAccountDeleted");
		case k_EResultExistingUserCancelledLicense:
			return String("k_EResultExistingUserCancelledLicense");
		case k_EResultCommunityCooldown:
			return String("k_EResultCommunityCooldown");
		case k_EResultNoLauncherSpecified:
			return String("k_EResultNoLauncherSpecified");
		case k_EResultMustAgreeToSSA:
			return String("k_EResultMustAgreeToSSA");
		case k_EResultLauncherMigrated:
			return String("k_EResultLauncherMigrated");
		case k_EResultSteamRealmMismatch:
			return String("k_EResultSteamRealmMismatch");
		case k_EResultInvalidSignature:
			return String("k_EResultInvalidSignature");
		case k_EResultParseFailure:
			return String("k_EResultParseFailure");
		case k_EResultNoVerifiedPhone:
			return String("k_EResultNoVerifiedPhone");
		case k_EResultInsufficientBattery:
			return String("k_EResultInsufficientBattery");
		case k_EResultChargerRequired:
			return String("k_EResultChargerRequired");
		case k_EResultCachedCredentialInvalid:
			return String("k_EResultCachedCredentialInvalid");
		case K_EResultPhoneNumberIsVOIP:
			return String("K_EResultPhoneNumberIsVOIP");
	}
	return "Unmatched";
}
