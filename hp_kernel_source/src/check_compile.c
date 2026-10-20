/* This program makes sure that cross-compiling (UP vs SMP)
 * will work.
 * This requires the Red Hat autoconf-* headers structure,
 * (or if you're not cross-compiling, you're okay.)
 */
#include <stdio.h>
#include <string.h>
#include "/boot/kernel.h"
#include <linux/config.h>

static void fail(char *wanted)
{
    printf("\
*******************************************************************\n\
* This code is designed to make use of Red Hat flexible autoconf  *\n\
* files (if present) to allow you to compile SMP modules while    *\n\
* running a UP kernel and UP modules while running an SMP kernel. *\n\
*                                                                 *\n\
* Since the autoconf.h file on the current machine is static,     *\n\
* cross compiling will not be possible.                           *\n\
*                                                                 *\n\
* To compile the %3s module, you'll have to reboot and rebuild    *\n\
* your autoconf.h file for %3s.                                   *\n\
*******************************************************************\n",
    wanted, wanted);
}


main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr,"Usage: %s [UP|SMP]\n",argv[0]);
	return(2);
    }
#if defined(__rh_kernel_autoconf_h__) || defined(__rh_config_h__)
    /* Red Hat style autoconf.h -- all is well */
    return(0);
#elif defined(__BOOT_KERNEL_SMP) && __BOOT_KERNEL_SMP == 1
    /* Running with SMP headers */
    if (strncmp(argv[1],"SMP",3) == 0) { return(0); }
    else { fail(argv[1]); return(1); }
#else
    /* Running with UP headers */
    if (strncmp(argv[1],"UP",2) == 0) { return(0); }
    else { fail(argv[1]); return(1); }
#endif
}
