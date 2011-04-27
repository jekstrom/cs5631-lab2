# Start comments by by CGP; 4/2/10
# Updates:
#	4/7/10; CGP; Fixed bug so other main files other than main.cpp can be used

# To build your own code files containing a main program, e.g., main.cpp or test.cpp, see NOTE1 below.
# To build additional files (e.g., for your client, server) see NOTE2 below.

# Assumes muscle version 5.22
# Assumes that you have done a "./configure; make" within muscle/zlib/zlib
# Assumes that the muscle directory is "above" the current directory (../muscle references muscle).
# Execute this Makefile by typing "make"

# This builds a program with Muscle that will have an executable called "main"
# Assumes "user code" (non-Muscle code) consists of a single program file "main.cpp"
# If you have other code files add them in as .o files in STDOBJS below.

# End comments by by CGP; 4/2/10

DEFINES += -DMUSCLE_ENABLE_ZLIB_ENCODING 
DEFINES += -DMUSCLE_AVOID_NEWNOTHROW 
DEFINES += -DMUSCLE_USE_PTHREADS 
DEFINES += -DMUSCLE_ENABLE_MULTICAST_API 
DEFINES += -DMUSCLE_AVOID_SIGNAL_HANDLING 
#DEFINES += -DMUSCLE_USE_IPV6 
#DEFINES += -DMUSCLE_COLLECT_HASHTABLE_COLLISION_STATISTICS 
#DEFINES += -DMUSCLE_POOL_SLAB_SIZE=0

CFLAGS = -I.. -I../muscle -I../zlib/zlib -g
GCCFLAGS = -DMUSCLE_NO_EXCEPTIONS -W -Wall -Wno-multichar
CXXFLAGS = $(CFLAGS) -O3 $(DEFINES)
LFLAGS =  
LIBS =  -lpthread
REGEXOBJS = 
ZLIBOBJS = adler32.o deflate.o trees.o zutil.o inflate.o inftrees.o inffast.o crc32.o compress.o gzio.o
ZIPOBJS = zip.o unzip.o ioapi.o

# CS5631 NOTE1: Change this to the name of the file containing your "main" function.
# CS5631 NOTE1: E.g., if your file is named "test.cpp", then just put "test" here.
EXECUTABLE = main

# CS5631 NOTE2: Add your other files here
# e.g., if you have X.cpp, add in X.o here
STDOBJS = $(ZLIBOBJS) Directory.o StringMatcher.o Block.o BlockLinkedList.o FreeList.o BlockGroup.o File.o FileDirectory.o

VPATH = ../muscle/hashtable ../muscle/message ../muscle/iogateway ../muscle/reflector ../muscle/regex ../muscle/util ../muscle/syslog ../muscle/system ../muscle/dataio ../muscle/zlib ../muscle/zlib/zlib ../muscle/zlib/zlib/contrib/minizip ../muscle/minimessage

# if the OS type variable is unset, try to set it using the uname shell command 
ifeq ($(OSTYPE),) 
  OSTYPE = $(strip $(shell uname))
endif 

# IRIX may report itself as IRIX or as IRIX64.  They are both the same to us. 
ifeq ($(OSTYPE),IRIX64) 
  OSTYPE = IRIX 
endif 

ifeq ($(OSTYPE),beos) 
   VPATH += ../besupport
   EXECUTABLES += testbesupport 
   ifeq ($(BE_HOST_CPU),ppc) 
      CXX = mwcc 
      OBJFILES += regcomp.o regerror.o regexec.o regfree.o 
      VPATH += ../regex/regex 
      CFLAGS += -I../regex/regex -I../zlib/zlib 
      MEMORY_TRACKING_SUPPORTED = no  # mwcc can't handle it correctly :^( 
   else # not ppc 
      CXXFLAGS += $(GCCFLAGS) $(CCOPTFLAGS) 
      LIBS = -lbe -lnet -lroot 
      ifeq ($(shell ls 2>/dev/null -1 /boot/develop/headers/be/bone/bone_api.h), /boot/develop/headers/be/bone/bone_api.h) 
         CFLAGS += -I/boot/develop/headers/be/bone -DBONE 
         LIBS = -nodefaultlibs -lbind -lsocket -lbe -lroot -L/boot/beos/system/lib 
      endif 
   endif 
else # not beos
    ifneq (,$(findstring g++,$(CXX)))
        CXXFLAGS += $(GCCFLAGS) $(CCOPTFLAGS)
    else
        CXXFLAGS += $(CCOPTFLAGS)
    endif
endif

ifeq ($(SYSTEM),AtheOS)
   VPATH += ../atheossupport
   EXECUTABLES += testatheossupport
   LIBS = -latheos
endif                           

ifeq ($(OSTYPE),freebsd4.0) 
   CXXFLAGS += -I/usr/include/machine 
endif 

ifeq ($(OSTYPE),DragonFly) 
   # void 
endif 

ifeq ($(OSTYPE),Darwin) 
   LIBS += -framework IOKit -framework Carbon
endif 

ifeq ($(OSTYPE),linux) 
   LIBS += -lutil
endif 

ifeq ($(OSTYPE),Linux) 
   LIBS += -lutil
endif 

ifeq ($(OSTYPE),IRIX) 
   CXXFLAGS += -DSGI -DMIPS 
   ifneq (,$(findstring g++,$(CXX))) # if we are using SGI's CC compiler, we gotta change a few things 
      CXX = CC 
      CCFLAGS = -g2 -n32 -LANG:std -woff 1110,1174,1552,1460,3303 
      LFLAGS  = -g2 -n32 
      CXXFLAGS += -DNEW_H_NOT_AVAILABLE 
      MEMORY_TRACKING_SUPPORTED = no # SGI CC can't handle it correctly :^( 
   endif 
endif 

ifeq ($(MEMORY_TRACKING_SUPPORTED),yes) 
   CXXFLAGS += -DMUSCLE_ENABLE_MEMORY_TRACKING 
endif 

$(EXECUTABLE) : $(STDOBJS) Message.o String.o SysLog.o ByteBuffer.o SetupSystem.o $(EXECUTABLE).o
	$(CXX) $(LFLAGS) -o $@ $^ $(LIBS)

clean :
	rm -f *.o *.xSYM $(EXECUTABLE)
