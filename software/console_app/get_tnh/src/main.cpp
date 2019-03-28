//==========================================================================
// Template
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
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>

#if (SYSTEM_MSW)
#include <windows.h>
#endif

#include "debug.h"
#include "utils.h"
#include "main.h"
#include "serial.h"
#include "safe_string.h"

//==========================================================================
// Defines
//==========================================================================
// ms
#define TIMEOUT_WAIT_RESP       200

#define MIN_RELAY_IDX           1
#define MAX_RELAY_IDX           4

// relay state
enum
{
    RELAY_ASK = 0,
    RELAY_SET,
    RELAY_CLEAR,
};

//==========================================================================
// Prototypes
//==========================================================================
int Go (void);

//==========================================================================
// Variables
//==========================================================================
static int gArgCnt;
static char **gArgList;
static const char *gDevice = NULL;
static int gVerbose = 0;
static CSerial gSerial;
static bool gReadTemperature = true;
static bool gReadHumidity = false;

//==========================================================================
// Main Entry
//==========================================================================
int main (int aArgc, char *aArgv[])
{
    // Non buffered output
    setvbuf (stdout, NULL, _IONBF, 0);

    DEBUG_INIT ();

    // Start the main
    gArgCnt = aArgc;
    gArgList = aArgv;

    int ret;
    ret = Go ();

    DEBUG_EXIT ();

    return ret;
}

//==========================================================================
// Convert string to upper case
//==========================================================================
static void StringToUpper (char *aSrc)
{
    while (*aSrc)
    {
        if ((*aSrc >= 'a') && (*aSrc <= 'z'))
            *aSrc = *aSrc - 'a' + 'A';
        aSrc ++;
    }
}


//==========================================================================
// Show Program info
//==========================================================================
static void ShowProgramInfo (void)
{
    printf (STR_PROGNAME " " STR_VERSION);
    if (DEBUG)
        printf (" DEBUG");
    printf (" (build on " __DATE__ " " __TIME__ ")");
    printf (" " STR_COPYRIGHT "\n");
}

//==========================================================================
// Show help
//==========================================================================
static void ShowHelp (void)
{
    printf ("\n");
    printf ("Usage: %s [options]\n", FileNameOnly (gArgList[0]));
    printf ("Options:\n"
                 "  --help                          Show this help.\n"
                 "  -v, --verbose                   Verbose mode.\n"
                 "  -d DEVICE, --device=DEVICE      Set device / port.\n"
                 "  -t, --temperature               Read Temperature (default)\n"
                 "  -h, --humidity                  Read Humidity\n"
                 "  -a, --all                       Read both Temperature and Humidity\n"
                 );
    printf ("\n");
    printf ("\n");
}

//==========================================================================
// Extract operation parameters from command line
//==========================================================================
static struct option KLongOptions[] =
{
    { "help", no_argument, NULL, '?'},
    { "verbose", no_argument, NULL, 'v'},
    { "device", required_argument, NULL, 'd'},
    { NULL, 0, NULL, 0}
};

static int ExtractParam (void)
{
    int i;

    while (1)
    {
        i = getopt_long (gArgCnt, gArgList, "vd:tha", KLongOptions, NULL);
        if (i == EOF)
            break;
        switch (i)
        {
            case 't':
                gReadTemperature    = true;
                gReadHumidity       = false;
                break;

            case 'h':
                gReadTemperature    = false;
                gReadHumidity       = true;
                break;

            case 'a':
                gReadTemperature    = true;
                gReadHumidity       = true;
                break;

            case 'd':
                gDevice = optarg;
               break;

            case 'v':
               gVerbose = 1;
               break;

            default:
                return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

//==========================================================================
// Get Temperature
//==========================================================================
static int GetTemperature (void)
{
    unsigned char rx_buf[128];
    int rx_len;
    float f;

    gSerial.Write ((const unsigned char *)"\n", 1);
    gSerial.Flush ();

    if (gSerial.Write ((const unsigned char *)"GT\n", 3) < 0)
    {
        if (gVerbose)
        {
            if (gSerial.ErrorMessage != NULL)
                printf ("ERROR: %s\n", gSerial.ErrorMessage);
            else
                printf ("ERROR: Write\n");
        }
        else
            printf ("NaN\n");
        return -1;
    }

#if (SYSTEM_MSW)
        Sleep (1);
#else
        usleep (500000);
#endif

    rx_len = gSerial.Read (rx_buf, sizeof (rx_buf) - 1);
    if (rx_len < 0)
    {
        if (gVerbose)
        {
            if (gSerial.ErrorMessage != NULL)
                printf ("ERROR: %s\n", gSerial.ErrorMessage);
            else
                printf ("ERROR: Read\n");
        }
        else
            printf ("NaN\n");
        return -1;
    }

    if (rx_len == 0)
    {
        if (gVerbose)
            printf ("ERROR: No response.\n");
        else
            printf ("NaN\n");
        return -1;
    }
    rx_buf[rx_len] = 0;

    f = atof ((char *)rx_buf);

    printf ("%.2f\n", f);

    return 0;
}

//==========================================================================
// Get Humidity
//==========================================================================
static int GetHumidity (void)
{
    unsigned char rx_buf[128];
    int rx_len;
    float f;

    gSerial.Write ((const unsigned char *)"\n", 1);
    gSerial.Flush ();

    if (gSerial.Write ((const unsigned char *)"GH\n", 3) < 0)
    {
        if (gVerbose)
        {
            if (gSerial.ErrorMessage != NULL)
                printf ("ERROR: %s\n", gSerial.ErrorMessage);
            else
                printf ("ERROR: Write\n");
        }
        else
            printf ("NaN\n");
        return -1;
    }

#if (SYSTEM_MSW)
        Sleep (1);
#else
        usleep (500000);
#endif

    rx_len = gSerial.Read (rx_buf, sizeof (rx_buf) - 1);
    if (rx_len < 0)
    {
        if (gVerbose)
        {
            if (gSerial.ErrorMessage != NULL)
                printf ("ERROR: %s\n", gSerial.ErrorMessage);
            else
                printf ("ERROR: Read\n");
        }
        else
            printf ("NaN\n");
        return -1;
    }

    if (rx_len == 0)
    {
        if (gVerbose)
            printf ("ERROR: No response.\n");
        else
            printf ("NaN\n");
        return -1;
    }
    rx_buf[rx_len] = 0;

    f = atof ((char *)rx_buf);

    printf ("%.2f\n", f);

    return 0;
}
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
// Start of process
//==========================================================================
int Go (void)
{
    char buf[64];

    // Extract cmd-line parameter
    if (ExtractParam () == EXIT_FAILURE)
    {
        ShowProgramInfo ();
        ShowHelp ();
        return EXIT_FAILURE;
    }
    if (gDevice == NULL)
    {
        ShowProgramInfo ();
        ShowHelp ();
        return EXIT_FAILURE;
    }


    if (gVerbose)
        ShowProgramInfo ();

    // Open Serial port
    safe_strncpy (buf, gDevice, sizeof (buf));
    if (SYSTEM_MSW)
    {
        StringToUpper (buf);
    }
    if (gSerial.Open (buf, "115200 N81") < 0)
    {
        if (gVerbose)
        {
            if (gSerial.ErrorMessage != NULL)
                printf ("%s: %s\n", buf, gSerial.ErrorMessage);
            else
                printf ("%s: OpenPort fail.\n", buf);
        }
        else
            printf ("NaN\n");
        return EXIT_FAILURE;
    }

    // Read Temperature
    if (gReadTemperature)
    {
        if (GetTemperature () < 0)
            return EXIT_FAILURE;
    }

    // Read Humidity
    if (gReadHumidity)
    {
        if (GetHumidity () < 0)
            return EXIT_FAILURE;
    }

    //
    return EXIT_SUCCESS;
}
