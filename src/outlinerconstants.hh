
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

