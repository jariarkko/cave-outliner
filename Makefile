
OBJS=	src/main.o \
	src/outlinerprocessor.o \
	src/outlinerindexedmesh.o \
	src/outlinermaterialmatrix.o \
	src/outlinerdescribe.o \
	src/outlinerdebug.o \
	src/outlinerdirection.o \
	src/outlinermath.o \
	src/outlinerhighprecision.o \
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
	src/outlinerhighprecision.hh \
	src/outlinersvg.hh
SRCS=	src/main.cc \
	src/outlinerdebug.cc \
	src/outlinerdirection.cc \
	src/outlinerprocessor.cc \
	src/outlinerindexedmesh.cc \
	src/outlinermaterialmatrix.cc \
	src/outlinerdescribe.cc \
	src/outlinermath.cc \
	src/outlinerhighprecision.cc \
	src/outlinersvg.cc
SUPP=Makefile
CPPFLAGS=-O3 -Wall -std=c++11 `pkg-config --cflags assimp`
LDFLAGS=-O3
LDLIBS=`pkg-config --libs-only-L assimp` -lassimp

all:	cave-outliner

.o.c:
	g++ $(CPPFLAGS) $< -o $>

$(OBJS): $(HDRS)

cave-outliner:	$(OBJS)
	g++ $(LDFLAGS) -o cave-outliner $(OBJS) $(LDLIBS)

test:	cave-outliner
	./cave-outliner --test

clean:
	rm -f cave-outliner */*.o

wc:
	wc $(HDRS) $(SRCS) $(SUPP)
