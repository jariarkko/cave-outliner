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

[Static Public Member Functions](#pub-static-methods) | 


OutlinerMath Class Reference

`#include <outlinermath.hh>`

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
<tr class="even memitem:ae1ed222bb08479634293c3770d20e4fb">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="ae1ed222bb08479634293c3770d20e4fb"></span>
static void </td>
<td class="memItemRight"
data-valign="bottom"><strong>triangleDescribe</strong> (const <a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_triangle3_d.md" class="el">OutlinerTriangle3D</a>
&amp;triangle, char *buf, unsigned int bufSize, bool full=0)</td>
</tr>
<tr class="odd memdesc:ae1ed222bb08479634293c3770d20e4fb">
<td class="mdescLeft"> </td>
<td class="mdescRight">Produce a string that describes a triangle. Used
in debugging.<br />
</td>
</tr>
<tr class="even separator:ae1ed222bb08479634293c3770d20e4fb">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a59471e97a02a85bc93ee1701b21f2ea3">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a59471e97a02a85bc93ee1701b21f2ea3"></span>
static void </td>
<td class="memItemRight"
data-valign="bottom"><strong>triangleDescribe</strong> (const <a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_triangle2_d.md" class="el">OutlinerTriangle2D</a>
&amp;triangle, char *buf, unsigned int bufSize, bool full=0)</td>
</tr>
<tr class="even memdesc:a59471e97a02a85bc93ee1701b21f2ea3">
<td class="mdescLeft"> </td>
<td class="mdescRight">Produce a string that describes a triangle. Used
in debugging.<br />
</td>
</tr>
<tr class="odd separator:a59471e97a02a85bc93ee1701b21f2ea3">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a95e46380618ea58995c6282d3451e48d">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a95e46380618ea58995c6282d3451e48d"></span>
static void </td>
<td class="memItemRight"
data-valign="bottom"><strong>boxDescribe</strong> (const <a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box3_d.md" class="el">OutlinerBox3D</a> &amp;box,
char *buf, unsigned int bufSize, bool full=0)</td>
</tr>
<tr class="odd memdesc:a95e46380618ea58995c6282d3451e48d">
<td class="mdescLeft"> </td>
<td class="mdescRight">Produce a string that describes a bounding box.
Used in debugging.<br />
</td>
</tr>
<tr class="even separator:a95e46380618ea58995c6282d3451e48d">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a6eeaf8a7823a28c4b44e75956504507b">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a6eeaf8a7823a28c4b44e75956504507b"></span>
static void </td>
<td class="memItemRight"
data-valign="bottom"><strong>boxDescribe</strong> (const <a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box2_d.md" class="el">OutlinerBox2D</a> &amp;box,
char *buf, unsigned int bufSize, bool full=0)</td>
</tr>
<tr class="even memdesc:a6eeaf8a7823a28c4b44e75956504507b">
<td class="mdescLeft"> </td>
<td class="mdescRight">Produce a string that describes a bounding box.
Used in debugging.<br />
</td>
</tr>
<tr class="odd separator:a6eeaf8a7823a28c4b44e75956504507b">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a8414e86cc4a4e168a5926aff75b5de4e">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a8414e86cc4a4e168a5926aff75b5de4e"></span>
static void </td>
<td class="memItemRight"
data-valign="bottom"><strong>triangleBoundingBox2D</strong> (const <a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_triangle2_d.md" class="el">OutlinerTriangle2D</a>
&amp;triangle, <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box2_d.md"
class="el">OutlinerBox2D</a> &amp;boundingBox)</td>
</tr>
<tr class="odd memdesc:a8414e86cc4a4e168a5926aff75b5de4e">
<td class="mdescLeft"> </td>
<td class="mdescRight">Calculate the smallest bounding box that holds a
given triangle (in a 2-dimension model).<br />
</td>
</tr>
<tr class="even separator:a8414e86cc4a4e168a5926aff75b5de4e">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:ab6b2bb0be2acdc2b9fcbdfe8a9cc6bdf">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="ab6b2bb0be2acdc2b9fcbdfe8a9cc6bdf"></span>
static void </td>
<td class="memItemRight"
data-valign="bottom"><strong>triangleBoundingBox3D</strong> (const <a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_triangle3_d.md" class="el">OutlinerTriangle3D</a>
&amp;triangle, <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box3_d.md"
class="el">OutlinerBox3D</a> &amp;boundingBox)</td>
</tr>
<tr class="even memdesc:ab6b2bb0be2acdc2b9fcbdfe8a9cc6bdf">
<td class="mdescLeft"> </td>
<td class="mdescRight">Calculate the smallest bounding box that holds a
given triangle (in a 3-dimension model).<br />
</td>
</tr>
<tr class="odd separator:ab6b2bb0be2acdc2b9fcbdfe8a9cc6bdf">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a6d0dca3240f2d97ad6d00f7973154e4f">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a6d0dca3240f2d97ad6d00f7973154e4f"></span>
static bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>pointInsideTriangle2D</strong> (const <a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_triangle2_d.md" class="el">OutlinerTriangle2D</a>
&amp;triangle, const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_vector2_d.md"
class="el">OutlinerVector2D</a> &amp;point)</td>
</tr>
<tr class="odd memdesc:a6d0dca3240f2d97ad6d00f7973154e4f">
<td class="mdescLeft"> </td>
<td class="mdescRight">Is a given point inside a triangle (in a
2-dimension model)?<br />
</td>
</tr>
<tr class="even separator:a6d0dca3240f2d97ad6d00f7973154e4f">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a6492576cbe435ac0ccdfb3ae1b8a2f70">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a6492576cbe435ac0ccdfb3ae1b8a2f70"></span>
static bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>boundingBoxIntersectsTriangle2D</strong>
(const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_triangle2_d.md"
class="el">OutlinerTriangle2D</a> &amp;triangle, const <a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box2_d.md" class="el">OutlinerBox2D</a>
&amp;box)</td>
</tr>
<tr class="even memdesc:a6492576cbe435ac0ccdfb3ae1b8a2f70">
<td class="mdescLeft"> </td>
<td class="mdescRight">Does a triangle intersect a bounding box?<br />
</td>
</tr>
<tr class="odd separator:a6492576cbe435ac0ccdfb3ae1b8a2f70">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:aca5779b5b445e27c65887622d3629988">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="aca5779b5b445e27c65887622d3629988"></span>
static bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>boundingBoxIntersectsTriangle3D</strong>
(const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_triangle3_d.md"
class="el">OutlinerTriangle3D</a> &amp;triangle, const <a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box3_d.md" class="el">OutlinerBox3D</a>
&amp;box)</td>
</tr>
<tr class="odd memdesc:aca5779b5b445e27c65887622d3629988">
<td class="mdescLeft"> </td>
<td class="mdescRight">Does a triangle intersect a bounding box?<br />
</td>
</tr>
<tr class="even separator:aca5779b5b445e27c65887622d3629988">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:af0de49f0522dbb81256ec793265ca789">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="af0de49f0522dbb81256ec793265ca789"></span>
static bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>lineIntersectsVerticalLine2D</strong>
(const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_line2_d.md"
class="el">OutlinerLine2D</a> &amp;line, const <a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_line2_d.md" class="el">OutlinerLine2D</a>
&amp;verticalLine, <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_vector2_d.md"
class="el">OutlinerVector2D</a> &amp;intersectionPoint)</td>
</tr>
<tr class="even memdesc:af0de49f0522dbb81256ec793265ca789">
<td class="mdescLeft"> </td>
<td class="mdescRight">Does a line intersect a vertical line?<br />
</td>
</tr>
<tr class="odd separator:af0de49f0522dbb81256ec793265ca789">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a96b2eae2b356b7d39cf01cac5ac38bc4">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a96b2eae2b356b7d39cf01cac5ac38bc4"></span>
static bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>lineIntersectsHorizontalLine2D</strong>
(const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_line2_d.md"
class="el">OutlinerLine2D</a> &amp;line, const <a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_line2_d.md" class="el">OutlinerLine2D</a>
&amp;horizontalLine, <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_vector2_d.md"
class="el">OutlinerVector2D</a> &amp;intersectionPoint)</td>
</tr>
<tr class="odd memdesc:a96b2eae2b356b7d39cf01cac5ac38bc4">
<td class="mdescLeft"> </td>
<td class="mdescRight">Does a line intersect a horizontal line?<br />
</td>
</tr>
<tr class="even separator:a96b2eae2b356b7d39cf01cac5ac38bc4">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:abe22bf1a1c72aa49f55ac20ff8d71874">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="abe22bf1a1c72aa49f55ac20ff8d71874"></span>
static void </td>
<td class="memItemRight" data-valign="bottom"><strong>vectorTo</strong>
(const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_vector2_d.md"
class="el">OutlinerVector2D</a> &amp;from, const <a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_vector2_d.md" class="el">OutlinerVector2D</a>
&amp;to, <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_vector2_d.md"
class="el">OutlinerVector2D</a> &amp;result)</td>
</tr>
<tr class="even memdesc:abe22bf1a1c72aa49f55ac20ff8d71874">
<td class="mdescLeft"> </td>
<td class="mdescRight">Create a vector from "from" to "to", placing the
result in "result".<br />
</td>
</tr>
<tr class="odd separator:abe22bf1a1c72aa49f55ac20ff8d71874">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a4d656c09cf20006dac2cf7ca673e317b">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a4d656c09cf20006dac2cf7ca673e317b"></span>
static void </td>
<td class="memItemRight" data-valign="bottom"><strong>mathTests</strong>
(void)</td>
</tr>
<tr class="odd memdesc:a4d656c09cf20006dac2cf7ca673e317b">
<td class="mdescLeft"> </td>
<td class="mdescRight">Run module tests.<br />
</td>
</tr>
<tr class="even separator:a4d656c09cf20006dac2cf7ca673e317b">
<td colspan="2" class="memSeparator"> </td>
</tr>
</tbody>
</table>

<span id="details"></span>

## Detailed Description

The <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_math.md" class="el">OutlinerMath</a> class
is a geometry and mathematics module. It hosts a number of static
methods that can be used to calculate bounding boxes for triangles,
whether points are inside a triangle, etc.

------------------------------------------------------------------------

The documentation for this class was generated from the following file:

-   src/<a href="outlinermath_8hh_source.md" class="el">outlinermath.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [<img src="doxygen.svg" class="footer" width="104" height="31"
alt="doxygen" />](https://www.doxygen.org/index.md) 1.9.4</span>
