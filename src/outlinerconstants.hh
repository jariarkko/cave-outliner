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

#ifndef OUTLINERCONSTANTS_HH
#define OUTLINERCONSTANTS_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Defines ////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#define outlinerdefaultthreads                                                         4
#define outlinermaxmeshes                                                              1
#define outlinertiledivision                                                          30
#define outlinermaxholethreshold                                                     100
#define outlinermaxlineholethreshold                                                  20
#define outlinermaxcrosssections                                                     100
#define outlinermaxtunnelsinoneslice                                                  16
#define outlinerunusablegreyscale                                                      8
#define outlinerdefaultlinewidth                                                     1.0
#define outlinerdefaultfont                                                           16
#define outlinerdefaultfontxsize                                                     5.6
#define outlinerdefaultfontxsizelarge                                                8.0
#define outlinerdefaultfontysize                                                      12
#define outlinerdefaultfontysizelarge                                                 15
#define outlinersmallfont                                                              9
#define outlinersmallfontxsize                                                       4.3
#define outlinersmallfontysize                                                       6.0
#define outlinertitlespaceempty                                                        2
#define outlinertitlespacex                                (outlinerdefaultfontxsize*15)
#define outlinercrosssectionextraline                                                  5
#define outlinerdimensionlinespaceempty                                                2
#define outlinerdimensionlinespacelinestart                                            4
#define outlinerdimensionlinespacewidth                                                3
#define outlinerdimensionlinespaceemptymore                                            1
#define outlinerdimensionlinespace                                                     \
                                                    (outlinerdimensionlinespaceempty + \
                                                 outlinerdimensionlinespaceemptymore + \
                                                        outlinerdimensionlinespacewidth)
#define outlinerdimensionspacex                               (outlinersmallfontxsize*5)
#define outlinerepsilon                                                         0.000001 // ppm

#define OUTLINER_PROG                                                    "cave-outliner"
#define OUTLINER_WARNPREFIX                                  OUTLINER_PROG ": Warning: "
#define OUTLINER_ERRPREFIX                                     OUTLINER_PROG ": Error: "
#define OUTLINER_INFOPREFIX                                     OUTLINER_PROG ": Info: "
#define OUTLINER_DEBUGPREFIX                                   OUTLINER_PROG ": Debug: "

#define OUTLINER_TEMPDIRECTORY                                                   "/tmp/"

#endif // OUTLINERCONSTANTS_HH

