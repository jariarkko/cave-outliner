<table data-cellspacing="0" data-cellpadding="0">
<colgroup>
<col style="width: 100%" />
</colgroup>
<tbody>
<tr id="projectrow" class="odd">
<td id="projectalign"><div id="projectname">
Cave Outliner
</div></td>
</tr>
</tbody>
</table>

[Public Member Functions](#pub-methods) | [Static Public Member
Functions](#pub-static-methods) | [Public Attributes](#pub-attribs) |
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
<td colspan="2"><h2 id="public-member-functions"
class="groupheader"><span id="pub-methods"></span> Public Member
Functions</h2></td>
</tr>
<tr class="even memitem:a15d38e887e1854ec847a015d7be6f3a6">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a15d38e887e1854ec847a015d7be6f3a6"></span>
 </td>
<td class="memItemRight"
data-valign="bottom"><strong>MainConfig</strong> ()</td>
</tr>
<tr class="odd memdesc:a15d38e887e1854ec847a015d7be6f3a6">
<td class="mdescLeft"> </td>
<td class="mdescRight">Create the configuration object, with default
values.<br />
</td>
</tr>
<tr class="even separator:a15d38e887e1854ec847a015d7be6f3a6">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a7f59a98ff0252fff656716c18a711d6d">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a7f59a98ff0252fff656716c18a711d6d"></span>
 </td>
<td class="memItemRight"
data-valign="bottom"><strong>~MainConfig</strong> ()</td>
</tr>
<tr class="even memdesc:a7f59a98ff0252fff656716c18a711d6d">
<td class="mdescLeft"> </td>
<td class="mdescRight">Release all resources for the configuration
object.<br />
</td>
</tr>
<tr class="odd separator:a7f59a98ff0252fff656716c18a711d6d">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a106c3302252b635648bfa601ebc95e5a">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a106c3302252b635648bfa601ebc95e5a"></span>
const char * </td>
<td class="memItemRight"
data-valign="bottom"><strong>getCrossSectionLabel</strong> (void)</td>
</tr>
<tr class="odd memdesc:a106c3302252b635648bfa601ebc95e5a">
<td class="mdescLeft"> </td>
<td class="mdescRight">Get a label suitable for a cross section ("A",
"B", etc.)<br />
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
<td colspan="2"><h2 id="static-public-member-functions"
class="groupheader"><span id="pub-static-methods"></span> Static Public
Member Functions</h2></td>
</tr>
<tr class="even memitem:ae7f0dfc9f6c39766aa7bef676e9715f3">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="ae7f0dfc9f6c39766aa7bef676e9715f3"></span>
static void </td>
<td class="memItemRight"
data-valign="bottom"><strong>convertToDate</strong> (time_t when, char
*buf, size_t bufSize)</td>
</tr>
<tr class="odd memdesc:ae7f0dfc9f6c39766aa7bef676e9715f3">
<td class="mdescLeft"> </td>
<td class="mdescRight">Auxiliary function to convert time values to
dates.<br />
</td>
</tr>
<tr class="even separator:ae7f0dfc9f6c39766aa7bef676e9715f3">
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
<td colspan="2"><h2 id="public-attributes" class="groupheader"><span
id="pub-attribs"></span> Public Attributes</h2></td>
</tr>
<tr class="even memitem:a746069bfe670f0de7aca12eb1beff1bd">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a746069bfe670f0de7aca12eb1beff1bd"></span>
char * </td>
<td class="memItemRight"
data-valign="bottom"><strong>inputFile</strong></td>
</tr>
<tr class="odd memdesc:a746069bfe670f0de7aca12eb1beff1bd">
<td class="mdescLeft"> </td>
<td class="mdescRight">Input 3D model file name.<br />
</td>
</tr>
<tr class="even separator:a746069bfe670f0de7aca12eb1beff1bd">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a682b5902ea929032c542d939db5cb036">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a682b5902ea929032c542d939db5cb036"></span>
char * </td>
<td class="memItemRight"
data-valign="bottom"><strong>planViewOutputFile</strong></td>
</tr>
<tr class="even memdesc:a682b5902ea929032c542d939db5cb036">
<td class="mdescLeft"> </td>
<td class="mdescRight">Output SVG image file name for the plan
view.<br />
</td>
</tr>
<tr class="odd separator:a682b5902ea929032c542d939db5cb036">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:aeebc04cbb9b295ddcdf680743022bd07">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="aeebc04cbb9b295ddcdf680743022bd07"></span>
char * </td>
<td class="memItemRight"
data-valign="bottom"><strong>compositeOutputFile</strong></td>
</tr>
<tr class="odd memdesc:aeebc04cbb9b295ddcdf680743022bd07">
<td class="mdescLeft"> </td>
<td class="mdescRight">Output SVG image file name for the composite
view.<br />
</td>
</tr>
<tr class="even separator:aeebc04cbb9b295ddcdf680743022bd07">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:ad017d93965c353339db7a31b07b68477">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="ad017d93965c353339db7a31b07b68477"></span>
bool </td>
<td class="memItemRight" data-valign="bottom"><strong>test</strong></td>
</tr>
<tr class="even memdesc:ad017d93965c353339db7a31b07b68477">
<td class="mdescLeft"> </td>
<td class="mdescRight">True if we should run unit tests.<br />
</td>
</tr>
<tr class="odd separator:ad017d93965c353339db7a31b07b68477">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a60ad99f0c041f9730c10a0f975916e76">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a60ad99f0c041f9730c10a0f975916e76"></span>
unsigned int </td>
<td class="memItemRight"
data-valign="bottom"><strong>nThreads</strong></td>
</tr>
<tr class="odd memdesc:a60ad99f0c041f9730c10a0f975916e76">
<td class="mdescLeft"> </td>
<td class="mdescRight">How many parallel threads we can run.<br />
</td>
</tr>
<tr class="even separator:a60ad99f0c041f9730c10a0f975916e76">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:ad7eb409fcc677af87aaaad7597cf2397">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="ad7eb409fcc677af87aaaad7597cf2397"></span>
bool </td>
<td class="memItemRight" data-valign="bottom"><strong>info</strong></td>
</tr>
<tr class="even memdesc:ad7eb409fcc677af87aaaad7597cf2397">
<td class="mdescLeft"> </td>
<td class="mdescRight">True if informational messages should be
displayed.<br />
</td>
</tr>
<tr class="odd separator:ad7eb409fcc677af87aaaad7597cf2397">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:af100bbcba49c61c01be6adcc5ebd5a5d">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="af100bbcba49c61c01be6adcc5ebd5a5d"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>debug</strong></td>
</tr>
<tr class="odd memdesc:af100bbcba49c61c01be6adcc5ebd5a5d">
<td class="mdescLeft"> </td>
<td class="mdescRight">True if level 1 debugging should be
provided.<br />
</td>
</tr>
<tr class="even separator:af100bbcba49c61c01be6adcc5ebd5a5d">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a9370cf33e6ff22245fcf9e82b210a9dc">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a9370cf33e6ff22245fcf9e82b210a9dc"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>deepdebug</strong></td>
</tr>
<tr class="even memdesc:a9370cf33e6ff22245fcf9e82b210a9dc">
<td class="mdescLeft"> </td>
<td class="mdescRight">True if level 2 debugging should be
provided.<br />
</td>
</tr>
<tr class="odd separator:a9370cf33e6ff22245fcf9e82b210a9dc">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a698f2ef432a1e0f4a652c1f7237ee3e1">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a698f2ef432a1e0f4a652c1f7237ee3e1"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>deepdeepdebug</strong></td>
</tr>
<tr class="odd memdesc:a698f2ef432a1e0f4a652c1f7237ee3e1">
<td class="mdescLeft"> </td>
<td class="mdescRight">True if level 3 debugging should be
provided.<br />
</td>
</tr>
<tr class="even separator:a698f2ef432a1e0f4a652c1f7237ee3e1">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a39d08da53f75d634f7470baa333954ff">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a39d08da53f75d634f7470baa333954ff"></span>
float </td>
<td class="memItemRight"
data-valign="bottom"><strong>stepx</strong></td>
</tr>
<tr class="even memdesc:a39d08da53f75d634f7470baa333954ff">
<td class="mdescLeft"> </td>
<td class="mdescRight">In how small steps the model should be scanned in
x coordinate direction.<br />
</td>
</tr>
<tr class="odd separator:a39d08da53f75d634f7470baa333954ff">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a9adf62267f6a04d243527ae0079ee7dc">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a9adf62267f6a04d243527ae0079ee7dc"></span>
float </td>
<td class="memItemRight"
data-valign="bottom"><strong>stepy</strong></td>
</tr>
<tr class="odd memdesc:a9adf62267f6a04d243527ae0079ee7dc">
<td class="mdescLeft"> </td>
<td class="mdescRight">In how small steps the model should be scanned in
y coordinate direction.<br />
</td>
</tr>
<tr class="even separator:a9adf62267f6a04d243527ae0079ee7dc">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a32f41fad1f479c9287136d33ba783808">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a32f41fad1f479c9287136d33ba783808"></span>
float </td>
<td class="memItemRight"
data-valign="bottom"><strong>stepz</strong></td>
</tr>
<tr class="even memdesc:a32f41fad1f479c9287136d33ba783808">
<td class="mdescLeft"> </td>
<td class="mdescRight">In how small steps the model should be scanned in
z coordinate direction.<br />
</td>
</tr>
<tr class="odd separator:a32f41fad1f479c9287136d33ba783808">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a751762be7967919edab5352222e70938">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a751762be7967919edab5352222e70938"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>boundingBoxSet</strong></td>
</tr>
<tr class="odd memdesc:a751762be7967919edab5352222e70938">
<td class="mdescLeft"> </td>
<td class="mdescRight">True if the bounding box has been
specified.<br />
</td>
</tr>
<tr class="even separator:a751762be7967919edab5352222e70938">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a67db8a9dbc362254f6bbc4f4be2e6fd8">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a67db8a9dbc362254f6bbc4f4be2e6fd8"></span>
<a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box3_d.md" class="el">OutlinerBox3D</a> </td>
<td class="memItemRight"
data-valign="bottom"><strong>boundingBox</strong></td>
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
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a7fbc03c7edd683c667ec1aad26928cd3"></span>
enum outlinerdirection </td>
<td class="memItemRight"
data-valign="bottom"><strong>direction</strong></td>
</tr>
<tr class="odd memdesc:a7fbc03c7edd683c667ec1aad26928cd3">
<td class="mdescLeft"> </td>
<td class="mdescRight">Direction of view for the plan view (usually
along z axis).<br />
</td>
</tr>
<tr class="even separator:a7fbc03c7edd683c667ec1aad26928cd3">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a6ebfc62c1697ee8f394eb3438e99c57d">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a6ebfc62c1697ee8f394eb3438e99c57d"></span>
enum outlineralgorithm </td>
<td class="memItemRight"
data-valign="bottom"><strong>algorithm</strong></td>
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
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a05d95cac036e6a49223f5137a0267769"></span>
float </td>
<td class="memItemRight"
data-valign="bottom"><strong>linewidth</strong></td>
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
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a0437d68ad9083b118d855941636472fb"></span>
unsigned int </td>
<td class="memItemRight"
data-valign="bottom"><strong>multiplier</strong></td>
</tr>
<tr class="even memdesc:a0437d68ad9083b118d855941636472fb">
<td class="mdescLeft"> </td>
<td class="mdescRight">Should one inspected step in the model result in
one pixel, or multiplied to a number of pixels in the image?<br />
</td>
</tr>
<tr class="odd separator:a0437d68ad9083b118d855941636472fb">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:aafa6ddf8f6e7bdde0471332e80664bd8">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="aafa6ddf8f6e7bdde0471332e80664bd8"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>smooth</strong></td>
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
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a123e679e27a2ea8531bf18b6e4b892c7"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>mergedLines</strong></td>
</tr>
<tr class="even memdesc:a123e679e27a2ea8531bf18b6e4b892c7">
<td class="mdescLeft"> </td>
<td class="mdescRight">True if short line segments should be merged in
an SVG image.<br />
</td>
</tr>
<tr class="odd separator:a123e679e27a2ea8531bf18b6e4b892c7">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a930e82157586e7339aeea41911cc9854">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a930e82157586e7339aeea41911cc9854"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>svgYSwap</strong></td>
</tr>
<tr class="odd memdesc:a930e82157586e7339aeea41911cc9854">
<td class="mdescLeft"> </td>
<td class="mdescRight">True if we should swap model and SVG y
coordinates from 0..n to n...0 (this is the default).<br />
</td>
</tr>
<tr class="even separator:a930e82157586e7339aeea41911cc9854">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:ab7dea6503e3065f583472dfed0b032cb">
<td class="memItemLeft" style="text-align: right;"
data-valign="top">unsigned int </td>
<td class="memItemRight" data-valign="bottom"><a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_main_config.md#ab7dea6503e3065f583472dfed0b032cb"
class="el">tiles</a></td>
</tr>
<tr class="even separator:ab7dea6503e3065f583472dfed0b032cb">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a8a948b1f3ef79cd3ed36434a3a961bda">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a8a948b1f3ef79cd3ed36434a3a961bda"></span>
unsigned int </td>
<td class="memItemRight"
data-valign="bottom"><strong>holeThreshold</strong></td>
</tr>
<tr class="even memdesc:a8a948b1f3ef79cd3ed36434a3a961bda">
<td class="mdescLeft"> </td>
<td class="mdescRight">If the model has holes, e.g., missing faces for
various reasons, should the software try to fill them?<br />
</td>
</tr>
<tr class="odd separator:a8a948b1f3ef79cd3ed36434a3a961bda">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a2ab9bf3a2a0d03ac4231a6994b2599a4">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a2ab9bf3a2a0d03ac4231a6994b2599a4"></span>
unsigned int </td>
<td class="memItemRight"
data-valign="bottom"><strong>lineHoleThreshold</strong></td>
</tr>
<tr class="odd memdesc:a2ab9bf3a2a0d03ac4231a6994b2599a4">
<td class="mdescLeft"> </td>
<td class="mdescRight">If the cross section lines have holes, e.g.,
missing faces for various reasons, should the software try to fill
them?<br />
</td>
</tr>
<tr class="even separator:a2ab9bf3a2a0d03ac4231a6994b2599a4">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a300cbe933bf0be13fa16ce213aaaaca5">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a300cbe933bf0be13fa16ce213aaaaca5"></span>
unsigned int </td>
<td class="memItemRight"
data-valign="bottom"><strong>dustThreshold</strong></td>
</tr>
<tr class="even memdesc:a300cbe933bf0be13fa16ce213aaaaca5">
<td class="mdescLeft"> </td>
<td class="mdescRight">Can remove small isolated pieces of model?<br />
</td>
</tr>
<tr class="odd separator:a300cbe933bf0be13fa16ce213aaaaca5">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a354d916bbd33ea81920f7328ff330a68">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a354d916bbd33ea81920f7328ff330a68"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>automaticCrossSections</strong></td>
</tr>
<tr class="odd memdesc:a354d916bbd33ea81920f7328ff330a68">
<td class="mdescLeft"> </td>
<td class="mdescRight">Should we create a number of automatic cross
sections?<br />
</td>
</tr>
<tr class="even separator:a354d916bbd33ea81920f7328ff330a68">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a7212cc83ab0c5c9b802d11f3cddf5317">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a7212cc83ab0c5c9b802d11f3cddf5317"></span>
enum outlinerdirection </td>
<td class="memItemRight"
data-valign="bottom"><strong>automaticCrossSectionsDirection</strong></td>
</tr>
<tr class="even memdesc:a7212cc83ab0c5c9b802d11f3cddf5317">
<td class="mdescLeft"> </td>
<td class="mdescRight">Direction of the automatic cross sections.<br />
</td>
</tr>
<tr class="odd separator:a7212cc83ab0c5c9b802d11f3cddf5317">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a85f1c3ffeffda3f50ab8a87ad3edfb94">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a85f1c3ffeffda3f50ab8a87ad3edfb94"></span>
unsigned int </td>
<td class="memItemRight"
data-valign="bottom"><strong>nAutomaticCrossSections</strong></td>
</tr>
<tr class="odd memdesc:a85f1c3ffeffda3f50ab8a87ad3edfb94">
<td class="mdescLeft"> </td>
<td class="mdescRight">How many automatic cross sections do we
wish?<br />
</td>
</tr>
<tr class="even separator:a85f1c3ffeffda3f50ab8a87ad3edfb94">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:ae25f8aab936149c64e547a7a15290db3">
<td class="memItemLeft" style="text-align: right;"
data-valign="top">const char * </td>
<td class="memItemRight" data-valign="bottom"><a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_main_config.md#ae25f8aab936149c64e547a7a15290db3"
class="el">automaticCrossSectionFilenamePattern</a></td>
</tr>
<tr class="even separator:ae25f8aab936149c64e547a7a15290db3">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a95ed0995830abb80cc33f6f090c2ed3e">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a95ed0995830abb80cc33f6f090c2ed3e"></span>
unsigned int </td>
<td class="memItemRight"
data-valign="bottom"><strong>nCrossSections</strong></td>
</tr>
<tr class="even memdesc:a95ed0995830abb80cc33f6f090c2ed3e">
<td class="mdescLeft"> </td>
<td class="mdescRight">How many cross sections did we in the end have?
(Manual + automatic).<br />
</td>
</tr>
<tr class="odd separator:a95ed0995830abb80cc33f6f090c2ed3e">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a943b8f45ebf2bcadb841f193e2676019">
<td class="memItemLeft" style="text-align: right;"
data-valign="top">struct <a
href="struct_processor_cross_section_info.md"
class="el">ProcessorCrossSectionInfo</a> </td>
<td class="memItemRight" data-valign="bottom"><a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_main_config.md#a943b8f45ebf2bcadb841f193e2676019"
class="el">crossSections</a> [outlinermaxcrosssections]</td>
</tr>
<tr class="odd separator:a943b8f45ebf2bcadb841f193e2676019">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a1ea93c50a9412141972de1c57374f9b5">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a1ea93c50a9412141972de1c57374f9b5"></span>
enum outlinerdirection </td>
<td class="memItemRight"
data-valign="bottom"><strong>crossSectionDirections</strong>
[outlinermaxcrosssections]</td>
</tr>
<tr class="odd separator:a1ea93c50a9412141972de1c57374f9b5">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:ad4033173ac5c13d59617c6eb307e9839">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="ad4033173ac5c13d59617c6eb307e9839"></span>
outlinerreal </td>
<td class="memItemRight"
data-valign="bottom"><strong>crossSectionPoints</strong>
[outlinermaxcrosssections]</td>
</tr>
<tr class="odd separator:ad4033173ac5c13d59617c6eb307e9839">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:add8090a6f001d2f1c954ae69f5951ae4">
<td class="memItemLeft" style="text-align: right;"
data-valign="top">outlinerreal </td>
<td class="memItemRight" data-valign="bottom"><a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_main_config.md#add8090a6f001d2f1c954ae69f5951ae4"
class="el">crossSectionWidth</a></td>
</tr>
<tr class="odd separator:add8090a6f001d2f1c954ae69f5951ae4">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a3a9a13ec687319ca5101f0b03c9aa680">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a3a9a13ec687319ca5101f0b03c9aa680"></span>
const char * </td>
<td class="memItemRight"
data-valign="bottom"><strong>floorDepthMap</strong></td>
</tr>
<tr class="odd memdesc:a3a9a13ec687319ca5101f0b03c9aa680">
<td class="mdescLeft"> </td>
<td class="mdescRight">Should we output the cave floor depth map to a
separate file?<br />
</td>
</tr>
<tr class="even separator:a3a9a13ec687319ca5101f0b03c9aa680">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:aafeb8c4c5864c28ee688d2825f3a71d5">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="aafeb8c4c5864c28ee688d2825f3a71d5"></span>
const char * </td>
<td class="memItemRight"
data-valign="bottom"><strong>roofDepthMap</strong></td>
</tr>
<tr class="even memdesc:aafeb8c4c5864c28ee688d2825f3a71d5">
<td class="mdescLeft"> </td>
<td class="mdescRight">Should we output the cave roof depth map to a
separate file?<br />
</td>
</tr>
<tr class="odd separator:aafeb8c4c5864c28ee688d2825f3a71d5">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:afd77e9249b60c6c2e1982d89e56e0700">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="afd77e9249b60c6c2e1982d89e56e0700"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>floorStyleDiff</strong></td>
</tr>
<tr class="odd memdesc:afd77e9249b60c6c2e1982d89e56e0700">
<td class="mdescLeft"> </td>
<td class="mdescRight">Floor style, either depth map (0) or differential
depth map (1).<br />
</td>
</tr>
<tr class="even separator:afd77e9249b60c6c2e1982d89e56e0700">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a3d0661ecb055ca1128553ecdfbead687">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a3d0661ecb055ca1128553ecdfbead687"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>tunnelSpine</strong></td>
</tr>
<tr class="even memdesc:a3d0661ecb055ca1128553ecdfbead687">
<td class="mdescLeft"> </td>
<td class="mdescRight">Should we show tunnel 'spines', i.e., tunnel
midpoints?<br />
</td>
</tr>
<tr class="odd separator:a3d0661ecb055ca1128553ecdfbead687">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a903dc0119ec4155627ce8bb3531f4d86">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a903dc0119ec4155627ce8bb3531f4d86"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>labelCrossSections</strong></td>
</tr>
<tr class="odd memdesc:a903dc0119ec4155627ce8bb3531f4d86">
<td class="mdescLeft"> </td>
<td class="mdescRight">Should we label cross sections in the
images?<br />
</td>
</tr>
<tr class="even separator:a903dc0119ec4155627ce8bb3531f4d86">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:ade3b2a5c0fdcf0498e926504267bdb47">
<td class="memItemLeft" style="text-align: right;"
data-valign="top">bool </td>
<td class="memItemRight" data-valign="bottom"><a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_main_config.md#ade3b2a5c0fdcf0498e926504267bdb47"
class="el">formAnalysis</a></td>
</tr>
<tr class="even separator:ade3b2a5c0fdcf0498e926504267bdb47">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a61daa40a3788ca081f7b506c35842062">
<td class="memItemLeft" style="text-align: right;"
data-valign="top">unsigned int </td>
<td class="memItemRight" data-valign="bottom"><a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_main_config.md#a61daa40a3788ca081f7b506c35842062"
class="el">formCondense</a></td>
</tr>
<tr class="even separator:a61daa40a3788ca081f7b506c35842062">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a936c42b6569e36fbc32cb0a2401fc4b0">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a936c42b6569e36fbc32cb0a2401fc4b0"></span>
const char * </td>
<td class="memItemRight" data-valign="bottom"><strong>name</strong></td>
</tr>
<tr class="even memdesc:a936c42b6569e36fbc32cb0a2401fc4b0">
<td class="mdescLeft"> </td>
<td class="mdescRight">What is the name of the cave?<br />
</td>
</tr>
<tr class="odd separator:a936c42b6569e36fbc32cb0a2401fc4b0">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:af9e8fae5d0d55ef9234df5cc77212cc2">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="af9e8fae5d0d55ef9234df5cc77212cc2"></span>
const char * </td>
<td class="memItemRight"
data-valign="bottom"><strong>surveyer</strong></td>
</tr>
<tr class="odd memdesc:af9e8fae5d0d55ef9234df5cc77212cc2">
<td class="mdescLeft"> </td>
<td class="mdescRight">Who performed the 3D scan?<br />
</td>
</tr>
<tr class="even separator:af9e8fae5d0d55ef9234df5cc77212cc2">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:ad86a93298d13059896f83bcaa6e99408">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="ad86a93298d13059896f83bcaa6e99408"></span>
const char * </td>
<td class="memItemRight"
data-valign="bottom"><strong>surveyTool</strong></td>
</tr>
<tr class="even memdesc:ad86a93298d13059896f83bcaa6e99408">
<td class="mdescLeft"> </td>
<td class="mdescRight">What tool was used in the 3D scan?<br />
</td>
</tr>
<tr class="odd separator:ad86a93298d13059896f83bcaa6e99408">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:aa76bd3e909e7f4a9eda791af10725f84">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="aa76bd3e909e7f4a9eda791af10725f84"></span>
const char * </td>
<td class="memItemRight"
data-valign="bottom"><strong>surveyDate</strong></td>
</tr>
<tr class="odd memdesc:aa76bd3e909e7f4a9eda791af10725f84">
<td class="mdescLeft"> </td>
<td class="mdescRight">When was the survey performed?<br />
</td>
</tr>
<tr class="even separator:aa76bd3e909e7f4a9eda791af10725f84">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:ae2c5541621aee7ff4a0e0c758b364351">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="ae2c5541621aee7ff4a0e0c758b364351"></span>
const char * </td>
<td class="memItemRight"
data-valign="bottom"><strong>mapDate</strong></td>
</tr>
<tr class="even memdesc:ae2c5541621aee7ff4a0e0c758b364351">
<td class="mdescLeft"> </td>
<td class="mdescRight">When was the map drawn (perhaps generated by this
tool)?<br />
</td>
</tr>
<tr class="odd separator:ae2c5541621aee7ff4a0e0c758b364351">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:aebc647da78206666ec6bc741e24090a4">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="aebc647da78206666ec6bc741e24090a4"></span>
const char * </td>
<td class="memItemRight"
data-valign="bottom"><strong>location</strong></td>
</tr>
<tr class="odd memdesc:aebc647da78206666ec6bc741e24090a4">
<td class="mdescLeft"> </td>
<td class="mdescRight">Where is the cave? What city or municipality or
country?<br />
</td>
</tr>
<tr class="even separator:aebc647da78206666ec6bc741e24090a4">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a484e9215aac07b1018cf6052edb3e572">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a484e9215aac07b1018cf6052edb3e572"></span>
const char * </td>
<td class="memItemRight"
data-valign="bottom"><strong>coordinates</strong></td>
</tr>
<tr class="even memdesc:a484e9215aac07b1018cf6052edb3e572">
<td class="mdescLeft"> </td>
<td class="mdescRight">Where is the cave? What are the
coordinates?<br />
</td>
</tr>
<tr class="odd separator:a484e9215aac07b1018cf6052edb3e572">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:ad77d9349d5862ec6c1878e2aa4ffa767">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="ad77d9349d5862ec6c1878e2aa4ffa767"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>compositeMap</strong></td>
</tr>
<tr class="odd memdesc:ad77d9349d5862ec6c1878e2aa4ffa767">
<td class="mdescLeft"> </td>
<td class="mdescRight">Create a composite map of the plan view and all
cross sections.<br />
</td>
</tr>
<tr class="even separator:ad77d9349d5862ec6c1878e2aa4ffa767">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a1525af4e5fd0a285c25bc7ca4471e8ed">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a1525af4e5fd0a285c25bc7ca4471e8ed"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>dimensions</strong></td>
</tr>
<tr class="even memdesc:a1525af4e5fd0a285c25bc7ca4471e8ed">
<td class="mdescLeft"> </td>
<td class="mdescRight">Should we show dimensions?<br />
</td>
</tr>
<tr class="odd separator:a1525af4e5fd0a285c25bc7ca4471e8ed">
<td colspan="2" class="memSeparator"> </td>
</tr>
</tbody>
</table>

<span id="details"></span>

## Detailed Description

The <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_main_config.md" class="el">MainConfig</a> class
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
<td class="memname">const char*
MainConfig::automaticCrossSectionFilenamePattern</td>
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
<td class="memname">struct <a
href="struct_processor_cross_section_info.md"
class="el">ProcessorCrossSectionInfo</a>
MainConfig::crossSections[outlinermaxcrosssections]</td>
</tr>
</tbody>
</table>

Specification of where the cross sections should be drawn, what file
names and labels they should use, etc.

<span id="add8090a6f001d2f1c954ae69f5951ae4"></span>

## <span class="permalink">[◆ ](#add8090a6f001d2f1c954ae69f5951ae4)</span>crossSectionWidth

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">outlinerreal MainConfig::crossSectionWidth</td>
</tr>
</tbody>
</table>

Width of cross sections (in unit of one step, e.g., 2.0 means analysis
of a cross slice looks at two steps of 3D model data).

<span id="ade3b2a5c0fdcf0498e926504267bdb47"></span>

## <span class="permalink">[◆ ](#ade3b2a5c0fdcf0498e926504267bdb47)</span>formAnalysis

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">bool MainConfig::formAnalysis</td>
</tr>
</tbody>
</table>

Should cave outliner perform form analysis of cave entrances, stones,
drops, stalactites, stalagmites, etc?

<span id="a61daa40a3788ca081f7b506c35842062"></span>

## <span class="permalink">[◆ ](#a61daa40a3788ca081f7b506c35842062)</span>formCondense

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">unsigned int MainConfig::formCondense</td>
</tr>
</tbody>
</table>

If forms are analysed, what's the compression factor with respect to the
picture size?

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
by [<img src="doxygen.svg" class="footer" width="104" height="31"
alt="doxygen" />](https://www.doxygen.org/index.md) 1.9.4</span>
