
OBJS=	src/main.o \
	src/outlinerprocessor.o \
	src/outlinerindexedmesh.o \
	src/outlinerdescribe.o \
	src/outlinerdebug.o \
	src/outlinermath.o \
	src/outlinersvg.o
HDRS=	src/outlinertypes.hh \
	src/outlinerconstants.hh \
	src/outlinerdebug.hh \
	src/outlinerprocessor.hh \
	src/outlinerindexedmesh.hh \
	src/outlinerdescribe.hh \
	src/outlinermath.hh \
	src/outlinersvg.hh
SRCS=	src/main.cc \
	src/outlinerdebug.cc \
	src/outlinerprocessor.cc \
	src/outlinerindexedmesh.cc \
	src/outlinerdescribe.cc \
	src/outlinermath.cc \
	src/outlinersvg.cc
SUPP=Makefile
CPPFLAGS=-g -Wall
LDFLAGS=-O4
LDLIBS=-lassimp

all:	cave-outliner

.o.c:
	g++ $(CPPFLAGS) $< -o $>

$(OBJS): $(HDRS) $(SRCS)

cave-outliner:	$(OBJS)
	g++ $(LDFLAGS) -o cave-outliner $(OBJS) $(LDLIBS)

wc:
	wc $(HDRS) $(SRCS) $(SUPP)
