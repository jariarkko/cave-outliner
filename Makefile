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
	$(OBJDIR)/mainoptionshelp.o \
	$(OBJDIR)/maintest.o \
	$(OBJDIR)/outlinerprocessor.o \
	$(OBJDIR)/outlinerprocessorcrosssection.o \
	$(OBJDIR)/outlinerprocessoroptions.o \
	$(OBJDIR)/outlinerprocessorforms.o \
	$(OBJDIR)/outlinercomposer.o \
	$(OBJDIR)/outlineroutlineanalyzer.o \
	$(OBJDIR)/outlinershaperecognizer.o \
	$(OBJDIR)/outlinerindexedmesh.o \
	$(OBJDIR)/outlinermaterialmatrix2d.o \
	$(OBJDIR)/outlinermaterialmatrix3d.o \
	$(OBJDIR)/outlinerformmatrix2d.o \
	$(OBJDIR)/outlinerdepthmap.o \
	$(OBJDIR)/outlinerdescribe.o \
	$(OBJDIR)/outlinerboundingboxer.o \
	$(OBJDIR)/outlinerdebug.o \
	$(OBJDIR)/outlinerdirection.o \
	$(OBJDIR)/outlinertemp.o \
	$(OBJDIR)/outlinermath.o \
	$(OBJDIR)/outlinerhighprecision.o \
	$(OBJDIR)/outlinersvg.o \
	$(OBJDIR)/outlinersvgreader.o \
	$(OBJDIR)/outlinersvgstacker.o \
	$(OBJDIR)/outlinersvgoptions.o \
	$(OBJDIR)/outlinerversion.o
HDRS=	src/main.hh \
	src/mainconfig.hh \
	src/mainoptions.hh \
	src/maintest.hh \
	src/outlinertypes.hh \
	src/outlinerconstants.hh \
	src/outlinerdebug.hh \
	src/outlinerdirection.hh \
	src/outlinertemp.hh \
	src/outlinerprocessor.hh \
	src/outlinerprocessorcrosssection.hh \
	src/outlinerprocessoroptions.hh \
	src/outlinerprocessorforms.hh \
	src/outlinercomposer.hh \
	src/outlineroutlineanalyzer.hh \
	src/outlinershaperecognizer.hh \
	src/outlinerindexedmesh.hh \
	src/outlinermaterialmatrix2d.hh \
	src/outlinermaterialmatrix3d.hh \
	src/outlinerformmatrix2d.hh \
	src/outlinerdepthmap.hh \
	src/outlinerdescribe.hh \
	src/outlinerboundingboxer.hh \
	src/outlinermath.hh \
	src/outlinerhighprecision.hh \
	src/outlinersvg.hh \
	src/outlinersvgreader.hh \
	src/outlinersvgstacker.hh \
	src/outlinersvgoptions.hh \
	src/outlinerversion.hh
SRCS=	src/main.cc \
	src/mainconfig.cc \
	src/mainoptions.cc \
	src/mainoptionshelp.cc \
	src/maintest.cc \
	src/outlinerdebug.cc \
	src/outlinerdirection.cc \
	src/outlinertemp.cc \
	src/outlinerprocessor.cc \
	src/outlinerprocessorcrosssection.cc \
	src/outlinerprocessoroptions.cc \
	src/outlinerprocessorforms.cc \
	src/outlinercomposer.cc \
	src/outlineroutlineanalyzer.cc \
	src/outlinershaperecognizer.cc \
	src/outlinerindexedmesh.cc \
	src/outlinermaterialmatrix2d.cc \
	src/outlinermaterialmatrix3d.cc \
	src/outlinerformmatrix2d.cc \
	src/outlinerdepthmap.cc \
	src/outlinerdescribe.cc \
	src/outlinerboundingboxer.cc \
	src/outlinermath.cc \
	src/outlinerhighprecision.cc \
	src/outlinersvg.cc \
	src/outlinersvgreader.cc \
	src/outlinersvgstacker.cc \
	src/outlinersvgoptions.cc \
	src/outlinerversion.cc
CLASSES=outliner_math \
	main_config \
	main_options \
	main_test \
	bounding_boxer \
	describer \
	direction_operations \
	temp_filer \
	indexed_mesh \
	material_matrix2_d \
	material_matrix3_d \
	form_matrix2_d \
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
	processor_options \
	processor_forms \
	composer \
	outline_analyzer \
	shape_recognizer \
	svg_creator \
	svg_stacker \
	svg_reader
CLASSMARKDOWNS=	doc/software/class_outliner_math.md \
		doc/software/class_main_config.md \
		doc/software/class_main_options.md \
		doc/software/class_bounding_boxer.md \
		doc/software/class_describer.md \
		doc/software/class_direction_operations.md \
		doc/software/class_temp_filer.md \
		doc/software/class_indexed_mesh.md \
		doc/software/class_material_matrix2_d.md \
		doc/software/class_material_matrix3_d.md \
		doc/software/class_form_matrix2_d.md \
		doc/software/class_depth_map.md \
		doc/software/class_outliner_box2_d.md \
		doc/software/class_outliner_box3_d.md \
		doc/software/class_outliner_line2_d.md \
		doc/software/class_outliner_line3_d.md \
		doc/software/class_outliner_triangle2_d.md \
		doc/software/class_outliner_triangle3_d.md \
		doc/software/class_outliner_vector2_d.md \
		doc/software/class_outliner_vector3_d.md \
		doc/software/class_processor.md \
		doc/software/class_processor_cross_section.md \
		doc/software/class_processor_options.md \
		doc/software/class_processor_forms.md \
		doc/software/class_composer.md \
		doc/software/class_outline_analyzer.md \
		doc/software/class_shape_recognizer.md \
		doc/software/class_svg_creator.md \
		doc/software/class_svg_reader.md \
		doc/software/class_svg_stacker.md \
		doc/software/class_svg_options.md
TOOLS=	doc/tools/markdowncleanup.sh
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

$(OBJDIR)/mainoptionshelp.o:	src/mainoptionshelp.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/mainoptionshelp.o

$(OBJDIR)/maintest.o:	src/maintest.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/maintest.o

$(OBJDIR)/outlinerprocessor.o:	src/outlinerprocessor.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinerprocessor.o

$(OBJDIR)/outlinerprocessorcrosssection.o:	src/outlinerprocessorcrosssection.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinerprocessorcrosssection.o

$(OBJDIR)/outlinerprocessoroptions.o:	src/outlinerprocessoroptions.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinerprocessoroptions.o

$(OBJDIR)/outlinerprocessorforms.o:	src/outlinerprocessorforms.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinerprocessorforms.o

$(OBJDIR)/outlinercomposer.o:	src/outlinercomposer.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinercomposer.o

$(OBJDIR)/outlineroutlineanalyzer.o:	src/outlineroutlineanalyzer.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlineroutlineanalyzer.o

$(OBJDIR)/outlinershaperecognizer.o:	src/outlinershaperecognizer.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinershaperecognizer.o

$(OBJDIR)/outlinerindexedmesh.o:	src/outlinerindexedmesh.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinerindexedmesh.o

$(OBJDIR)/outlinermaterialmatrix2d.o:	src/outlinermaterialmatrix2d.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinermaterialmatrix2d.o

$(OBJDIR)/outlinermaterialmatrix3d.o:	src/outlinermaterialmatrix3d.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinermaterialmatrix3d.o

$(OBJDIR)/outlinerformmatrix2d.o:	src/outlinerformmatrix2d.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinerformmatrix2d.o

$(OBJDIR)/outlinerdepthmap.o:	src/outlinerdepthmap.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinerdepthmap.o

$(OBJDIR)/outlinerdescribe.o:	src/outlinerdescribe.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinerdescribe.o

$(OBJDIR)/outlinerboundingboxer.o:	src/outlinerboundingboxer.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinerboundingboxer.o

$(OBJDIR)/outlinerdebug.o:	src/outlinerdebug.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinerdebug.o

$(OBJDIR)/outlinerdirection.o:	src/outlinerdirection.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinerdirection.o

$(OBJDIR)/outlinertemp.o:	src/outlinertemp.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinertemp.o

$(OBJDIR)/outlinermath.o:	src/outlinermath.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinermath.o

$(OBJDIR)/outlinerhighprecision.o:	src/outlinerhighprecision.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinerhighprecision.o

$(OBJDIR)/outlinersvg.o:	src/outlinersvg.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinersvg.o

$(OBJDIR)/outlinersvgreader.o:	src/outlinersvgreader.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinersvgreader.o

$(OBJDIR)/outlinersvgstacker.o:	src/outlinersvgstacker.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinersvgstacker.o

$(OBJDIR)/outlinersvgoptions.o:	src/outlinersvgoptions.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinersvgoptions.o

$(OBJDIR)/outlinerversion.o:	src/outlinerversion.cc $(HDRS)
	$(CPPCOMPILER) $(CPPFLAGS) -c $< -o $(OBJDIR)/outlinerversion.o

$(OBJS): $(HDRS)

cave-outliner:	$(OBJS)
	$(CPPCOMPILER) $(LDFLAGS) -o cave-outliner $(OBJS) $(LDLIBS)

docs:	doc/images/Design-Structure-Small.jpg \
	doc/images/example1-model-small.jpg \
	doc/images/example1-planview-small.jpg \
	doc/images/example1-crosssection-small.jpg \
	doc/images/example1-longplanview-small.jpg \
	doc/images/example1-longcrosssection-small.jpg

docs-generation:	$(CLASSMARKDOWNS)

$(CLASSMARKDOWNS):	$(HDRS) Makefile $(TOOLS)
	-rm -rf doc/software/xml doc/software/html doc/software/search.json doc/software/index.html doc/software/latex
	doxygen doc/tools/doxygen.cfg
	for x in $(CLASSES); do pandoc doc/software/html/class_$$x.html -f html -t markdown_strict -o /tmp/gen.md; sh doc/tools/markdowncleanup.sh /tmp/gen.md doc/software/class_$$x.md; done
	-rm -rf doc/software/xml doc/software/html doc/software/search.json doc/software/index.html doc/software/latex

doc/images/Design-Structure-Small.jpg:	doc/images/Design-Structure.jpg Makefile
	convert -quality 0.97 -resize 1400x doc/images/Design-Structure.jpg doc/images/Design-Structure-Small.jpg

doc/images/example1-model-small.jpg:	doc/images/example1-model.jpg Makefile
	convert -quality 0.97 -resize 800x doc/images/example1-model.jpg doc/images/example1-model-small.jpg

doc/images/example1-planview-small.jpg:	doc/images/example1-planview.jpg Makefile
	convert -quality 0.97 -resize 800x doc/images/example1-planview.jpg doc/images/example1-planview-small.jpg

doc/images/example1-crosssection-small.jpg:	doc/images/example1-crosssection.jpg Makefile
	convert -quality 0.97 -resize 400x doc/images/example1-crosssection.jpg doc/images/example1-crosssection-small.jpg

doc/images/example1-longplanview-small.jpg:	doc/images/example1-longplanview.jpg Makefile
	convert -quality 0.97 -resize 800x doc/images/example1-longplanview.jpg doc/images/example1-longplanview-small.jpg

doc/images/example1-longcrosssection-small.jpg:	doc/images/example1-longcrosssection.jpg Makefile
	convert -quality 0.97 -resize 800x doc/images/example1-longcrosssection.jpg doc/images/example1-longcrosssection-small.jpg

test:	cave-outliner \
	unit-tests \
	basic-tests \
	cave-tests \
	heavy-tests \
	example-tests \
	failing-tests

unit-tests:	cave-outliner
	@echo 'Running test case unit-tests...'
	@./cave-outliner --quiet --test
	@diff /tmp/cave-outliner-unit-test-depthmap.svg test/cave-outliner-unit-test-depthmap.svg.expected
	@diff /tmp/cave-outliner-unit-test-depthdiffmap.svg test/cave-outliner-unit-test-depthdiffmap.svg.expected

basic-tests:	cave-outliner \
		empty-test \
		empty-bounding-test \
		cube-pixel-test \
		cube-line-test \
		cube-pixel-defaultbounding-test \
		cube-line-defaultbounding-test \
		cube-line-bounding-test \
		cube-triangle-test \
		cube-full-analysis-form-test \
		cube-full-analysis-test \
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
		cube-cross-section-test \
		cube-cross-section-dimensions-test \
		cube-cross-section-dimensions-highres-test \
		cube-cross-section-dimensions-highres-multiplier-test \
		cube-cross-section-highres-test \
		cube-cross-section-label-test \
		cube-cross-section-highres-label-test \
		cube-cross-section-multiplier-label-test \
		house-cross-section-test \
		steps-test \
		steps-cross-section-test \
		steps-depthmap-test \
		steps-depthdiffmap-test

cave-tests:	cave1-test \
		cave1-line-test \
		cave1-x-cross-section-test \
		cave1-x-cross-section-width-test \
		cave1-y-cross-section-test \
		cave1-depthmap-test \
		cave1-depthmap-cut-test \
		cave1-depthdiffmap-cut-test \
		cave1-depthmap-highres-test \
		cave1-depthdiffmap-test \
		cave1-form-analysis-test \
		cave1-form-analysis-pixel-test \
		cave1-form-analysis-line-test \
		cave1-form-analysis-smooth-line-test \
		cave1-spine-test \
		cave1-floor-test \
		cave1-floor-depth-test \
		cave1-composite-test \
		cave1-composite-multiplier-test

heavy-tests:	heavy-tests-note \
		cave1-floor-highres-test \
		cave2-form-test

failing-tests:	failing-tests-note \
		cave1-floor-condense-test \
		cave1-floor-condense-more-test \
		house-cross-section-side-test \
		house-cross-section-another-side-test \
		house-cross-section-highres-test

heavy-tests-note:
	@echo ''
	@echo 'Note: The subsequent tests may take some minutes'
	@echo ''

failing-tests-note:
	@echo ''
	@echo 'Note: The subsequent tests may fail'
	@echo ''

empty-test:
	@echo 'Running test case empty-test...'
	-@./cave-outliner --quiet --borderline --formanalysis 2 --step 0.5 test/empty.stl test/empty.svg > test/empty.out 2>&1
	@diff -q test/empty.out test/empty.out.expected

empty-bounding-test:
	@echo 'Running test case empty-bounding-test...'
	-@./cave-outliner --quiet --borderline --label --floordepthmap test/empty-bounding-floor.svg --roofdepthmap test/empty-bounding-roof.svg --formanalysis 2 --tunnelspine --step 0.1 --bounding 5 7 5 7 5 7 test/cube.stl test/empty-bounding.svg > test/empty-bounding.out 2>&1
	@diff -q test/empty-bounding.svg test/empty-bounding.svg.expected
	@diff -q test/empty-bounding.out test/empty-bounding.out.expected
	@diff -q test/empty-bounding-roof.svg test/empty-bounding-roof.svg.expected
	@diff -q test/empty-bounding-floor.svg test/empty-bounding-floor.svg.expected

cube-pixel-test:
	@echo 'Running test case cube-pixel-test...'
	@./cave-outliner --quiet --pixel --step 0.5 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-pixel.svg
	@diff -q test/cube-pixel.svg test/cube-pixel.svg.expected

cube-line-test:
	@echo 'Running test case cube-line-test...'
	@./cave-outliner --quiet --borderline --step 0.5 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-line.svg
	@diff -q test/cube-line.svg test/cube-line.svg.expected

cube-pixel-defaultbounding-test:
	@echo 'Running test case cube-pixel-defaultbounding-test...'
	@./cave-outliner --quiet --pixel --step 0.5 test/cube.stl test/cube-pixel-defaultbounding.svg
	@diff -q test/cube-pixel-defaultbounding.svg test/cube-pixel-defaultbounding.svg.expected

cube-line-defaultbounding-test:
	@echo 'Running test case cube-line-defaultbounding-test...'
	@./cave-outliner --quiet --borderline --step 0.5 test/cube.stl test/cube-line-defaultbounding.svg
	@diff -q test/cube-line-defaultbounding.svg test/cube-line-defaultbounding.svg.expected

cube-line-bounding-test:
	@echo 'Running test case cube-line-bounding-test...'
	@./cave-outliner --quiet --borderline --step 0.5 --bounding -10 10 -20 20 -3 3 test/cube.stl test/cube-line-bounding.svg
	@diff -q test/cube-line-bounding.svg test/cube-line-bounding.svg.expected

cube-triangle-test:
	@echo 'Running test case cube-triangle-test...'
	@./cave-outliner --quiet --triangle --multiplier 10 --step 0.5 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-triangle.svg
	@diff -q test/cube-triangle.svg test/cube-triangle.svg.expected

cube-full-analysis-form-test:
	@echo 'Running test case cube-full-analysis-form-test...'
	@./cave-outliner --quiet --label --formanalysis 1 --pixelform --step 0.05 test/cube.stl test/cube-full-analysis-form.svg
	@diff -q test/cube-full-analysis-form.svg test/cube-full-analysis-form.svg.expected

cube-full-analysis-test:
	@echo 'Running test case cube-full-analysis-test...'
	@./cave-outliner --quiet --borderline --label --formanalysis 1 --floordepthmap test/cube-full-analysis-floor.svg --roofdepthmap test/cube-full-analysis-roof.svg --step 0.05 test/cube.stl test/cube-full-analysis.svg
	@diff -q test/cube-full-analysis.svg test/cube-full-analysis.svg.expected
	@diff -q test/cube-full-analysis-floor.svg test/cube-full-analysis-floor.svg.expected
	@diff -q test/cube-full-analysis-roof.svg test/cube-full-analysis-roof.svg.expected

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
	@./cave-outliner --quiet --pixel --crosssections y 1 test/house-cross-section-%.svg --step 1 --bounding -5 5 -5 5 -5 5 test/house.stl test/house-cross-section.svg
	@diff -q test/house-cross-section.svg test/house-cross-section.svg.expected
	@diff -q test/house-cross-section-0.svg test/house-cross-section-0.svg.expected

house-cross-section-side-test:
	@echo 'Running test case house-cross-section-side-test...'
	@./cave-outliner --quiet --pixel --crosssections x 1 test/house-cross-section-side-%.svg --step 0.5 --bounding -5 5 -5 5 -5 5 test/house.stl test/house-cross-section-side.svg
	@diff -q test/house-cross-section-side.svg test/house-cross-section-side.svg.expected
	@diff -q test/house-cross-section-side-0.svg test/house-cross-section-side-0.svg.expected

house-cross-section-another-side-test:
	@echo 'Running test case house-cross-section-another-side-test...'
	@./cave-outliner --quiet --pixel --y --crosssections x 1 test/house-cross-section-another-side-%.svg --step 0.5 --bounding -5 5 -5 5 -5 5 test/house.stl test/house-cross-section-another-side.svg
	@diff -q test/house-cross-section-another-side.svg test/house-cross-section-another-side.svg.expected
	@diff -q test/house-cross-section-another-side-0.svg test/house-cross-section-another-side-0.svg.expected

house-cross-section-highres-test:
	@echo 'Running test case house-cross-section-highres-test...'
	@./cave-outliner --quiet --pixel --crosssections y 1 test/house-cross-section-highres-%.svg --step 0.05 --bounding -5 5 -5 5 -5 5 test/house.stl test/house-cross-section-highres.svg
	@diff -q test/house-cross-section-highres.svg test/house-cross-section-highres.svg.expected
	@diff -q test/house-cross-section-highres-0.svg test/house-cross-section-highres-0.svg.expected

cube-cross-section-simple-test:
	@echo 'Running test case cube-cross-section-simple-test...'
	@./cave-outliner --quiet --pixel --dustthreshold 0 --crosssections x 1 test/cube-cross-section-simple-%.svg --step 1 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-cross-section-simple.svg
	@diff -q test/cube-cross-section-simple.svg test/cube-cross-section-simple.svg.expected
	@diff -q test/cube-cross-section-simple-0.svg test/cube-cross-section-simple-0.svg.expected

cube-reduced-cross-section-test:
	@echo 'Running test case cube-reduced-cross-section-test...'
	@./cave-outliner --quiet --pixel --crosssections x 1 test/cube-reduced-cross-section-%.svg --step 0.5 --bounding -2 2 -2 2 -2 2 test/cube-reduced.stl test/cube-reduced-cross-section.svg
	@diff -q test/cube-reduced-cross-section.svg test/cube-reduced-cross-section.svg.expected
	@diff -q test/cube-reduced-cross-section-0.svg test/cube-reduced-cross-section-0.svg.expected

cube-reduced-highres-cross-section-test:
	@echo 'Running test case cube-reduced-highres-cross-section-test...'
	@./cave-outliner --quiet --pixel --crosssections x 1 test/cube-reduced-highres-cross-section-%.svg --step 0.1 --bounding -2 2 -2 2 -2 2 test/cube-reduced.stl test/cube-reduced-highres-cross-section.svg
	@diff -q test/cube-reduced-highres-cross-section.svg test/cube-reduced-highres-cross-section.svg.expected
	@diff -q test/cube-reduced-highres-cross-section-0.svg test/cube-reduced-highres-cross-section-0.svg.expected

cube-cross-section-test:
	@echo 'Running test case cube-cross-section-test...'
	@./cave-outliner --quiet --pixel --crosssections x 1 test/cube-cross-section-%.svg --step 0.5 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-cross-section.svg
	@diff -q test/cube-cross-section.svg test/cube-cross-section.svg.expected
	@diff -q test/cube-cross-section-0.svg test/cube-cross-section-0.svg.expected

cube-cross-section-dimensions-test:
	@echo 'Running test case cube-cross-section-dimensions-test...'
	@./cave-outliner --quiet --pixel --dimensions --crosssections x 1 test/cube-cross-section-dimensions-%.svg --step 0.5 test/cube.stl test/cube-cross-section-dimensions.svg
	@diff -q test/cube-cross-section-dimensions.svg test/cube-cross-section-dimensions.svg.expected
	@diff -q test/cube-cross-section-dimensions-0.svg test/cube-cross-section-dimensions-0.svg.expected

cube-cross-section-dimensions-highres-test:
	@echo 'Running test case cube-cross-section-dimensions-highres-test...'
	@./cave-outliner --quiet --pixel --dimensions --crosssections x 1 test/cube-cross-section-dimensions-highres-%.svg --step 0.05 test/cube.stl test/cube-cross-section-dimensions-highres.svg
	@diff -q test/cube-cross-section-dimensions-highres.svg test/cube-cross-section-dimensions-highres.svg.expected
	@diff -q test/cube-cross-section-dimensions-highres-0.svg test/cube-cross-section-dimensions-highres-0.svg.expected

cube-cross-section-dimensions-highres-multiplier-test:
	@echo 'Running test case cube-cross-section-dimensions-highres-multiplier-test...'
	@./cave-outliner --quiet --pixel --dimensions --multiplier 2 --crosssections x 1 test/cube-cross-section-dimensions-highres-multiplier-%.svg --step 0.05 test/cube.stl test/cube-cross-section-dimensions-highres-multiplier.svg
	@diff -q test/cube-cross-section-dimensions-highres-multiplier.svg test/cube-cross-section-dimensions-highres-multiplier.svg.expected
	@diff -q test/cube-cross-section-dimensions-highres-multiplier-0.svg test/cube-cross-section-dimensions-highres-multiplier-0.svg.expected

cube-cross-section-width-test:
	@echo 'Running test case cube-cross-section-width-test...'
	@./cave-outliner --quiet --pixel --crosssectionwith 2 --crosssections x 1 test/cube-cross-section-width-%.svg --step 0.5 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-cross-section-width.svg
	@diff -q test/cube-cross-section-width.svg test/cube-cross-section-width.svg.expected
	@diff -q test/cube-cross-section-width-0.svg test/cube-cross-section-width-0.svg.expected

cube-cross-section-highres-test:
	@echo 'Running test case cube-cross-section-highres-test...'
	@./cave-outliner --quiet --pixel --crosssections x 1 test/cube-cross-section-highres-%.svg --step 0.1 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-cross-section-highres.svg
	@diff -q test/cube-cross-section-highres.svg test/cube-cross-section-highres.svg.expected
	@diff -q test/cube-cross-section-highres-0.svg test/cube-cross-section-highres-0.svg.expected

cube-cross-section-label-test:
	@echo 'Running test case cube-cross-section-label-test...'
	@./cave-outliner --quiet --pixel --label --crosssections x 1 test/cube-cross-section-label-%.svg --step 0.5 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-cross-section-label.svg
	@diff -q test/cube-cross-section-label.svg test/cube-cross-section-label.svg.expected
	@diff -q test/cube-cross-section-label-0.svg test/cube-cross-section-label-0.svg.expected

cube-cross-section-highres-label-test:
	@echo 'Running test case cube-cross-section-highres-label-test...'
	@./cave-outliner --quiet --pixel --label --crosssections x 1 test/cube-cross-section-highres-label-%.svg --step 0.05 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-cross-section-highres-label.svg
	@diff -q test/cube-cross-section-highres-label.svg test/cube-cross-section-highres-label.svg.expected
	@diff -q test/cube-cross-section-highres-label-0.svg test/cube-cross-section-highres-label-0.svg.expected

cube-cross-section-multiplier-label-test:
	@echo 'Running test case cube-cross-section-multiplier-label-test...'
	@./cave-outliner --quiet --pixel --label --crosssections x 1 test/cube-cross-section-multiplier-label-%.svg --multiplier 10 --step 0.05 --bounding -2 2 -2 2 -2 2 test/cube.stl test/cube-cross-section-multiplier-label.svg
	@diff -q test/cube-cross-section-multiplier-label.svg test/cube-cross-section-multiplier-label.svg.expected
	@diff -q test/cube-cross-section-multiplier-label-0.svg test/cube-cross-section-multiplier-label-0.svg.expected

cave1-test:
	@echo 'Running test case cave1-test...'
	@./cave-outliner --quiet --pixel --step 0.05 --holethreshold 10 test/cave1.stl test/cave1.svg
	@diff -q test/cave1.svg test/cave1.svg.expected

cave1-line-test:
	@echo 'Running test case cave1-line-test...'
	@./cave-outliner --quiet --borderline --step 0.05 --holethreshold 10 test/cave1.stl test/cave1-line.svg
	@diff -q test/cave1-line.svg test/cave1-line.svg.expected

cave1-x-cross-section-test:
	@echo 'Running test case cave1-x-cross-section-test...'
	@./cave-outliner --quiet --label --borderline --multiplier 5 --crosssections x 3 test/cave1-x-cross-section-%.svg --step 0.05 --holethreshold 10 test/cave1.stl test/cave1-x-cross-section.svg
	@diff -q test/cave1-x-cross-section.svg test/cave1-x-cross-section.svg.expected
	@diff -q test/cave1-x-cross-section-0.svg test/cave1-x-cross-section-0.svg.expected
	@diff -q test/cave1-x-cross-section-1.svg test/cave1-x-cross-section-1.svg.expected
	@diff -q test/cave1-x-cross-section-2.svg test/cave1-x-cross-section-2.svg.expected

cave1-x-cross-section-width-test:
	@echo 'Running test case cave1-x-cross-section-width-test...'
	@./cave-outliner --quiet --label --dimensions --borderline --multiplier 5 --crosssectionwidth 3 --crosssections x 3 test/cave1-x-cross-section-width-%.svg --step 0.05 --holethreshold 10 test/cave1.stl test/cave1-x-cross-section-width.svg
	@diff -q test/cave1-x-cross-section-width.svg test/cave1-x-cross-section-width.svg.expected
	@diff -q test/cave1-x-cross-section-width-0.svg test/cave1-x-cross-section-width-0.svg.expected
	@diff -q test/cave1-x-cross-section-width-1.svg test/cave1-x-cross-section-width-1.svg.expected
	@diff -q test/cave1-x-cross-section-width-2.svg test/cave1-x-cross-section-width-2.svg.expected

cave1-y-cross-section-test:
	@echo 'Running test case cave1-y-cross-section-test...'
	@./cave-outliner --quiet --label --borderline --multiplier 5 --crosssections y 1 test/cave1-y-cross-section-%.svg --step 0.05 --holethreshold 10 test/cave1.stl test/cave1-y-cross-section.svg
	@diff -q test/cave1-y-cross-section.svg test/cave1-y-cross-section.svg.expected
	@diff -q test/cave1-y-cross-section-0.svg test/cave1-y-cross-section-0.svg.expected

steps-test:
	@echo 'Running test case steps-test...'
	@./cave-outliner --quiet --pixel --step 0.1 --holethreshold 10 test/steps.stl test/steps.svg
	@diff -q test/steps.svg test/steps.svg.expected

steps-cross-section-test:
	@echo 'Running test case steps-cross-section test...'
	@./cave-outliner --quiet --label --dimensions --pixel --crosssections x 1 test/steps-cross-section-%.svg --step 0.1 --holethreshold 10 test/steps.stl test/steps-cross-section.svg
	@diff -q test/steps-cross-section.svg test/steps-cross-section.svg.expected
	@diff -q test/steps-cross-section-0.svg test/steps-cross-section-0.svg.expected

steps-depthmap-test:
	@echo 'Running test case steps-depthmap-test...'
	@./cave-outliner --quiet --depthmap --step 0.5 --holethreshold 10 test/steps.stl test/steps-depthmap.svg
	@diff -q test/steps-depthmap.svg test/steps-depthmap.svg.expected

steps-depthdiffmap-test:
	@echo 'Running test case steps-depthdiffmap-test...'
	@./cave-outliner --quiet --depthdiffmap --step 0.2 --holethreshold 10 test/steps.stl test/steps-depthdiffmap.svg
	@diff -q test/steps-depthdiffmap.svg test/steps-depthdiffmap.svg.expected

cave1-depthmap-test:
	@echo 'Running test case cave1-depthmap-test...'
	@./cave-outliner --quiet --depthmap --step 0.2 --holethreshold 10 test/cave1.stl test/cave1-depthmap.svg
	@diff -q test/cave1-depthmap.svg test/cave1-depthmap.svg.expected

cave1-depthmap-cut-test:
	@echo 'Running test case cave1-depthmap-cut-test...'
	@./cave-outliner --quiet --depthmap --step 0.05 --bounding 0.4 12.6 -0.06 3.7 0 0.8 --holethreshold 10 test/cave1.stl test/cave1-depthmap-cut.svg
	@diff -q test/cave1-depthmap-cut.svg test/cave1-depthmap-cut.svg.expected

cave1-depthdiffmap-cut-test:
	@echo 'Running test case cave1-depthdiffmap-cut-test...'
	@./cave-outliner --quiet --depthdiffmap --step 0.05 --bounding 0.4 12.6 -0.06 3.7 0 0.8 --holethreshold 10 test/cave1.stl test/cave1-depthdiffmap-cut.svg
	@diff -q test/cave1-depthdiffmap-cut.svg test/cave1-depthdiffmap-cut.svg.expected

cave1-depthmap-highres-test:
	@echo 'Running test case cave1-depthmap-highres-test...'
	@./cave-outliner --quiet --depthmap --step 0.05 --holethreshold 10 test/cave1.stl test/cave1-depthmap-highres.svg
	@diff -q test/cave1-depthmap-highres.svg test/cave1-depthmap-highres.svg.expected

cave1-depthdiffmap-test:
	@echo 'Running test case cave1-depthdiffmap-test...'
	@./cave-outliner --quiet --depthdiffmap --step 0.1 --holethreshold 10 test/cave1.stl test/cave1-depthdiffmap.svg
	@diff -q test/cave1-depthdiffmap.svg test/cave1-depthdiffmap.svg.expected

cave1-form-analysis-test:
	@echo 'Running test case cave1-form-analysis-test...'
	@./cave-outliner --quiet --svgyreverse --formanalysis 2 --pixelform --step 0.1 --holethreshold 10 test/cave1.stl test/cave1-form-analysis.svg
	@diff -q test/cave1-form-analysis.svg test/cave1-form-analysis.svg.expected

cave1-form-analysis-pixel-test:
	@echo 'Running test case cave1-form-analysis-pixel-test...'
	@./cave-outliner --quiet --formanalysis 2 --pixel --step 0.1 --holethreshold 10 test/cave1.stl test/cave1-form-analysis-pixel.svg
	@diff -q test/cave1-form-analysis-pixel.svg test/cave1-form-analysis-pixel.svg.expected

cave1-form-analysis-line-test:
	@echo 'Running test case cave1-form-analysis-line-test...'
	@./cave-outliner --quiet --multiplier 5 --formanalysis 2 --borderline --step 0.05 --holethreshold 10 test/cave1.stl test/cave1-form-analysis-line.svg
	@diff -q test/cave1-form-analysis-line.svg test/cave1-form-analysis-line.svg.expected

cave1-form-analysis-smooth-line-test:
	@echo 'Running test case cave1-form-analysis-smooth-line-test...'
	@./cave-outliner --quiet --multiplier 5 --smooth --formanalysis 2 --borderline --step 0.05 --holethreshold 10 test/cave1.stl test/cave1-form-analysis-smooth-line.svg
	@diff -q test/cave1-form-analysis-smooth-line.svg test/cave1-form-analysis-smooth-line.svg.expected

cave1-spine-test:
	@echo 'Running test case cave1-spine-test...'
	@./cave-outliner --quiet --tunnelspine --multiplier 5 --formanalysis 1 --borderline --step 0.2 --holethreshold 10 test/cave1.stl test/cave1-spine.svg
	@diff -q test/cave1-spine.svg test/cave1-spine.svg.expected

cave1-floor-test:
	@echo 'Running test case cave1-floor-test...'
	@./cave-outliner --quiet --floordepthmap test/cave1-floor-floor.svg --roofdepthmap test/cave1-floor-roof.svg --tunnelspine --multiplier 5 --formanalysis 1 --borderline --step 0.1 --holethreshold 10 test/cave1.stl test/cave1-floor.svg
	@diff -q test/cave1-floor.svg test/cave1-floor.svg.expected
	@diff -q test/cave1-floor-floor.svg test/cave1-floor-floor.svg.expected
	@diff -q test/cave1-floor-roof.svg test/cave1-floor-roof.svg.expected

cave1-floor-highres-test:
	@echo 'Running test case cave1-floor-highres-test...'
	@./cave-outliner --quiet --floordepthmap test/cave1-floor-highres-floor.svg --roofdepthmap test/cave1-floor-highres-roof.svg --tunnelspine --multiplier 5 --formanalysis 1 --borderline --step 0.05 --holethreshold 10 test/cave1.stl test/cave1-floor-highres.svg
	@diff -q test/cave1-floor-highres.svg test/cave1-floor-highres.svg.expected
	@diff -q test/cave1-floor-highres-floor.svg test/cave1-floor-highres-floor.svg.expected
	@diff -q test/cave1-floor-highres-roof.svg test/cave1-floor-highres-roof.svg.expected

cave1-floor-condense-test:
	@echo 'Running test case cave1-floor-condense-test...'
	@./cave-outliner --quiet --floordepthmap test/cave1-floor-condense-floor.svg --roofdepthmap test/cave1-floor-condense-roof.svg --tunnelspine --multiplier 5 --formanalysis 2 --borderline --step 0.1 --holethreshold 10 test/cave1.stl test/cave1-floor-condense.svg
	@diff -q test/cave1-floor-condense.svg test/cave1-floor-condense.svg.expected
	@diff -q test/cave1-floor-condense-floor.svg test/cave1-floor-condense-floor.svg.expected
	@diff -q test/cave1-floor-condense-roof.svg test/cave1-floor-condense-roof.svg.expected

cave1-floor-condense-more-test:
	@echo 'Running test case cave1-floor-condense-more-test...'
	@./cave-outliner --quiet --floordepthmap test/cave1-floor-condense-more-floor.svg --roofdepthmap test/cave1-floor-condense-more-roof.svg --tunnelspine --multiplier 5 --formanalysis 5 --borderline --step 0.1 --holethreshold 10 test/cave1.stl test/cave1-floor-condense-more.svg
	@diff -q test/cave1-floor-condense-more.svg test/cave1-floor-condense-more.svg.expected
	@diff -q test/cave1-floor-condense-more-floor.svg test/cave1-floor-condense-more-floor.svg.expected
	@diff -q test/cave1-floor-condense-more-roof.svg test/cave1-floor-condense-more-roof.svg.expected

cave1-floor-depth-test:
	@echo 'Running test case cave1-floor-depth-test...'
	@./cave-outliner --quiet --floorstyle depth \
			 --floordepthmap test/cave1-floor-depth-floor.svg --roofdepthmap test/cave1-floor-depth-roof.svg \
			 --tunnelspine --multiplier 5 --formanalysis 1 --borderline --step 0.1 --holethreshold 10 \
			 test/cave1.stl test/cave1-floor-depth.svg
	@diff -q test/cave1-floor-depth.svg test/cave1-floor-depth.svg.expected
	@diff -q test/cave1-floor-depth-floor.svg test/cave1-floor-depth-floor.svg.expected
	@diff -q test/cave1-floor-depth-roof.svg test/cave1-floor-depth-roof.svg.expected

cave1-composite-test:
	@echo 'Running test case cave1-composite-test...'
	@./cave-outliner --quiet --label --borderline --smooth --multiplier 1 \
			 --composite --location "Siuntio, Finland" --coordinates "N 60.161948 E 24.137638" \
			 --crosssections x 3 --crosssections y 1 \
			 --step 0.05 --holethreshold 10 \
			 test/cave1.stl test/cave1-composite.svg
	@diff -q test/cave1-composite.svg test/cave1-composite.svg.expected

cave1-composite-multiplier-test:
	@echo 'Running test case cave1-composite-multiplier-test...'
	@./cave-outliner --quiet --label --borderline --smooth --multiplier 5 \
			 --composite --location "Siuntio, Finland" --coordinates "N 60.161948 E 24.137638" \
			 --crosssections x 3 --crosssections y 1 \
			 --step 0.05 --holethreshold 10 \
			 test/cave1.stl test/cave1-composite-multiplier.svg
	@diff -q test/cave1-composite-multiplier.svg test/cave1-composite-multiplier.svg.expected

example-tests:	cave1-example-orig-test \
		cave1-example-form-test \
		cave1-example-form-long-test \
		cave1-example-composite-test \
		cave1-example-composite-dimension-test

cave1-example-orig-test:
	@echo 'Running test case cave1-example-orig-test...'
	@./cave-outliner --label --dimensions --borderline \
                  --multiplier 2 --step 0.05 --holethreshold 10 \
                  --crosssectionwidth 3 --crosssections x 3 test/cave1-example-orig-cross%.svg \
                  test/cave1.stl test/cave1-example-orig-planview.svg > test/cave1-example-orig.out
	@diff -q test/cave1-example-orig-planview.svg test/cave1-example-orig-planview.svg.expected
	@diff -q test/cave1-example-orig-cross0.svg test/cave1-example-orig-cross0.svg.expected
	@diff -q test/cave1-example-orig-cross1.svg test/cave1-example-orig-cross1.svg.expected
	@diff -q test/cave1-example-orig-cross2.svg test/cave1-example-orig-cross2.svg.expected

cave1-example-form-test:
	@echo 'Running test case cave1-example-form-test...'
	@./cave-outliner --label --dimensions --borderline \
                  --multiplier 4 --step 0.05 --holethreshold 10 --formanalysis 1 \
		  --crosssectionwidth 3 --crosssections x 3 test/cave1-example-form-cross%.svg \
                  test/cave1.stl test/cave1-example-form-planview.svg > test/cave1-example-form.out
	@diff -q test/cave1-example-form-planview.svg test/cave1-example-form-planview.svg.expected
	@diff -q test/cave1-example-form-cross0.svg test/cave1-example-form-cross0.svg.expected
	@diff -q test/cave1-example-form-cross1.svg test/cave1-example-form-cross1.svg.expected
	@diff -q test/cave1-example-form-cross2.svg test/cave1-example-form-cross2.svg.expected

cave1-example-form-long-test:
	@echo 'Running test case cave1-example-form-long-test...'
	@./cave-outliner --label --dimensions --borderline \
                  --multiplier 4 --step 0.05 --holethreshold 10 --formanalysis 1 \
		  --crosssectionwidth 3 --crosssections y 1 test/cave1-example-form-long-cross%.svg \
		  test/cave1.stl test/cave1-example-form-long-planview.svg > test/cave1-example-form-long.out
	@diff -q test/cave1-example-form-long-planview.svg test/cave1-example-form-long-planview.svg.expected
	@diff -q test/cave1-example-form-long-cross0.svg test/cave1-example-form-long-cross0.svg.expected

cave1-example-composite-test:
	@echo 'Running test case cave1-example-composite-test...'
	@./cave-outliner --label --dimensions --borderline \
                  --multiplier 4 --step 0.05 --holethreshold 10 --formanalysis 1 \
		  --name "Grottberget" \
		  --location "Siuntio, Finland" \
		  --coordinates "N 12.34 E 56.78" \
		  --mapdate "January 1, 2099" \
		  --crosssectionwidth 3 \
		  --crosssections x 3 test/cave1-example-composite-x-cross%.svg \
		  --crosssections y 1 test/cave1-example-composite-y-cross%.svg \
                  test/cave1.stl test/cave1-example-composite.svg > test/cave1-example-composite.out
	@diff -q test/cave1-example-composite.svg test/cave1-example-form-long-planview.svg.expected

cave2-form-test:
	@echo 'Running test case cave2-form-test...'
	@./cave-outliner --label --dimensions --borderline \
                  --multiplier 4 --step 0.05 --holethreshold 10 --formanalysis 1 \
		  --crosssectionwidth 3 --crosssections x 1 test/cave2-form-cross%.svg \
                  test/cave2.stl test/cave2-form-planview.svg > test/cave2-form.out
	@diff -q test/cave2-form-planview.svg test/cave2-form-planview.svg.expected
	@diff -q test/cave2-form-cross0.svg test/cave2-form-cross0.svg.expected

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
