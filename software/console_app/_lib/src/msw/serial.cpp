//==========================================================================
// Serial Interface
//==========================================================================
//==========================================================================
// Naming conventions
// ~~~~~~~~~~~~~~~~~~
//                Class : Leading C
//               Struct : Leading T
//             Constant : Leading K
//      Global Variable : Leading g
//    Function argument : Leading a
//       Local Variable : All lower case
//==========================================================================
#include <stdio.h>
#include "serial.h"

//==========================================================================
// Defines
//==========================================================================
// Rxstate values
#define S_RX_IDLE               0
#define S_RX_FIRST_CHAR         1
#define S_RX_LEN_MSB            2
#define S_RX_LEN_LSB            3
#define S_RX_SEQ                4
#define S_RX_DATA               5
#define S_RX_END_CHAR           6
#define S_RX_CRC16_MSB          7
#define S_RX_RESP               8

// Txstate values
#define S_TX_IDLE               0
#define S_TX_SEND               1
#define S_TX_WAIT_RESP          2
#define S_TX_EOT                3
#define S_TX_POLL               4
#define S_TX_WAIT_EOT           5

// in ms
#define TIME_FORCE_TX           500     // Time to force Tx when no poll
#define TIME_RX_TO_TX_DELAY     100     // in ms, RX to TX interval
#define TIMEOUT_LINK            500     // Time to set link break when no rx
#define TIME_TX_POLL_INTERVAL   5       // Poll char interval at host mode
#define TIME_TX_WAIT_EOT        100     // Time to wait respond of poll


//------------------------------
//------------------------------
struct TBaudTable               // Baud rate table
{
    const char *Str;            // String of config input by user
    unsigned long BaudValue;    // Value pass to Win API
    int BaudShow;               // Value return to user
};

struct TModeTable               // Comm mode table
{
    const char *Str;            // String of config input by user
    char ByteSize;              // Byte size in bits
    char Parity;                // Parity
    char StopBit;               // Stop bit length
};

//==========================================================================
// Constants
//==========================================================================
static const struct TBaudTable KBaudTable[] = 
{
    {"1200", CBR_1200, 1200},
    {"2400", CBR_2400, 2400},
    {"4800", CBR_4800, 4800},
    {"9600", CBR_9600, 9600},
    {"14400", CBR_14400, 14400},
    {"19200", CBR_19200, 19200},
    {"28800", 28800, 28800},
    {"38400", CBR_38400, 38400},
    {"57600", CBR_57600, 57600},
    {"115200", CBR_115200, 115200},
    {NULL, 0, 0}
};

static const struct TModeTable KModeTable[] = 
{
    {"N81", 8, NOPARITY, ONESTOPBIT},
    {"E81", 8, EVENPARITY, ONESTOPBIT},
    {"O81", 8, ODDPARITY, ONESTOPBIT},
    {"N71", 7, NOPARITY, ONESTOPBIT},
    {"E71", 7, EVENPARITY, ONESTOPBIT},
    {"O71", 7, ODDPARITY, ONESTOPBIT},
    {"N82", 8, NOPARITY, TWOSTOPBITS},
    {"E82", 8, EVENPARITY, TWOSTOPBITS},
    {"O82", 8, ODDPARITY, TWOSTOPBITS},
    {"N72", 7, NOPARITY, TWOSTOPBITS},
    {"E72", 7, EVENPARITY, TWOSTOPBITS},
    {"O72", 7, ODDPARITY, TWOSTOPBITS},
    {"N81.5", 8, NOPARITY, ONE5STOPBITS},
    {"E81.5", 8, EVENPARITY, ONE5STOPBITS},
    {"O81.5", 8, ODDPARITY, ONE5STOPBITS},
    {"N71.5", 7, NOPARITY, ONE5STOPBITS},
    {"E71.5", 7, EVENPARITY, ONE5STOPBITS},
    {"O71.5", 7, ODDPARITY, ONE5STOPBITS},
    {"8N1", 8, NOPARITY, ONESTOPBIT},
    {"8E1", 8, EVENPARITY, ONESTOPBIT},
    {"8O1", 8, ODDPARITY, ONESTOPBIT},
    {"7N1", 7, NOPARITY, ONESTOPBIT},
    {"7E1", 7, EVENPARITY, ONESTOPBIT},
    {"7O1", 7, ODDPARITY, ONESTOPBIT},
    {"8N2", 8, NOPARITY, TWOSTOPBITS},
    {"8E2", 8, EVENPARITY, TWOSTOPBITS},
    {"8O2", 8, ODDPARITY, TWOSTOPBITS},
    {"7N2", 7, NOPARITY, TWOSTOPBITS},
    {"7E2", 7, EVENPARITY, TWOSTOPBITS},
    {"7O2", 7, ODDPARITY, TWOSTOPBITS},
    {"8N1.5", 8, NOPARITY, ONE5STOPBITS},
    {"8E1.5", 8, EVENPARITY, ONE5STOPBITS},
    {"8O1.5", 8, ODDPARITY, ONE5STOPBITS},
    {"7N1.5", 7, NOPARITY, ONE5STOPBITS},
    {"7E1.5", 7, EVENPARITY, ONE5STOPBITS},
    {"7O1.5", 7, ODDPARITY, ONE5STOPBITS},
    {NULL, 0, 0}
};

//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================

//==========================================================================
// Create of Object
//==========================================================================
CSerial::CSerial (void) :
    BaudRate (9600),
    Mode ("N81"),
    ErrorMessage (NULL),
    ComFid (INVALID_HANDLE_VALUE)
{
    strcpy (PortName, "COM1");
}

//==========================================================================
// Remove of Object
//==========================================================================
CSerial::~CSerial (void)
{
    Close ();
}


//==========================================================================
// Search config string input by user
//  Return the pointer of the *str start point inside *cfg
//==========================================================================
const char *CSerial::SearchCfg (const char *aCfg, const char *aStr)
{
    int len;

    len = strlen (aStr);
    if (len == 0)
        return NULL;

    while ((strncmp (aCfg, aStr, len)) && (aCfg[0] != 0))
        aCfg++;

    if (aCfg[0] == 0)
        return NULL;

    return aCfg;
}

//==========================================================================
// Open Port
//==========================================================================
int CSerial::Open (const char *aPortName, const char *aCfg)
{
    COMMTIMEOUTS timeout;
    DCB dcb;
    COMMPROP prop;
    const char *ptr;
    int i;

    // Extract Port
    ptr = SearchCfg (aPortName, "COM");
    if (ptr == NULL)
        i = 1;
    else
        i = atoi (&ptr[3]);
    if ((i < 1) || (i > 99))        // Limit to COM1 - COM99
        i = 1;
    
    if (i > 9)
        snprintf (PortName, sizeof (PortName), "\\\\.\\COM%d", i);      // Special name above COM9
    else
        snprintf (PortName, sizeof (PortName), "COM%d", i);             // Normal name


    // Open Port
    if (ComFid == INVALID_HANDLE_VALUE)
    {
        ComFid = CreateFile (PortName,
                           GENERIC_READ | GENERIC_WRITE,
                           0, NULL, OPEN_EXISTING, 0, NULL);
    }
    else
    {
        ErrorMessage = "Error: Already Open";
        return -1;
    }

    if (ComFid == INVALID_HANDLE_VALUE)
    {
        ErrorMessage = "Error: Open Port";
        return -1;
    }

    if (!GetCommState (ComFid, &dcb))     // Config Port
    {
        ErrorMessage = "Error: Get DCB Error";
        return -1;
    }

    i = 0;                      // Extract Baud
    while (KBaudTable[i].Str != NULL)
    {
        if (SearchCfg (aCfg, KBaudTable[i].Str) != NULL)
        {
            dcb.BaudRate = KBaudTable[i].BaudValue;
            BaudRate = KBaudTable[i].BaudShow;
            break;
        }
        i++;
    }

    i = 0;                      // Extract Mode
    while (KModeTable[i].Str != NULL)
    {
        if (SearchCfg (aCfg, KModeTable[i].Str) != NULL)
        {
            dcb.ByteSize = KModeTable[i].ByteSize;
            dcb.Parity = KModeTable[i].Parity;
            dcb.StopBits = KModeTable[i].StopBit;
            Mode = KModeTable[i].Str;
            break;
        }
        i++;
    }

    dcb.fOutxCtsFlow = false;   // CTS Flow control for output
    dcb.fOutxDsrFlow = false;   // DSR Flow control for output
    dcb.fDsrSensitivity = false;
    dcb.fOutX = false;
    dcb.fInX = false;
    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    dcb.fRtsControl = RTS_CONTROL_ENABLE;       // RTS keep on

    if (!SetCommState (ComFid, &dcb))
    {
        ErrorMessage = "Error: Set DCB";
        return -1;
    }

    timeout.ReadIntervalTimeout = 1;
    timeout.ReadTotalTimeoutMultiplier = 0;
    timeout.ReadTotalTimeoutConstant = 1;
    timeout.WriteTotalTimeoutMultiplier = 0;
    timeout.WriteTotalTimeoutConstant = 0;
    if (!SetCommTimeouts (ComFid, &timeout))
    {
        ErrorMessage = "Error: Set Timeout";
        return -1;
    }

    // Get properties
    if (!GetCommProperties (ComFid, &prop))
    {
        ErrorMessage = "Error: Get properties";
        return -1;
    }

    return 0;
}

//==========================================================================
// Close Port
//==========================================================================
void CSerial::Close (void)
{
    // Close COM
    if (ComFid != INVALID_HANDLE_VALUE)
    {
        PurgeComm (ComFid, PURGE_TXABORT | PURGE_RXABORT);
        CloseHandle (ComFid);
    }
    ComFid = INVALID_HANDLE_VALUE;
}

//==========================================================================
// Check Port opened
//==========================================================================
bool CSerial::IsOpened (void)
{
    if (ComFid == INVALID_HANDLE_VALUE)
        return false;
    else
        return true;
}

//==========================================================================
// Get RXed data
//==========================================================================
int CSerial::Read (unsigned char *aDest, int aDestLen)
{
    DWORD ret_len;

    if (ComFid == INVALID_HANDLE_VALUE)
        return -1;

    if (ReadFile (ComFid, aDest, aDestLen, &ret_len, NULL) == false)
    {
        ErrorMessage = "Error: Read";
        return -1;
    }

    return (int)ret_len;
}

//==========================================================================
// Send data
//==========================================================================
int CSerial::Write (const unsigned char *aSrc, int aSrcLen)
{
    DWORD ret_len;

    if (ComFid == INVALID_HANDLE_VALUE)
        return -1;

    if (WriteFile (ComFid, aSrc, aSrcLen, &ret_len, NULL) == false)
    {
        ErrorMessage = "Error: Write";
        return -1;
    }

    return (int)ret_len;
}

//==========================================================================
// Send Null-Terminal Sting
//==========================================================================
int CSerial::Puts (const char *aSrc)
{
    unsigned long src_len;
    unsigned long ret_len;

    if (ComFid == INVALID_HANDLE_VALUE)
        return -1;

    src_len = 0;
    while (aSrc[src_len] != 0)
        src_len++;
    if (src_len)
    {
        if (WriteFile (ComFid, aSrc, src_len, &ret_len, NULL) == false)
        {
            ErrorMessage = "Error: Write";
            return -1;
        }
    }

    return ret_len;
}

//==========================================================================
// Flush buffer
//==========================================================================
void CSerial::Flush (void)
{
    if (ComFid == INVALID_HANDLE_VALUE)
        return;

    PurgeComm (ComFid, PURGE_RXCLEAR | PURGE_TXCLEAR);
}

//==========================================================================
// Control DTR signal
//==========================================================================
void CSerial::Dtr (bool aState)
{

    if (ComFid == INVALID_HANDLE_VALUE)
        return;

    if (aState)
        EscapeCommFunction (ComFid, SETDTR);
    else
        EscapeCommFunction (ComFid, CLRDTR);
}

//==========================================================================
// Read DSR signal
//==========================================================================
bool CSerial::Dsr (void)
{
    unsigned long status;

    if (ComFid == INVALID_HANDLE_VALUE)
        return false;

    if (GetCommModemStatus (ComFid, &status) == FALSE)
        return false;

    
    if (status & MS_DSR_ON)
        return true;
    else
        return false;
}


//==========================================================================
// Control RTS signal
//==========================================================================
void CSerial::Rts (bool aState)
{

    if (ComFid == INVALID_HANDLE_VALUE)
        return;

    if (aState)
        EscapeCommFunction (ComFid, SETRTS);
    else
        EscapeCommFunction (ComFid, CLRRTS);
}

//==========================================================================
// Read CTS signal
//==========================================================================
bool CSerial::Cts (void)
{
    unsigned long status;

    if (ComFid == INVALID_HANDLE_VALUE)
        return false;

    if (GetCommModemStatus (ComFid, &status) == FALSE)
        return false;
    
    if (status & MS_CTS_ON)
        return true;
    else
        return false;
}



