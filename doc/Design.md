
# Cave-Outliner Software Design

## Structure

The structure of the software is shown in the below figure:

![structure](https://raw.githubusercontent.com/jariarkko/cave-outliner/main/doc/images/Design-Structure-Small.jpg)

Overall, the system consists of six major parts:

* Main program ("main").
* 3D model import library ("assimp"), an external library.
* Core functionality ("core"), used for calculating the actual results, such as plan views from the 3D model.
* The SVG image export module, "outlinersvg".
* The 3D model description utility, "outlinerdescribe", used for debugging.
* The utilities, including math, debugging, and other capabilities.

## Modules

### Main

This is the main program, which lives in src/main.cc. However, bulk of the functionality is in associated classes.

See the detailed description of the [Class MainConfig API](software/class_main_config.md), [Class MainOptions API](software/class_main_options.md), and [Class MainTest API](software/class_main_test.md).

### Assimp

This is an external library, The Open-Asset-Importer-Lib. See https://www.assimp.org/

As a result of a successful 3D model import operation, a number of data structures are created for:

* Meshes (typically, a single model has just one, but could have more). Meshes contain faces.
* Faces. These are the key ingredient of a 3D model, triangle-shaped small parts that together make up a mesh. Each face is represented by a triangle that has three corners defined as points in 3D space.

### Core

This is the main functionality for actually calculating plan views, etc. from a 3D model. The indexed mesh and processor objects are instantiated by the main program, and then the processor object creates an additional material matrix object. It then proceeds to actually calculate what kind of resulting image should be output.

#### Outlinerindexedmesh

This object represents an optimized index to the mesh faces contained in an imported 3D model. The imported model has a large datastructure of 'faces' -- typically millions or even tens of millions of faces. There is no efficient way to search for faces at a given location in the 3D or 2D space, however. The indexed mesh object sorts the faces into a 2D matrix of 'tiles'. For instance, a large model could be split into 20x20 or 400 tiles, so that when we are looking for a face within given (x,y) coordinates, we only need to look at the tile where (x,y) falls into. The indexing is performed only once, and all searches after the indexing operation can use the more efficient search.

The number of tiles is configurable with the --tiling option of the cave-outliner program.

See the detailed description of the [Class IndexedMesh API](software/class_indexed_mesh.md). 

#### Outlinermaterialmatrix2d

This object represents a quantized 2D view into a 3D cave model. Given a desired resolution (N x M pixels), it provides a boolean matrix. Each element (x,y) in the matrix is set to 1 if there's a a face within the model in that pixel location. The resolution is configurable, so for instance for a 1 x 1 resolution all faces within the 3D model would be at the resulting single pixel.

Note that the material matrix resolution is not the same as tile resolution in the indexed mesh object. Typically, the indexed mesh has low resolution, just sufficient for fast searches, while the material matrix needs to support the desired high resolution of the output.

See the detailed description of the [Class MaterialMatrix2D API](software/class_material_matrix2_d.md). 

#### Outlinermaterialmatrix3d

This object represents a quantized 3D view into a 3D cave model. Given a desired resolution (N x M pixels), it provides a boolean matrix. Each element (x,y) in the matrix is set to 1 if there's a a face within the model in that pixel location. The resolution is configurable, so for instance for a 1 x 1 resolution all faces within the 3D model would be at the resulting single pixel.

Note that the material matrix resolution is not the same as tile resolution in the indexed mesh object. Typically, the indexed mesh has low resolution, just sufficient for fast searches, while the material matrix needs to support the desired high resolution of the output.

See the detailed description of the [Class MaterialMatrix3D API](software/class_material_matrix3_d.md). 

#### Outlinerformmatrix2d

This object represents a semantic 2D view into a 3D cave model. Given a desired resolution (N x M pixels), it provides a matrix of what's in each pixel, e.g., there's a cave with ceiling and floor, there's an entrance or hole with no roof, there's a stalactite, etc.

Note that the form matrix resolution is not the same as tile resolution in the indexed mesh object. Typically, the indexed mesh has low resolution, just sufficient for fast searches, while the material matrix needs to support the desired high resolution of the output.

See the detailed description of the [Class MaterialMatrix2D API](software/class_material_matrix2_d.md). 

#### Outlinerprocessor

This is the main program of the Core module, it performs the actual mapping from a given model and indexed mesh to an SVG image. It creates a material matrix as part of the process.

See the detailed description of the [Class Processor API](software/class_processor.md). 

#### Outlinerprocessorcrosssection

This is the module inside of the Core module that performs the drawing of cross sections.

See the detailed description of the [Class ProcessorCrossSection API](software/class_processor_cross_section.md). 

#### Outlinerprocessorforms

This is the module inside of the Core module that performs the analysis of what cave shapes and forms there are. These include entrances, stones, drops, stalactites, etc.

See the detailed description of the [Class ProcessorForms API](software/class_processor_forms.md). 

### Outlinersvg

This module can be used to output SVG images to a file. For a description of what an SVG image format is, see https://www.w3schools.com/graphics/svg_intro.asp. 

See the detailed description of the [Class SvgCreator API](software/class_svg_creator.md). 

### Outlinerdesribe

This module outputs information about a 3D model read into the memory. It is only used for debugging.

See the detailed description of the [Class Describer API](software/class_describer.md). 

### Outlinerboundingboxer

This module determines the size of the model, i.e., the bounding box the 3D model fits in.

See the detailed description of the [Class BoundingBoxer API](software/class_bounding_boxer.md). 

### Utilities

This module is a collection of small submodules that provide utility functions for math, debugging, etc. to the rest of the software.

#### Outlinermath

This module provides a number of math routines that the Core parts of the software need, such as a function for calculating when a point is inside a triangle.

See the detailed description of the [Class OutlinerMath API](software/class_outliner_math.md). 

#### Outlinerhighprecision

This module defines high-precision 2D and 3D vector types.

See the detailed description of the vector objects [Class OutlinerVector2D API](software/class_outliner_vector2_d.md) and [Class OutlinerVector3D API](software/class_outliner_vector3_d.md). 

See the detailed description of the line objects [Class OutlinerLine2D API](software/class_outliner_line2_d.md) and [Class OutlinerLine3D API](software/class_outliner_line3_d.md). 

See the detailed description of the bounding box objects [Class OutlinerBox1D API](software/class_outliner_box1_d.md), [Class OutlinerBox2D API](software/class_outliner_box2_d.md) and [Class OutlinerBox3D API](software/class_outliner_box3_d.md). 

See the detailed description of the triangle objects [Class OutlinerTriangle2D API](software/class_outliner_triangle2_d.md) and [Class OutlinerTriangle3D API](software/class_outliner_triangle3_d.md). 

#### Outlinerdirection

This module defines a direction enumerated type, which is used to represent the direction of view for the desired plan view. There's also an associated class that helps perform mappings between coordinates from a given direction.

See the detailed description of the [Class DirectionOperations API](software/class_direction_operations.md). 

#### Outlinertypes

This module defines some key data types used in the Cave-Outliner, such as algorithm enumerated types.

It also defines a data type that is used to represent the internal floating point calculations within Cave-Outliner. Currently we use 'double' for better accuracy, while the 3D models read by the Assimp library use 'float'.

#### Outlinerconstants

This module only provides a set of constants for maximum sizes of tables, option defaults, and so on.

#### Outlinerdebug

This module provides a reporting and debugging facility, with printf-like functions to report on various events. The level of reporting can be controlled by the debuginit function, and the actual reporting functions are infof, debugf, deepdebugf, and deepdeepdebugf.

#### Outlinerversion

This module only includes a version number string. It is manipulated by the 'make updateversion' command (see further down in the document).

## Coding Guidelines

The software is written in C++, and is open source. We collaborate at the GitHub site:

* All contributions should be made in [GitHub](https://github.com/jariarkko/cave-outliner).
* Small contributions by experienced developers can be made directly on the master branch.
* Anything large should generally be done in a separate branch, and a suitable Pull  Request be created to request its integration. This allows testing and review and modification by other contributors.
* All PRs and commits to the GitHub master branch must be made only if "make" succeeds. This will ensure tests are run and their results are correct, i.e., no existing functionality is broken.
* All code is subject to the [BSD license](https://github.com/jariarkko/cave-outliner/blob/master/LICENSE.txt).

All bugs and feature requests should be documented in GitHub issue [list](https://github.com/jariarkko/cave-outliner/issues).

## Releases

All weekly and major releases should be tagged with a tag "v(i).(j).(k)". E.g., to view current tags, use

    git tag

And making a new version:

    git tag -a v2.0.0
    make updateversion
    git add src/outlinerversion.cc
    git commit -m"new version"
    git push
    git push --tags

Major versions increase the first number in the version string. New functionality should increase the second number; weekly releases and bug fixes should increase the last one.

The currently active master branch version must always be runnable; critical bugs found in it should be promptly corrected. However, the master branch version is not meant to be directly used by invidiual installations. We recommend a weekly update of live production sites, after testing and getting an agreement from main contributors that all major features seem to be fine.

## Testing

The Makefile runs various code checks and unit tests on the entire software. Its use is mandatory after changes.

Please add a test when you add a new feature that can be tested in the unit tests. Sometimes new features and tests change the expected outputs from already implemented tests. For instance, you may change the format of some outputs to be better in some way. If this happens, you need to carefully review the expected and actual outputs and make the necessary changes. Old tests are part of the software and need to be retained and maintained just like other pieces of software.

## Community behaviour

We strive for polite collaboration, welcome all contibutors, and respect the different needs diffferent people may have for the software and data formats.

We also strive for technical excellence (even if at times this means pointing out problems in the software -- but not in people).

## Detailed coding style

The following quidelines apply:

* Please comment all data structures, all functions (incl. input parameters and return values), and all complex pieces of code.
* Please use defensive programming style, i.e,. for most functions this means that all input parameters should be checked, and appropriate exceptions thrown upon error. Use "assert" extensively.
* Please indent your code appropriately, using 4 character indent settings. Please avoid the use of TABs, as their usage can sometimes be dependent on the contributor's programming environment, and look different to the other contributors.
* Avoid leaving "FIXIT" type comments in the code, as we may never find them :-) Please use the GitHub issue list instead.
* Please limit the use of external libraries to the minimum; we attempt to keep cave-outliner as portable and independent of other systems as possible.

