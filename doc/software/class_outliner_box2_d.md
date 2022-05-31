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
 members](class_outliner_box2_d-members.md)

OutlinerBox2D Class Reference

`#include <outlinerhighprecision.hh>`

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
<tr class="even memitem:afd80d51de4976ed91fbd63fcf5332ad0">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="afd80d51de4976ed91fbd63fcf5332ad0"></span>
 </td>
<td class="memItemRight"
data-valign="bottom"><strong>OutlinerBox2D</strong> ()</td>
</tr>
<tr class="odd memdesc:afd80d51de4976ed91fbd63fcf5332ad0">
<td class="mdescLeft"> </td>
<td class="mdescRight">Create an empty object, all values initialized to
zero.<br />
</td>
</tr>
<tr class="even separator:afd80d51de4976ed91fbd63fcf5332ad0">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a77672a7d5994f41c4b8d4c56c06725f1">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a77672a7d5994f41c4b8d4c56c06725f1"></span>
 </td>
<td class="memItemRight"
data-valign="bottom"><strong>OutlinerBox2D</strong> (const <a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_line2_d.md" class="el">OutlinerLine2D</a>
&amp;in)</td>
</tr>
<tr class="even memdesc:a77672a7d5994f41c4b8d4c56c06725f1">
<td class="mdescLeft"> </td>
<td class="mdescRight">Make an exact copy of another similar
object.<br />
</td>
</tr>
<tr class="odd separator:a77672a7d5994f41c4b8d4c56c06725f1">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:acbb553458d1bae36f7a920d4bacd0a16">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="acbb553458d1bae36f7a920d4bacd0a16"></span>
 </td>
<td class="memItemRight"
data-valign="bottom"><strong>OutlinerBox2D</strong> (const <a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_vector2_d.md" class="el">OutlinerVector2D</a>
&amp;startIn, const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_vector2_d.md"
class="el">OutlinerVector2D</a> &amp;endIn)</td>
</tr>
<tr class="odd memdesc:acbb553458d1bae36f7a920d4bacd0a16">
<td class="mdescLeft"> </td>
<td class="mdescRight">Make a bounding box based on given two
points.<br />
</td>
</tr>
<tr class="even separator:acbb553458d1bae36f7a920d4bacd0a16">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:ac5df1a466dedeb376a8eeb724288eb84">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"> </td>
<td class="memItemRight" data-valign="bottom"><a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box2_d.md#ac5df1a466dedeb376a8eeb724288eb84"
class="el">OutlinerBox2D</a> (const outlinerreal startX, const
outlinerreal startY, const outlinerreal endX, const outlinerreal
endY)</td>
</tr>
<tr class="even separator:ac5df1a466dedeb376a8eeb724288eb84">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:aeb8a3d57a4fda13948e9844ad7b36bba">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="aeb8a3d57a4fda13948e9844ad7b36bba"></span>
<a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box2_d.md" class="el">OutlinerBox2D</a>
&amp; </td>
<td class="memItemRight" data-valign="bottom"><strong>operator=</strong>
(const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box2_d.md" class="el">OutlinerBox2D</a>
&amp;in)</td>
</tr>
<tr class="even memdesc:aeb8a3d57a4fda13948e9844ad7b36bba">
<td class="mdescLeft"> </td>
<td class="mdescRight">Assignment.<br />
</td>
</tr>
<tr class="odd separator:aeb8a3d57a4fda13948e9844ad7b36bba">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:ae7909e320812130d353ad424607e12a7">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="ae7909e320812130d353ad424607e12a7"></span>
bool </td>
<td class="memItemRight" data-valign="bottom"><strong>equal</strong>
(const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box2_d.md" class="el">OutlinerBox2D</a>
&amp;box2) const</td>
</tr>
<tr class="odd memdesc:ae7909e320812130d353ad424607e12a7">
<td class="mdescLeft"> </td>
<td class="mdescRight">Test for equality.<br />
</td>
</tr>
<tr class="even separator:ae7909e320812130d353ad424607e12a7">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:af895cbe23599dab75ad4df554ffc17b2">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="af895cbe23599dab75ad4df554ffc17b2"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>pointInside</strong> (const <a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_vector2_d.md" class="el">OutlinerVector2D</a>
&amp;point) const</td>
</tr>
<tr class="even memdesc:af895cbe23599dab75ad4df554ffc17b2">
<td class="mdescLeft"> </td>
<td class="mdescRight">Is a given point inside a bounding box (in
2-dimension model)?<br />
</td>
</tr>
<tr class="odd separator:af895cbe23599dab75ad4df554ffc17b2">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:ab39c55ca240be52707607c32b8dd8287">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="ab39c55ca240be52707607c32b8dd8287"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>doesIntersect</strong> (const <a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box2_d.md" class="el">OutlinerBox2D</a>
&amp;boundingBox2) const</td>
</tr>
<tr class="odd memdesc:ab39c55ca240be52707607c32b8dd8287">
<td class="mdescLeft"> </td>
<td class="mdescRight">Does a bounding box intersect another one?<br />
</td>
</tr>
<tr class="even separator:ab39c55ca240be52707607c32b8dd8287">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a3a8e08ddb95dd75a40d0607c64d70d33">
<td class="memItemLeft" style="text-align: right;"
data-valign="top">void </td>
<td class="memItemRight" data-valign="bottom"><a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box2_d.md#a3a8e08ddb95dd75a40d0607c64d70d33"
class="el">intersection</a> (const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box2_d.md"
class="el">OutlinerBox2D</a> &amp;box2, <a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box2_d.md" class="el">OutlinerBox2D</a>
&amp;resultBox) const</td>
</tr>
<tr class="even separator:a3a8e08ddb95dd75a40d0607c64d70d33">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:aacd004948e4007f3f0d30370b63655bf">
<td class="memItemLeft" style="text-align: right;"
data-valign="top">void </td>
<td class="memItemRight" data-valign="bottom"><a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box2_d.md#aacd004948e4007f3f0d30370b63655bf"
class="el">boxUnion</a> (const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box2_d.md"
class="el">OutlinerBox2D</a> &amp;box2, <a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box2_d.md" class="el">OutlinerBox2D</a>
&amp;resultBox) const</td>
</tr>
<tr class="even separator:aacd004948e4007f3f0d30370b63655bf">
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
<tr class="even memitem:ae30c2b3cc74f2dd6d577f3605593ef1f">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="ae30c2b3cc74f2dd6d577f3605593ef1f"></span>
static void </td>
<td class="memItemRight" data-valign="bottom"><strong>test</strong>
(void)</td>
</tr>
<tr class="odd memdesc:ae30c2b3cc74f2dd6d577f3605593ef1f">
<td class="mdescLeft"> </td>
<td class="mdescRight">Run unit tests associated with this class.<br />
</td>
</tr>
<tr class="even separator:ae30c2b3cc74f2dd6d577f3605593ef1f">
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
<tr class="even memitem:abee81ccc4fac6434dfd0b369375ff21c">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="abee81ccc4fac6434dfd0b369375ff21c"></span>
<a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_vector2_d.md"
class="el">OutlinerVector2D</a> </td>
<td class="memItemRight"
data-valign="bottom"><strong>start</strong></td>
</tr>
<tr class="odd memdesc:abee81ccc4fac6434dfd0b369375ff21c">
<td class="mdescLeft"> </td>
<td class="mdescRight">Lower left corner of the bounding box.<br />
</td>
</tr>
<tr class="even separator:abee81ccc4fac6434dfd0b369375ff21c">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a4152c9f6a809fa7f707d09251b0928eb">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a4152c9f6a809fa7f707d09251b0928eb"></span>
<a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_vector2_d.md"
class="el">OutlinerVector2D</a> </td>
<td class="memItemRight" data-valign="bottom"><strong>end</strong></td>
</tr>
<tr class="even memdesc:a4152c9f6a809fa7f707d09251b0928eb">
<td class="mdescLeft"> </td>
<td class="mdescRight">Upper right corner of the bounding box.<br />
</td>
</tr>
<tr class="odd separator:a4152c9f6a809fa7f707d09251b0928eb">
<td colspan="2" class="memSeparator"> </td>
</tr>
</tbody>
</table>

<span id="details"></span>

## Detailed Description

This object represents a "bounding box", a rectangular area in 2D space.

## Constructor & Destructor Documentation

<span id="ac5df1a466dedeb376a8eeb724288eb84"></span>

## <span class="permalink">[◆ ](#ac5df1a466dedeb376a8eeb724288eb84)</span>OutlinerBox2D()

<table class="mlabels">
<colgroup>
<col style="width: 50%" />
<col style="width: 50%" />
</colgroup>
<tbody>
<tr class="odd">
<td class="mlabels-left"><table class="memname">
<tbody>
<tr class="odd">
<td class="memname">OutlinerBox2D::OutlinerBox2D</td>
<td>(</td>
<td class="paramtype">const outlinerreal </td>
<td class="paramname"><em>startX</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const outlinerreal </td>
<td class="paramname"><em>startY</em>,</td>
</tr>
<tr class="odd">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const outlinerreal </td>
<td class="paramname"><em>endX</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const outlinerreal </td>
<td class="paramname"><em>endY</em> </td>
</tr>
<tr class="odd">
<td></td>
<td>)</td>
<td></td>
<td></td>
</tr>
</tbody>
</table></td>
<td class="mlabels-right"><span class="mlabels"><span
class="mlabel">inline</span></span></td>
</tr>
</tbody>
</table>

Make a line based on given two points, both given as x,y coordinates,
four arguments in total.

## Member Function Documentation

<span id="aacd004948e4007f3f0d30370b63655bf"></span>

## <span class="permalink">[◆ ](#aacd004948e4007f3f0d30370b63655bf)</span>boxUnion()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">void OutlinerBox2D::boxUnion</td>
<td>(</td>
<td class="paramtype">const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box2_d.md"
class="el">OutlinerBox2D</a> &amp; </td>
<td class="paramname"><em>box2</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box2_d.md"
class="el">OutlinerBox2D</a> &amp; </td>
<td class="paramname"><em>resultBox</em> </td>
</tr>
<tr class="odd">
<td></td>
<td>)</td>
<td></td>
<td>const</td>
</tr>
</tbody>
</table>

Take a union of this and another box, placing the resulting bounding box
in "resultBox".

<span id="a3a8e08ddb95dd75a40d0607c64d70d33"></span>

## <span class="permalink">[◆ ](#a3a8e08ddb95dd75a40d0607c64d70d33)</span>intersection()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">void OutlinerBox2D::intersection</td>
<td>(</td>
<td class="paramtype">const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box2_d.md"
class="el">OutlinerBox2D</a> &amp; </td>
<td class="paramname"><em>box2</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box2_d.md"
class="el">OutlinerBox2D</a> &amp; </td>
<td class="paramname"><em>resultBox</em> </td>
</tr>
<tr class="odd">
<td></td>
<td>)</td>
<td></td>
<td>const</td>
</tr>
</tbody>
</table>

Take an intersection of this and another box, placing the resulting
bounding box in "resultBox".

------------------------------------------------------------------------

The documentation for this class was generated from the following file:

-   src/<a href="outlinerhighprecision_8hh_source.md"
    class="el">outlinerhighprecision.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [<img src="doxygen.svg" class="footer" width="104" height="31"
alt="doxygen" />](https://www.doxygen.org/index.md) 1.9.4</span>
