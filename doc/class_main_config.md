<table data-cellspacing="0" data-cellpadding="0">
<colgroup>
<col style="width: 100%" />
</colgroup>
<tbody>
<tr class="odd" style="height: 56px;">
<td id="projectalign" style="padding-left: 0.5em"><div id="projectname">
Cave Outliner
</div></td>
</tr>
</tbody>
</table>

[Public Member Functions](#pub-methods) | [Public
Attributes](#pub-attribs) | 
members](class_main_config-members.md)

MainConfig Class Reference

`#include <mainconfig.hh>`

<table class="memberdecls">
<colgroup>
<col style="width: 50%" />
<col style="width: 50%" />
</colgroup>
<tbody>
<tr class="odd heading">
<td colspan="2"><h2 id="public-member-functions" class="groupheader"><span id="pub-methods"></span> Public Member Functions</h2></td>
</tr>
<tr class="even memitem:a15d38e887e1854ec847a015d7be6f3a6">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a15d38e887e1854ec847a015d7be6f3a6"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>MainConfig</strong> ()</td>
</tr>
<tr class="odd memdesc:a15d38e887e1854ec847a015d7be6f3a6">
<td class="mdescLeft"> </td>
<td class="mdescRight">Create the configuration object, with default values.<br />
</td>
</tr>
<tr class="even separator:a15d38e887e1854ec847a015d7be6f3a6">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a7f59a98ff0252fff656716c18a711d6d">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a7f59a98ff0252fff656716c18a711d6d"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>~MainConfig</strong> ()</td>
</tr>
<tr class="even memdesc:a7f59a98ff0252fff656716c18a711d6d">
<td class="mdescLeft"> </td>
<td class="mdescRight">Release all resources for the configuration object.<br />
</td>
</tr>
<tr class="odd separator:a7f59a98ff0252fff656716c18a711d6d">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a106c3302252b635648bfa601ebc95e5a">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a106c3302252b635648bfa601ebc95e5a"></span> const char * </td>
<td class="memItemRight" data-valign="bottom"><strong>getCrossSectionLabel</strong> (void)</td>
</tr>
<tr class="odd memdesc:a106c3302252b635648bfa601ebc95e5a">
<td class="mdescLeft"> </td>
<td class="mdescRight">Get a label suitable for a cross section ("A", "B", etc.)<br />
</td>
</tr>
<tr class="even separator:a106c3302252b635648bfa601ebc95e5a">
<td colspan="2" class="memSeparator"> </td>
</tr>
</tbody>
</table>

<table class="memberdecls">
<colgroup>
<col style="width: 50%" />
<col style="width: 50%" />
</colgroup>
<tbody>
<tr class="odd heading">
<td colspan="2"><h2 id="public-attributes" class="groupheader"><span id="pub-attribs"></span> Public Attributes</h2></td>
</tr>
<tr class="even memitem:a746069bfe670f0de7aca12eb1beff1bd">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a746069bfe670f0de7aca12eb1beff1bd"></span> char * </td>
<td class="memItemRight" data-valign="bottom"><strong>inputFile</strong></td>
</tr>
<tr class="odd memdesc:a746069bfe670f0de7aca12eb1beff1bd">
<td class="mdescLeft"> </td>
<td class="mdescRight">Input 3D model file name.<br />
</td>
</tr>
<tr class="even separator:a746069bfe670f0de7aca12eb1beff1bd">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a291975eca113a2088bd720e8a50f6d26">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a291975eca113a2088bd720e8a50f6d26"></span> char * </td>
<td class="memItemRight" data-valign="bottom"><strong>outputFile</strong></td>
</tr>
<tr class="even memdesc:a291975eca113a2088bd720e8a50f6d26">
<td class="mdescLeft"> </td>
<td class="mdescRight">Output SVG image file name.<br />
</td>
</tr>
<tr class="odd separator:a291975eca113a2088bd720e8a50f6d26">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:ad017d93965c353339db7a31b07b68477">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="ad017d93965c353339db7a31b07b68477"></span> bool </td>
<td class="memItemRight" data-valign="bottom"><strong>test</strong></td>
</tr>
<tr class="odd memdesc:ad017d93965c353339db7a31b07b68477">
<td class="mdescLeft"> </td>
<td class="mdescRight">True if we should run unit tests.<br />
</td>
</tr>
<tr class="even separator:ad017d93965c353339db7a31b07b68477">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:ad7eb409fcc677af87aaaad7597cf2397">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="ad7eb409fcc677af87aaaad7597cf2397"></span> bool </td>
<td class="memItemRight" data-valign="bottom"><strong>info</strong></td>
</tr>
<tr class="even memdesc:ad7eb409fcc677af87aaaad7597cf2397">
<td class="mdescLeft"> </td>
<td class="mdescRight">True if informational messages should be displayed.<br />
</td>
</tr>
<tr class="odd separator:ad7eb409fcc677af87aaaad7597cf2397">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:af100bbcba49c61c01be6adcc5ebd5a5d">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="af100bbcba49c61c01be6adcc5ebd5a5d"></span> bool </td>
<td class="memItemRight" data-valign="bottom"><strong>debug</strong></td>
</tr>
<tr class="odd memdesc:af100bbcba49c61c01be6adcc5ebd5a5d">
<td class="mdescLeft"> </td>
<td class="mdescRight">True if level 1 debugging should be provided.<br />
</td>
</tr>
<tr class="even separator:af100bbcba49c61c01be6adcc5ebd5a5d">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a9370cf33e6ff22245fcf9e82b210a9dc">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a9370cf33e6ff22245fcf9e82b210a9dc"></span> bool </td>
<td class="memItemRight" data-valign="bottom"><strong>deepdebug</strong></td>
</tr>
<tr class="even memdesc:a9370cf33e6ff22245fcf9e82b210a9dc">
<td class="mdescLeft"> </td>
<td class="mdescRight">True if level 2 debugging should be provided.<br />
</td>
</tr>
<tr class="odd separator:a9370cf33e6ff22245fcf9e82b210a9dc">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a698f2ef432a1e0f4a652c1f7237ee3e1">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a698f2ef432a1e0f4a652c1f7237ee3e1"></span> bool </td>
<td class="memItemRight" data-valign="bottom"><strong>deepdeepdebug</strong></td>
</tr>
<tr class="odd memdesc:a698f2ef432a1e0f4a652c1f7237ee3e1">
<td class="mdescLeft"> </td>
<td class="mdescRight">True if level 3 debugging should be provided.<br />
</td>
</tr>
<tr class="even separator:a698f2ef432a1e0f4a652c1f7237ee3e1">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a39d08da53f75d634f7470baa333954ff">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a39d08da53f75d634f7470baa333954ff"></span> float </td>
<td class="memItemRight" data-valign="bottom"><strong>stepx</strong></td>
</tr>
<tr class="even memdesc:a39d08da53f75d634f7470baa333954ff">
<td class="mdescLeft"> </td>
<td class="mdescRight">In how small steps the model should be scanned in x coordinate direction.<br />
</td>
</tr>
<tr class="odd separator:a39d08da53f75d634f7470baa333954ff">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a9adf62267f6a04d243527ae0079ee7dc">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a9adf62267f6a04d243527ae0079ee7dc"></span> float </td>
<td class="memItemRight" data-valign="bottom"><strong>stepy</strong></td>
</tr>
<tr class="odd memdesc:a9adf62267f6a04d243527ae0079ee7dc">
<td class="mdescLeft"> </td>
<td class="mdescRight">In how small steps the model should be scanned in y coordinate direction.<br />
</td>
</tr>
<tr class="even separator:a9adf62267f6a04d243527ae0079ee7dc">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a32f41fad1f479c9287136d33ba783808">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a32f41fad1f479c9287136d33ba783808"></span> float </td>
<td class="memItemRight" data-valign="bottom"><strong>stepz</strong></td>
</tr>
<tr class="even memdesc:a32f41fad1f479c9287136d33ba783808">
<td class="mdescLeft"> </td>
<td class="mdescRight">In how small steps the model should be scanned in z coordinate direction.<br />
</td>
</tr>
<tr class="odd separator:a32f41fad1f479c9287136d33ba783808">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a751762be7967919edab5352222e70938">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a751762be7967919edab5352222e70938"></span> bool </td>
<td class="memItemRight" data-valign="bottom"><strong>boundingBoxSet</strong></td>
</tr>
<tr class="odd memdesc:a751762be7967919edab5352222e70938">
<td class="mdescLeft"> </td>
<td class="mdescRight">True if the bounding box has been specified.<br />
</td>
</tr>
<tr class="even separator:a751762be7967919edab5352222e70938">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a67db8a9dbc362254f6bbc4f4be2e6fd8">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a67db8a9dbc362254f6bbc4f4be2e6fd8"></span> <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_box3_d.md" class="el">OutlinerBox3D</a> </td>
<td class="memItemRight" data-valign="bottom"><strong>boundingBox</strong></td>
</tr>
<tr class="even memdesc:a67db8a9dbc362254f6bbc4f4be2e6fd8">
<td class="mdescLeft"> </td>
<td class="mdescRight">The bounding box we should be looking at.<br />
</td>
</tr>
<tr class="odd separator:a67db8a9dbc362254f6bbc4f4be2e6fd8">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a7fbc03c7edd683c667ec1aad26928cd3">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a7fbc03c7edd683c667ec1aad26928cd3"></span> enum outlinerdirection </td>
<td class="memItemRight" data-valign="bottom"><strong>direction</strong></td>
</tr>
<tr class="odd memdesc:a7fbc03c7edd683c667ec1aad26928cd3">
<td class="mdescLeft"> </td>
<td class="mdescRight">Direction of view for the plan view (usually along z axis).<br />
</td>
</tr>
<tr class="even separator:a7fbc03c7edd683c667ec1aad26928cd3">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a6ebfc62c1697ee8f394eb3438e99c57d">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a6ebfc62c1697ee8f394eb3438e99c57d"></span> enum outlineralgorithm </td>
<td class="memItemRight" data-valign="bottom"><strong>algorithm</strong></td>
</tr>
<tr class="even memdesc:a6ebfc62c1697ee8f394eb3438e99c57d">
<td class="mdescLeft"> </td>
<td class="mdescRight">What drawing algorithm should be used.<br />
</td>
</tr>
<tr class="odd separator:a6ebfc62c1697ee8f394eb3438e99c57d">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a05d95cac036e6a49223f5137a0267769">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a05d95cac036e6a49223f5137a0267769"></span> float </td>
<td class="memItemRight" data-valign="bottom"><strong>linewidth</strong></td>
</tr>
<tr class="odd memdesc:a05d95cac036e6a49223f5137a0267769">
<td class="mdescLeft"> </td>
<td class="mdescRight">For lines, how thick should the lines be?<br />
</td>
</tr>
<tr class="even separator:a05d95cac036e6a49223f5137a0267769">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a0437d68ad9083b118d855941636472fb">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a0437d68ad9083b118d855941636472fb"></span> unsigned int </td>
<td class="memItemRight" data-valign="bottom"><strong>multiplier</strong></td>
</tr>
<tr class="even memdesc:a0437d68ad9083b118d855941636472fb">
<td class="mdescLeft"> </td>
<td class="mdescRight">Should one inspected step in the model result in one pixel, or multiplied to a number of pixels in the image?<br />
</td>
</tr>
<tr class="odd separator:a0437d68ad9083b118d855941636472fb">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:aafa6ddf8f6e7bdde0471332e80664bd8">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="aafa6ddf8f6e7bdde0471332e80664bd8"></span> bool </td>
<td class="memItemRight" data-valign="bottom"><strong>smooth</strong></td>
</tr>
<tr class="odd memdesc:aafa6ddf8f6e7bdde0471332e80664bd8">
<td class="mdescLeft"> </td>
<td class="mdescRight">True if lines should be smoothed.<br />
</td>
</tr>
<tr class="even separator:aafa6ddf8f6e7bdde0471332e80664bd8">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a123e679e27a2ea8531bf18b6e4b892c7">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a123e679e27a2ea8531bf18b6e4b892c7"></span> bool </td>
<td class="memItemRight" data-valign="bottom"><strong>mergedLines</strong></td>
</tr>
<tr class="even memdesc:a123e679e27a2ea8531bf18b6e4b892c7">
<td class="mdescLeft"> </td>
<td class="mdescRight">True if short line segments should be merged in an SVG image.<br />
</td>
</tr>
<tr class="odd separator:a123e679e27a2ea8531bf18b6e4b892c7">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:ab7dea6503e3065f583472dfed0b032cb">
<td style="text-align: right;" class="memItemLeft" data-valign="top">unsigned int </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_main_config.md#ab7dea6503e3065f583472dfed0b032cb" class="el">tiles</a></td>
</tr>
<tr class="odd separator:ab7dea6503e3065f583472dfed0b032cb">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a2f778fa9720229cf2820dc9d7bfdb581">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a2f778fa9720229cf2820dc9d7bfdb581"></span> unsigned int </td>
<td class="memItemRight" data-valign="bottom"><strong>holethreshold</strong></td>
</tr>
<tr class="odd memdesc:a2f778fa9720229cf2820dc9d7bfdb581">
<td class="mdescLeft"> </td>
<td class="mdescRight">If the model has holes, e.g., missing faces for various reasons, should the software try to fill them?<br />
</td>
</tr>
<tr class="even separator:a2f778fa9720229cf2820dc9d7bfdb581">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a354d916bbd33ea81920f7328ff330a68">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a354d916bbd33ea81920f7328ff330a68"></span> bool </td>
<td class="memItemRight" data-valign="bottom"><strong>automaticCrossSections</strong></td>
</tr>
<tr class="even memdesc:a354d916bbd33ea81920f7328ff330a68">
<td class="mdescLeft"> </td>
<td class="mdescRight">Should we create a number of automatic cross sections?<br />
</td>
</tr>
<tr class="odd separator:a354d916bbd33ea81920f7328ff330a68">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a85f1c3ffeffda3f50ab8a87ad3edfb94">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a85f1c3ffeffda3f50ab8a87ad3edfb94"></span> unsigned int </td>
<td class="memItemRight" data-valign="bottom"><strong>nAutomaticCrossSections</strong></td>
</tr>
<tr class="odd memdesc:a85f1c3ffeffda3f50ab8a87ad3edfb94">
<td class="mdescLeft"> </td>
<td class="mdescRight">How many automatic cross sections do we wish?<br />
</td>
</tr>
<tr class="even separator:a85f1c3ffeffda3f50ab8a87ad3edfb94">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:ae25f8aab936149c64e547a7a15290db3">
<td style="text-align: right;" class="memItemLeft" data-valign="top">const char * </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_main_config.md#ae25f8aab936149c64e547a7a15290db3" class="el">automaticCrossSectionFilenamePattern</a></td>
</tr>
<tr class="even separator:ae25f8aab936149c64e547a7a15290db3">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a95ed0995830abb80cc33f6f090c2ed3e">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a95ed0995830abb80cc33f6f090c2ed3e"></span> unsigned int </td>
<td class="memItemRight" data-valign="bottom"><strong>nCrossSections</strong></td>
</tr>
<tr class="even memdesc:a95ed0995830abb80cc33f6f090c2ed3e">
<td class="mdescLeft"> </td>
<td class="mdescRight">How many cross sections did we in the end have? (Manual + automatic).<br />
</td>
</tr>
<tr class="odd separator:a95ed0995830abb80cc33f6f090c2ed3e">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a943b8f45ebf2bcadb841f193e2676019">
<td style="text-align: right;" class="memItemLeft" data-valign="top">struct <a href="struct_processor_cross_section_info.md" class="el">ProcessorCrossSectionInfo</a> </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_main_config.md#a943b8f45ebf2bcadb841f193e2676019" class="el">crossSections</a> [outlinermaxcrosssections]</td>
</tr>
<tr class="odd separator:a943b8f45ebf2bcadb841f193e2676019">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a903dc0119ec4155627ce8bb3531f4d86">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a903dc0119ec4155627ce8bb3531f4d86"></span> bool </td>
<td class="memItemRight" data-valign="bottom"><strong>labelCrossSections</strong></td>
</tr>
<tr class="odd memdesc:a903dc0119ec4155627ce8bb3531f4d86">
<td class="mdescLeft"> </td>
<td class="mdescRight">Should we label cross sections in the images?<br />
</td>
</tr>
<tr class="even separator:a903dc0119ec4155627ce8bb3531f4d86">
<td colspan="2" class="memSeparator"> </td>
</tr>
</tbody>
</table>

<span id="details"></span>

## Detailed Description

The <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_main_config.md" class="el">MainConfig</a> class
represents an object of all configuration information related to the
cave-outliner program. It is initialized to default values, and the main
program can read options and other command line arguments to set
specific values in the configuration. Once the processing of the actual
function in cave-outliner begins, the configuration data is read from
this object to decide how to process.

## Member Data Documentation

<span id="ae25f8aab936149c64e547a7a15290db3"></span>

## <span class="permalink">[◆ ](#ae25f8aab936149c64e547a7a15290db3)</span>automaticCrossSectionFilenamePattern

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">const char* MainConfig::automaticCrossSectionFilenamePattern</td>
</tr>
</tbody>
</table>

What is the name of the cross section files? Use '' to represent the
cross section counter, so that each image gets a different file name.

<span id="a943b8f45ebf2bcadb841f193e2676019"></span>

## <span class="permalink">[◆ ](#a943b8f45ebf2bcadb841f193e2676019)</span>crossSections

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">struct <a href="struct_processor_cross_section_info.md" class="el">ProcessorCrossSectionInfo</a> MainConfig::crossSections[outlinermaxcrosssections]</td>
</tr>
</tbody>
</table>

Specification of where the cross sections should be drawn, what file
names and labels they should use, etc.

<span id="ab7dea6503e3065f583472dfed0b032cb"></span>

## <span class="permalink">[◆ ](#ab7dea6503e3065f583472dfed0b032cb)</span>tiles

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">unsigned int MainConfig::tiles</td>
</tr>
</tbody>
</table>

How many tiles should we have in each of the two (x,y) dimensions? Each
tile represents a fast index to the 3D model faces within that space.

------------------------------------------------------------------------

The documentation for this class was generated from the following file:

-   src/<a href="mainconfig_8hh_source.md" class="el">mainconfig.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [Doxygen](https://www.doxygen.org/index.md)
1.9.2</span>
