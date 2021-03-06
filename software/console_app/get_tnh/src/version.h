//==========================================================================
// Application Information
//==========================================================================
#ifndef _INC_VERSION_H
#define _INC_VERSION_H

//==========================================================================
//==========================================================================
#define STR_PRJ_NAME1_(x) #x
#define STR_PRJ_NAME_(x) STR_PRJ_NAME1_(x)
#define STR_PROGNAME STR_PRJ_NAME_(PRJ_NAME)

#define STR_VERSION "V1.02.00"
#define STR_COPYRIGHT "Copyright DogRatIan"

//==========================================================================
// Version Informations
// Rule: For major changes, inc the 1st digit. (V1.00.00 -> V2.00.00)
//       For minor changes, inc the 2nd digit. (V1.00.00 -> V1.01.00)
//       For branches, inc the 3rd digit.      (V1.00.00 -> V1.00.01)
//==========================================================================
/*
    History
   ------------------------------------------------------------------------
    2019-DEC-21 (Version 1.02.00)
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    1) Add retry on open port, get reading.

    2010-JAN-01 (Version 1.01.00)
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     1) Changed.    Accept lower case on device name under MSW.

    2011-APR-25 (Version 1.00.00)
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     1st edition
*/
//==========================================================================
//==========================================================================
#endif
