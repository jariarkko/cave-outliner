
OBJS=	src/main.o \
	src/outlinerprocessor.o \
	src/outlinerindexedmesh.o \
	src/outlinermaterialmatrix.o \
	src/outlinerdescribe.o \
	src/outlinerboundingboxer.o \
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
	src/outlinerboundingboxer.hh \
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
	src/outlinerboundingboxer.cc \
	src/outlinermath.cc \
	src/outlinerhighprecision.cc \
	src/outlinersvg.cc
SUPP=Makefile
CPPFLAGS=-O3 -Wall -std=c++11 `pkg-config --cflags assimp`
LDFLAGS=-O3
LDLIBS=`pkg-config --libs-only-L assimp` -lassimp

all:	cave-outliner test

.o.c:
	g++ $(CPPFLAGS) $< -o $>

$(OBJS): $(HDRS)

cave-outliner:	$(OBJS)
	g++ $(LDFLAGS) -o cave-outliner $(OBJS) $(LDLIBS)

test:	cave-outliner \
	unit-tests \
	basic-tests

unit-tests:	cave-outliner
	./cave-outliner --test

basic-tests:	cave-outliner \
		cube-pixel-test \
		cube-line-test \
		cube-line-defaultbounding-test \
		cube-line-bounding-test \
		cube-angled-pixel-test \
		cube-angled-line-test  \
		cube-angled-line-linewidth-test  \
		cube-angled-line-multiplier-test

cube-pixel-test:
	./cave-outliner --pixel --step 0.5 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-pixel.svg
	diff test/cube-pixel.svg test/cube-pixel.svg.expected

cube-line-test:
	./cave-outliner --borderline --step 0.5 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-line.svg
	diff test/cube-line.svg test/cube-line.svg.expected

cube-line-defaultbounding-test:
	./cave-outliner --borderline --step 0.5 test/cube.stl test/cube-line-defaultbounding.svg
	diff test/cube-line-defaultbounding.svg test/cube-line-defaltbounding.svg.expected

cube-line-bounding-test:
	./cave-outliner --borderline --step 0.5 --bounding -10 10 -20 20 -3 3 test/cube.stl test/cube-line-bounding.svg
	diff test/cube-line-bounding.svg test/cube-line-bounding.svg.expected

cube-angled-pixel-test:
	./cave-outliner --pixel --step 0.1 --bounding -2 2 -2 2 -2 2 test/cube-angled.stl test/cube-angled-pixel.svg
	diff test/cube-angled-pixel.svg test/cube-angled-pixel.svg.expected

cube-angled-line-test:
	./cave-outliner --borderline --step 0.1 --bounding -2 2 -2 2 -2 2 test/cube-angled.stl test/cube-angled-line.svg
	diff test/cube-angled-line.svg test/cube-angled-line.svg.expected

cube-angled-line-linewidth-test:
	./cave-outliner --borderline --step 0.1 --linewidth 4 --bounding -2 2 -2 2 -2 2 \
		test/cube-angled.stl test/cube-angled-line-linewidth.svg
	diff test/cube-angled-line-linewidth.svg test/cube-angled-line-linewidth.svg.expected

cube-angled-line-multiplier-test:
	./cave-outliner --borderline --step 0.1 --multiplier 10 --bounding -2 2 -2 2 -2 2 \
		test/cube-angled.stl test/cube-angled-line-multiplier.svg
	diff test/cube-angled-line-multiplier.svg test/cube-angled-line-multiplier.svg.expected

clean:
	rm -f cave-outliner */*.o

wc:
	wc $(HDRS) $(SRCS) $(SUPP)
