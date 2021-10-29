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

#define outlinermaxmeshes                                                              1
#define outlinertiledivision                                                          30
#define outlinermaxholethreshold                                                      50
#define outlinermaxcrosssections                                                     100
#define outlinerdefaultlinewidth                                                     1.0
#define outlinerdefaultfontxsize                                                       8
#define outlinerdefaultfontysize                                                      12
#define outlinertitlespaceempty                                                        5
#define outlinertitlespacey           (outlinerdefaultfontysize+outlinertitlespaceempty)
#define outlinertitlespacex                                (outlinerdefaultfontxsize*15)
#define outlinercrosssectionextraline                                                  5

#define OUTLINER_PROG                                                    "cave-outliner"
#define OUTLINER_ERRPREFIX                                     OUTLINER_PROG ": Error: "
#define OUTLINER_INFOPREFIX                                     OUTLINER_PROG ": Info: "
#define OUTLINER_DEBUGPREFIX                                   OUTLINER_PROG ": Debug: "

#endif // OUTLINERCONSTANTS_HH

