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

[Public Member Functions](#pub-methods) | [Static Public Member
Functions](#pub-static-methods) | [Public Attributes](#pub-attribs) |
 members](class_outliner_box3_d-members.md)

OutlinerBox3D Class Reference

`#include <outlinerhighprecision.hh>`

<table class="memberdecls">
<colgroup>
<col style="width: 50%" />
<col style="width: 50%" />
</colgroup>
<tbody>
<tr class="odd heading">
<td colspan="2"><h2 id="public-member-functions" class="groupheader"><span id="pub-methods"></span> Public Member Functions</h2></td>
</tr>
<tr class="even memitem:a12eecf060552c563a0bc03b81914c380">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a12eecf060552c563a0bc03b81914c380"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>OutlinerBox3D</strong> ()</td>
</tr>
<tr class="odd memdesc:a12eecf060552c563a0bc03b81914c380">
<td class="mdescLeft"> </td>
<td class="mdescRight">Create an empty object, all values initialized to zero.<br />
</td>
</tr>
<tr class="even separator:a12eecf060552c563a0bc03b81914c380">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:ab3e293681156968ffffda3e136195642">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="ab3e293681156968ffffda3e136195642"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>OutlinerBox3D</strong> (const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_box3_d.md" class="el">OutlinerBox3D</a> &amp;in)</td>
</tr>
<tr class="even memdesc:ab3e293681156968ffffda3e136195642">
<td class="mdescLeft"> </td>
<td class="mdescRight">Make an exact copy of another similar object.<br />
</td>
</tr>
<tr class="odd separator:ab3e293681156968ffffda3e136195642">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a08a3d66d927236353b74be92f04242e5">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a08a3d66d927236353b74be92f04242e5"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>OutlinerBox3D</strong> (const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_vector3_d.md" class="el">OutlinerVector3D</a> &amp;startIn, const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_vector3_d.md" class="el">OutlinerVector3D</a> &amp;endIn)</td>
</tr>
<tr class="odd memdesc:a08a3d66d927236353b74be92f04242e5">
<td class="mdescLeft"> </td>
<td class="mdescRight">Make a bounding box based on given two points.<br />
</td>
</tr>
<tr class="even separator:a08a3d66d927236353b74be92f04242e5">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a9cfee8597e02440661b126850e7844ef">
<td style="text-align: right;" class="memItemLeft" data-valign="top"> </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_box3_d.md#a9cfee8597e02440661b126850e7844ef" class="el">OutlinerBox3D</a> (const outlinerreal startX, const outlinerreal startY, const outlinerreal startZ, const outlinerreal endX, const outlinerreal endY, const outlinerreal endZ)</td>
</tr>
<tr class="even separator:a9cfee8597e02440661b126850e7844ef">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:ae2e2616f80b372443ed60d919de82088">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="ae2e2616f80b372443ed60d919de82088"></span> <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_box3_d.md" class="el">OutlinerBox3D</a> &amp; </td>
<td class="memItemRight" data-valign="bottom"><strong>operator=</strong> (const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_box3_d.md" class="el">OutlinerBox3D</a> &amp;in)</td>
</tr>
<tr class="even memdesc:ae2e2616f80b372443ed60d919de82088">
<td class="mdescLeft"> </td>
<td class="mdescRight">Assignment.<br />
</td>
</tr>
<tr class="odd separator:ae2e2616f80b372443ed60d919de82088">
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
<td colspan="2"><h2 id="static-public-member-functions" class="groupheader"><span id="pub-static-methods"></span> Static Public Member Functions</h2></td>
</tr>
<tr class="even memitem:a0279055d0cb166b0a495b25ace5084a3">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a0279055d0cb166b0a495b25ace5084a3"></span> static void </td>
<td class="memItemRight" data-valign="bottom"><strong>test</strong> (void)</td>
</tr>
<tr class="odd memdesc:a0279055d0cb166b0a495b25ace5084a3">
<td class="mdescLeft"> </td>
<td class="mdescRight">Run unit tests associated with this class.<br />
</td>
</tr>
<tr class="even separator:a0279055d0cb166b0a495b25ace5084a3">
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
<tr class="even memitem:a14c9f2a61a054a8a875268025a59ecc2">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a14c9f2a61a054a8a875268025a59ecc2"></span> <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_vector3_d.md" class="el">OutlinerVector3D</a> </td>
<td class="memItemRight" data-valign="bottom"><strong>start</strong></td>
</tr>
<tr class="odd memdesc:a14c9f2a61a054a8a875268025a59ecc2">
<td class="mdescLeft"> </td>
<td class="mdescRight">Lower left corner of the bounding box/cube.<br />
</td>
</tr>
<tr class="even separator:a14c9f2a61a054a8a875268025a59ecc2">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:adb9b4fcadca176fb9963e8e3ff7ae695">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="adb9b4fcadca176fb9963e8e3ff7ae695"></span> <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_vector3_d.md" class="el">OutlinerVector3D</a> </td>
<td class="memItemRight" data-valign="bottom"><strong>end</strong></td>
</tr>
<tr class="even memdesc:adb9b4fcadca176fb9963e8e3ff7ae695">
<td class="mdescLeft"> </td>
<td class="mdescRight">Upper right corner of the bounding box/cube.<br />
</td>
</tr>
<tr class="odd separator:adb9b4fcadca176fb9963e8e3ff7ae695">
<td colspan="2" class="memSeparator"> </td>
</tr>
</tbody>
</table>

<span id="details"></span>

## Detailed Description

This object represents a "bounding box", a cubic/rectangular area in 3D
space.

## Constructor & Destructor Documentation

<span id="a9cfee8597e02440661b126850e7844ef"></span>

## <span class="permalink">[◆ ](#a9cfee8597e02440661b126850e7844ef)</span>OutlinerBox3D()

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
<td class="memname">OutlinerBox3D::OutlinerBox3D</td>
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
<td class="paramname"><em>startZ</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const outlinerreal </td>
<td class="paramname"><em>endX</em>,</td>
</tr>
<tr class="odd">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const outlinerreal </td>
<td class="paramname"><em>endY</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const outlinerreal </td>
<td class="paramname"><em>endZ</em> </td>
</tr>
<tr class="odd">
<td></td>
<td>)</td>
<td></td>
<td></td>
</tr>
</tbody>
</table></td>
<td class="mlabels-right"><span class="mlabels"><span class="mlabel">inline</span></span></td>
</tr>
</tbody>
</table>

Make a line based on given two points, both given as x,y,z coordinates,
six arguments in total.

------------------------------------------------------------------------

The documentation for this class was generated from the following file:

-   src/<a href="outlinerhighprecision_8hh_source.md" class="el">outlinerhighprecision.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [Doxygen](https://www.doxygen.org/index.md)
1.9.2</span>
