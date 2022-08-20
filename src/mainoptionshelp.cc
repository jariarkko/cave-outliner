
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//
//      CCC    AAA   V     V EEEEE       OOO   UU   UU TTTTTT LL    II NN   NN EEEEE RRRRR
//    CC   CC AA  AA  V   V  EE        OO   OO UU   UU   TT   LL    II NNN  NN EE    RR  RR
//    CC      AA  AA  V   V  EEE       OO   OO UU   UU   TT   LL    II NN N NN EEE   RRRRR
//    CC   CC AAAAAA   V V   EE        OO   OO UU   UU   TT   LL    II NN  NNN EE    RR R
//      CCc   AA  AA    V    EEEEE       OOO    UUUUU    TT   LLLLL II NN   NN EEEEE RR  R
//
//    CAVE OUTLINER -- Cave 3D model processing software
//
//    Copyright (C) 2021 by Jari Arkko -- See LICENSE.txt for license information.
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinerdirection.hh"
#include "outlinerhighprecision.hh"
#include "outlinerprocessor.hh"
#include "outlinerversion.hh"
#include "outlinerdebug.hh"
#include "mainconfig.hh"
#include "mainoptions.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Class functions, help //////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
MainOptions::processHelp(void) {
  std::cout << "\n";
  std::cout << OUTLINER_PROG " [options] inputfile outputfile\n";
  std::cout << "\n";
  std::cout << "Processes an input 3D model in STL format to a SVG picture that\n";
  std::cout << "represents the cave horizontal plane. This can be used to produce\n";
  std::cout << "maps.\n";
  std::cout << "\n";
  std::cout << "Options:\n";
  std::cout << "\n";
  std::cout << "  --auto                    Use default options for best cave map generation.\n";
  std::cout << "  --bounding x x y y z z    Set the bounding box area. Default is the model's bounding box.\n";
  std::cout << "  --step i                  Set the granularity increment. Default is 1.\n";
  std::cout << "  --z                       Generate output as viewed from the z direction, i.e.,\n";
  std::cout << "                            showing x/y picture.\n";
  std::cout << "  --x                       Generate output as viewed from the x direction, i.e.,\n";
  std::cout << "                            showing z/y picture.\n";
  std::cout << "  --y                       Generate output as viewed from the y direction, i.e.,\n";
  std::cout << "                            showing x/z picture.\n";
  std::cout << "  --pixel                   Use the pixel output drawing algorithm (default, fills cave\n";
  std::cout << "                            with pixels).\n";
  std::cout << "  --pixelform               Same as --pixel, but color pixels based form analysis.\n";
  std::cout << "  --triangle                Use the triangle output drawing algorithm (draws model faces\n";
  std::cout << "                            in plan view).\n";
  std::cout << "  --depthmap                Same as --pixel, but color pixels based on their height.\n";
  std::cout << "  --depthdiffmap            Same as --pixel, but color pixels based on their height\n";
  std::cout << "                            difference from neighbouring pixels.\n";
  std::cout << "  --borderpixel             Use the border-only drawing algorithm, draws only the cave\n";
  std::cout << "                            walls, with pixels.\n";
  std::cout << "  --borderline              Use the border-only drawing algorithm, draws only the cave\n";
  std::cout << "                            walls, with lines.\n";
  std::cout << "  --borderactual            Use the border-only drawing algorithm, draws the cave walls using\n";
  std::cout << "                            model triangle sides.\n";
  std::cout << "  --crosssection d p [file] Produce also a cross section at a given direction (d = x or y),\n";
  std::cout << "                            position p, output to file.\n";
  std::cout << "  --crosssections d n [pat] Produce n cross sections at different direction (d = x or y)\n";
  std::cout << "                            positions, output to files (percent sign denotes the cross\n";
  std::cout << "                            section number in the file name pattern).\n";
  std::cout << "  --crossectionwidth n      Width of the analysis for a cross section, by default 1.0, i.e.,\n";
  std::cout << "                            one step.\n";
  std::cout << "  --formanalysis f          Analyze cave forms, e.g., entrances, stones, stalactites, etc.\n";
  std::cout << "                            Factor f specifies how much the analysis compresses pixels,\n";
  std::cout << "                            value of 1 implies no compression.\n";
  std::cout << "  --no-formanalysis         Turn off form analysis.\n";
  std::cout << "  --composite               Create a composite cave map file from the plan view and all\n";
  std::cout << "                            cross sections.\n";
  std::cout << "  --name name               The name of the cave.\n";
  std::cout << "  --surveyer name           Who performed the 3D survey? Default is the user's name who is\n";
  std::cout << "                            running this software.\n";
  std::cout << "  --surveytool name         What tool was used for 3D scan? Default is iPhone 12 Pro.\n";
  std::cout << "  --surveydate date         When was the survey taken? Default is the creation date of the\n";
  std::cout << "                            input file.\n";
  std::cout << "  --location place          The location of the cave, e.g., city or country.\n";
  std::cout << "  --coordinates coords      Coordinates of the cave.\n";
  std::cout << "  --length text             Text to use as the length of the cave in the map.\n";
  std::cout << "  --floordepthmap file      Output cave tunnel floor depth map to given file.\n";
  std::cout << "  --roofdepthmap file       Output cave tunnel roof depth map to given file.\n";
  std::cout << "  --floorstyle s            Specify floor and roof depthmap style, either depth\n";
  std::cout << "                            or diff.\n";
  std::cout << "  --tunnelspine             Mark cave tunnel centerpoints.\n";
  std::cout << "  --label                   Label cross sections.\n";
  std::cout << "  --dimensions              Show dimensions of each cave plan view and cross section.\n";
  std::cout << "  --multiplier n            Multiply image size by n (default 1).\n";
  std::cout << "  --linewidth n             Set the width of the lines in output picture. The value can be a\n";
  std::cout << "                            decimal number.\n";
  std::cout << "  --smooth                  Set the line drawings use smooth curves.\n";
  std::cout << "  --jagged                  Set the line drawings use hard lines (default).\n";
  std::cout << "  --svgyreverse             Reverse Y axis in the SVG (to maintain same Y coordinate\n";
  std::cout << "                            direction as in the model)\n";
  std::cout << "  --holethreshold n         Ignore holes in the model if they are n or less pixels.\n";
  std::cout << "  --lineholethreshold n     Ignore holes in cross-section  lines if they are n or less pixels.\n";
  std::cout << "  --dustthreshold n         Ignore small amount of material if not connected and less\n";
  std::cout << "                            than or equal to n pixels.\n";
  std::cout << "  --tiling n                Optimize search process with n x n tiles.\n";
  std::cout << "                            Default is " << outlinertiledivision << ",\n";
  std::cout << "                            and --tiling 1 implies no optimization.\n";
  std::cout << "  --quiet                   Turn on informative messages (default is they are on).\n";
  std::cout << "  --parallel n              How many parallel threads can be run.\n";
  std::cout << "  --debug                   Turn on debugging messages (level 0, least)\n";
  std::cout << "  --deepdebug               Turn on debugging messages (level 1)\n";
  std::cout << "  --deepdeepdebug           Turn on debugging messages (level 2, most)\n";
  std::cout << "  --version                 Output version information.\n";
  std::cout << "  --help                    Print this message\n";
  std::cout << "\n"; 
}

