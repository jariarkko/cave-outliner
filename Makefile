
OBJS=	src/main.o \
	src/outlinerprocessor.o \
	src/outlinerprocessorcrosssection.o \
	src/outlinerindexedmesh.o \
	src/outlinermaterialmatrix.o \
	src/outlinerdescribe.o \
	src/outlinerboundingboxer.o \
	src/outlinerdebug.o \
	src/outlinerdirection.o \
	src/outlinermath.o \
	src/outlinerhighprecision.o \
	src/outlinersvg.o \
	src/outlinerversion.o
HDRS=	src/outlinertypes.hh \
	src/outlinerconstants.hh \
	src/outlinerdebug.hh \
	src/outlinerdirection.hh \
	src/outlinerprocessor.hh \
	src/outlinerprocessorcrosssection.hh \
	src/outlinerindexedmesh.hh \
	src/outlinermaterialmatrix.hh \
	src/outlinerdescribe.hh \
	src/outlinerboundingboxer.hh \
	src/outlinermath.hh \
	src/outlinerhighprecision.hh \
	src/outlinersvg.hh \
	src/outlinerversion.hh
SRCS=	src/main.cc \
	src/outlinerdebug.cc \
	src/outlinerdirection.cc \
	src/outlinerprocessor.cc \
	src/outlinerprocessorcrosssection.cc \
	src/outlinerindexedmesh.cc \
	src/outlinermaterialmatrix.cc \
	src/outlinerdescribe.cc \
	src/outlinerboundingboxer.cc \
	src/outlinermath.cc \
	src/outlinerhighprecision.cc \
	src/outlinersvg.cc \
	src/outlinerversion.cc
SUPP=Makefile
CPPFLAGS=-O3 -Wall -std=c++11 `pkg-config --cflags assimp`
LDFLAGS=-O3
LDLIBS=`pkg-config --libs-only-L assimp` -lassimp

all:	cave-outliner docs test

.o.c:
	g++ $(CPPFLAGS) $< -o $>

$(OBJS): $(HDRS)

cave-outliner:	$(OBJS)
	g++ $(LDFLAGS) -o cave-outliner $(OBJS) $(LDLIBS)

docs:	doc/Design-Structure-Small.jpg

doc/Design-Structure-Small.jpg:	doc/Design-Structure.jpg
	convert -quality 0.97 -resize 1400x doc/Design-Structure.jpg doc/Design-Structure-Small.jpg

test:	cave-outliner \
	unit-tests \
	basic-tests

unit-tests:	cave-outliner
	@echo 'Running test case unit-tests...'
	@./cave-outliner --quiet --test

basic-tests:	cave-outliner \
		cube-pixel-test \
		cube-line-test \
		cube-line-defaultbounding-test \
		cube-line-bounding-test \
		cube-angled-pixel-test \
		cube-angled-line-test  \
		cube-angled-line-linewidth-test  \
		cube-angled-line-multiplier-test \
		house-pixel-test \
		house-line-test \
		cube-cross-section-simple-test \
		cube-reduced-cross-section-test \
		cube-cross-section-test \
		cube-cross-section-label-test \
		cube-cross-section-highres-label-test \
		house-cross-section-test

cube-pixel-test:
	@echo 'Running test case cube-pixel-test...'
	@./cave-outliner --quiet --pixel --step 0.5 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-pixel.svg
	@diff test/cube-pixel.svg test/cube-pixel.svg.expected

cube-line-test:
	@echo 'Running test case cube-line-test...'
	@./cave-outliner --quiet --borderline --step 0.5 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-line.svg
	@diff test/cube-line.svg test/cube-line.svg.expected

cube-line-defaultbounding-test:
	@echo 'Running test case cube-line-defaultbounding-test...'
	@./cave-outliner --quiet --borderline --step 0.5 test/cube.stl test/cube-line-defaultbounding.svg
	@diff test/cube-line-defaultbounding.svg test/cube-line-defaltbounding.svg.expected

cube-line-bounding-test:
	@echo 'Running test case cube-line-bounding-test...'
	@./cave-outliner --quiet --borderline --step 0.5 --bounding -10 10 -20 20 -3 3 test/cube.stl test/cube-line-bounding.svg
	@diff test/cube-line-bounding.svg test/cube-line-bounding.svg.expected

cube-angled-pixel-test:
	@echo 'Running test case cube-angled-pixel-test...'
	@./cave-outliner --quiet --pixel --step 0.1 --bounding -2 2 -2 2 -2 2 test/cube-angled.stl test/cube-angled-pixel.svg
	@diff test/cube-angled-pixel.svg test/cube-angled-pixel.svg.expected

cube-angled-line-test:
	@echo 'Running test case cube-angled-line-test...'
	@./cave-outliner --quiet --borderline --step 0.1 --bounding -2 2 -2 2 -2 2 test/cube-angled.stl test/cube-angled-line.svg
	@diff test/cube-angled-line.svg test/cube-angled-line.svg.expected

cube-angled-line-linewidth-test:
	@echo 'Running test case cube-angled-line-linewidth-test...'
	@./cave-outliner --quiet --borderline --step 0.1 --linewidth 4 --bounding -2 2 -2 2 -2 2 \
		test/cube-angled.stl test/cube-angled-line-linewidth.svg
	@diff test/cube-angled-line-linewidth.svg test/cube-angled-line-linewidth.svg.expected

cube-angled-line-multiplier-test:
	@echo 'Running test case cube-angled-line-multiplier-test...'
	@./cave-outliner --quiet --borderline --step 0.1 --multiplier 10 --bounding -2 2 -2 2 -2 2 \
		test/cube-angled.stl test/cube-angled-line-multiplier.svg
	@diff test/cube-angled-line-multiplier.svg test/cube-angled-line-multiplier.svg.expected

house-pixel-test:
	@echo 'Running test case house-pixel-test...'
	@./cave-outliner --quiet --pixel --step 0.2 --y --bounding -5 5 -5 5 -5 5 test/house.stl test/house-pixel.svg
	@diff test/house-pixel.svg test/house-pixel.svg.expected

house-line-test:
	@echo 'Running test case house-line-test...'
	@./cave-outliner --quiet --borderline --step 0.5 --bounding -5 5 -5 5 -5 5 test/house.stl test/house-line.svg
	@diff test/house-line.svg test/house-line.svg.expected

house-cross-section-test:
	@echo 'Running test case house-cross-section-test...'
	@./cave-outliner --quiet --pixel --crosssections 1 test/house-cross-section-%.svg --step 0.2 --bounding -5 5 -5 5 -5 5 test/house.stl test/house-cross-section.svg
	@diff test/house-cross-section.svg test/house-cross-section.svg.expected
	@diff test/house-cross-section-0.svg test/house-cross-section-0.svg.expected

cube-cross-section-simple-test:
	@echo 'Running test case cube-cross-section-simple-test...'
	@./cave-outliner --quiet --pixel --crosssections 1 test/cube-cross-section-simple-%.svg --step 1 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-cross-section-simple.svg
	@diff test/cube-cross-section-simple.svg test/cube-cross-section-simple.svg.expected
	@diff test/cube-cross-section-simple-0.svg test/cube-cross-section-simple-0.svg.expected

cube-reduced-cross-section-test:
	@echo 'Running test case cube-reduced-cross-section-test...'
	@./cave-outliner --quiet --pixel --crosssections 1 test/cube-reduced-cross-section-%.svg --step 0.5 --bounding -2 2 -2 2 -2 2 test/cube-reduced.stl test/cube-reduced-cross-section.svg
	@diff test/cube-reduced-cross-section.svg test/cube-reduced-cross-section.svg.expected
	@diff test/cube-reduced-cross-section-0.svg test/cube-reduced-cross-section-0.svg.expected

cube-cross-section-test:
	@echo 'Running test case cube-cross-section-test...'
	@./cave-outliner --quiet --pixel --crosssections 1 test/cube-cross-section-%.svg --step 0.5 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-cross-section.svg
	@diff test/cube-cross-section.svg test/cube-cross-section.svg.expected
	@diff test/cube-cross-section-0.svg test/cube-cross-section-0.svg.expected

cube-cross-section-label-test:
	@echo 'Running test case cube-cross-section-label-test...'
	@./cave-outliner --quiet --pixel --label --crosssections 1 test/cube-cross-section-label-%.svg --step 0.5 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-cross-section-label.svg
	@diff test/cube-cross-section-label.svg test/cube-cross-section-label.svg.expected
	@diff test/cube-cross-section-label-0.svg test/cube-cross-section-label-0.svg.expected

cube-cross-section-highres-label-test:
	@echo 'Running test case cube-cross-section-high-res-label-test...'
	@./cave-outliner --quiet --pixel --label --crosssections 1 test/cube-cross-section-highres-label-%.svg --step 0.05 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-cross-section-highres-label.svg
	@diff test/cube-cross-section-highres-label.svg test/cube-cross-section-highres-label.svg.expected
	@diff test/cube-cross-section-highres-label-0.svg test/cube-cross-section-highres-label-0.svg.expected

updateversion:
	@echo This makefile target updates one software source file based on tags in GitHub,
	@echo to set the version number of the software.
	@echo ''
	@echo Only run this if you know what you are doing. Press Control-C now otherwise.
	@sleep 5
	@git describe > /tmp/ver.res
	@sed 's/ outlinerVersion = .*;/ outlinerVersion = "'`cat /tmp/ver.res`'";/' < src/outlinerversion.cc > /tmp/ver.cc
	cp -i /tmp/ver.cc src/outlinerversion.cc

clean:
	rm -f cave-outliner */*.o test/*.svg

wc:
	wc $(HDRS) $(SRCS) $(SUPP)
