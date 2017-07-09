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
#include <math.h>
#include <string>

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
static bool gReadTemperature = false;
static bool gReadHumidity = false;
static bool gReadPressure = true;
static bool gJsonFormat = false;
static bool gReadName = false;

static const char *KStrNan = "NaN";


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
                 "  -p, --pressure                  Read Pressure\n"
                 "  -n, --name                      Read Sensor name\n"
                 "  -a, --all                       Read Temperature, Humidity and Pressure\n"
                 "  -j, --json                      Result in JSON string\n"
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
    { "temperature", no_argument, NULL, 't'},
    { "humidity", no_argument, NULL, 'h'},
    { "pressure", no_argument, NULL, 'p'},
    { "name", no_argument, NULL, 'n'},
    { "all", no_argument, NULL, 'a'},
    { "json", no_argument, NULL, 'j'},
    { NULL, 0, NULL, 0}
};

static int ExtractParam (void)
{
    int i;

    while (1)
    {
        i = getopt_long (gArgCnt, gArgList, "vd:thpajn", KLongOptions, NULL);
        if (i == EOF)
            break;
        switch (i)
        {
            case 't':
                gReadTemperature    = true;
                gReadHumidity       = false;
                gReadPressure       = false;
                gReadName           = false;
                break;

            case 'h':
                gReadTemperature    = false;
                gReadHumidity       = true;
                gReadPressure       = false;
                break;

            case 'p':
                gReadTemperature    = false;
                gReadHumidity       = false;
                gReadPressure       = true;
                break;

            case 'n':
                gReadTemperature    = false;
                gReadHumidity       = false;
                gReadPressure       = false;
                gReadName           = true;
                break;

            case 'a':
                gReadTemperature    = true;
                gReadHumidity       = true;
                gReadPressure       = true;
                gReadName           = true;
                break;

            case 'j':
                gJsonFormat = true;
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
static float GetTemperature (void)
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
        return NAN;
    }

#if (SYSTEM_MSW)
        Sleep (1);
#else
        usleep (500000);
#endif

    memset (rx_buf, 0, sizeof (rx_buf));
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
        return NAN;
    }

    if (rx_len == 0)
    {
        if (gVerbose)
            printf ("ERROR: No response.\n");
        return NAN;
    }
    rx_buf[rx_len] = 0;

    f = atof ((char *)rx_buf);

    return f;
}

//==========================================================================
// Get Humidity
//==========================================================================
static float GetHumidity (void)
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
        return NAN;
    }

#if (SYSTEM_MSW)
        Sleep (1);
#else
        usleep (500000);
#endif

    memset (rx_buf, 0, sizeof (rx_buf));
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
        return NAN;
    }

    if (rx_len == 0)
    {
        if (gVerbose)
            printf ("ERROR: No response.\n");
        return NAN;
    }
    rx_buf[rx_len] = 0;

    f = atof ((char *)rx_buf);

    return f;
}

//==========================================================================
// Get Pressure
//==========================================================================
static float GetPressure (void)
{
    unsigned char rx_buf[128];
    int rx_len;
    float f;

    gSerial.Write ((const unsigned char *)"\n", 1);
    gSerial.Flush ();

    if (gSerial.Write ((const unsigned char *)"GP\n", 3) < 0)
    {
        if (gVerbose)
        {
            if (gSerial.ErrorMessage != NULL)
                printf ("ERROR: %s\n", gSerial.ErrorMessage);
            else
                printf ("ERROR: Write\n");
        }
        return NAN;
    }

#if (SYSTEM_MSW)
        Sleep (1);
#else
        usleep (500000);
#endif

    memset (rx_buf, 0, sizeof (rx_buf));
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
        return NAN;
    }

    if (rx_len == 0)
    {
        if (gVerbose)
            printf ("ERROR: No response.\n");
        return NAN;
    }
    rx_buf[rx_len] = 0;

    f = atof ((char *)rx_buf);

    return f;
}

//==========================================================================
// Get Name
//==========================================================================
static int GetName (char *aName, int aSize)
{
    unsigned char rx_buf[128];
    int rx_len;
    int idx;

    gSerial.Write ((const unsigned char *)"\n", 1);
    gSerial.Flush ();

    if (gSerial.Write ((const unsigned char *)"GN\n", 3) < 0)
    {
        if (gVerbose)
        {
            if (gSerial.ErrorMessage != NULL)
                printf ("ERROR: %s\n", gSerial.ErrorMessage);
            else
                printf ("ERROR: Write\n");
        }
        return -1;
    }

#if (SYSTEM_MSW)
        Sleep (1);
#else
        usleep (500000);
#endif

    memset (rx_buf, 0, sizeof (rx_buf));
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
        return -1;
    }

    if (rx_len == 0)
    {
        if (gVerbose)
            printf ("ERROR: No response.\n");
        return -1;
    }
    rx_buf[rx_len] = 0;

    // Trim tailing space and control codes
    idx = strlen ((char *)rx_buf);
    while (idx > 0)
    {
        idx --;
        if (rx_buf[idx] == ' ')
            rx_buf[idx] = 0;
        else if ((rx_buf[idx] > 0) && (rx_buf[idx] < 0x20))
            rx_buf[idx] = 0;
        else
            break;
    }

    memset (aName, 0, aSize);
    strncpy (aName, (char *)rx_buf, aSize -1);

    return (strlen (aName));
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
    float temperature = NAN;
    float humidity = NAN;
    float pressure = NAN;
    char sensor_name[64];
    bool port_opened;
    std::string str;
     
    memset (sensor_name , 0, sizeof (sensor_name));

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
        port_opened = false;
    }
    else
        port_opened = true;

    // Start of JSON
    if (gJsonFormat)
        printf ("{ ");

    // Read Temperature
    if (gReadTemperature)
    {
        if (port_opened)
            temperature = GetTemperature ();
        if (gJsonFormat)
        {
            if (isnan (temperature))
                printf ("\"temperature\": null");
            else
                printf ("\"temperature\": %.1f", temperature);
        }
        else
        {
            if (isnan (temperature))
                printf ("%s\n", KStrNan);
            else
                printf ("%.1f\n", temperature);
        }
    }

    // Read Humidity
    if (gReadHumidity)
    {
        if (port_opened)
            humidity = GetHumidity ();
        if (gJsonFormat)
        {
            if (gReadTemperature)
                printf (", ");
            if (isnan (humidity))
                printf ("\"humidity\": null");
            else
                printf ("\"humidity\": %.1f", humidity);
        }
        else
        {
            if (isnan (humidity))
                printf ("%s\n", KStrNan);
            else
                printf ("%.1f\n", humidity);
        }
    }

    // Read Pressure
    if (gReadPressure)
    {
        if (port_opened)
            pressure = GetPressure ();
        if (gJsonFormat)
        {
            if ((gReadTemperature) || (gReadHumidity))
                printf (", ");
            if (isnan (pressure))
                printf ("\"pressure\": null");
            else
                printf ("\"pressure\": %.1f", pressure);
        }
        else
        {
            if (isnan (pressure))
                printf ("%s\n", KStrNan);
            else
                printf ("%.1f\n", pressure);
        }
    }

    // Read Name
    if (gReadName)
    {
        strncpy (sensor_name, "UNKNOWN", sizeof (sensor_name) - 1);
        if (port_opened)
        {
            if (GetName (sensor_name, sizeof (sensor_name)) < 0)
            {
                strncpy (sensor_name, "UNKNOWN", sizeof (sensor_name) - 1);
            }
        }
        if (gJsonFormat)
        {
            if ((gReadTemperature) || (gReadHumidity) || (gReadPressure))
                printf (", ");
            printf ("\"name\": \"%s\"", sensor_name);
        }
        else
        {
            printf ("%s\n", sensor_name);
        }

    }


    // End of JSON
    if (gJsonFormat)
        printf (" }\n");

    //
    return EXIT_SUCCESS;
}
