# cave-outliner
A small tool to draw horizontal plane outlines of caves, given an input of a 3D model.

The input is in the form a Standard Triangle Language (STL) file. The output is a Standard Vector Graphics (SVG) image. This is very much work in progress for now.

Usage:

    cave-outliner [options] inputfile outputfile

Options:

    --bounding x x y y z z   Set the bounding box area. Default is -2 2 -2 2 -2 2.
    --step i                 Set the granularity increment. Default is 1.
    --z                      Generate output as viewed from the z direction, i.e., showing x/y picture.
    --x                      Generate output as viewed from the x direction, i.e., showing z/y picture.
    --y                      Generate output as viewed from the y direction, i.e., showing x/z picture.
    --pixel                  Use the pixel output drawing algorithm (default, fills cave with pixels).
    --borderpixel            Use the border-only drawing algorithm, draws only the cave walls, with pixels.
    --borderline             Use the border-only drawing algorithm, draws only the cave walls, with lines.
    --borderactual           Use the border-only drawing algorithm, draws the cave walls using model triangle sides.
    --holethreshold n        Ignore holes in the model if they are n or less pixels.
    --tiling n               Optimize search process with n x n tiles. Default is 30,
                             and --tiling 1 implies no optimization.
    --debug                  Turn on debugging messages (level 0, least)
    --deepdebug              Turn on debugging messages (level 1)
    --deepdeepdebug          Turn on debugging messages (level 2, most)
    --help                   Print this message

