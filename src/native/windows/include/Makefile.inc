# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#
# ====================================================================
#
# Makefile.inc Master makefile definitions.
#                 This file defines CPU architecture and basic compiler
#                 and linker parameters.
# Common params:
#                 CPU       Compile for specified CPU. Supported CPUs are:
#                           X86 (Common x86 architecture)
#                           X64 (AMD64/EMT64 architecture)
#                           If not specified it will default to the
#                           BUILD_CPU environment variable, failing that
#                           It will default to the
#                           PROCESSOR_ARCHITECTURE/ARCHITEW6432 environment variables
#                           or failing that it will default to X86.
#                 BUILD     Build version
#                           RETAIL or RELEASE (default)
#                           DEBUG
#                 TARGET    Build application target
#                           EXE Console executable (default)
#                           GUI Windows GUI executable
#                           DLL Dynamic Link Library
#                           LIB Static library
# Environment variables  used:
#           EXTRA_CFLAGS    Added to the common CFLAGS
#           EXTRA_CXXFLAGS  Added to the common CXXFLAGS
#           EXTRA_LIBS      Added to the common LIBS
#           EXTRA_LFLAGS    Added to the common LFLAGS
#           EXTRA_RCFLAGS   Added to the common RCFLAGS
#           STATIC_CRT      Enables a hybrid CRT technique
#
# Compiler tools environment variables:
#                 CC        C compiler  (defaults to cl.exe)
#                 CXX       C++ compiler  (defaults to $CC -TP)
#                 CPP       C preprocessor  (defaults to $CC -EP)
#                 LINK      Linker (defaults to link.exe)
#                 RC        Resource compiler (defaults to rc.exe)
#                 MT        Manifest toolkit (defaults to mt.exe)
#                 ML        Assembler (defaults to ml.exe or ml64.exe)
#
# Originally contributed by Mladen Turk <mturk jboss.com>
#
# ====================================================================
#

# C/C++ compiler
!IF !DEFINED(CC) || "$(CC)" == ""
CC = cl.exe
!ENDIF
!IF !DEFINED(CXX) || "$(CXX)" == ""
CXX = $(CC) -TP
!ENDIF
!IF !DEFINED(CPP) || "$(CPP)" == ""
CPP = $(CC) -EP
!ENDIF
# Linker
!IF !DEFINED(LINK) || "$(LINK)" == ""
LINK = link.exe
!ENDIF
# Resource Compiler
!IF !DEFINED(RC) || "$(RC)" == ""
RC = rc.exe
!ENDIF
# Manifest toolkit
!IF !DEFINED(MT) || "$(MT)" == ""
MT = mt.exe
!ENDIF

# If CPU is not defined, apply defaults
!IF !DEFINED(CPU) || "$(CPU)" == ""
# Set BUILD_CPU if it is not yet set
!IF !DEFINED(BUILD_CPU) || "$(BUILD_CPU)" == ""
!IF "$(PROCESSOR_ARCHITECTURE)" == ""
!IF "$(PROCESSOR_ARCHITEW6432)" == ""
# Default to x86, will be upcased later
BUILD_CPU=x86
!ELSE
BUILD_CPU=$(PROCESSOR_ARCHITEW6432)
!ENDIF
!ELSE
BUILD_CPU=$(PROCESSOR_ARCHITECTURE)
!ENDIF
!ENDIF
# Check BUILD_CPU and reset value if necessary
!IF "$(BUILD_CPU)" == "i386" || "$(BUILD_CPU)" == "x86" || "$(BUILD_CPU)" == "i686"
CPU=X86
!ENDIF
!IF "$(BUILD_CPU)" == "AMD64" || "$(BUILD_CPU)" == "amd64" || "$(BUILD_CPU)" == "x86_64" || "$(BUILD_CPU)" == "x64"
CPU=X64
!ENDIF
# did we manage to set CPU?
!IF !DEFINED(CPU) || "$(CPU)" == ""
!ERROR Unexpected value of BUILD_CPU: $(BUILD_CPU) or PROCESSOR_ARCHITECTURE=$(PROCESSOR_ARCHITECTURE) or PROCESSOR_ARCHITEW6432=$(PROCESSOR_ARCHITEW6432).
!ENDIF
!ENDIF

!IF "$(CPU)" != "X86"
!IF "$(CPU)" != "X64"
!ERROR Must specify CPU environment variable (X86, X64) $(CPU)
!ENDIF
!ENDIF

!IF !DEFINED(TARGET) || "$(TARGET)" == ""
TARGET=EXE
!ENDIF

!IF "$(TARGET)" != "EXE"
!IF "$(TARGET)" != "GUI"
!IF "$(TARGET)" != "DLL"
!IF "$(TARGET)" != "LIB"
!ERROR Must specify TARGET environment variable (EXE, GUI, DLL, LIB)
!ENDIF
!ENDIF
!ENDIF
!ENDIF

WINVER=WIN10
NMAKE_WINVER = 0x0A00
_WIN32_IE = 0x0A00

NMAKE_WINNT = -D_WINNT -D_WIN32_WINNT=$(NMAKE_WINVER) -DWINVER=$(NMAKE_WINVER) -D_WIN32_IE=$(_WIN32_IE)

!IF !DEFINED(BUILD) || "$(BUILD)" == ""
BUILD=RELEASE
!ENDIF
!IFDEF RELEASE
BUILD=RELEASE
!ENDIF
!IFDEF DEBUG
BUILD=DEBUG
!ENDIF
!IFDEF NODEBUG
BUILD=RELEASE
!ENDIF

!IF "$(BUILD)" != "RELEASE"
!IF "$(BUILD)" != "DEBUG"
!ERROR Must specify BUILD environment variable (RELEASE, DEBUG)
!ENDIF
!ENDIF

# Common flags for all platforms
CMN_CFLAGS = -c -nologo -DWIN32 -D_WIN32 -D_WINDOWS $(NMAKE_WINNT) -W3
!IF "$(TARGET)" == "EXE"
CMN_CFLAGS = $(CMN_CFLAGS) -D_CONSOLE
!ENDIF
# Mark that extern C newer throws C++ exception
CMN_CFLAGS = $(CMN_CFLAGS) -EHsc

# All supported platforms support unicode
CMN_CFLAGS = $(CMN_CFLAGS) -D_UNICODE -DUNICODE

!IF "$(CPU)" == "X86"
CPU_CFLAGS = -D_X86_=1
MACHINE=X86
MACHINE_LC=i386
!ELSEIF "$(CPU)" == "X64"
CPU_CFLAGS = -D_AMD64_=1 -DWIN64 -D_WIN64
MACHINE=AMD64
MACHINE_LC=amd64
!ENDIF

!IF "$(BUILD)" == "RELEASE"
!IF "$(CPU)" == "X86"
OPT_CFLAGS = -O2 -Ob2 -Oy- -Zi -DNDEBUG -guard:cf
!ELSE
OPT_CFLAGS = -O2 -Ob2 -Zi -DNDEBUG -guard:cf
!ENDIF
!ELSE
OPT_CFLAGS = -Od -Zi -DDEBUG -D_DEBUG
!ENDIF

!IF DEFINED(STATIC_CRT)
CRT_CFLAGS = -D_MT -MT
!ELSE
CRT_CFLAGS = -D_MT -MD
!ENDIF

!IF "$(BUILD)" == "DEBUG"
CRT_CFLAGS = $(CRT_CFLAGS)d
!ENDIF

CFLAGS = $(CMN_CFLAGS) $(CPU_CFLAGS) $(OPT_CFLAGS) $(CRT_CFLAGS)

!IF DEFINED(EXTRA_CFLAGS)
CFLAGS = $(CFLAGS) $(EXTRA_CFLAGS)
!ENDIF

# Cleanup CXXFLAGS
CXXFLAGS =
!IF DEFINED(EXTRA_CXXFLAGS)
CXXFLAGS = $(EXTRA_CXXFLAGS)
!ENDIF

# Linker section
LIBS = kernel32.lib advapi32.lib ole32.lib shell32.lib
# Extra libs from command line or env
!IF DEFINED(EXTRA_LIBS)
LIBS = $(LIBS) $(EXTRA_LIBS)
!ENDIF

# Run-Time Error Checks
!IF "$(BUILD)" == "DEBUG"
!IF DEFINED(RTC_CHECK)
LIBS = $(LIBS) RunTmChk.lib
CFLAGS = $(CFLAGS) -RTC1
!ENDIF
!ENDIF

COMMON_LFLAGS = /NOLOGO

# Always add debugging to the linker
OPT_LFLAGS = /INCREMENTAL:NO /DEBUG
!IF "$(BUILD)" == "RELEASE"
OPT_LFLAGS = $(OPT_LFLAGS) /OPT:REF /guard:cf
!IF DEFINED(STATIC_CRT) && "$(STATIC_CRT)" == "Hybrid"
OPT_LFLAGS = $(OPT_LFLAGS) /NODEFAULTLIB:libucrt.lib /DEFAULTLIB:ucrt.lib
!ENDIF
!ENDIF

!IF "$(TARGET)" == "EXE"
LFLAGS = $(COMMON_LFLAGS) /SUBSYSTEM:CONSOLE /MACHINE:$(MACHINE)
!ELSEIF "$(TARGET)" == "GUI"
LFLAGS = $(COMMON_LFLAGS) /SUBSYSTEM:WINDOWS /MACHINE:$(MACHINE)
!ELSEIF "$(TARGET)" == "DLL"
LFLAGS = $(COMMON_LFLAGS) /DLL /SUBSYSTEM:WINDOWS /MACHINE:$(MACHINE)
!ELSEIF "$(TARGET)" == "LIB"
LFLAGS = -lib $(COMMON_LFLAGS)
!ENDIF

!IF DEFINED(EXTRA_LFLAGS)
LFLAGS = $(LFLAGS) $(EXTRA_LFLAGS)
!ENDIF

!IF "$(TARGET)" != "LIB"
LFLAGS = $(LFLAGS) $(OPT_LFLAGS)
!ENDIF

# Resource compiler flags

RCFLAGS=/l 0x409
!IF "$(BUILD)" == "RELEASE"
RCFLAGS = $(RCFLAGS) /d "NDEBUG"
!ELSE
RCFLAGS = $(RCFLAGS) /d "_DEBUG" /d "DEBUG"
!ENDIF
RCFLAGS = $(RCFLAGS)
!IF DEFINED(EXTRA_RCFLAGS)
RCFLAGS = $(RCFLAGS) $(EXTRA_RCFLAGS)
!ENDIF


# Build Target dir e.g. WINNT_I386_RELEASE_DLL
!IF !DEFINED(WORKDIR) || "$(WORKDIR)" == ""
!IF !DEFINED(WORKDIR_EXT) || "$(WORKDIR_EXT)" == ""
WORKDIR = $(WINVER)_$(CPU)_$(TARGET)_$(BUILD)
WORKDIR_DLL = $(WINVER)_$(CPU)_DLL_$(BUILD)
WORKDIR_LIB = $(WINVER)_$(CPU)_LIB_$(BUILD)
WORKDIR_EXE = $(WINVER)_$(CPU)_EXE_$(BUILD)
!ELSE
WORKDIR = $(WINVER)_$(CPU)_$(BUILDIR_EXT)_$(BUILD)
!ENDIF
!ENDIF

CLEANTARGET=if exist "$(WORKDIR)\$(NULL)" rd /s /q $(WORKDIR)
MAKEWORKDIR=if not exist "$(WORKDIR)\$(NULL)" mkdir $(WORKDIR)
MAKEINSTALL=if not exist "$(INSTALLLOC)\$(NULL)" mkdir $(INSTALLLOC)

!IF DEFINED(JAVA_HOME) && "$(JAVA_HOME)" != ""
JAVA_INCLUDES=-I "$(JAVA_HOME)\include" -I "$(JAVA_HOME)\include\win32"
!ENDIF

# Assembler Section
!IF !DEFINED(ML) || "$(ML)" == ""
!IF "$(CPU)" == "X86"
ML = ml.exe
AFLAGS = /coff /Zi /c
!ELSEIF "$(CPU)" == "X64"
ML = ml64.exe
AFLAGS = /Zi /c
!ENDIF
!ENDIF
