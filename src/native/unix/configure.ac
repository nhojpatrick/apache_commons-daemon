dnl
dnl Licensed to the Apache Software Foundation (ASF) under one or more
dnl contributor license agreements.  See the NOTICE file distributed with
dnl this work for additional information regarding copyright ownership.
dnl The ASF licenses this file to You under the Apache License, Version 2.0
dnl (the "License"); you may not use this file except in compliance with
dnl the License.  You may obtain a copy of the License at
dnl
dnl     http://www.apache.org/licenses/LICENSE-2.0
dnl
dnl Unless required by applicable law or agreed to in writing, software
dnl distributed under the License is distributed on an "AS IS" BASIS,
dnl WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
dnl See the License for the specific language governing permissions and
dnl limitations under the License.
dnl

dnl -------------------------------------------------------------------------
dnl Author  Pier Fumagalli <mailto:pier.fumagalli@eng.sun.com>
dnl -------------------------------------------------------------------------

dnl -------------------------------------------------------------------------
dnl Initialize AutoConf
dnl -------------------------------------------------------------------------

AC_PREREQ(2.53)

sinclude(./support/apfunctions.m4)dnl
sinclude(./support/apjava.m4)dnl
sinclude(./support/apsupport.m4)dnl
AC_INIT(configure.ac)
AC_CONFIG_AUX_DIR(./support)

dnl Generate ./config.nice for reproducing runs of configure
dnl
AP_CONFIG_NICE(config.nice)

dnl -------------------------------------------------------------------------
dnl Check current host (forget about cross compilation) and validate it
dnl against the cache (fail if the cache differs)
dnl -------------------------------------------------------------------------
AP_MSG_HEADER([Current host])
AC_CANONICAL_HOST()
AP_CANONICAL_HOST_CHECK()

dnl -------------------------------------------------------------------------
dnl Check C environment
dnl -------------------------------------------------------------------------
AP_MSG_HEADER([C-Language compilation tools])
AC_PROG_CC()
AC_CHECK_TOOL(RANLIB, ranlib, :)
AC_CHECK_TOOL(STRIP, strip, :)

dnl -------------------------------------------------------------------------
dnl Check if this host is supported
dnl -------------------------------------------------------------------------
AP_MSG_HEADER([Host support])
AP_SUPPORTED_HOST()
if test "$supported_os" = "darwin"
then
  if test -z "$JAVA_HOME" -a -x /usr/libexec/java_home
  then
    JAVA_HOME="`/usr/libexec/java_home 2>dev/null`"
  fi
  if test -z "$JAVA_HOME" -a -d /System/Library/Frameworks/JavaVM.framework/Home; then
    JAVA_HOME=/System/Library/Frameworks/JavaVM.framework/Home
  fi

  _prevdir=`/bin/pwd`
  if test -n "$JAVA_HOME" -a -d "$JAVA_HOME/include"; then
    cd "$JAVA_HOME/include"
  elif test -n "$JAVA_HOME" -a -d "$JAVA_HOME/../Headers"; then
    cd "$JAVA_HOME/../Headers"
  else
    cd /System/Library/Frameworks/JavaVM.framework/Headers
  fi
  INCLUDES="$INCLUDES -I`/bin/pwd -P`"
  cd $_prevdir
  unset _prevdir
fi

dnl -------------------------------------------------------------------------
dnl Check JAVA environment
dnl -------------------------------------------------------------------------
AP_MSG_HEADER([Java compilation tools])
AP_FIND_JAVA()
dnl MAC OS X does not used include but Headers
if test -d $JAVA_HOME/Headers
then
  JAVA_INC=Headers
else
  JAVA_INC=include
fi
AP_FIND_JAVA_OS()
if test -z "${JAVA_OS}"
then
  AC_MSG_RESULT([jni_md.h found in $JAVA_HOME/$JAVA_INC])
  INCLUDES="$INCLUDES -I$JAVA_HOME/$JAVA_INC"
else
  INCLUDES="$INCLUDES -I$JAVA_HOME/$JAVA_INC -I$JAVA_HOME/$JAVA_INC/$JAVA_OS"
fi

dnl -------------------------------------------------------------------------
dnl Add gcc specific CFLAGS.
dnl -------------------------------------------------------------------------
if test "$GCC" = "yes"
then
   CFLAGS="$CFLAGS -Wall -Wstrict-prototypes"
   AC_MSG_RESULT([gcc flags added])
fi
dnl -------------------------------------------------------------------------
dnl Add gcc specific CFLAGS.
dnl -------------------------------------------------------------------------
if test -z "$LDCMD"
then
   LDCMD="$CC"
fi
AC_SUBST(LDCMD)
if test "$supported_os" = "linux"
then
AC_CHECK_HEADER([sys/capability.h],
                [CFLAGS="$CFLAGS -DHAVE_LIBCAP"],
                AC_MSG_WARN([cannot find headers for libcap]))
fi

if test -z "$STRIPFLAGS"
then
   STRIP="@: "
else
   STRIP="$STRIP $STRIPFLAGS"
fi

AC_SUBST(INCLUDES)
dnl -------------------------------------------------------------------------
dnl Random programs we need to compile locally
dnl -------------------------------------------------------------------------
AP_MSG_HEADER([Writing output files])
AC_CONFIG_FILES(Makefile Makedefs native/Makefile)
AC_OUTPUT

dnl -------------------------------------------------------------------------
dnl Done
dnl -------------------------------------------------------------------------
AP_MSG_HEADER([All done])
AC_MSG_RESULT([Now you can issue "make"])
