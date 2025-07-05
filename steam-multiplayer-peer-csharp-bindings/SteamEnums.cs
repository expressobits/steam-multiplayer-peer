namespace SteamMultiplayerPeerCSharp;

public enum SteamNetworkingConfig : long
{
    // Invalid
    Invalid = 0, // k_ESteamNetworkingConfig_Invalid

    // Simulating network conditions
    FakePacketLossSend               = 2,  // k_ESteamNetworkingConfig_FakePacketLoss_Send
    FakePacketLossRecv               = 3,  // k_ESteamNetworkingConfig_FakePacketLoss_Recv
    FakePacketLagSend                = 4,  // k_ESteamNetworkingConfig_FakePacketLag_Send
    FakePacketLagRecv                = 5,  // k_ESteamNetworkingConfig_FakePacketLag_Recv
    FakePacketReorderSend            = 6,  // k_ESteamNetworkingConfig_FakePacketReorder_Send
    FakePacketReorderRecv            = 7,  // k_ESteamNetworkingConfig_FakePacketReorder_Recv
    FakePacketReorderTime            = 8,  // k_ESteamNetworkingConfig_FakePacketReorder_Time
    FakePacketDupSend                = 26, // k_ESteamNetworkingConfig_FakePacketDup_Send
    FakePacketDupRecv                = 27, // k_ESteamNetworkingConfig_FakePacketDup_Recv
    FakePacketDupTimeMax             = 28, // k_ESteamNetworkingConfig_FakePacketDup_TimeMax
    PacketTraceMaxBytes              = 41, // k_ESteamNetworkingConfig_PacketTraceMaxBytes
    FakeRateLimitSendRate            = 42, // k_ESteamNetworkingConfig_FakeRateLimit_Send_Rate
    FakeRateLimitSendBurst           = 43, // k_ESteamNetworkingConfig_FakeRateLimit_Send_Burst
    FakeRateLimitRecvRate            = 44, // k_ESteamNetworkingConfig_FakeRateLimit_Recv_Rate
    FakeRateLimitRecvBurst           = 45, // k_ESteamNetworkingConfig_FakeRateLimit_Recv_Burst
    OutOfOrderCorrectionWindowMicroseconds = 51, // k_ESteamNetworkingConfig_OutOfOrderCorrectionWindowMicroseconds

    // Connection options
    ConnectionUserData               = 40, // k_ESteamNetworkingConfig_ConnectionUserData
    TimeoutInitial                   = 24, // k_ESteamNetworkingConfig_TimeoutInitial
    TimeoutConnected                 = 25, // k_ESteamNetworkingConfig_TimeoutConnected
    SendBufferSize                   = 9,  // k_ESteamNetworkingConfig_SendBufferSize
    RecvBufferSize                   = 47, // k_ESteamNetworkingConfig_RecvBufferSize
    RecvBufferMessages               = 48, // k_ESteamNetworkingConfig_RecvBufferMessages
    RecvMaxMessageSize               = 49, // k_ESteamNetworkingConfig_RecvMaxMessageSize
    RecvMaxSegmentsPerPacket         = 50, // k_ESteamNetworkingConfig_RecvMaxSegmentsPerPacket
    SendRateMin                      = 10, // k_ESteamNetworkingConfig_SendRateMin
    SendRateMax                      = 11, // k_ESteamNetworkingConfig_SendRateMax
    NagleTime                        = 12, // k_ESteamNetworkingConfig_NagleTime
    IPAllowWithoutAuth               = 23, // k_ESteamNetworkingConfig_IP_AllowWithoutAuth
    IPLocalHostAllowWithoutAuth      = 52, // k_ESteamNetworkingConfig_IPLocalHost_AllowWithoutAuth
    MTUPacketSize                    = 32, // k_ESteamNetworkingConfig_MTU_PacketSize
    MTUDataSize                      = 33, // k_ESteamNetworkingConfig_MTU_DataSize
    Unencrypted                      = 34, // k_ESteamNetworkingConfig_Unencrypted
    SymmetricConnect                 = 37, // k_ESteamNetworkingConfig_SymmetricConnect
    LocalVirtualPort                 = 38, // k_ESteamNetworkingConfig_LocalVirtualPort
    DualWifiEnable                   = 39, // k_ESteamNetworkingConfig_DualWifi_Enable
    EnableDiagnosticsUI              = 46, // k_ESteamNetworkingConfig_EnableDiagnosticsUI

    // SDR settings
    SDRClientConsecutivePingTimeoutsFailInitial = 19, // k_ESteamNetworkingConfig_SDRClient_ConsecutitivePingTimeoutsFailInitial
    SDRClientConsecutivePingTimeoutsFail        = 20, // k_ESteamNetworkingConfig_SDRClient_ConsecutitivePingTimeoutsFail
    SDRClientMinPingsBeforePingAccurate         = 21, // k_ESteamNetworkingConfig_SDRClient_MinPingsBeforePingAccurate
    SDRClientSingleSocket                       = 22, // k_ESteamNetworkingConfig_SDRClient_SingleSocket
    SDRClientForceRelayCluster                  = 29, // k_ESteamNetworkingConfig_SDRClient_ForceRelayCluster
    SDRClientDevTicket                          = 30, // k_ESteamNetworkingConfig_SDRClient_DevTicket
    SDRClientForceProxyAddr                     = 31, // k_ESteamNetworkingConfig_SDRClient_ForceProxyAddr
    SDRClientFakeClusterPing                    = 36, // k_ESteamNetworkingConfig_SDRClient_FakeClusterPing
    SDRClientLimitPingProbesToNearestN          = 60, // k_ESteamNetworkingConfig_SDRClient_LimitPingProbesToNearestN

    // Log levels
    LogLevelAckRTT           = 13, // k_ESteamNetworkingConfig_LogLevel_AckRTT
    LogLevelPacketDecode     = 14, // k_ESteamNetworkingConfig_LogLevel_PacketDecode
    LogLevelMessage          = 15, // k_ESteamNetworkingConfig_LogLevel_Message
    LogLevelPacketGaps       = 16, // k_ESteamNetworkingConfig_LogLevel_PacketGaps
    LogLevelP2PRendezvous    = 17, // k_ESteamNetworkingConfig_LogLevel_P2PRendezvous
    LogLevelSDRRelayPings    = 18, // k_ESteamNetworkingConfig_LogLevel_SDRRelayPings

    // Callbacks
    CallbackConnectionStatusChanged = 201, // k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged
    CallbackAuthStatusChanged       = 202, // k_ESteamNetworkingConfig_Callback_AuthStatusChanged
    CallbackRelayNetworkStatusChanged = 203, // k_ESteamNetworkingConfig_Callback_RelayNetworkStatusChanged
    CallbackMessageSessionRequest   = 204, // k_ESteamNetworkingConfig_Callback_MessagesSessionRequest
    CallbackMessagesSessionFailed   = 205, // k_ESteamNetworkingConfig_Callback_MessagesSessionFailed
    CallbackCreateConnectionSignaling = 206, // k_ESteamNetworkingConfig_Callback_CreateConnectionSignaling
    CallbackFakeIPResult            = 207, // k_ESteamNetworkingConfig_Callback_FakeIPResult

    // P2P settings
    P2PStunServerList          = 103, // k_ESteamNetworkingConfig_P2P_STUN_ServerList
    P2PTransportIceEnable      = 104, // k_ESteamNetworkingConfig_P2P_Transport_ICE_Enable
    P2PTransportIcePenalty     = 105, // k_ESteamNetworkingConfig_P2P_Transport_ICE_Penalty
    P2PTransportSdrPenalty     = 106, // k_ESteamNetworkingConfig_P2P_Transport_SDR_Penalty
    P2PTurnServerList          = 107, // k_ESteamNetworkingConfig_P2P_TURN_ServerList
    P2PTurnUserList            = 108, // k_ESteamNetworkingConfig_P2P_TURN_UserList
    P2PTurnPassList            = 109, // k_ESteamNetworkingConfig_P2P_TURN_PassList
    P2PTransportIceImplementation = 110, // k_ESteamNetworkingConfig_P2P_Transport_ICE_Implementation

    // Experimental / other
    ECN            = 999,          // k_ESteamNetworkingConfig_ECN
    Force32Bit     = 0x7fffffff,  // k_ESteamNetworkingConfig_Value_Force32Bit
}

public enum SocketConnectionType : long
{
    NotConnected = 0, // k_ESNetSocketConnectionTypeNotConnected
    UDP          = 1, // k_ESNetSocketConnectionTypeUDP
    UDPRelay     = 2, // k_ESNetSocketConnectionTypeUDPRelay
}

public enum SocketState : long
{
    Invalid                  = 0,  // k_ESNetSocketStateInvalid
    Connected                = 1,  // k_ESNetSocketStateConnected
    Initiated                = 10, // k_ESNetSocketStateInitiated
    LocalCandidatesFound     = 11, // k_ESNetSocketStateLocalCandidatesFound
    ReceivedRemoteCandidates = 12, // k_ESNetSocketStateReceivedRemoteCandidates
    ChallengeHandshake       = 15, // k_ESNetSocketStateChallengeHandshake
    Disconnecting            = 21, // k_ESNetSocketStateDisconnecting
    LocalDisconnect          = 22, // k_ESNetSocketStateLocalDisconnect
    TimeoutDuringConnect     = 23, // k_ESNetSocketStateTimeoutDuringConnect
    RemoteEndDisconnected    = 24, // k_ESNetSocketStateRemoteEndDisconnected
    ConnectionBroken         = 25, // k_ESNetSocketStateConnectionBroken
}

public enum NetworkingConnectionState : long
{
    None                   = 0, // k_ESteamNetworkingConnectionState_None
    Connecting             = 1, // k_ESteamNetworkingConnectionState_Connecting
    FindingRoute           = 2, // k_ESteamNetworkingConnectionState_FindingRoute
    Connected              = 3, // k_ESteamNetworkingConnectionState_Connected
    ClosedByPeer           = 4, // k_ESteamNetworkingConnectionState_ClosedByPeer
    ProblemDetectedLocally = 5, // k_ESteamNetworkingConnectionState_ProblemDetectedLocally
    FinWait                = -1, // k_ESteamNetworkingConnectionState_FinWait
    Linger                 = -2, // k_ESteamNetworkingConnectionState_Linger
    Dead                   = -3, // k_ESteamNetworkingConnectionState_Dead
    Force32Bit             = 0x7fffffff,
}
