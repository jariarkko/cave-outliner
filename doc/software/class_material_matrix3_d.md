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
 members](class_material_matrix3_d-members.md)

MaterialMatrix3D Class Reference

`#include <outlinermaterialmatrix3d.hh>`

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
<tr class="even memitem:a0f757fef3629d8152bf619cc73baf9c1">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a0f757fef3629d8152bf619cc73baf9c1"></span>
 </td>
<td class="memItemRight"
data-valign="bottom"><strong>MaterialMatrix3D</strong> (const <a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box3_d.md" class="el">OutlinerBox3D</a>
&amp;boundingbox, const outlinerreal stepxIn, const outlinerreal
stepyIn, const outlinerreal stepzIn, const unsigned int multiplier)</td>
</tr>
<tr class="odd memdesc:a0f757fef3629d8152bf619cc73baf9c1">
<td class="mdescLeft"> </td>
<td class="mdescRight">Create a <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_material_matrix3_d.md"
class="el">MaterialMatrix3D</a> object.<br />
</td>
</tr>
<tr class="even separator:a0f757fef3629d8152bf619cc73baf9c1">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:ab5681cef0def50f3ac86b41ee5116af4">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="ab5681cef0def50f3ac86b41ee5116af4"></span>
 </td>
<td class="memItemRight"
data-valign="bottom"><strong>~MaterialMatrix3D</strong> ()</td>
</tr>
<tr class="even memdesc:ab5681cef0def50f3ac86b41ee5116af4">
<td class="mdescLeft"> </td>
<td class="mdescRight">Destruct the material matrix.<br />
</td>
</tr>
<tr class="odd separator:ab5681cef0def50f3ac86b41ee5116af4">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:ae2be5cd6acdb01578df7e976d397327a">
<td class="memItemLeft" style="text-align: right;"
data-valign="top">void </td>
<td class="memItemRight" data-valign="bottom"><a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_material_matrix3_d.md#ae2be5cd6acdb01578df7e976d397327a"
class="el">setMaterialMatrix</a> (const unsigned int xIndex, const
unsigned int yIndex, const unsigned int zIndex)</td>
</tr>
<tr class="odd separator:ae2be5cd6acdb01578df7e976d397327a">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:af97dd7214d60de668799f851d3fe1c98">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="af97dd7214d60de668799f851d3fe1c98"></span>
void </td>
<td class="memItemRight"
data-valign="bottom"><strong>setMaterialMatrixSlice</strong> (const
unsigned int xIndex, const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box2_d.md"
class="el">OutlinerBox2D</a> &amp;sliceBoundingBox, <a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_material_matrix2_d.md" class="el">MaterialMatrix2D</a>
*sliceMatrix)</td>
</tr>
<tr class="odd memdesc:af97dd7214d60de668799f851d3fe1c98">
<td class="mdescLeft"> </td>
<td class="mdescRight">Set the entire z slice in one go.<br />
</td>
</tr>
<tr class="even separator:af97dd7214d60de668799f851d3fe1c98">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a915d85a9266858c28e4efb06024dd7f6">
<td class="memItemLeft" style="text-align: right;"
data-valign="top">bool </td>
<td class="memItemRight" data-valign="bottom"><a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_material_matrix3_d.md#a915d85a9266858c28e4efb06024dd7f6"
class="el">getMaterialMatrix</a> (const unsigned int xIndex, const
unsigned int yIndex, const unsigned int zIndex) const</td>
</tr>
<tr class="even separator:a915d85a9266858c28e4efb06024dd7f6">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a0cf559a6ac9362a852bd8f7afb568e08">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a0cf559a6ac9362a852bd8f7afb568e08"></span>
unsigned int </td>
<td class="memItemRight"
data-valign="bottom"><strong>coordinateXToIndex</strong> (outlinerreal
x) const</td>
</tr>
<tr class="even memdesc:a0cf559a6ac9362a852bd8f7afb568e08">
<td class="mdescLeft"> </td>
<td class="mdescRight">Map an actual coordinate value to an index.<br />
</td>
</tr>
<tr class="odd separator:a0cf559a6ac9362a852bd8f7afb568e08">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a22e7e4b348e79cb357b75f57d74996d7">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a22e7e4b348e79cb357b75f57d74996d7"></span>
unsigned int </td>
<td class="memItemRight"
data-valign="bottom"><strong>coordinateYToIndex</strong> (outlinerreal
y) const</td>
</tr>
<tr class="odd memdesc:a22e7e4b348e79cb357b75f57d74996d7">
<td class="mdescLeft"> </td>
<td class="mdescRight">Map an actual coordinate value to an index.<br />
</td>
</tr>
<tr class="even separator:a22e7e4b348e79cb357b75f57d74996d7">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a5b21ad03cde9e764ae6b9fdeb3e5e71e">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a5b21ad03cde9e764ae6b9fdeb3e5e71e"></span>
unsigned int </td>
<td class="memItemRight"
data-valign="bottom"><strong>coordinateZToIndex</strong> (outlinerreal
z) const</td>
</tr>
<tr class="even memdesc:a5b21ad03cde9e764ae6b9fdeb3e5e71e">
<td class="mdescLeft"> </td>
<td class="mdescRight">Map an actual coordinate value to an index.<br />
</td>
</tr>
<tr class="odd separator:a5b21ad03cde9e764ae6b9fdeb3e5e71e">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a3707961fb88dba90217ac80240dc3421">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a3707961fb88dba90217ac80240dc3421"></span>
outlinerreal </td>
<td class="memItemRight"
data-valign="bottom"><strong>indexToCoordinateX</strong> (unsigned int
xIndex) const</td>
</tr>
<tr class="odd memdesc:a3707961fb88dba90217ac80240dc3421">
<td class="mdescLeft"> </td>
<td class="mdescRight">Map a coordinate index to an actual coordinate
value.<br />
</td>
</tr>
<tr class="even separator:a3707961fb88dba90217ac80240dc3421">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a8c12377b72e87b4e4b9de82184804a27">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a8c12377b72e87b4e4b9de82184804a27"></span>
outlinerreal </td>
<td class="memItemRight"
data-valign="bottom"><strong>indexToCoordinateY</strong> (unsigned int
yIndex) const</td>
</tr>
<tr class="even memdesc:a8c12377b72e87b4e4b9de82184804a27">
<td class="mdescLeft"> </td>
<td class="mdescRight">Map a coordinate index to an actual coordinate
value.<br />
</td>
</tr>
<tr class="odd separator:a8c12377b72e87b4e4b9de82184804a27">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a849b5ead18bcae1247b01e6b01db25ca">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a849b5ead18bcae1247b01e6b01db25ca"></span>
outlinerreal </td>
<td class="memItemRight"
data-valign="bottom"><strong>indexToCoordinateZ</strong> (unsigned int
zIndex) const</td>
</tr>
<tr class="odd memdesc:a849b5ead18bcae1247b01e6b01db25ca">
<td class="mdescLeft"> </td>
<td class="mdescRight">Map a coordinate index to an actual coordinate
value.<br />
</td>
</tr>
<tr class="even separator:a849b5ead18bcae1247b01e6b01db25ca">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a5c0d878337b6646f43385dd14427cb83">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a5c0d878337b6646f43385dd14427cb83"></span>
unsigned int </td>
<td class="memItemRight" data-valign="bottom"><strong>count</strong>
(void) const</td>
</tr>
<tr class="even memdesc:a5c0d878337b6646f43385dd14427cb83">
<td class="mdescLeft"> </td>
<td class="mdescRight">Count the number of matrix elements with flags
on.<br />
</td>
</tr>
<tr class="odd separator:a5c0d878337b6646f43385dd14427cb83">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a61105a682973c9fee5f14ffa3642ba20">
<td class="memItemLeft" style="text-align: right;"
data-valign="top">outlinerreal </td>
<td class="memItemRight" data-valign="bottom"><a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_material_matrix3_d.md#a61105a682973c9fee5f14ffa3642ba20"
class="el">filledPercentage</a> (unsigned int &amp;memory, unsigned int
&amp;theoretical) const</td>
</tr>
<tr class="odd separator:a61105a682973c9fee5f14ffa3642ba20">
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
<tr class="even memitem:adc4d463604e38c058ad47c5e29986598">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="adc4d463604e38c058ad47c5e29986598"></span>
static void </td>
<td class="memItemRight" data-valign="bottom"><strong>test</strong>
(void)</td>
</tr>
<tr class="odd memdesc:adc4d463604e38c058ad47c5e29986598">
<td class="mdescLeft"> </td>
<td class="mdescRight">Run unit tests for this module.<br />
</td>
</tr>
<tr class="even separator:adc4d463604e38c058ad47c5e29986598">
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
<tr class="even memitem:a049def148a181d19e018529380de4414">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a049def148a181d19e018529380de4414"></span>
const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box3_d.md"
class="el">OutlinerBox3D</a> </td>
<td class="memItemRight"
data-valign="bottom"><strong>boundingBox</strong></td>
</tr>
<tr class="odd separator:a049def148a181d19e018529380de4414">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:af5d4be48adcc6b6031a33c33f677b5fc">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="af5d4be48adcc6b6031a33c33f677b5fc"></span>
const unsigned int </td>
<td class="memItemRight"
data-valign="bottom"><strong>xIndexSize</strong></td>
</tr>
<tr class="odd memdesc:af5d4be48adcc6b6031a33c33f677b5fc">
<td class="mdescLeft"> </td>
<td class="mdescRight">Matrix size in x-coordinate direction.<br />
</td>
</tr>
<tr class="even separator:af5d4be48adcc6b6031a33c33f677b5fc">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:aebcd4baba5c2c9eb378e74a1a8797828">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="aebcd4baba5c2c9eb378e74a1a8797828"></span>
const unsigned int </td>
<td class="memItemRight"
data-valign="bottom"><strong>yIndexSize</strong></td>
</tr>
<tr class="even memdesc:aebcd4baba5c2c9eb378e74a1a8797828">
<td class="mdescLeft"> </td>
<td class="mdescRight">Matrix size in y-coordinate direction.<br />
</td>
</tr>
<tr class="odd separator:aebcd4baba5c2c9eb378e74a1a8797828">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a7f1aced6d50a7b1867632a23c6b47b5b">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a7f1aced6d50a7b1867632a23c6b47b5b"></span>
const unsigned int </td>
<td class="memItemRight"
data-valign="bottom"><strong>zIndexSize</strong></td>
</tr>
<tr class="odd memdesc:a7f1aced6d50a7b1867632a23c6b47b5b">
<td class="mdescLeft"> </td>
<td class="mdescRight">Matrix size in z-coordinate direction.<br />
</td>
</tr>
<tr class="even separator:a7f1aced6d50a7b1867632a23c6b47b5b">
<td colspan="2" class="memSeparator"> </td>
</tr>
</tbody>
</table>

<span id="details"></span>

## Detailed Description

This object represents a quantized 3D view into a 3D cave model. Given a
desired resolution (N x M x L pixels), it provides a boolean matrix.
Each element (x,y,z) in the matrix is set to 1 if there's a a face
within the model in that pixel location. The resolution is configurable,
so for instance for a 1 x 1 resolution all faces within the 3D model
would be at the resulting single pixel.

Note that the material matrix resolution is not the same as tile
resolution in the indexed mesh object. Typically, the indexed mesh has
low resolution, just sufficient for fast searches, while the material
matrix needs to support the desired high resolution of the output.

## Member Function Documentation

<span id="a61105a682973c9fee5f14ffa3642ba20"></span>

## <span class="permalink">[◆ ](#a61105a682973c9fee5f14ffa3642ba20)</span>filledPercentage()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">outlinerreal MaterialMatrix3D::filledPercentage</td>
<td>(</td>
<td class="paramtype">unsigned int &amp; </td>
<td class="paramname"><em>memory</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">unsigned int &amp; </td>
<td class="paramname"><em>theoretical</em> </td>
</tr>
<tr class="odd">
<td></td>
<td>)</td>
<td></td>
<td>const</td>
</tr>
</tbody>
</table>

Percentage of the 3D matrix actually constructed (the rest consumes no
memory).

<span id="a915d85a9266858c28e4efb06024dd7f6"></span>

## <span class="permalink">[◆ ](#a915d85a9266858c28e4efb06024dd7f6)</span>getMaterialMatrix()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">bool MaterialMatrix3D::getMaterialMatrix</td>
<td>(</td>
<td class="paramtype">const unsigned int </td>
<td class="paramname"><em>xIndex</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const unsigned int </td>
<td class="paramname"><em>yIndex</em>,</td>
</tr>
<tr class="odd">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const unsigned int </td>
<td class="paramname"><em>zIndex</em> </td>
</tr>
<tr class="even">
<td></td>
<td>)</td>
<td></td>
<td>const</td>
</tr>
</tbody>
</table>

Get the flag indicating whether there is material in a given (x,y) index
in the matrix.

<span id="ae2be5cd6acdb01578df7e976d397327a"></span>

## <span class="permalink">[◆ ](#ae2be5cd6acdb01578df7e976d397327a)</span>setMaterialMatrix()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">void MaterialMatrix3D::setMaterialMatrix</td>
<td>(</td>
<td class="paramtype">const unsigned int </td>
<td class="paramname"><em>xIndex</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const unsigned int </td>
<td class="paramname"><em>yIndex</em>,</td>
</tr>
<tr class="odd">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const unsigned int </td>
<td class="paramname"><em>zIndex</em> </td>
</tr>
<tr class="even">
<td></td>
<td>)</td>
<td></td>
<td></td>
</tr>
</tbody>
</table>

Set the flag indicating that there is material in a given (x,y) index in
the matrix.

------------------------------------------------------------------------

The documentation for this class was generated from the following file:

-   src/<a href="outlinermaterialmatrix3d_8hh_source.md"
    class="el">outlinermaterialmatrix3d.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [<img src="doxygen.svg" class="footer" width="104" height="31"
alt="doxygen" />](https://www.doxygen.org/index.md) 1.9.4</span>
