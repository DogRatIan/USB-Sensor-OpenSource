//==========================================================================
// Utilities
//==========================================================================
#if (SYSTEM_MSW)
    #include <windows.h>
#else
    #include <pthread.h>
#endif

#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "utils.h"

//==========================================================================
// Defines
//==========================================================================

//==========================================================================
// Variables
//==========================================================================

//==========================================================================
// Get 1ms tick counter
//==========================================================================
unsigned long GetTick (void)
{
    unsigned long ret;
    struct timeval curr_time;

    gettimeofday (&curr_time, NULL);

    ret = curr_time.tv_usec / 1000;
    ret += (curr_time.tv_sec * 1000);
    
    return ret;
}

//==========================================================================
// Calculate the number of tick elapsed
//==========================================================================
unsigned long TickElapsed (unsigned long aTick)
{
    unsigned long curr_tick;

    curr_tick = GetTick ();
    if (curr_tick >= aTick)
        return (curr_tick - aTick);
    else
        return (0xffffffff - aTick + curr_tick);
}

//==========================================================================
// Gen Date string
//==========================================================================
static const char *KStrMonth[12] = 
{
    "JAN",
    "FEB",
    "MAR",
    "APR",
    "MAY",
    "JUN",
    "JUL",
    "AUG",
    "SEP",
    "OCT",
    "NOV",
    "DEC"
};

void GenDateString (char *aDest, time_t aTimeStamp, int aLen, int aLong = false)
{
    struct tm tm;

    memcpy (&tm, localtime(&aTimeStamp), sizeof (tm));

//    localtime_r (&aTimeStamp, &tm);
    if (aLong)
        snprintf (aDest, aLen, "%s-%02d-%04d %02d:%02d", KStrMonth[tm.tm_mon], tm.tm_mday, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
    else
        snprintf (aDest, aLen, "%s-%02d %02d:%02d", KStrMonth[tm.tm_mon], tm.tm_mday, tm.tm_hour, tm.tm_min);

}

//==========================================================================
// Get the Filename from a path
//==========================================================================
const char *FileNameOnly (const char *aSrc)
{
    int i = strlen (aSrc);
    while (i)
    {
        if ((aSrc[i] == '\\') || (aSrc[i] == '/'))
        {
            i++;
            break;
        }
        i--;
    }
    return &aSrc[i];
}

