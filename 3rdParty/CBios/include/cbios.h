/*****************************************************************************
  Copyright:   : MARX(R) CryptoTech LP
  Project      : CBIOS: CRYPTO-BOX Input/Output System API
  Created      : 25Feb2003                                    
  Tools        : MSVC++ 6.0 SP5                               
  Description  : CBIOS API definitions
******************************************************************************
  VER         DATE        WHO   DETAIL                        
******************************************************************************
  1.00        25Feb2003   DI    Initial version (MPCB)
  1.10        31May2003   VS    CBIOS project started
  1.10        18Jun2003   VS    
  1.20        21Mar2005   VS    CT-M2048 limited support added
  1.30        29Aug2005   VS    CBIOS_GetDeveloperID
*****************************************************************************/

#ifndef _CBIOS_H_
#define _CBIOS_H_

#define CBIOS_MAX_BOXES       32
#define CBIOS_MAX_APPS        32

#define CBIOS_APPID_SYSTEM         0
#define CBIOS_APPID_SPO            1
#define CBIOS_APPID_XENODISC       2
#define CBIOS_APPID_GINA           3
#define CBIOS_APPID_AUTOPROTECTION 4
#define CBIOS_APPID_LMS            5
#define CBIOS_APPID_PDFPROTECTION  6                            
#define CBIOS_APPID_CSP            7
#define CBIOS_APPID_PKCS11         8
#define CBIOS_APPID_PKCS11_LIGHT   9
#define CBIOS_APPID_PRJ_EDITIONS  10

#define CBIOS_LABEL_LEN 8

#define CBIOS_WARNING_MASK             0x80        // nonfatal error codes mask
//CBU error codes
#define CBIOS_SUCCESS                  0x00        // no error
#define CBIOS_ERR_NOT_FOUND            0x01        // No box with BoxName
#define CBIOS_ERR_NOT_LOGGED           0x02        // Missing UPW login
#define CBIOS_ERR_WRONG_PARAM          0x03        // Bad input parameter
#define CBIOS_ERR_RAM1_NOT_EXIST       0x04        // Range of RAM1 incorect
#define CBIOS_ERR_RAM2_NOT_EXIST       0x05        // ...
#define CBIOS_ERR_RAM3_NOT_EXIST       0x06        // ...
#define CBIOS_ERR_BOX_IS_LOCKED        0x07        // Box used another application
#define CBIOS_ERR_EXT_ERROR            0x08                        // see SM_Last_Error
#define CBIOS_ERR_NO_MEMORY            0x09                  // allocation error
//CBIOS error codes
#define CBIOS_ERR_BOX_NOT_OPENED       0x21        // attempt to access the box, 
                                                   //  which wasn't opened with CBIOS_OpenBox call
#define CBIOS_ERR_BOX_ALREADY_OPENED   0x22        // attempt to open box repeatedly
//#define CBIOS_ERR_NO_RAM               0x23        // (obsolete) memory allocation error (computer, not box RAM)
#define CBIOS_ERR_TOO_MANY_BOXES_OPEN  0x23        // (!has same code as CBIOS_ERR_NO_RAM!) opened boxes table is full
#define CBIOS_ERR_BOX_NOT_FOUND        0x24
#define CBIOS_ERR_APP_NOT_FOUND        0x25
#define CBIOS_ERR_BOX_NOT_SUPPORTED    0x26        // not supported firmware version 1.5 or earlier
#define CBIOS_ERR_OUT_OF_RANGE         0x27        // invalid index, address or block size
#define CBIOS_ERR_BOX_NOT_FORMATTED    0x28        
#define CBIOS_ERR_CORRUPTED_CBIOS_DATA 0x29        
#define CBIOS_ERR_NO_SPACE_IN_RAM1     0x2A        
#define CBIOS_ERR_NO_SPACE_IN_RAM2     0x2B        
#define CBIOS_ERR_NO_SPACE_IN_RAM3     0x2C        
#define CBIOS_ERR_APP_ALREADY_EXISTS   0x2D
#define CBIOS_ERR_BOX_ALREADY_LOGGED   0x2E        
#define CBIOS_ERR_BOX_NOT_LOGGED       0x2F        
#define CBIOS_ERR_EXTENDED_MODE        0x30        // some application use extended mode
#define CBIOS_ERR_NOT_EXTENDED_MODE    0x31        // can't execute extended function in standart mode
#define CBIOS_ERR_LOCK_TIMEOUT         0x32        // box locked more then 20 sec
#define CBIOS_ERR_WRONG_SIGNATURE      0x33        // CRC error while decrypt box signature
#define CBIOS_ERR_WRONG_KEY            0x34        // wrong internal RSA key
#define CBIOS_ERR_NOT_INITIALIZED      0x35        // CBIOS not initialized (CBIOS_Startup was not completed correctly)
#define CBIOS_ERR_NOT_LICENCED         0x36        // box not licenced for application
#define CBIOS_ERR_NO_SPACE             0x37        // no free space in box for operation
#define CBIOS_ERR_CRC_ERROR            0x38        
#define CBIOS_ERR_NO_FREE_LICENCE      0x39        // no free licence - can't open application
#define CBIOS_ERR_INVALID_HANDLE       0x3A
#define CBIOS_WRN_INIT_BOX_NOTIFIC    (0x3B | CBIOS_WARNING_MASK) // NONFATAL ERROR: box insert notifications subsystem is not initialized correctly,
                                                   //   but CBIOS run ok. Your need to call CBIOS_Finish if that error received from CBIOS_Startup
#define CBIOS_WRN_CTM_NOT_INITIALIZED (0x3C | CBIOS_WARNING_MASK) // NONFATAL ERROR: CTM2048 subsystem is not initialized correctly,
                                                   //   but CBIOS run ok. Your need to call CBIOS_Finish if that error received from CBIOS_Startup
#define CBIOS_ERR_NOT_IMPLEMENTED      0x40

#define CBIOS_ERR_CONN_REFUSED         0x60
#define CBIOS_ERR_WRONG_SRV_PASSWD     0x61
#define CBIOS_ERR_HANDSHAKE            0x62
#define CBIOS_ERR_NOT_CONNECTED        0x63
#define CBIOS_ERR_ALREADY_CONNECTED    0x64
#define CBIOS_ERR_TOO_MANY_CONNECTIONS 0x65

// CBIOS_BOX_INFO.bLogin
#define CBIOS_UPW_LOGIN       0x01              // Flags from function SM_GetSecurityFlags
#define CBIOS_APW_LOGIN       0x02              // ...
#define CBIOS_PPW_LOGIN       0x04              // ...
#define CBIOS_ERROR_LOGIN     0x80              // ...

#define CBIOS_RAM1            0x01    
#define CBIOS_RAM2            0x02    
#define CBIOS_RAM3            0x03    

#define CBIOS_CT_FLAG 0x10
#define CBIOS_CB_MODEL_LEGACY   0x00 // old box
#define CBIOS_CB_MODEL_VERSA    0x01 // CRYPTO-BOX(r) VERSA
#define CBIOS_CB_MODEL_XS       0x02 // CRYPTO-BOX(r) XS
#define CBIOS_CB_MODEL_XL       0x03 // CRYPTO-BOX(r) XL
#define CBIOS_CT_MODEL_XS       0x12 // CrypToken(r) XS
#define CBIOS_CT_MODEL_XL       0x13 // CrypToken(r) XL
#define CBIOS_CT_MODEL_2048     0x20 // CT2048

// internal RSA
#define CBIOS_INTERNAL_RSA_BITS 1024
#define CBIOS_INTERNAL_RSA_LEN  (CBIOS_INTERNAL_RSA_BITS/8)
#define CBIOS_INTERNAL_RSA_ID_DISTR_PUBL    1
#define CBIOS_INTERNAL_RSA_ID_CLIENT_PRIV  2

// box signature
#define CBIOS_BOXSIGN_RSA_BITS  1024
#define CBIOS_BOXSIGN_RSA_LEN  (CBIOS_BOXSIGN_RSA_BITS/8)
#define CBIOS_BOXSIGN_RSA_PUBLIC_MODULUS {0xb5, 0xf5, 0x83, 0x2d, 0x13, 0x51, 0x67, 0x76,  0xc0, 0x94, 0x08, 0x31, 0x07, 0xdd, 0x65, 0x4c,  0x6c, 0xfc, 0x28, 0x0f, 0x1e, 0xfc, 0xfb, 0x0f,  0x86, 0x7d, 0x5d, 0x79, 0x01, 0x4e, 0x2b, 0xb6,  0xd8, 0xe6, 0xfc, 0x22, 0x53, 0xeb, 0x8d, 0x7d,  0xe4, 0x75, 0xc5, 0xa5, 0xf1, 0x5f, 0xd2, 0xb5,  0x07, 0xf6, 0x08, 0xa9, 0x73, 0x73, 0x05, 0x58,  0x82, 0xb0, 0xbd, 0x63, 0x10, 0xe4, 0x27, 0xd2,  0xb9, 0x2f, 0x5f, 0xc2, 0x29, 0xb3, 0xa6, 0x4c,  0x60, 0x43, 0xc2, 0xa9, 0x2b, 0xd3, 0x3e, 0xdd,  0xa1, 0xe7, 0xb2, 0x61, 0x38, 0xe5, 0x62, 0x12,  0xde, 0x62, 0xb9, 0xd2, 0x34, 0xf0, 0x71, 0xd6,  0x9a, 0x7d, 0xb7, 0x36, 0x3f, 0x73, 0x48, 0xc0,  0x33, 0xea, 0x1f, 0xbf, 0x28, 0x1d, 0xf1, 0x07,  0x0b, 0xa5, 0x41, 0x6c, 0x8c, 0x52, 0x26, 0x2d,  0x82, 0x31, 0x28, 0xf9, 0x3c, 0x0d, 0x0b, 0x5f};

#pragma pack(1)

typedef struct
{ 
  DWORD dwBoxName;
  DWORD dwModel;
  DWORD dwApp;          // application ID or applications qty
  WORD  wCBIOS;         // CBIOS version. 0 - non CBIOS box. 
  BYTE  bAppPolicy;     // Application Policy
  BYTE  bLogin;         // login status from cbusb_flag structure
  BYTE  bHiVersion;
  BYTE  bLoVersion;
  DWORD dwRAMLen;       // in bytes
  DWORD dwSize_RAM1;    // Size in RAM1
  DWORD dwSize_RAM2;    // Size in RAM2
  DWORD dwSize_RAM3;    // Size in RAM3
  BYTE  bBoxLabel[CBIOS_LABEL_LEN];      // Box label
} CBIOS_BOX_INFO;

typedef struct
{
  DWORD dwStructSize;
  DWORD dwBoxName;
  DWORD dwModel;
  DWORD dwApp;          // application ID or applications qty
  WORD  wCBIOS;         // CBIOS version. 0 - non CBIOS box. 
  BYTE  bAppPolicy;     // Application Policy
  BYTE  bLogin;         // login status from cbusb_flag structure
  BYTE  bHiVersion;
  BYTE  bLoVersion;
  DWORD dwRAMLen;       // in bytes
  DWORD dwSize_RAM1;    // Size in RAM1
  DWORD dwSize_RAM2;    // Size in RAM2
  DWORD dwSize_RAM3;    // Size in RAM3
  BYTE  bBoxLabel[CBIOS_LABEL_LEN];      // Box label
  DWORD dwDeveloperID;   // Developer ID
  BYTE  bMPI;           // Box is MPI formatted
} CBIOS_BOX_INFO_ADV;

typedef struct
{
  WORD  wAppID;                // APPID
  WORD  wRAM1Size;             // Size in RAM1
  WORD  wRAM2Size;             // Size in RAM2
  WORD  wRAM3Size;             // Size in RAM3
} CBIOS_APP_INFO;

#define CBIOS_BOX_ATTACHED 1
#define CBIOS_BOX_REMOVED  2

typedef struct
{
  void* pNotificationParam;
  DWORD dwNotificationID;
  DWORD dwNotificationType;
} CBIOS_NOTIFY_DATA;

//RSA KEY definition
#define CBIOS_MAX_RSA_MODULUS_BITS 4096
#define CBIOS_MAX_RSA_MODULUS_LEN ((CBIOS_MAX_RSA_MODULUS_BITS + 7) / 8)
#define CBIOS_MAX_RSA_PRIME_BITS ((CBIOS_MAX_RSA_MODULUS_BITS + 1) / 2)
#define CBIOS_MAX_RSA_PRIME_LEN ((CBIOS_MAX_RSA_PRIME_BITS + 7) / 8)

#define CBIOS_RSA_MODULUS_LEN(bits) ((bits + 7) / 8)
#define CBIOS_RSA_PRIME_BITS(bits) ((bits + 1) / 2)
#define CBIOS_RSA_PRIME_LEN(bits) ((CBIOS_RSA_PRIME_BITS(bits) + 7) / 8)

#define CBIOS_RSA_PUBLIC_LEN(bits) (sizeof(WORD)+2*CBIOS_RSA_MODULUS_LEN(bits))
#define CBIOS_RSA_PRIVATE_LEN(bits) (sizeof(WORD)+3*CBIOS_RSA_MODULUS_LEN(bits)+3*CBIOS_RSA_PRIME_LEN(bits))


// Length of digit in bits
#define CBIOS_NN_DIGIT_BITS 32
// Length of digit in bytes
#define CBIOS_NN_DIGIT_LEN (CBIOS_NN_DIGIT_BITS / 8)
// Length of random pool
#define CBIOS_RAND_POOL_SIZE(x) (CBIOS_NN_DIGIT_LEN*((((x) + CBIOS_NN_DIGIT_BITS - 1) / CBIOS_NN_DIGIT_BITS) + 1))


typedef struct 
{
  unsigned int bits;                           // length in bits of modulus
  unsigned int iUnused1[3];                                             // structure alignment (8 bytes)
  unsigned char *modulus;                    // modulus
  unsigned int iUnused2[2];                                     // structure alignment (8 bytes)
  unsigned char *exponent;           // public exponent
  unsigned int iUnused3[2];                             // structure alignment (8 bytes)
} CBIOS_RSA_KEY;

/* Random structure. */
typedef struct 
{
  unsigned int bytesNeeded;
  unsigned char state[16];
  unsigned int outputAvailable;
  unsigned char output[16];
} CBIOS_RANDOM_STRUCT;


/* RSA public and private key. */
typedef struct 
{
  unsigned int bits;                           /* length in bits of modulus */
  unsigned char modulus[CBIOS_MAX_RSA_MODULUS_LEN];                    /* modulus */
  unsigned char exponent[CBIOS_MAX_RSA_MODULUS_LEN];           /* public exponent */
} CBIOS_RSA_PUBLIC_KEY;

typedef struct 
{
  unsigned int bits;                           /* length in bits of modulus */
  unsigned char modulus[CBIOS_MAX_RSA_MODULUS_LEN];                    /* modulus */
  unsigned char publicExponent[CBIOS_MAX_RSA_MODULUS_LEN];     /* public exponent */
  unsigned char exponent[CBIOS_MAX_RSA_MODULUS_LEN];          /* private exponent */
  unsigned char prime[2][CBIOS_MAX_RSA_PRIME_LEN];               /* prime factors */
  unsigned char primeExponent[2][CBIOS_MAX_RSA_PRIME_LEN];   /* exponents for CRT */
  unsigned char coefficient[CBIOS_MAX_RSA_PRIME_LEN];          /* CRT coefficient */
} CBIOS_RSA_PRIVATE_KEY;

typedef struct
{ 
  CHAR  szServerIp[16]; // server IP address like "127.0.0.1" with terminating zero
  WORD  wServerPort;    // server port for connection
  INT32 iBoxesNumber;   // number of attached boxes on server
} CBIOS_SERVER_INFO;

#pragma pack()

#ifdef __cplusplus
extern "C"
{
#endif

extern BYTE CBIOS_DEMO_DISTR_RSA_KEY_MODULUS[CBIOS_INTERNAL_RSA_LEN];
extern BYTE CBIOS_DEMO_DISTR_RSA_KEY_EXPONENT[CBIOS_INTERNAL_RSA_LEN];
extern BYTE CBIOS_DEMO_CLIENT_RSA_KEY_MODULUS[CBIOS_INTERNAL_RSA_LEN];
extern BYTE CBIOS_DEMO_CLIENT_RSA_KEY_EXPONENT[CBIOS_INTERNAL_RSA_LEN];

DWORD WINAPI CBIOS_Startup(void);
DWORD WINAPI CBIOS_Finish(void);

DWORD WINAPI CBIOS_StartupDLL(void);
DWORD WINAPI CBIOS_FinishDLL(void);

DWORD WINAPI CBIOS_Connect(CHAR *ServerName, WORD wPort);
INT32 WINAPI CBIOS_ScanBoxes(void);
DWORD WINAPI CBIOS_Disconnect(void);

DWORD WINAPI CBIOS_GetBoxInfoI(INT32 iBoxIndex, CBIOS_BOX_INFO* pBoxInfo);
DWORD WINAPI CBIOS_GetBoxInfoAdvI(INT32 iBoxIndex, CBIOS_BOX_INFO_ADV* pBoxInfo);
DWORD WINAPI CBIOS_GetDeveloperIDI(INT32 iBoxIndex, DWORD* pdwDeveloperID);
DWORD WINAPI CBIOS_GetAppInfoI(INT32 iBoxIndex, INT32 iAppIndex, CBIOS_APP_INFO* pAppInfo);

DWORD WINAPI CBIOS_OpenByIndex(INT32 iBoxIndex, WORD wAppID);
DWORD WINAPI CBIOS_OpenByName(DWORD dwBoxName, WORD wAppID);
DWORD WINAPI CBIOS_OpenByLabel(const BYTE bBoxLabel[CBIOS_LABEL_LEN], WORD wAppID);
DWORD WINAPI CBIOS_OpenByApp(WORD wAppID);
DWORD WINAPI CBIOS_Close(void);

DWORD WINAPI CBIOS_LockBox(void);
DWORD WINAPI CBIOS_UnlockBox(void);

DWORD WINAPI CBIOS_LockLicence(void);
DWORD WINAPI CBIOS_ReleaseLicence(void);

DWORD WINAPI CBIOS_GetDriverLastError(void);
DWORD WINAPI CBIOS_CheckBox(void);
//---------------------------------------------------------

DWORD WINAPI CBIOS_GetAppInfo(INT32 iAppIndex, CBIOS_APP_INFO* pAppInfo);
DWORD WINAPI CBIOS_GetBoxInfo(CBIOS_BOX_INFO* pBoxInfo);
DWORD WINAPI CBIOS_GetBoxInfoAdv(CBIOS_BOX_INFO_ADV* pBoxInfo);
DWORD WINAPI CBIOS_GetDeveloperID(DWORD* pdwDeveloperID);

DWORD WINAPI CBIOS_SetLabel(const BYTE bBoxLabel[CBIOS_LABEL_LEN]);

DWORD WINAPI CBIOS_UPWLogin(BYTE bUPW[0x10]);
DWORD WINAPI CBIOS_APWLogin(BYTE bAPW[0x10]);
DWORD WINAPI CBIOS_Logout(void);

DWORD WINAPI CBIOS_GetHWRand(DWORD dwLen, PVOID pBuffer);
DWORD WINAPI CBIOS_ReadRAM1(DWORD dwAddress, DWORD dwLen, PVOID pBuffer, BYTE bPass[0x10]);
DWORD WINAPI CBIOS_WriteRAM1(DWORD dwAddress, DWORD dwLen, PVOID pBuffer, BYTE bPass[0x10]);
DWORD WINAPI CBIOS_ReadRAM2(DWORD dwAddress, DWORD dwLen, PVOID pBuffer, BYTE bPass[0x10]);
DWORD WINAPI CBIOS_WriteRAM2(DWORD dwAddress, DWORD dwLen, PVOID pBuffer, BYTE bPass[0x10]);
DWORD WINAPI CBIOS_ReadRAM3(DWORD dwAddress, DWORD dwLen, PVOID pBuffer, BYTE bPass[0x10]);
DWORD WINAPI CBIOS_WriteRAM3(DWORD dwAddress, DWORD dwLen, PVOID pBuffer, BYTE bPass[0x10]);
DWORD WINAPI CBIOS_SetUPW(BYTE bOldPass[0x10], BYTE bNewPass[0x10]);
DWORD WINAPI CBIOS_SetAPW(BYTE bOldPass[0x10], BYTE bNewPass[0x10]);
//---------------------------------------------------------
DWORD WINAPI CBIOS_SetKeySession(BYTE bKEY[0x10]);
DWORD WINAPI CBIOS_SetIVSession(BYTE bIV[0x10]);
DWORD WINAPI CBIOS_SetKeyPrivate(BYTE bAPW[0x10], BYTE bKEY[0x10]);
DWORD WINAPI CBIOS_SetIVPrivate(BYTE bAPW[0x10], BYTE bIV[0x10]);
//---------------------------------------------------------
DWORD WINAPI CBIOS_CryptFixed(DWORD dwLen, PVOID pBuffer);
DWORD WINAPI CBIOS_CryptPrivate(DWORD dwLen, PVOID pBuffer);
DWORD WINAPI CBIOS_CryptSession(DWORD dwLen, PVOID pBuffer);
//---------------------------------------------------------
DWORD WINAPI CBIOS_EncryptRSA(DWORD dwKeyMemory, DWORD dwKeyOffset, PVOID pInBuffer,
                              DWORD dwInBufferLen, PVOID pOutBuffer, DWORD *dwOutBufferLen, BYTE bPass[16]);
DWORD WINAPI CBIOS_DecryptRSA(DWORD dwKeyMemory,DWORD dwKeyOffset, PVOID pInBuffer,
                               DWORD dwInBufferLen, PVOID pOutBuffer, DWORD *dwOutBufferLen, BYTE bPass[16]);

//---------------------------------------------------------
DWORD WINAPI CBIOS_GetKeyPublicRSA(DWORD dwKeyMemory,DWORD dwKeyOffset,CBIOS_RSA_PUBLIC_KEY *pRSAKey, BYTE bPass[16]);
DWORD WINAPI CBIOS_GetKeyPrivateRSA(DWORD dwKeyMemory,DWORD dwKeyOffset,CBIOS_RSA_PRIVATE_KEY *pRSAKey, BYTE bPass[16]);
DWORD WINAPI CBIOS_SetKeyPublicRSA(DWORD dwKeyMemory,DWORD dwKeyOffset,CBIOS_RSA_PUBLIC_KEY *pRSAKey, BYTE bPass[16]);
DWORD WINAPI CBIOS_SetKeyPrivateRSA(DWORD dwKeyMemory,DWORD dwKeyOffset,CBIOS_RSA_PRIVATE_KEY *pRSAKey, BYTE bPass[16]);
//---------------------------------------------------------
DWORD WINAPI CBIOS_GenerateKeyPairRSA(DWORD dwPublicKeyMemory, DWORD dwPublicKeyOffset,
                                       DWORD dwPrivateKeyMemory, DWORD dwPrivateKeyOffset, WORD bits, BYTE *randomPool, BYTE bPass[16]);
//---------------------------------------------------------
DWORD WINAPI CBIOS_EncryptInternalRSA(DWORD dwKeyID, PVOID pInBuffer, DWORD dwInBufferLen, PVOID pOutBuffer, DWORD *pdwOutBufferLen, BYTE bPass[16]);
DWORD WINAPI CBIOS_DecryptInternalRSA(DWORD dwKeyID, PVOID pInBuffer, DWORD dwInBufferLen, PVOID pOutBuffer, DWORD *pdwOutBufferLen, BYTE bPass[16]);

void  WINAPI CBIOS_PrepareRSAKey(WORD bits, BYTE* pbModulus, BYTE* pbExponent, BYTE* pbRSAkey);
//---------------------------------------------------------
DWORD WINAPI CBIOS_MD5Hash(PVOID pInBuffer, DWORD dwInBufferLen, PVOID pOutBuffer, DWORD *pdwOutBufferLen);
//=====================================================================================================================
DWORD WINAPI CBIOS_GetAppLicences(WORD wAppID, DWORD* pdwLocalLic, DWORD* pdwNetLic);
DWORD WINAPI CBIOS_SetAppLicences(WORD wAppID, DWORD dwLocalLic, DWORD dwNetLic, BYTE bPass[0x10]);
DWORD WINAPI CBIOS_CheckAppLicence(WORD wAppID, DWORD* pdwLocalLic, DWORD* pdwNetLic);
//=====================================================================================================================
DWORD WINAPI CBIOS_GetTransactionMark(DWORD* pdwTransactionMark);
DWORD WINAPI CBIOS_SetTransactionMark(DWORD dwTransactionMark);

DWORD WINAPI CBIOS_GetAppPolicyI(INT32 iBoxIndex, OUT BYTE AppPolicy[100]);

//=====================================================================================================================
typedef DWORD CALLBACK F_CBIOS_NOTIFY_CALLBACK(CBIOS_NOTIFY_DATA NotificationData);

DWORD WINAPI CBIOS_RegisterNotificationCallback(F_CBIOS_NOTIFY_CALLBACK* fNotify, void* pParam);
DWORD WINAPI CBIOS_UnregisterNotificationCallback(F_CBIOS_NOTIFY_CALLBACK* fNotify);
#ifdef WIN32
DWORD WINAPI CBIOS_RegisterNotificationMessage(HWND hWnd, UINT   message, LPARAM lParam);
DWORD WINAPI CBIOS_UnregisterNotificationMessage(HWND hWnd);
#endif // WIN32
// obsolete =====================================================================================================================
DWORD WINAPI CBIOS_GetSerialNumI(INT32 iBoxIndex, BYTE SerNum[16]);
DWORD WINAPI CBIOS_GetSerialNum(OUT BYTE bSerNum[16]);

//==========================================================================
DWORD WINAPI CBIOS_ErrorToText(DWORD dwError, char* szError);

//==========================================================================
INT32 WINAPI CBIOS_ScanNetwork(DWORD dwScanTimeMs);
// Set/get UDP port used for network scanning (default port is used if not set)
DWORD WINAPI CBIOS_SetScanPort(WORD wUDPPort);
DWORD WINAPI CBIOS_GetScanPort(WORD* pwUDPPort);
// iServerIndex starts from 1
DWORD WINAPI CBIOS_GetServerInfo(INT32 iServerIndex, CBIOS_SERVER_INFO* pServerInfo);

#ifdef __cplusplus
}
#endif

#endif //_CBIOS_H_
