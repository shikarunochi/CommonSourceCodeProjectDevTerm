CC	 = gcc
CXX	 = c++
CXXLINK	 = $(CXX)
RM	 = rm -f
TAGS	 = etags
DEPEND	 = gccmakedep
DEPEND_DEFINES =

SDL2 = 1

# for debug
#CDEBUGFLAGS = -g -O0 -fno-strict-aliasing

#
# for Opt.
#
CDEBUGFLAGS= -O3
CDEBUGFLAGS+= -funroll-loops
CDEBUGFLAGS+= -fomit-frame-pointer
CDEBUGFLAGS+= -ffast-math
CDEBUGFLAGS+= -DOSD_DEVTERM -D_DEVTERM

# CDEBUGFLAGS+= -march=pentium-m
# CDEBUGFLAGS+= -msse -mfpmath=sse

#
# for DEBUG
#
# CDEBUGFLAGS= -O0
# CDEBUGFLAGS+= -g
# CDEBUGFLAGS+= -W -Wall -Wuninitialized
# CDEBUGFLAGS+= -Wunused
# CDEBUGFLAGS+= -Werror
# CDEBUGFLAGS+= -DINLINE=
# CDEBUGFLAGS+= -DUSE_GAS

ifdef SDL2
# SDL 2.0
SDL_CONFIG?= sdl2-config
else
# SDL 1.2
SDL_CONFIG?= sdl-config
endif

SDL_INCLUDE=	`$(SDL_CONFIG) --cflags`
ifdef SDL2
SDL_LIB=	`$(SDL_CONFIG) --libs` -lSDL2_gfx
else
SDL_LIB=	`$(SDL_CONFIG) --libs` -lSDL_gfx
endif

ifeq ($(shell uname -m),armv6l)
	MOPT=
else
	MOPT=
endif

LDLIBS = -lm -lz

GTK_LIB = `pkg-config --libs gtk+-3.0`
GTK_INCLUDE= `pkg-config --cflags gtk+-3.0`


UOBJS = DevTerm/emuMain.o \
        common.o \
        config.o \
        emu.o \
        fifo.o \
        fileio.o \
        DevTerm/osd.o \
        DevTerm/osd_input.o \
        DevTerm/osd_screen.o \
        DevTerm/osd_sound.o \
        DevTerm/osd_file_select.o \
        DevTerm/osd_dialog.o

ifdef machine
  include Makefile.$(machine)
endif

EXTRA_INCLUDES= -I./ -I./vm -I./DevTerm $(SDL_INCLUDE) $(GTK_INCLUDE)

CXXDEBUGFLAGS= $(CDEBUGFLAGS)

CFLAGS= $(MOPT) $(CDEBUGFLAGS) $(EXTRA_INCLUDES)
CXXFLAGS= $(MOPT) $(CXXDEBUGFLAGS) $(EXTRA_INCLUDES)
CXXLDOPTIONS= $(CXXDEBUGFLAGS)


CXXOBJS=	$(UOBJS)
OBJS=		$(CXXOBJS)

#CSRCS=		$(COBJS:.o=.c)
CXXSRCS=	$(CXXOBJS:.o=.cpp)
SRCS=		$(CXXSRCS)

.SUFFIXES: .c .cpp

.c.o:
	$(CC) -o $@ $(CFLAGS) -c $*.c

.cpp.o:
	$(CXX) -o $@ $(CXXFLAGS) -c $*.cpp

all:: $(TARGET_NAME)

$(TARGET_NAME): $(OBJS)
	$(RM) $@
	$(CXXLINK) $(MOPT) -o $@ $(CXXLDOPTIONS) $(OBJS) $(SDL_LIB) $(LDLIBS) $(GTK_LIB)

depend::
	$(DEPEND) -- $(CXXFLAGS) $(DEPEND_DEFINES) -- $(SRCS)

clean::
	$(RM) $(TARGET_NAME)
	$(RM) $(OBJS)
	$(RM) *.CKP *.ln *.BAK *.bak *.o core errs ,* *~ *.a .emacs_* tags TAGS make.log MakeOut   "#"*

tags::
	find . -name "*.h" -o -name "*.c" -o -name "*.cpp" | $(TAGS) -

