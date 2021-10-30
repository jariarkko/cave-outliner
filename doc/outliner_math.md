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

[Static Public Member Functions](#pub-static-methods) | [List of all
members](class_outliner_math-members.html)

OutlinerMath Class Reference

`#include <outlinermath.hh>`

<table class="memberdecls">
<colgroup>
<col style="width: 50%" />
<col style="width: 50%" />
</colgroup>
<tbody>
<tr class="odd heading">
<td colspan="2"><h2 id="static-public-member-functions" class="groupheader"><span id="pub-static-methods"></span> Static Public Member Functions</h2></td>
</tr>
<tr class="even memitem:ad3416480394fa1ae3cbe4ad6f72aab0e">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="ad3416480394fa1ae3cbe4ad6f72aab0e"></span> static void </td>
<td class="memItemRight" data-valign="bottom"><strong>triangleDescribe</strong> (const <a href="class_outliner_triangle3_d.html" class="el">OutlinerTriangle3D</a> &amp;triangle, char *buf, unsigned int bufSize)</td>
</tr>
<tr class="odd memdesc:ad3416480394fa1ae3cbe4ad6f72aab0e">
<td class="mdescLeft"> </td>
<td class="mdescRight">Produce a string that describes a triangle. Used in debugging.<br />
</td>
</tr>
<tr class="even separator:ad3416480394fa1ae3cbe4ad6f72aab0e">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a8414e86cc4a4e168a5926aff75b5de4e">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a8414e86cc4a4e168a5926aff75b5de4e"></span> static void </td>
<td class="memItemRight" data-valign="bottom"><strong>triangleBoundingBox2D</strong> (const <a href="class_outliner_triangle2_d.html" class="el">OutlinerTriangle2D</a> &amp;triangle, <a href="class_outliner_box2_d.html" class="el">OutlinerBox2D</a> &amp;boundingBox)</td>
</tr>
<tr class="even memdesc:a8414e86cc4a4e168a5926aff75b5de4e">
<td class="mdescLeft"> </td>
<td class="mdescRight">Calculate the smallest bounding box that holds a given triangle (in a 2-dimension model).<br />
</td>
</tr>
<tr class="odd separator:a8414e86cc4a4e168a5926aff75b5de4e">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:ab6b2bb0be2acdc2b9fcbdfe8a9cc6bdf">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="ab6b2bb0be2acdc2b9fcbdfe8a9cc6bdf"></span> static void </td>
<td class="memItemRight" data-valign="bottom"><strong>triangleBoundingBox3D</strong> (const <a href="class_outliner_triangle3_d.html" class="el">OutlinerTriangle3D</a> &amp;triangle, <a href="class_outliner_box3_d.html" class="el">OutlinerBox3D</a> &amp;boundingBox)</td>
</tr>
<tr class="odd memdesc:ab6b2bb0be2acdc2b9fcbdfe8a9cc6bdf">
<td class="mdescLeft"> </td>
<td class="mdescRight">Calculate the smallest bounding box that holds a given triangle (in a 3-dimension model).<br />
</td>
</tr>
<tr class="even separator:ab6b2bb0be2acdc2b9fcbdfe8a9cc6bdf">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a6d0dca3240f2d97ad6d00f7973154e4f">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a6d0dca3240f2d97ad6d00f7973154e4f"></span> static bool </td>
<td class="memItemRight" data-valign="bottom"><strong>pointInsideTriangle2D</strong> (const <a href="class_outliner_triangle2_d.html" class="el">OutlinerTriangle2D</a> &amp;triangle, const <a href="class_outliner_vector2_d.html" class="el">OutlinerVector2D</a> &amp;point)</td>
</tr>
<tr class="even memdesc:a6d0dca3240f2d97ad6d00f7973154e4f">
<td class="mdescLeft"> </td>
<td class="mdescRight">Is a given point inside a triangle (in a 2-dimension model)?<br />
</td>
</tr>
<tr class="odd separator:a6d0dca3240f2d97ad6d00f7973154e4f">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a256f7ce85590deccd7ccd9c7c0cbfeda">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a256f7ce85590deccd7ccd9c7c0cbfeda"></span> static bool </td>
<td class="memItemRight" data-valign="bottom"><strong>pointInsideBoundingBox2D</strong> (const <a href="class_outliner_box2_d.html" class="el">OutlinerBox2D</a> &amp;box, const <a href="class_outliner_vector2_d.html" class="el">OutlinerVector2D</a> &amp;point)</td>
</tr>
<tr class="odd memdesc:a256f7ce85590deccd7ccd9c7c0cbfeda">
<td class="mdescLeft"> </td>
<td class="mdescRight">Is a given point inside a bounding box (in 2-dimension model)?<br />
</td>
</tr>
<tr class="even separator:a256f7ce85590deccd7ccd9c7c0cbfeda">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a6492576cbe435ac0ccdfb3ae1b8a2f70">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a6492576cbe435ac0ccdfb3ae1b8a2f70"></span> static bool </td>
<td class="memItemRight" data-valign="bottom"><strong>boundingBoxIntersectsTriangle2D</strong> (const <a href="class_outliner_triangle2_d.html" class="el">OutlinerTriangle2D</a> &amp;triangle, const <a href="class_outliner_box2_d.html" class="el">OutlinerBox2D</a> &amp;box)</td>
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
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="aca5779b5b445e27c65887622d3629988"></span> static bool </td>
<td class="memItemRight" data-valign="bottom"><strong>boundingBoxIntersectsTriangle3D</strong> (const <a href="class_outliner_triangle3_d.html" class="el">OutlinerTriangle3D</a> &amp;triangle, const <a href="class_outliner_box3_d.html" class="el">OutlinerBox3D</a> &amp;box)</td>
</tr>
<tr class="odd memdesc:aca5779b5b445e27c65887622d3629988">
<td class="mdescLeft"> </td>
<td class="mdescRight">Does a triangle intersect a bounding box?<br />
</td>
</tr>
<tr class="even separator:aca5779b5b445e27c65887622d3629988">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a20c993dbecea94a9b4843f3b1c3e3f75">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a20c993dbecea94a9b4843f3b1c3e3f75"></span> static bool </td>
<td class="memItemRight" data-valign="bottom"><strong>boundingBoxesIntersect3D</strong> (<a href="class_outliner_box3_d.html" class="el">OutlinerBox3D</a> &amp;boundingBox1, <a href="class_outliner_box3_d.html" class="el">OutlinerBox3D</a> &amp;boundingBox2)</td>
</tr>
<tr class="even memdesc:a20c993dbecea94a9b4843f3b1c3e3f75">
<td class="mdescLeft"> </td>
<td class="mdescRight">Does a bounding box intersect another one?<br />
</td>
</tr>
<tr class="odd separator:a20c993dbecea94a9b4843f3b1c3e3f75">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:aa959a9730dc1dd2ce9ddd1a96ade17e6">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="aa959a9730dc1dd2ce9ddd1a96ade17e6"></span> static bool </td>
<td class="memItemRight" data-valign="bottom"><strong>pointOnLine2D</strong> (const <a href="class_outliner_line2_d.html" class="el">OutlinerLine2D</a> &amp;line, const <a href="class_outliner_vector2_d.html" class="el">OutlinerVector2D</a> &amp;point)</td>
</tr>
<tr class="odd memdesc:aa959a9730dc1dd2ce9ddd1a96ade17e6">
<td class="mdescLeft"> </td>
<td class="mdescRight">Is a given point on a line?<br />
</td>
</tr>
<tr class="even separator:aa959a9730dc1dd2ce9ddd1a96ade17e6">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:af0de49f0522dbb81256ec793265ca789">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="af0de49f0522dbb81256ec793265ca789"></span> static bool </td>
<td class="memItemRight" data-valign="bottom"><strong>lineIntersectsVerticalLine2D</strong> (const <a href="class_outliner_line2_d.html" class="el">OutlinerLine2D</a> &amp;line, const <a href="class_outliner_line2_d.html" class="el">OutlinerLine2D</a> &amp;verticalLine, <a href="class_outliner_vector2_d.html" class="el">OutlinerVector2D</a> &amp;intersectionPoint)</td>
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
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a96b2eae2b356b7d39cf01cac5ac38bc4"></span> static bool </td>
<td class="memItemRight" data-valign="bottom"><strong>lineIntersectsHorizontalLine2D</strong> (const <a href="class_outliner_line2_d.html" class="el">OutlinerLine2D</a> &amp;line, const <a href="class_outliner_line2_d.html" class="el">OutlinerLine2D</a> &amp;horizontalLine, <a href="class_outliner_vector2_d.html" class="el">OutlinerVector2D</a> &amp;intersectionPoint)</td>
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
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="abe22bf1a1c72aa49f55ac20ff8d71874"></span> static void </td>
<td class="memItemRight" data-valign="bottom"><strong>vectorTo</strong> (const <a href="class_outliner_vector2_d.html" class="el">OutlinerVector2D</a> &amp;from, const <a href="class_outliner_vector2_d.html" class="el">OutlinerVector2D</a> &amp;to, <a href="class_outliner_vector2_d.html" class="el">OutlinerVector2D</a> &amp;result)</td>
</tr>
<tr class="even memdesc:abe22bf1a1c72aa49f55ac20ff8d71874">
<td class="mdescLeft"> </td>
<td class="mdescRight">Create a vector from "from" to "to", placing the result in "result".<br />
</td>
</tr>
<tr class="odd separator:abe22bf1a1c72aa49f55ac20ff8d71874">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a4d656c09cf20006dac2cf7ca673e317b">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a4d656c09cf20006dac2cf7ca673e317b"></span> static void </td>
<td class="memItemRight" data-valign="bottom"><strong>mathTests</strong> (void)</td>
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

The <a href="class_outliner_math.html" class="el">OutlinerMath</a> class
is a geometry and mathematics module. It hosts a number of static
methods that can be used to calculate bounding boxes for triangles,
whether points are inside a triangle, etc.

------------------------------------------------------------------------

The documentation for this class was generated from the following file:

-   src/<a href="outlinermath_8hh_source.html" class="el">outlinermath.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [<img src="doxygen.svg" class="footer" width="104" height="31" alt="doxygen" />](https://www.doxygen.org/index.html)
1.9.2</span>
