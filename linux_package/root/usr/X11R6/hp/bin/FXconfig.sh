#! /bin/sh
#
# Install or remove FX5/10 X11 Server and OpenGL
#

PATH=/bin:/usr/bin

move_to_rpmsave_dir() {
  if [ -f $1 -o -h $1 ] 
  then
    dirname=`dirname $1`
    basename=`basename $1`
    if [ ! -d $dirname/rpmsave ]
    then
      mkdir $dirname/rpmsave
    fi
    rm -rf $dirname/rpmsave/$basename
    mv $1 $dirname/rpmsave/$basename
  fi
}

# moveGL() looks at all of the libGL.so*
# files/links in /usr/lib, and removes
# all links and moves files to /usr/lib/rpmsave.

moveGL() {
  for x in /usr/lib/libGL* 
  do
     move_to_rpmsave_dir $x 
  done
}

copyit() {
  move_to_rpmsave_dir $2 
  cp -f $1 $2
}

linkit() {
  move_to_rpmsave_dir $2 
  ln -sf $1 $2
}

rmit() {
  rm -f $1
  dirname=`dirname $1`
  basename=`basename $1`
  if [ -e $dirname/rpmsave/$basename ]
  then
    mv $dirname/rpmsave/$basename $1
  fi
  rmdir $dirname/rpmsave >/dev/null 2>&1
}

mkdirit () {
  move_to_rpmsave_dir $1 
  mkdir -p $1
}

usage() {
  if [ -n "$1" ]
  then
    echo >&2 "Usage --
One option of
  -e - Erase
  -i - Install (default)
  -v - Verify
  -q - Quiet"
    exit 1
  else
    exit 0
  fi
}

HP_XLIB=/usr/X11R6/hp/lib
HP_GLLIB=/usr/GL/hp/lib

mode=""
quiet=""
while getopts eiqv c
do
  case $c in
   e) if [ -z "$mode" ]
      then
	mode="erase"
      else
	usage $quiet
      fi
      ;;
   i) if [ -z "$mode" ]
      then
	mode="install"
      else
	usage $quiet
      fi
      ;;
   q) quiet="quiet"
      ;;
   v) if [ -z "$mode" ]
      then
	mode="verify"
      else
	usage $quiet
      fi
      ;;
   ?) usage $quiet
      ;;
  esac
done
if [ -z "$mode" ]
then
  mode="install"
fi

case $mode in
 verify)
    error=""
    if ! grep '^/usr/GL/hp/lib' /etc/ld.so.conf >/dev/null 2>&1
    then
      echo "Warning: missing /usr/GL/hp/lib from /etc/ld.so.conf" >&2
      error="error"
    fi
    if [ -L /etc/X11/X ]
    then
      if [ "`readlink /etc/X11/X`" != "/usr/X11R6/hp/bin/Xhp" ]
      then
        echo "Warning: /etc/X11/X linked to wrong X server" >&2
	error="error"
      fi
    else
      echo "Warning: missing /etc/X11/X link" >&2
      error="error"
    fi
    if [ -z "$quiet" -a -z "$error" ]
    then
       echo "FX verify succeeded"
    fi
    ;;

 install)
    linkit /usr/X11R6/hp/bin/Xhp /etc/X11/X       # Link HP X server
  
    if ! grep '/usr/GL/hp' /etc/ld.so.conf >/dev/null 2>&1
    then 
      LDCONF=/tmp/ld.so.conf$$
      rm -f $LDCONF
      cat >$LDCONF << EOF
/usr/GL/hp/lib  
EOF
      cat >>$LDCONF </etc/ld.so.conf
      cp $LDCONF /etc/ld.so.conf
      rm -f $LDCONF
    fi

    moveGL

    if [ ! -e /usr/include/GL ] ; then
      # /usr/include/GL doesn't exist.
      # Previous versions of this script moved it GL.rpmsave,
      # use it if its available. Otherwise, create it.
      if [ -e /usr/include/GL.rpmsave ] ; then
        mv /usr/include/GL.rpmsave /usr/include/GL
      else
        mkdir /usr/include/GL
      fi
    fi

    for h in /usr/GL/hp/include/GL/*.h
    do
      linkit $h "/usr/include/GL/`basename $h`"
    done

    if [ ! -e /usr/include/GL/glut.h ]
    then
      linkit /usr/X11R6/include/GL/glut.h /usr/include/GL/glut.h
    fi

    if [ ! -e /usr/lib/libglut.so ]
    then
      for l in /usr/X11R6/lib/libglut*
      do
        linkit $l "/usr/lib/`basename $l`"
      done
    fi
    
    # Two versions of graphics libraries are included in the hpgraphics rpm --
    # one set for use with glibc-2.2.4-19 and another for glibc-2.1.3.21.
    # Link the correct set of libraries into /usr/lib, /usr/GL/hp/lib, and
    # /usr/X11R6/hp/lib.
    # Note that glibc-2.1.3-21 is what is in Red Hat 6.2, and
    # glibc-2.2.4-19 is what is in Red Hat 7.1 + errata as of 10/19/2001.

    set `rpm -q glibc | sed 's/[^0-9][^0-9]*/ /g'`  # Extract version numbers from rpm -q output
    if [ $# -ge 3 ] ; then
       GLIBC_VERSION=`expr $1 \* 10000 + $2 \* 100 + $3`
       if [ $GLIBC_VERSION -ge 020200 ] ; then
          # glibc.2.2.0 or later is installed, use glibc.2.2.4 libraries
	  GLIBSDIR=${HP_GLLIB}/glibc2.2.4
	  XLIBSDIR=${HP_XLIB}/glibc2.2.4
       else
          # glibc.2.1.x or earlier is installed, use glibc.2.1.3 libraries
	  GLIBSDIR=${HP_GLLIB}/glibc2.1.3
	  XLIBSDIR=${HP_XLIB}/glibc2.1.3
       fi

       for l in ${GLIBSDIR}/* ; do
             rm -f ${HP_GLLIB}/`basename $l`
             linkit $l ${HP_GLLIB}/`basename $l`
	     linkit $l /usr/lib/`basename $l`
       done
       for l in ${XLIBSDIR}/* ; do
             rm -f ${HP_XLIB}/`basename $l`
             linkit $l ${HP_XLIB}/`basename $l`
	     linkit $l /usr/lib/`basename $l`
       done

    else
       echo Warning: Unrecognized version for glibc, graphics libraries will not work
    fi

    /sbin/ldconfig

    for f in /usr/X11R6/hp/default/*
    do
      copyit $f "/etc/X11/`basename $f`"
    done
    /etc/rc.d/init.d/gpm stop >/dev/null 2>&1
    /sbin/chkconfig --level 2345 gpm off
  
    /sbin/chkconfig --add hpgraphics
    if [ `/sbin/runlevel | awk '{print $2+0}'` -gt 1 ]
    then
      if ! /etc/rc.d/init.d/hpgraphics start >/dev/null
      then
        echo "Please reboot system to install graphics kernel module."
      fi
    fi
    ;;

 erase)
    /etc/rc.d/init.d/hpgraphics stop >/dev/null  2>&1
    /sbin/chkconfig --del hpgraphics
    if [ -L /usr/include/GL ]
    then
      rmit /usr/include/GL
    else
      if [ -h /usr/include/GL/glut.h ] 
      then
        rmit /usr/include/GL/glut.h
      fi
      for h in /usr/GL/hp/include/GL/*.h
      do
        rmit /usr/include/GL/`basename $h`
      done
    fi
    for l in /usr/lib/libXsls* /usr/lib/libXhp11* /usr/lib/libgrm*
    do
      rmit $l
    done
    if [ -e /usr/X11R6/lib/libglut.so ] ; then
        for l in /usr/lib/libglut*
        do
          rmit $l
        done
    fi
    for l in /usr/lib/libGL.so* /usr/lib/libGLU* /usr/lib/libdd*
    do
      rmit $l
    done
    rmit /usr/lib/lib35acda30.1
    rmit /etc/X11/X
    for f in /usr/X11R6/hp/default/*
    do
      rmit "/etc/X11/`basename $f`"
    done

    for f in /etc/X11/logs/*
    do
      rm -f $f
    done
    LDCONF=/tmp/ld.so.conf$$
    rm -f $LDCONF
    grep -v '^/usr/GL/hp/lib' /etc/ld.so.conf >$LDCONF
    cp $LDCONF /etc/ld.so.conf
    rm -f $LDCONF
    /sbin/ldconfig
    ;;
esac

