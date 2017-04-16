
#ifndef RP1210_h__
#define RP1210_h__

// 2017/04/16 : ZH :  RP1210协议头文件，定义协议文档中规定的数据

#include <windows.h>

//-----------------------------------------------------------------------------------------------------
// RP1210   RP1210_SendCommand Defines ( From RP1210 Document )
//-----------------------------------------------------------------------------------------------------

typedef unsigned char U8;

#define RP1210_Reset_Device                            0
#define RP1210_Set_All_Filters_States_to_Pass          3
#define RP1210_Set_Message_Filtering_For_J1939         4
#define RP1210_Set_Message_Filtering_For_CAN           5
#define RP1210_Set_Message_Filtering_For_J1708         7
#define RP1210_Set_Message_Filtering_For_J1850         8
#define RP1210_Set_Message_Filtering_For_ISO15765      9
#define RP1210_Generic_Driver_Command                 14
#define RP1210_Set_J1708_Mode                         15
#define RP1210_Echo_Transmitted_Messages              16
#define RP1210_Set_All_Filters_States_to_Discard      17
#define RP1210_Set_Message_Receive                    18
#define RP1210_Protect_J1939_Address                  19
#define RP1210_Set_Broadcast_For_J1708                20
#define RP1210_Set_Broadcast_For_CAN                  21
#define RP1210_Set_Broadcast_For_J1939                22
#define RP1210_Set_Broadcast_For_J1850                23
#define RP1210_Set_J1708_Filter_Type                  24
#define RP1210_Set_J1939_Filter_Type                  25
#define RP1210_Set_CAN_Filter_Type                    26
#define RP1210_Set_J1939_Interpacket_Time             27
#define RP1210_SetMaxErrorMsgSize                     28
#define RP1210_Disallow_Further_Connections           29
#define RP1210_Set_J1850_Filter_Type                  30
#define RP1210_Release_J1939_Address                  31
#define RP1210_Set_ISO15765_Filter_Type               32
#define RP1210_Set_Broadcast_For_ISO15765             33
#define RP1210_Set_ISO15765_Flow_Control              34
#define RP1210_Clear_ISO15765_Flow_Control            35
#define RP1210_Set_ISO15765_Link_Type                 36
#define RP1210_Set_J1939_Baud                         37
#define RP1210_Set_ISO15765_Baud                      38
#define RP1210_Set_BlockTimeout                      215
#define RP1210_Set_J1708_Baud                        305

//-----------------------------------------------------------------------------------------------------
// RP1210 Constants - Check RP1210 document for any updates.
//-----------------------------------------------------------------------------------------------------

#define BIT0                                           1  // Bit 0 - Used for masking bits
#define BIT1                                           2  // Bit 1 - Used for masking bits
#define BIT2                                           4  // Bit 2 - Used for masking bits
#define BIT3                                           8  // Bit 3 - Used for masking bits
#define BIT4                                          16  // Bit 4 - Used for masking bits
#define BIT5                                          32  // Bit 5 - Used for masking bits
#define BIT6                                          64  // Bit 6 - Used for masking bits
#define BIT7                                         128  // Bit 7 - Used for masking bits

#define CONNECTED                                      1  // Connection state = Connected 
#define NOT_CONNECTED                                 -1  // Connection state = Disconnected

#define FILTER_PASS_NONE                               0  // Filter state = DISCARD ALL MESSAGES
#define FILTER_PASS_SOME                               1  // Filter state = PASS SOME
#define FILTER_PASS_ALL                                2  // Filter state = PASS ALL

#define NULL_WINDOW                                    0  // Windows 3.1 is no longer supported.

#define BLOCKING_IO                                    1  // For Blocking calls to send/read.
#define NON_BLOCKING_IO                                0  // For Non-Blocking calls to send/read.
#define BLOCK_INFINITE                                 0  // For Non-Blocking calls to send/read.

#define BLOCK_UNTIL_DONE                               0  // J1939 Address claim, wait until done
#define RETURN_BEFORE_COMPLETION                       2  // J1939 Address claim, don't wait

#define CONVERTED_MODE                                 1  // J1708 RP1210Mode="Converted"
#define RAW_MODE                                       0  // J1708 RP1210Mode="Raw"

//4/14/2017 ZH :消息最大长度
#define MAX_J1708_MESSAGE_LENGTH                     508  // Maximum size of J1708 message (+1)
#define MAX_J1939_MESSAGE_LENGTH                    1796  // Maximum size of J1939 message (+1)
#define MAX_ISO15765_MESSAGE_LENGTH                 4108  // Maximum size of ISO15765 message (+1)

#define ECHO_OFF                                    0x00  // EchoMode
#define ECHO_ON                                     0x01  // EchoMode

#define RECEIVE_ON                                  0x01  // Set Message Receive
#define RECEIVE_OFF                                 0x00  // Set Message Receive

#define ADD_LIST                                    0x01  // Add a message to the list.
#define VIEW_B_LIST                                 0x02  // View an entry in the list.
#define DESTROY_LIST                                0x03  // Remove all entries in the list.
#define REMOVE_ENTRY                                0x04  // Remove a specific entry from the list.
#define LIST_LENGTH                                 0x05  // Returns number of items in list.

//4/14/2017 ZH :J1939过滤器相关
#define FILTER_PGN                            0x00000001  // Setting of J1939 filters
#define FILTER_PRIORITY                       0x00000002  // Setting of J1939 filters
#define FILTER_SOURCE                         0x00000004  // Setting of J1939 filters
#define FILTER_DESTINATION                    0x00000008  // Setting of J1939 filters
#define FILTER_INCLUSIVE                            0x00  // FilterMode
#define FILTER_EXCLUSIVE                            0x01  // FilterMode

//4/14/2017 ZH :J1939地址相关
#define SILENT_J1939_CLAIM                          0x00  // Claim J1939 Address
#define PASS_J1939_CLAIM_MESSAGES                   0x01  // Claim J1939 Address

//4/14/2017 ZH :波特率相关
#define CHANGE_BAUD_NOW                             0x00  // Change Baud
#define MSG_FIRST_CHANGE_BAUD                       0x01  // Change Baud
#define RP1210_BAUD_9600                            0x00  // Change Baud
#define RP1210_BAUD_19200                           0x01  // Change Baud
#define RP1210_BAUD_38400                           0x02  // Change Baud
#define RP1210_BAUD_57600                           0x03  // Change Baud
#define RP1210_BAUD_125k                            0x04  // Change Baud
#define RP1210_BAUD_250k                            0x05  // Change Baud
#define RP1210_BAUD_500k                            0x06  // Change Baud
#define RP1210_BAUD_1000k                           0x07  // Change Baud

//4/14/2017 ZH :CAN和ISO15765相关
#define STANDARD_CAN                                0x00  // Filters
#define EXTENDED_CAN                                0x01  // Filters
#define STANDARD_CAN_ISO15765_EXTENDED              0x02  // 11-bit with ISO15765 extended address
#define EXTENDED_CAN_ISO15765_EXTENDED              0x03  // 29-bit with ISO15765 extended address
#define STANDARD_MIXED_CAN_ISO15765                 0x04  // 11-bit identifier with mixed addressing
#define ISO15765_ACTUAL_MESSAGE                     0x00  // ISO15765 ReadMessage - type of data
#define ISO15765_CONFIRM                            0x01  // ISO15765 ReadMessage - type of data
#define ISO15765_FF_INDICATION                      0x02  // ISO15765 ReadMessage - type of data

#define LINKTYPE_GENERIC_CAN                        0x00  // Set_ISO15765_Link_Type argument
#define LINKTYPE_J1939_ISO15765_2_ANNEX_A           0x01  // Set_ISO15765_Link_Type argument
#define LINKTYPE_J1939_ISO15765_3                   0x02  // Set_ISO15765_Link_Type argument

#ifndef TRUE
#define TRUE                                        0x01
#define FALSE                                       0x00
#endif

//-----------------------------------------------------------------------------------------------------
// RP1210 Return Definitions 返回值定义
//-----------------------------------------------------------------------------------------------------

#define NO_ERRORS                                      0
#define ERR_DLL_NOT_INITIALIZED                      128
#define ERR_INVALID_CLIENT_ID                        129
#define ERR_CLIENT_ALREADY_CONNECTED                 130
#define ERR_CLIENT_AREA_FULL                         131
#define ERR_FREE_MEMORY                              132
#define ERR_NOT_ENOUGH_MEMORY                        133
#define ERR_INVALID_DEVICE                           134
#define ERR_DEVICE_IN_USE                            135
#define ERR_INVALID_PROTOCOL                         136
#define ERR_TX_QUEUE_FULL                            137
#define ERR_TX_QUEUE_CORRUPT                         138
#define ERR_RX_QUEUE_FULL                            139
#define ERR_RX_QUEUE_CORRUPT                         140
#define ERR_MESSAGE_TOO_LONG                         141
#define ERR_HARDWARE_NOT_RESPONDING                  142
#define ERR_COMMAND_NOT_SUPPORTED                    143
#define ERR_INVALID_COMMAND                          144
#define ERR_TXMESSAGE_STATUS                         145
#define ERR_ADDRESS_CLAIM_FAILED                     146
#define ERR_CANNOT_SET_PRIORITY                      147
#define ERR_CLIENT_DISCONNECTED                      148
#define ERR_CONNECT_NOT_ALLOWED                      149
#define ERR_CHANGE_MODE_FAILED                       150
#define ERR_BUS_OFF                                  151
#define ERR_COULD_NOT_TX_ADDRESS_CLAIMED             152
#define ERR_ADDRESS_LOST                             153
#define ERR_CODE_NOT_FOUND                           154
#define ERR_BLOCK_NOT_ALLOWED                        155
#define ERR_MULTIPLE_CLIENTS_CONNECTED               156
#define ERR_ADDRESS_NEVER_CLAIMED                    157
#define ERR_WINDOW_HANDLE_REQUIRED                   158
#define ERR_MESSAGE_NOT_SENT                         159
#define ERR_MAX_NOTIFY_EXCEEDED                      160
#define ERR_MAX_FILTERS_EXCEEDED                     161
#define ERR_HARDWARE_STATUS_CHANGE                   162
#define ERR_FIRMWARE_BEING_UPDATED                   193
#define ERR_INI_FILE_NOT_IN_WIN_DIR                  202
#define ERR_INI_SECTION_NOT_FOUND                    204
#define ERR_INI_KEY_NOT_FOUND                        205
#define ERR_INVALID_KEY_STRING                       206
#define ERR_DEVICE_NOT_SUPPORTED                     207
#define ERR_INVALID_PORT_PARAM                       208
#define ERR_COMMAND_TIMED_OUT                        213
#define ERR_OS_NOT_SUPPORTED                         220
#define ERR_COMMAND_QUEUE_IS_FULL                    222
#define ERR_CANNOT_SET_CAN_BAUDRATE                  224
#define ERR_CANNOT_CLAIM_BROADCAST_ADDRESS           225
#define ERR_OUT_OF_ADDRESS_RESOURCES                 226
#define ERR_ADDRESS_RELEASE_FAILED                   227
#define ERR_COMM_DEVICE_IN_USE                       230
#define ERR_DATA_LINK_CONFLICT                       441
#define ERR_ADAPTER_NOT_RESPONDING                   453
#define ERR_CAN_BAUD_SET_NONSTANDARD                 454
#define ERR_MULTIPLE_CONNECTIONS_NOT_ALLOWED_NOW     455
#define ERR_J1708_BAUD_SET_NONSTANDARD               456
#define ERR_J1939_BAUD_SET_NONSTANDARD               457
#define ERR_ISO15765_BAUD_SET_NONSTANDARD            458

//-----------------------------------------------------------------------------------------------------
// Used to help in unpacking bytes from unsigned 2 and 4-byte integer values.  用来帮助解析消息的各种宏~~
//-----------------------------------------------------------------------------------------------------

#define HIWORD_OF_INT4( x )   ( ( x >> 16 ) & 0xFFFF )
#define LOWORD_OF_INT4( x )   ( x & 0x0000FFFF )
#define HIBYTE_OF_WORD( x )   ( ( x >> 8 ) & 0xFF ) 
#define LOBYTE_OF_WORD( x )   ( x & 0x00FF ) 
#define HINIBBLE_OF_CHAR( x ) ( ( x & 0xF0 ) >> 4 )
#define LONIBBLE_OF_CHAR( x ) (   x & 0x0F ) 
#define BYTE0_OF_INT4( x )    LOBYTE_OF_WORD( LOWORD_OF_INT4( x ) )
#define BYTE1_OF_INT4( x )    HIBYTE_OF_WORD( LOWORD_OF_INT4( x ) )
#define BYTE2_OF_INT4( x )    LOBYTE_OF_WORD( HIWORD_OF_INT4( x ) )
#define BYTE3_OF_INT4( x )    HIBYTE_OF_WORD( HIWORD_OF_INT4( x ) )

//-----------------------------------------------------------------------------------------------------
// RP1210 Defined Function Prototypes  导出函数声明
//-----------------------------------------------------------------------------------------------------

#define DLLEXPORT __declspec( dllexport )

#ifdef __cplusplus
extern "C" {
#endif

	short DLLEXPORT WINAPI RP1210_ClientConnect(
		HWND   hwndClient,
		short  nDeviceId,
		char  *fpchProtocol,
		long   lSendBuffer,
		long   lReceiveBuffer,
		short  nIsAppPacketizingIncomingMsgs
	);

	short DLLEXPORT WINAPI RP1210_ClientDisconnect(
		short  nClientID
	);

	short DLLEXPORT WINAPI RP1210_SendMessage(
		short  nClientID,
		char  *fpchClientMessage,
		short  nMessageSize,
		short  nNotifyStatusOnTx,
		short  nBlockOnSend
	);

	short DLLEXPORT WINAPI RP1210_ReadMessage(
		short  nClientID,
		char  *fpchAPIMessage,
		short  nBufferSize,
		short  nBlockOnSend
	);

	short DLLEXPORT WINAPI RP1210_SendCommand(
		short  nCommandNumber,
		short  nClientID,
		char  *fpchClientCommand,
		short  nMessageSize
	);

	void  DLLEXPORT WINAPI RP1210_ReadVersion(
		char  *fpchDLLMajorVersion,
		char  *fpchDLLMinorVersion,
		char  *fpchAPIMajorVersion,
		char  *fpchAPIMinorVersion
	);

	short  DLLEXPORT WINAPI RP1210_ReadDetailedVersion(
		short  nClientID,
		char  *fpchAPIVersionInfo,
		char  *fpchDLLVersionInfo,
		char  *fpchFWVersionInfo
	);

	short DLLEXPORT WINAPI RP1210_GetHardwareStatus(
		short  nClientID,
		char  *fpchClientInfo,
		short  nInfoSize,
		short  nBlockOnRequest
	);

	short DLLEXPORT WINAPI RP1210_GetErrorMsg(
		short  err_code,
		char  *fpchMessage
	);

	short DLLEXPORT WINAPI RP1210_GetLastErrorMsg(
		short  err_code,
		int   *SubErrorCode,
		char  *fpchDescription,
		short  nClientID
	);

#ifdef __cplusplus
}
#endif

//-----------------------------------------------------------------------------------------------------
// RP1210 Function Type Definitions      函数指针类型定义
//----------------------------------------------------------------------------------------------------- 
typedef short (WINAPI *fxRP1210_ClientConnect)(HWND, short, char *, long, long, short);
typedef short (WINAPI *fxRP1210_ClientDisconnect)(short);
typedef short (WINAPI *fxRP1210_SendMessage)(short, char *, short, short, short);
typedef short (WINAPI *fxRP1210_ReadMessage)(short, char *, short, short);
typedef short (WINAPI *fxRP1210_SendCommand)(short, short, char *, short);
typedef short (WINAPI *fxRP1210_ReadVersion)(char *, char *, char *, char *);
typedef short (WINAPI *fxRP1210_ReadDetailedVersion)(short, char *, char *, char *);
typedef short (WINAPI *fxRP1210_GetHardwareStatus)(short, char *, short, short);
typedef short (WINAPI *fxRP1210_GetErrorMsg)(short, char *);
typedef short (WINAPI *fxRP1210_GetLastErrorMsg)(short, int  *, char *, short);

#endif // RP1210_h__