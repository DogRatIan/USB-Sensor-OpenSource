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

#include "nlohmann/json.hpp"

#include "debug.h"
#include "utils.h"
#include "main.h"
#include "serial.h"

//==========================================================================
// Defines
//==========================================================================
// ms
#define TIMEOUT_WAIT_RESP 200

#define MIN_RELAY_IDX 1
#define MAX_RELAY_IDX 4

// relay state
enum {
    RELAY_ASK = 0,
    RELAY_SET,
    RELAY_CLEAR,
};

//==========================================================================
// Prototypes
//==========================================================================
int Go(void);

//==========================================================================
// Variables
//==========================================================================
static int gArgCnt;
static char **gArgList;
static const char *gDevice = NULL;
static int gVerbose = 0;
static CSerial gSerial;
static bool gReadVoc = true;
static bool gReadCo2 = true;
static bool gJsonFormat = false;
static bool gReadName = false;

static const char *KStrNan = "NaN";

//==========================================================================
// Main Entry
//==========================================================================
int main (int aArgc, char *aArgv[]) {
    // Non buffered output
    setvbuf(stdout, NULL, _IONBF, 0);

    DEBUG_INIT();

    // Start the main
    gArgCnt = aArgc;
    gArgList = aArgv;

    auto ret = Go();

    DEBUG_EXIT();

    return ret;
}

//==========================================================================
// Convert string to upper case
//==========================================================================
static void StringToUpper (char *aSrc) {
    while (*aSrc) {
        if ((*aSrc >= 'a') && (*aSrc <= 'z'))
            *aSrc = *aSrc - 'a' + 'A';
        aSrc++;
    }
}

//==========================================================================
// Show Program info
//==========================================================================
static void ShowProgramInfo(void) {
    printf(STR_PROGNAME " " STR_VERSION);
    if (DEBUG)
        printf(" DEBUG");
    printf(" (build on " __DATE__ " " __TIME__ ")");
    printf(" " STR_COPYRIGHT "\n");
}

//==========================================================================
// Show help
//==========================================================================
static void ShowHelp(void) {
    printf("\n");
    printf("Usage: %s [options]\n", FileNameOnly(gArgList[0]));
    printf("Options:\n"
           "  --help                          Show this help.\n"
           "  -v, --verbose                   Verbose mode.\n"
           "  -d DEVICE, --device=DEVICE      Set device / port.\n"
           "  -t, --VOC                       Read Total VOC in ppb\n"
           "  -c, --CO2                       Read CO2 equivalent in ppm\n"
           "  -a, --all                       Read all values\n"
           "  -n, --name                      Read the sensor name\n"
           "  -j, --json                      Result in JSON string\n");
    printf("\n");
    printf("\n");
}

//==========================================================================
// Extract operation parameters from command line
//==========================================================================
static struct option KLongOptions[] = {
    {"help", no_argument, NULL, '?'},
    {"verbose", no_argument, NULL, 'v'},
    {"device", required_argument, NULL, 'd'},
    {"VOC", no_argument, NULL, 't'},
    {"CO2", no_argument, NULL, 'c'},
    {"name", no_argument, NULL, 'n'},
    {"all", no_argument, NULL, 'a'},
    {"json", no_argument, NULL, 'j'},
    {NULL, 0, NULL, 0}
};

static int ExtractParam(void) {
    while (1) {
        auto i = getopt_long(gArgCnt, gArgList, "vd:jtcan", KLongOptions, NULL);
        if (i == EOF)
            break;
        switch (i) {
        case 't':
            gReadVoc = true;
            gReadCo2 = false;
            gReadName = false;
            break;

        case 'c':
            gReadVoc = false;
            gReadCo2 = true;
            gReadName = false;
            break;

        case 'n':
            gReadVoc = false;
            gReadCo2 = false;
            gReadName = true;
            break;

        case 'a':
            gReadVoc = true;
            gReadCo2 = true;
            gReadName = true;
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
// Get JSON
//==========================================================================
static int GetJson (char *aDest, int aMaxDestLen) {
    unsigned char rx_buf[128];

    DEBUG_PRINTF (__func__);
    gSerial.Write((const unsigned char *)"\n", 1);
    gSerial.Flush();

    if (gSerial.Write((const unsigned char *)"GJSON\n", 6) < 0) {
        if (gVerbose) {
            if (gSerial.ErrorMessage != NULL)
                printf("ERROR: %s\n", gSerial.ErrorMessage);
            else
                printf("ERROR: Write\n");
        }
        return -1;
    }

#if (SYSTEM_MSW)
    Sleep(1);
#else
    usleep(500000);
#endif

    memset(rx_buf, 0, sizeof(rx_buf));
    auto rx_len = gSerial.Read(rx_buf, sizeof(rx_buf) - 1);
    if (rx_len < 0) {
        if (gVerbose) {
            if (gSerial.ErrorMessage != NULL)
                printf("ERROR: %s\n", gSerial.ErrorMessage);
            else
                printf("ERROR: Read\n");
        }
        return -1;
    }

    if (rx_len == 0) {
        if (gVerbose)
            printf("ERROR: No response.\n");
        return -1;
    }
    rx_buf[rx_len] = 0;

    DEBUG_PRINTF("rx_buf: %s", (char *)rx_buf);
    std::strncpy (aDest, reinterpret_cast<char *>(rx_buf), aMaxDestLen);

    return 0;
}

//==========================================================================
// Get Name
//==========================================================================
static int GetName(char *aName, int aSize) {
    unsigned char rx_buf[128];

    gSerial.Write((const unsigned char *)"\n", 1);
    gSerial.Flush();

    if (gSerial.Write((const unsigned char *)"GN\n", 3) < 0) {
        if (gVerbose) {
            if (gSerial.ErrorMessage != NULL)
                printf("ERROR: %s\n", gSerial.ErrorMessage);
            else
                printf("ERROR: Write\n");
        }
        return -1;
    }

#if (SYSTEM_MSW)
    Sleep(1);
#else
    usleep(500000);
#endif

    memset(rx_buf, 0, sizeof(rx_buf));
    auto rx_len = gSerial.Read(rx_buf, sizeof(rx_buf) - 1);
    if (rx_len < 0) {
        if (gVerbose) {
            if (gSerial.ErrorMessage != NULL)
                printf("ERROR: %s\n", gSerial.ErrorMessage);
            else
                printf("ERROR: Read\n");
        }
        return -1;
    }

    if (rx_len == 0) {
        if (gVerbose)
            printf("ERROR: No response.\n");
        return -1;
    }
    rx_buf[rx_len] = 0;

    // Trim tailing space and control codes
    auto idx = strlen((char *)rx_buf);
    while (idx > 0) {
        idx--;
        if (rx_buf[idx] == ' ')
            rx_buf[idx] = 0;
        else if ((rx_buf[idx] > 0) && (rx_buf[idx] < 0x20))
            rx_buf[idx] = 0;
        else
            break;
    }

    std::strncpy (aName, reinterpret_cast<char *>(rx_buf), aSize);

    return (strlen(aName));
}

//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
// Start of process
//==========================================================================
int Go(void) {
    char buf[64];
    float voc = NAN;
    float co2 = NAN;
    char sensor_name[64];
    bool port_opened;
    std::string str;

    memset(sensor_name, 0, sizeof(sensor_name));

    // Extract cmd-line parameter
    if (ExtractParam() == EXIT_FAILURE) {
        ShowProgramInfo();
        ShowHelp();
        return EXIT_FAILURE;
    }
    if (gDevice == NULL) {
        ShowProgramInfo();
        ShowHelp();
        return EXIT_FAILURE;
    }

    if (gVerbose)
        ShowProgramInfo();

    // Open Serial port
    std::strncpy (buf, gDevice, sizeof(buf));
    if (SYSTEM_MSW) {
        StringToUpper(buf);
    }
    if (gSerial.Open(buf, "115200 N81") < 0) {
        if (gVerbose) {
            if (gSerial.ErrorMessage != NULL)
                printf("%s: %s\n", buf, gSerial.ErrorMessage);
            else
                printf("%s: OpenPort fail.\n", buf);
        }
        port_opened = false;
    }
    else
        port_opened = true;

    // Get reading
    if ((gReadVoc) || (gReadCo2)) {
        if (port_opened) {
            if (GetJson (buf, sizeof (buf)) >= 0) {
                try {
                    auto j_data = nlohmann::json::parse (buf);
                    if (j_data["TVOC"].is_number())
                        voc = j_data["TVOC"].get<double>();
                    if (j_data["CO2eq"].is_number())
                        co2 = j_data["CO2eq"].get<double>();
                }
                catch (...) {
                    if (gVerbose)
                        printf ("Unable to parse response.");
                }
            }
        }
    }

    // Read Name
    if (gReadName) {
        std::strncpy (sensor_name, "UNKNOWN", sizeof(sensor_name));
        if (port_opened) {
            if (GetName(sensor_name, sizeof(sensor_name)) < 0) {
                std::strncpy(sensor_name, "UNKNOWN", sizeof(sensor_name));
            }
        }
    }

    // Show outputs
    if (gJsonFormat) {
        nlohmann::json j_data;

        if (gReadVoc) {
            if (std::isnan (voc))
                j_data["TVOC"] = "NAN";
            else
                j_data["TVOC"] = voc;           
        }
        if (gReadCo2) {
            if (std::isnan (co2))
                j_data["CO2eq"] = "NAN";
            else
                j_data["CO2eq"] = co2;           
        }
        if (gReadName) {
            j_data["name"] = sensor_name;
        }
        printf ("%s\n", j_data.dump().c_str());
    }
    else {
        if (gReadVoc) {
            if (std::isnan (voc))
                printf ("%s\n", KStrNan);
            else
                printf ("%.2f\n", voc);
        }
        if (gReadCo2) {
            if (std::isnan (co2))
                printf ("%s\n", KStrNan);
            else
                printf ("%.2f\n", co2);
        }
        if (gReadName) {
            printf ("%s\n", sensor_name);
        }
    }

    //
    return EXIT_SUCCESS;
}
