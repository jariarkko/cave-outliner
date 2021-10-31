##############################################################################################
##############################################################################################
##############################################################################################
##
##      CCC    AAA   V     V EEEEE       OOO   UU   UU TTTTTT LL    II NN   NN EEEEE RRRRR
##    CC   CC AA  AA  V   V  EE        OO   OO UU   UU   TT   LL    II NNN  NN EE    RR  RR
##    CC      AA  AA  V   V  EEE       OO   OO UU   UU   TT   LL    II NN N NN EEE   RRRRR
##    CC   CC AAAAAA   V V   EE        OO   OO UU   UU   TT   LL    II NN  NNN EE    RR R
##      CCc   AA  AA    V    EEEEE       OOO    UUUUU    TT   LLLLL II NN   NN EEEEE RR  R
##
##    CAVE OUTLINER -- Cave 3D model processing software
##
##    Copyright (C) 2021 by Jari Arkko -- See LICENSE.txt for license information.
##
##############################################################################################
##############################################################################################
##############################################################################################

OBJDIR=	src
OBJS=	$(OBJDIR)/main.o \
	$(OBJDIR)/mainconfig.o \
	$(OBJDIR)/mainoptions.o \
	$(OBJDIR)/outlinerprocessor.o \
	$(OBJDIR)/outlinerprocessorcrosssection.o \
	$(OBJDIR)/outlinerindexedmesh.o \
	$(OBJDIR)/outlinermaterialmatrix.o \
	$(OBJDIR)/outlinerdescribe.o \
	$(OBJDIR)/outlinerboundingboxer.o \
	$(OBJDIR)/outlinerdebug.o \
	$(OBJDIR)/outlinerdirection.o \
	$(OBJDIR)/outlinermath.o \
	$(OBJDIR)/outlinerhighprecision.o \
	$(OBJDIR)/outlinersvg.o \
	$(OBJDIR)/outlinerversion.o
HDRS=	src/main.hh \
	src/mainconfig.hh \
	src/mainoptions.hh \
	src/outlinertypes.hh \
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
	src/mainconfig.cc \
	src/mainoptions.cc \
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
CLASSES=outliner_math \
	main_config \
	main_options \
	bounding_boxer \
	describer \
	direction_operations \
	indexed_mesh \
	material_matrix \
	outliner_box2_d \
	outliner_box3_d \
	outliner_line2_d \
	outliner_line3_d \
	outliner_triangle2_d \
	outliner_triangle3_d \
	outliner_vector2_d \
	outliner_vector3_d \
	processor \
	processor_cross_section \
	svg_creator
CLASSMARKDOWNS=	doc/class_outliner_math.md \
		doc/class_main_config.md \
		doc/class_main_options.md \
		doc/class_bounding_boxer.md \
		doc/class_describer.md \
		doc/class_direction_operations.md \
		doc/class_indexed_mesh.md \
		doc/class_material_matrix.md \
		doc/class_outliner_box2_d.md \
		doc/class_outliner_box3_d.md \
		doc/class_outliner_line2_d.md \
		doc/class_outliner_line3_d.md \
		doc/class_outliner_triangle2_d.md \
		doc/class_outliner_triangle3_d.md \
		doc/class_outliner_vector2_d.md \
		doc/class_outliner_vector3_d.md \
		doc/class_processor.md \
		doc/class_processor_cross_section.md \
		doc/class_svg_creator.md
TOOLS=	doc/markdowncleanup.sh
SUPP=	Makefile $(TOOLS)
CPPFLAGS=-O3 -Wall -std=c++11 `pkg-config --cflags assimp`
#ISYSROOT=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.0.sdk 
LDFLAGS=-O3
LDLIBS=`pkg-config --libs-only-L assimp` -lassimp
#CPPCOMPILER=clang++
CPPCOMPILER=g++

all:	cave-outliner docs test

$(OBJDIR)/main.o:	src/main.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/main.o

$(OBJDIR)/mainconfig.o:	src/mainconfig.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/mainconfig.o

$(OBJDIR)/mainoptions.o:	src/mainoptions.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/mainoptions.o

$(OBJDIR)/outlinerprocessor.o:	src/outlinerprocessor.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinerprocessor.o

$(OBJDIR)/outlinerprocessorcrosssection.o:	src/outlinerprocessorcrosssection.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinerprocessorcrosssection.o

$(OBJDIR)/outlinerindexedmesh.o:	src/outlinerindexedmesh.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinerindexedmesh.o

$(OBJDIR)/outlinermaterialmatrix.o:	src/outlinermaterialmatrix.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinermaterialmatrix.o

$(OBJDIR)/outlinerdescribe.o:	src/outlinerdescribe.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinerdescribe.o

$(OBJDIR)/outlinerboundingboxer.o:	src/outlinerboundingboxer.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinerboundingboxer.o

$(OBJDIR)/outlinerdebug.o:	src/outlinerdebug.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinerdebug.o

$(OBJDIR)/outlinerdirection.o:	src/outlinerdirection.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinerdirection.o

$(OBJDIR)/outlinermath.o:	src/outlinermath.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinermath.o

$(OBJDIR)/outlinerhighprecision.o:	src/outlinerhighprecision.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinerhighprecision.o

$(OBJDIR)/outlinersvg.o:	src/outlinersvg.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinersvg.o

$(OBJDIR)/outlinerversion.o:	src/outlinerversion.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinerversion.o

$(OBJS): $(HDRS)

cave-outliner:	$(OBJS)
	$(CPPCOMPILER) $(LDFLAGS) -o cave-outliner $(OBJS) $(LDLIBS)

docs:	doc/Design-Structure-Small.jpg

docs-generation:	$(CLASSMARKDOWNS)

$(CLASSMARKDOWNS):	$(HDRS) Makefile $(TOOLS)
	-rm -rf doc/xml doc/html doc/search.json doc/index.html doc/latex
	doxygen doc/doxygen.cfg
	for x in $(CLASSES); do pandoc doc/html/class_$$x.html -f html -t markdown_strict -o /tmp/gen.md; sh doc/markdowncleanup.sh /tmp/gen.md doc/class_$$x.md; done
	-rm -rf doc/xml doc/html doc/search.json doc/index.html doc/latex

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
		house-pixel-side-test \
		house-pixel-another-side-test \
		house-line-test \
		cube-cross-section-simple-test \
		cube-reduced-cross-section-test \
		cube-reduced-highres-cross-section-test \
		cube-cross-section-test \
		cube-cross-section-label-test \
		cube-cross-section-highres-label-test \
		cube-cross-section-multiplier-label-test \
		house-cross-section-test \
		house-cross-section-highres-test

cube-pixel-test:
	@echo 'Running test case cube-pixel-test...'
	@./cave-outliner --quiet --pixel --step 0.5 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-pixel.svg
	@diff -q test/cube-pixel.svg test/cube-pixel.svg.expected

cube-line-test:
	@echo 'Running test case cube-line-test...'
	@./cave-outliner --quiet --borderline --step 0.5 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-line.svg
	@diff -q test/cube-line.svg test/cube-line.svg.expected

cube-line-defaultbounding-test:
	@echo 'Running test case cube-line-defaultbounding-test...'
	@./cave-outliner --quiet --borderline --step 0.5 test/cube.stl test/cube-line-defaultbounding.svg
	@diff -q test/cube-line-defaultbounding.svg test/cube-line-defaltbounding.svg.expected

cube-line-bounding-test:
	@echo 'Running test case cube-line-bounding-test...'
	@./cave-outliner --quiet --borderline --step 0.5 --bounding -10 10 -20 20 -3 3 test/cube.stl test/cube-line-bounding.svg
	@diff -q test/cube-line-bounding.svg test/cube-line-bounding.svg.expected

cube-angled-pixel-test:
	@echo 'Running test case cube-angled-pixel-test...'
	@./cave-outliner --quiet --pixel --step 0.1 --bounding -2 2 -2 2 -2 2 test/cube-angled.stl test/cube-angled-pixel.svg
	@diff -q test/cube-angled-pixel.svg test/cube-angled-pixel.svg.expected

cube-angled-line-test:
	@echo 'Running test case cube-angled-line-test...'
	@./cave-outliner --quiet --borderline --step 0.1 --bounding -2 2 -2 2 -2 2 test/cube-angled.stl test/cube-angled-line.svg
	@diff -q test/cube-angled-line.svg test/cube-angled-line.svg.expected

cube-angled-line-linewidth-test:
	@echo 'Running test case cube-angled-line-linewidth-test...'
	@./cave-outliner --quiet --borderline --step 0.1 --linewidth 4 --bounding -2 2 -2 2 -2 2 \
		test/cube-angled.stl test/cube-angled-line-linewidth.svg
	@diff -q test/cube-angled-line-linewidth.svg test/cube-angled-line-linewidth.svg.expected

cube-angled-line-multiplier-test:
	@echo 'Running test case cube-angled-line-multiplier-test...'
	@./cave-outliner --quiet --borderline --step 0.1 --multiplier 10 --bounding -2 2 -2 2 -2 2 \
		test/cube-angled.stl test/cube-angled-line-multiplier.svg
	@diff -q test/cube-angled-line-multiplier.svg test/cube-angled-line-multiplier.svg.expected

house-pixel-test:
	@echo 'Running test case house-pixel-test...'
	@./cave-outliner --quiet --pixel --step 0.2 --bounding -5 5 -5 5 -5 5 test/house.stl test/house-pixel.svg
	@diff -q test/house-pixel.svg test/house-pixel.svg.expected

house-pixel-side-test:
	@echo 'Running test case house-pixel-side-test...'
	@./cave-outliner --quiet --pixel --step 0.2 --y --bounding -5 5 -5 5 -5 5 test/house.stl test/house-pixel-side.svg
	@diff -q test/house-pixel-side.svg test/house-pixel-side.svg.expected

house-pixel-another-side-test:
	@echo 'Running test case house-pixel-another-side-test...'
	@./cave-outliner --quiet --pixel --step 0.2 --x --bounding -5 5 -5 5 -5 5 test/house.stl test/house-pixel-another-side.svg
	@diff -q test/house-pixel-another-side.svg test/house-pixel-another-side.svg.expected

house-line-test:
	@echo 'Running test case house-line-test...'
	@./cave-outliner --quiet --borderline --step 0.5 --bounding -5 5 -5 5 -5 5 test/house.stl test/house-line.svg
	@diff -q test/house-line.svg test/house-line.svg.expected

house-cross-section-test:
	@echo 'Running test case house-cross-section-test...'
	@./cave-outliner --quiet --pixel --crosssections 1 test/house-cross-section-%.svg --step 0.5 --bounding -5 5 -5 5 -5 5 test/house.stl test/house-cross-section.svg
	@diff -q test/house-cross-section.svg test/house-cross-section.svg.expected
	@diff -q test/house-cross-section-0.svg test/house-cross-section-0.svg.expected

house-cross-section-highres-test:
	@echo 'Running test case house-cross-section-highres-test...'
	@./cave-outliner --quiet --pixel --crosssections 1 test/house-cross-section-highres-%.svg --step 0.05 --bounding -5 5 -5 5 -5 5 test/house.stl test/house-cross-section-highres.svg
	@diff -q test/house-cross-section-highres.svg test/house-cross-section-highres.svg.expected
	@diff -q test/house-cross-section-highres-0.svg test/house-cross-section-highres-0.svg.expected

cube-cross-section-simple-test:
	@echo 'Running test case cube-cross-section-simple-test...'
	@./cave-outliner --quiet --pixel --crosssections 1 test/cube-cross-section-simple-%.svg --step 1 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-cross-section-simple.svg
	@diff -q test/cube-cross-section-simple.svg test/cube-cross-section-simple.svg.expected
	@diff -q test/cube-cross-section-simple-0.svg test/cube-cross-section-simple-0.svg.expected

cube-reduced-cross-section-test:
	@echo 'Running test case cube-reduced-cross-section-test...'
	@./cave-outliner --quiet --pixel --crosssections 1 test/cube-reduced-cross-section-%.svg --step 0.5 --bounding -2 2 -2 2 -2 2 test/cube-reduced.stl test/cube-reduced-cross-section.svg
	@diff -q test/cube-reduced-cross-section.svg test/cube-reduced-cross-section.svg.expected
	@diff -q test/cube-reduced-cross-section-0.svg test/cube-reduced-cross-section-0.svg.expected

cube-reduced-highres-cross-section-test:
	@echo 'Running test case cube-reduced-highres-cross-section-test...'
	@./cave-outliner --quiet --pixel --crosssections 1 test/cube-reduced-highres-cross-section-%.svg --step 0.1 --bounding -2 2 -2 2 -2 2 test/cube-reduced.stl test/cube-reduced-highres-cross-section.svg
	@diff -q test/cube-reduced-highres-cross-section.svg test/cube-reduced-highres-cross-section.svg.expected
	@diff -q test/cube-reduced-highres-cross-section-0.svg test/cube-reduced-highres-cross-section-0.svg.expected

cube-cross-section-test:
	@echo 'Running test case cube-cross-section-test...'
	@./cave-outliner --quiet --pixel --crosssections 1 test/cube-cross-section-%.svg --step 0.5 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-cross-section.svg
	@diff -q test/cube-cross-section.svg test/cube-cross-section.svg.expected
	@diff -q test/cube-cross-section-0.svg test/cube-cross-section-0.svg.expected

cube-cross-section-label-test:
	@echo 'Running test case cube-cross-section-label-test...'
	@./cave-outliner --quiet --pixel --label --crosssections 1 test/cube-cross-section-label-%.svg --step 0.5 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-cross-section-label.svg
	@diff -q test/cube-cross-section-label.svg test/cube-cross-section-label.svg.expected
	@diff -q test/cube-cross-section-label-0.svg test/cube-cross-section-label-0.svg.expected

cube-cross-section-highres-label-test:
	@echo 'Running test case cube-cross-section-highres-label-test...'
	@./cave-outliner --quiet --pixel --label --crosssections 1 test/cube-cross-section-highres-label-%.svg --step 0.05 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-cross-section-highres-label.svg
	@diff -q test/cube-cross-section-highres-label.svg test/cube-cross-section-highres-label.svg.expected
	@diff -q test/cube-cross-section-highres-label-0.svg test/cube-cross-section-highres-label-0.svg.expected

cube-cross-section-multiplier-label-test:
	@echo 'Running test case cube-cross-section-multiplier-label-test...'
	@./cave-outliner --quiet --pixel --label --crosssections 1 test/cube-cross-section-multiplier-label-%.svg --multiplier 10 --step 0.05 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-cross-section-multiplier-label.svg
	@diff -q test/cube-cross-section-multiplier-label.svg test/cube-cross-section-multiplier-label.svg.expected
	@diff -q test/cube-cross-section-multiplier-label-0.svg test/cube-cross-section-multiplier-label-0.svg.expected

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
	rm -f cave-outliner *.o */*.o test/*.svg
	rm -rf doc/xml doc/search.json doc/latex doc/html

wc:
	wc $(HDRS) $(SRCS) $(SUPP)
