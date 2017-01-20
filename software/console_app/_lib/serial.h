//==================================================================
// Serial Port Class
//  Using Windows API to access
//==================================================================
#ifndef _INC_SERIAL_H
#define _INC_SERIAL_H

#if (SYSTEM_MSW)
    #include <windows.h>
#else
    #ifndef HANDLE
    #define HANDLE int
    #endif
#endif

//==================================================================
// Class defines
//==================================================================
class CSerial
{
public:
    CSerial (void);                                         // Creation
    ~CSerial (void);                                        // Removal
    int Open (const char *aPortName, const char *aCfg);     // Open
    void Close (void);                                      // Close Port
    bool IsOpened (void);                                   // Check Port opened
    int Read (unsigned char *aDest, int aDestLen);          // Read data
    int Write (const unsigned char *aSrc, int aSrcLen);     // Send data
    int Puts (const char *aSrc);                            // Send String
    void Flush (void);                                      // Flush buffer
    void Dtr (bool aState);                                 // Set DTR state
    bool Dsr (void);                                        // Read DSR state
    void Rts (bool aState);                                 // Set RTS state
    bool Cts (void);                                        // Read CTS state

    char PortName[64];                                      // Port Name
    int BaudRate;                                           // Port baud
    const char *Mode;                                       // Port mode str
    const char *ErrorMessage;                               // Error Message

private:
    HANDLE ComFid;                                          // COM Port Fid
    const char *SearchCfg (const char *aCfg, const char *aStr);

};

//==================================================================
//==================================================================
#endif
