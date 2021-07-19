
OBJS=	src/main.o \
	src/outlinerprocessor.o \
	src/outlinerindexedmesh.o \
	src/outlinermaterialmatrix.o \
	src/outlinerdescribe.o \
	src/outlinerdebug.o \
	src/outlinerdirection.o \
	src/outlinermath.o \
	src/outlinersvg.o
HDRS=	src/outlinertypes.hh \
	src/outlinerconstants.hh \
	src/outlinerdebug.hh \
	src/outlinerdirection.hh \
	src/outlinerprocessor.hh \
	src/outlinerindexedmesh.hh \
	src/outlinermaterialmatrix.hh \
	src/outlinerdescribe.hh \
	src/outlinermath.hh \
	src/outlinersvg.hh
SRCS=	src/main.cc \
	src/outlinerdebug.cc \
	src/outlinerdirection.cc \
	src/outlinerprocessor.cc \
	src/outlinerindexedmesh.cc \
	src/outlinermaterialmatrix.cc \
	src/outlinerdescribe.cc \
	src/outlinermath.cc \
	src/outlinersvg.cc
SUPP=Makefile
CPPFLAGS=-g -Wall -std=c++11 `pkg-config --cflags assimp`
LDFLAGS=-O4
LDLIBS=`pkg-config --libs-only-L assimp` -lassimp

all:	cave-outliner

.o.c:
	g++ $(CPPFLAGS) $< -o $>

$(OBJS): $(HDRS) $(SRCS)

cave-outliner:	$(OBJS)
	g++ $(LDFLAGS) -o cave-outliner $(OBJS) $(LDLIBS)

wc:
	wc $(HDRS) $(SRCS) $(SUPP)
