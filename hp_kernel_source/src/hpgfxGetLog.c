/******************************************************************************
 Program to retrieve all output from HPGFX_LOG calls in the hpgfx kernel
 module.  To use this logging method, just run this program while the kernel
 module is active. The output will show up in stdout. There's no need to load
 the module with special debug flags, and you don't have to wade through a
 bunch of other kernel diagnostic output!
 
 There doesn't exist a makefile, so just use this compile line:
     cc -I /src/multi_product_headers           \
     -I /src/multi_product_headers/dsm_headers  \
     -I /src/multi_product_code/hpgfx           \
     -I /src/ogl/src/headers                    \
     -o hpgfxGetLog -g hpgfxGetLog.c
******************************************************************************/

#include <stdio.h>
#include "lego_adapter.h"
#include "hpgfx_utils.h"
#include "hpgfx_utils.c"
#include "hpgfx_dma.h"


char *hpgfx_getLog(int fd)
{
    static hpgfx_GetLogData iodata;
    int err;

    REQUIRE(fd >= 0, exit(1));

    err = ioctl(fd, HPGFX_IOCTL_GET_LOG, &iodata);
    if (err)
        return NULL;
    else
        return iodata.logBuffer;
}


int fildes;
LegoAdapterPrivateStateType *x3dSharedPrivate;
unsigned int myContext;
unsigned int *lockWord;

void LegoMapDevice(int argc, char **argv)
{
    int rval;
    int a1 = 1,a2 = 1,a3 =0;

    if (argc == 4) {
        sscanf(argv[1], "%d", &a1);
        sscanf(argv[2], "%d", &a2);
        sscanf(argv[3], "%d", &a3);
    } else if (argc != 1) {
        printf("Usage: %s <bus> <dev> <func>\n", argv[0]);
	printf("<bus> <dev> <func> are the three digits in, for example, /dev/dri/hpgfx1:1:0\n");
	exit(1);
    }

    printf("Opening /dev/dri/hpgfx%d:%d:%d\n", a1,a2,a3);

    fildes = hpgfx_open_graphics_device(a1, a2, a3); /* KLUDGE */

    if (fildes < 0) {
        printf("Could not open device\n");
	exit(1);
    }

    hpgfx_map_sarea(fildes, sizeof(LegoAdapterPrivateStateType),
            DRM_CONTAINS_LOCK, (drmAddressPtr)&x3dSharedPrivate);

    drmCreateContext(fildes, &myContext);
    drmSetContextFlags(fildes, myContext, DRM_CONTEXT_PRESERVED);
    lockWord = (int *)&x3dSharedPrivate->lock;
}


main(int argc, char **argv)
{
    char *log;
    LegoMapDevice(argc, argv);

    while(1) {
	log = hpgfx_getLog(fildes);
	if (!log) {
	    printf("hpgfx_getlog failed\n");
	    exit(1);
	}
	if (strlen(log) > 0) {
	    printf("%s", log);
	    fflush(stdout);
	}
	sleep(1);
    }
}
