/*
        $Source: /hmscopy/update.7416/./common_datatypes.h,v $
        $Revision: 1.1 $
        $Date: 96/05/16 15:20:40 $
*/

/*    (c) Copyright Hewlett-Packard Company, 1992.  All rights are reserved.
      Copying or other  reproduction  of this  program  except for  archival
      purposes  is  prohibited   without  the  prior   written   consent  of
      Hewlett-Packard Company.


                             RESTRICTED RIGHTS LEGEND

      Use,  duplication, or disclosure by the U.S.  Government is subject to
      restrictions as set forth in subdivision (b) (3) (ii) of the Rights in
      Technical Data and Computer Software clause at 52.227-7013.

                  HEWLETT-PACKARD COMPANY Fort Collins, Colorado
*/

/*
    This file is used by Lego drivers & tools.
*/
#ifndef _LEGO_BASICTYPES_H_
#define _LEGO_BASICTYPES_H_

typedef char		Sint8;
typedef short		Sint16;
typedef int		Sint32;

#ifndef OGL_TYPES_H   /* [ */
typedef unsigned char	Uint8;
typedef unsigned short	Uint16;
typedef unsigned int	Uint32;

typedef unsigned char	Bool8;
typedef unsigned short	Bool16;
typedef unsigned int	Bool32;
#endif

#ifndef FALSE
#define FALSE	0
#endif
#ifndef TRUE
#define TRUE	1
#endif

#ifndef NULL
#define NULL	0
#endif

#endif /* _LEGO_BASICTYPES_H_ */
