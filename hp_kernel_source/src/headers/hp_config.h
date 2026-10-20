/*******************************************************************************
**  (c) Copyright Hewlett-Packard Company, 1998.   All rights are reserved.
**  Copying or other reproduction of this program except for archival purposes
**  is prohibited without the prior written consent of Hewlett-Packard Company. 
**                
**                RESTRICTED RIGHTS LEGEND
**    
**  Use, duplication, or disclosure by the U.S. Government is subject to
**  restrictions as set forth in subdivision (b) (3) (ii) of the Rights in
**  Technical Data and Computer Software clause at  52.227-7013.
**    
**  HEWLETT-PACKARD COMPANY
**  Fort Collins, Colorado
*******************************************************************************/

#ifndef _HP_CONFIG_H_
#define _HP_CONFIG_H_ 1

#include <stdint.h>

/* Device characteristics structure */
typedef struct _HpConfigInfo
{
    uint32_t      deviceId;   /* the device "class" for Lego: Silverton or Durango */
    uint32_t      deviceRev;  /* device revision (board + chip revs) */
    uint32_t      deviceFlags;    /* Useful flags */
    uint32_t      rtRev;      /* Revision for RTs on the board (all the same!) */
    uint32_t      gaRev;      /* Revision for B7s on the board (all the same!) */
    uint32_t      feRev;      /* Revision for Heathrow (Front End) */
    uint32_t      gaUnits;    /* number of Blitzen slices */
    uint32_t      rtUnits;    /* number of rasterizer/texturizers (RT chips) */
    uint32_t      fbSize;     /* size in bytes of FB memory */
} HpConfigInfoType;

#define DEVICE_ID_DURANGO       (0x0001)
#define DEVICE_ID_SILVERTON     (0x0002)
#define DEVICE_ID_SUPER         (0x0003)

#define CONFIG_SINGLE_RT        (0x0001)
#define CONFIG_DUAL_RT          (0x0002)
#define CONFIG_QUAD_RT          (0x0003)


#define DEVICE_FLAGS_SLS_CONFIG (0x0001)
#define DEVICE_FLAGS_SDR_CONFIG (0x0002)        /* Uses SDR RAM */
#define DEVICE_FLAGS_DDR_CONFIG (0x0004)        /* Uses DDR RAM */


/*
    For the gaRev field if the value is 0 - 0xff then we
    are using a version of blitzen.
    If it's 0x100 - 0x1ff then we are working with a B7.
    Different revs of B7 will increment the revision number
*/
#define GA_REV_BLITZEN              (0x000)
#define GA_REV_B7                   (0x100)

#define IS_BLITZEN(_rev)            ((_rev >> 8) == 0)
#define IS_B7(_rev)                 ((_rev >> 8) == 1)

/* If SLS_CONFIG bit is set, then the system actually has twice the number of RTs */
/* it's just that each RT (or pair of RTs) is driving a monitor independently */
/* It also implies that we are using a very wide primary surface (>1920) and */
/* using multiple RTs to display it all. */

#endif /* _HP_CONFIG_H_ */
