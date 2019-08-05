//==========================================================================
// Showing debug message
//==========================================================================
#include <stdio.h>
#include <stdarg.h>
#include "debug.h"

//==========================================================================
//==========================================================================

//==========================================================================
// Initialize of debug mode
//==========================================================================
void DebugInit (void)
{
}

//==========================================================================
// Exit of Debug
//==========================================================================
void DebugExit (void)
{
}

//==========================================================================
// Show binary array in HEX string
//==========================================================================
void DebugHex2String (const unsigned char *aSrc, int aLen)
{
    printf ("[DEBUG]");
    for (int i = 0; i < aLen; i ++)
    {
        printf ("%02X ", *aSrc);
        aSrc ++;
    }
    printf ("\n");
}

//==========================================================================
// Format and show string
//==========================================================================
void DebugPrintf (const char *aMsg, ...)
{
    va_list args;
    printf ("[DEBUG]");
    va_start (args, aMsg);
    vprintf (aMsg, args);
    va_end (args);
    printf ("\n");
}
