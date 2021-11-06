# cave-outliner
A small tool to draw horizontal plane outlines of caves, given an input of a 3D model.

The input is in the form a Standard Triangle Language (STL) file. The output is a Standard Vector Graphics (SVG) image. This is very much work in progress for now.

Usage:

    cave-outliner [options] inputfile outputfile

Options:

    --bounding x x y y z z   Set the bounding box area. Default is the the model's bounding box.
    --step i                 Set the granularity increment. Default is 1.
    --z                      Generate output as viewed from the z direction, i.e., showing x/y picture.
    --x                      Generate output as viewed from the x direction, i.e., showing z/y picture.
    --y                      Generate output as viewed from the y direction, i.e., showing x/z picture.
    --pixel                  Use the pixel output drawing algorithm (default, fills cave with pixels).
    --triangle               Use the triangle output drawing algorithm (draws model faces in plan view).
    --borderpixel            Use the border-only drawing algorithm, draws only the cave walls,
                             with pixels.
    --borderline             Use the border-only drawing algorithm, draws only the cave walls,
                             with lines.
    --borderactual           Use the border-only drawing algorithm, draws the cave walls using
                             model triangle sides.
    --crosssection d p file  Produce also a cross section at a given direction (d = x or y) position p,
                             output to file.
    --crosssections d n pat  Produce n cross sections at different direction (d = x or y) positions,
                             output to files (percent sign denotes the cross section number in the
                             file name pattern).
    --label                  Label cross sections
    --linewidth n            Set the width of the lines in output picture. The value can be a
                             decimal number.
    --multiplier n           Multiply image size by n (default 1).
    --smooth                 Set the line drawings use smooth curves.\n";
    --jagged                 Set the line drawings use hard lines (default).\n";
    --holethreshold n        Ignore holes in the model if they are n or less pixels.
    --lineholethreshold n    Ignore holes in cross-section  lines if they are n or less pixels.
    --tiling n               Optimize search process with n x n tiles. Default is 30,
                             and --tiling 1 implies no optimization.
    --quiet                  Turn on informative messages (default is they are on).
    --debug                  Turn on debugging messages (level 0, least).
    --deepdebug              Turn on debugging messages (level 1).
    --deepdeepdebug          Turn on debugging messages (level 2, most).
    --version                Output version information.
    --help                   Print this message.

# Installation

Please read the [installation guide](https://github.com/jariarkko/cave-outliner/blob/master/doc/INSTALL.md).

# Algorithms

Algorithms for drawing plan and other views based on a 3D are discussed in the [paper](https://github.com/jariarkko/cave-outliner/blob/master/doc/Paper.md).

# Design

The structure of the software is discussed in the [design document](https://github.com/jariarkko/cave-outliner/blob/master/doc/Design.md).

