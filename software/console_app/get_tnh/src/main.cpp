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
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cmath>
#include <string>

#if defined(PLATFORM_MSW) && (PLATFORM_MSW != 0)
#include <windows.h>
#endif

#include "debug.h"
#include "define_type.h"
#include "main.h"
#include "safe_string.h"
#include "serial.h"
#include "utils.h"

//==========================================================================
// Defines
//==========================================================================
#define NUM_OF_RETRY 3
#define MAX_LOG_FILE_SIZE (100 * 1024)

// ms
#define TIME_RETRY_INTERVAL 1000

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
static bool gReadTemperature = true;
static bool gReadHumidity = false;
static bool gSaveErrorLog = false;
static int gLogFd = -1;

//==========================================================================
// Main Entry
//==========================================================================
int main(int aArgc, char *aArgv[]) {
  // Non buffered output
  setvbuf(stdout, NULL, _IONBF, 0);

  DEBUG_INIT();

  // Start the main
  gArgCnt = aArgc;
  gArgList = aArgv;

  int ret;
  ret = Go();

  DEBUG_EXIT();

  return ret;
}

//==========================================================================
// Convert string to upper case
//==========================================================================
static void StringToUpper(char *aSrc) {
  while (*aSrc) {
    if ((*aSrc >= 'a') && (*aSrc <= 'z')) *aSrc = *aSrc - 'a' + 'A';
    aSrc++;
  }
}

//==========================================================================
// Show Program info
//==========================================================================
static void ShowProgramInfo(void) {
  printf(STR_PROGNAME " " STR_VERSION);
  if (DEBUG) {
    printf(" DEBUG");
  }
  printf(" (build on " __DATE__ " " __TIME__ ")");
  printf(" " STR_COPYRIGHT "\n");
}

//==========================================================================
// Show help
//==========================================================================
static void ShowHelp(void) {
  printf("\n");
  printf("Usage: %s [options]\n", FileNameOnly(gArgList[0]));
  printf(
      "Options:\n"
      "  --help                          Show this help.\n"
      "  -v, --verbose                   Verbose mode.\n"
      "  -d DEVICE, --device=DEVICE      Set device / port.\n"
      "  -t, --temperature               Read Temperature (default)\n"
      "  -h, --humidity                  Read Humidity\n"
      "  -l, --log                       Save error to log file\n"
      "  -a, --all                       Read both Temperature and Humidity\n");
  printf("\n");
  printf("\n");
}

//==========================================================================
// Extract operation parameters from command line
//==========================================================================
static struct option KLongOptions[] = {{"help", no_argument, NULL, '?'},         {"verbose", no_argument, NULL, 'v'},
                                       {"temperature", no_argument, NULL, 't'},  {"humidity", no_argument, NULL, 'h'},
                                       {"log", no_argument, NULL, 'l'},          {"all", no_argument, NULL, 'a'},
                                       {"device", required_argument, NULL, 'd'}, {NULL, 0, NULL, 0}};

static int ExtractParam(void) {
  int i;

  while (1) {
    i = getopt_long(gArgCnt, gArgList, "vd:thal", KLongOptions, NULL);
    if (i == EOF) break;
    switch (i) {
      case 't':
        gReadTemperature = true;
        gReadHumidity = false;
        break;

      case 'h':
        gReadTemperature = false;
        gReadHumidity = true;
        break;

      case 'a':
        gReadTemperature = true;
        gReadHumidity = true;
        break;

      case 'd':
        gDevice = optarg;
        break;

      case 'v':
        gVerbose = 1;
        break;

      case 'l':
        gSaveErrorLog = true;
        break;

      default:
        return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}

//==========================================================================
// Error Log
//==========================================================================
static void ErrorLog(const char *aMessage) {
#if defined(PLATFORM_MSW) && (PLATFORM_MSW != 0)
  int open_flag = O_RDWR | O_CREAT | O_BINARY;
#else
  int open_flag = O_RDWR | O_CREAT;
#endif

  if (gVerbose) {
    printf("ERROR: %s\n", aMessage);
  }

  if (gSaveErrorLog) {
    // Open log file
    if (gLogFd < 0) {
      gLogFd = open("get_tnh.log", open_flag, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
      if ((gLogFd < 0) && (gVerbose)){
        printf("Fail to open log file.");
      }
      else {
        lseek(gLogFd, 0, SEEK_END);
      }
    }

    // Gen date string
    char str_now[64];
    time_t now;
    time(&now);
    GenDateString(str_now, now, sizeof(str_now), true);

    // Write to log file
    if (gLogFd >= 0) {
      write(gLogFd, str_now, strlen(str_now));
      write(gLogFd, "> ", 2);
      write(gLogFd, aMessage, strlen(aMessage));
      write(gLogFd, "\n", 1);
    }
  }
}

//==========================================================================
// Limit log file size
//==========================================================================
static void LimitLogFileSize(void) {
#if defined(PLATFORM_MSW) && (PLATFORM_MSW != 0)
  int open_flag = O_RDWR | O_CREAT | O_BINARY;
#else
  int open_flag = O_RDWR | O_CREAT;
#endif

  if (gSaveErrorLog) {
    // Open log file
    if (gLogFd < 0) {
      gLogFd = open("get_tnh.log", open_flag, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    }

    //
    if (gLogFd >= 0) {
      size_t file_size = lseek(gLogFd, 0, SEEK_END);
      if (file_size > MAX_LOG_FILE_SIZE) {
        uint8_t file_buf[MAX_LOG_FILE_SIZE];
        lseek(gLogFd, file_size - MAX_LOG_FILE_SIZE, SEEK_SET);
        if (read(gLogFd, file_buf, MAX_LOG_FILE_SIZE) > 0) {
          ftruncate(gLogFd, MAX_LOG_FILE_SIZE);
          lseek(gLogFd, 0, SEEK_SET);
          write(gLogFd, file_buf, MAX_LOG_FILE_SIZE);
        }
      }
    }
  }
}

//==========================================================================
// Wait response
//==========================================================================
static void WaitResponse(void) {
#if defined(PLATFORM_MSW) && (PLATFORM_MSW != 0)
  SleepEx(250, false);
#else
  usleep(250000);
#endif
}

//==========================================================================
// Wait for retry
//==========================================================================
static void WaitForRetry(void) {
#if defined(PLATFORM_MSW) && (PLATFORM_MSW != 0)
  SleepEx(TIME_RETRY_INTERVAL, false);
#else
  usleep(TIME_RETRY_INTERVAL * 1000);
#endif
}

//==========================================================================
// Get Temperature
//==========================================================================
static float32_t GetTemperature(void) {
  unsigned char rx_buf[128];
  int rx_len;
  float32_t ret = NAN;

  gSerial.Write((const unsigned char *)"\n", 1);
  gSerial.Flush();

  for (int16_t retry = 0; retry < NUM_OF_RETRY; retry++) {
    if (gSerial.Write((const unsigned char *)"GT\n", 3) < 0) {
      if (gSerial.ErrorMessage != NULL) {
        std::string msg;
        msg = "GetTemperature ";
        msg += gSerial.ErrorMessage;
        ErrorLog(msg.c_str());
      } else {
        ErrorLog("GetTemperature Write");
      }
    } else {
      WaitResponse();

      rx_len = gSerial.Read(rx_buf, sizeof(rx_buf) - 1);
      if (rx_len < 0) {
        if (gSerial.ErrorMessage != NULL) {
          std::string msg;
          msg = "GetTemperature ";
          msg += gSerial.ErrorMessage;
          ErrorLog(msg.c_str());
        } else {
          ErrorLog("GetTemperature Read");
        }
      } else if (rx_len == 0) {
        ErrorLog("GetTemperature No response.");
      } else {
        rx_buf[rx_len] = 0;

        ret = atof((char *)rx_buf);
      }
    }
    // Exit retry loop if success
    if (!std::isnan(ret)) {
      break;
    }
    //
    WaitForRetry();
  }

  return ret;
}

//==========================================================================
// Get Humidity
//==========================================================================
static float32_t GetHumidity(void) {
  unsigned char rx_buf[128];
  int rx_len;
  float32_t ret = NAN;

  gSerial.Write((const unsigned char *)"\n", 1);
  gSerial.Flush();

  for (int16_t retry = 0; retry < NUM_OF_RETRY; retry++) {
    if (gSerial.Write((const unsigned char *)"GH\n", 3) < 0) {
      if (gSerial.ErrorMessage != NULL) {
        std::string msg;
        msg = "GetHumidity ";
        msg += gSerial.ErrorMessage;
        ErrorLog(msg.c_str());
      } else {
        ErrorLog("GetHumidity Write");
      }
    } else {
      WaitResponse();

      rx_len = gSerial.Read(rx_buf, sizeof(rx_buf) - 1);
      if (rx_len < 0) {
        if (gSerial.ErrorMessage != NULL) {
          std::string msg;
          msg = "GetHumidity ";
          msg += gSerial.ErrorMessage;
          ErrorLog(msg.c_str());
        } else {
          ErrorLog("GetHumidity Read");
        }
      } else if (rx_len == 0) {
        ErrorLog("No response.");
      } else {
        rx_buf[rx_len] = 0;

        ret = atof((char *)rx_buf);
      }
    }

    // Exit retry loop if success
    if (!std::isnan(ret)) {
      break;
    }
    //
    WaitForRetry();
  }

  return ret;
}

//==========================================================================
// Open Serial port
//==========================================================================
int16_t OpenPort(void) {
  char buf[64];

  safe_strncpy(buf, gDevice, sizeof(buf));
  // Convert port name to upper case at Windows
#if defined(PLATFORM_MSW) && (PLATFORM_MSW != 0)
  StringToUpper(buf);
#endif

  int16_t ret = -1;
  for (int16_t retry = 0; retry < NUM_OF_RETRY; retry++) {
    if (gSerial.Open(buf, "115200 N81") < 0) {
      if (gVerbose) {
        if (gSerial.ErrorMessage != NULL) {
          std::string msg;
          msg = buf;
          msg += " OpenPort ";
          msg += gSerial.ErrorMessage;
          ErrorLog(msg.c_str());
        } else {
          std::string msg;
          msg = buf;
          msg += " OpenPort fail.";
          ErrorLog(msg.c_str());
        }
      }
    } else {
      ret = 0;
    }
    // Exit retry loop if success
    if (ret == 0) {
      break;
    }
    //
    WaitForRetry();
  }

  return ret;
}

//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
// Start of process
//==========================================================================
int Go(void) {
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

  if (gVerbose) {
    ShowProgramInfo();
  }

  //
  float32_t temperature = NAN;
  float32_t humidity = NAN;

  // Open Serial port
  if (OpenPort() >= 0) {
    // Get readings
    if (gReadTemperature) {
      temperature = GetTemperature();
    }
    if (gReadHumidity) {
      humidity = GetHumidity();
    }
  }

  // Show readings
  if (gReadTemperature) {
    if (std::isnan(temperature)) {
      printf("NaN\n");
    } else {
      printf("%.2f\n", temperature);
    }
  }

  // Read Humidity
  if (gReadHumidity) {
    if (std::isnan(humidity)) {
      printf("NaN\n");
    } else {
      printf("%.2f\n", humidity);
    }
  }

  // Close port
  gSerial.Close();

  // Close log file
  LimitLogFileSize();
  if (gLogFd >= 0) {
    close(gLogFd);
  }

  //
  return EXIT_SUCCESS;
}
