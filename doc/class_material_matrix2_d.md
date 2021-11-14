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
 members](class_material_matrix2_d-members.md)

MaterialMatrix2D Class Reference

`#include <outlinermaterialmatrix2d.hh>`

<table class="memberdecls">
<colgroup>
<col style="width: 50%" />
<col style="width: 50%" />
</colgroup>
<tbody>
<tr class="odd heading">
<td colspan="2"><h2 id="public-member-functions" class="groupheader"><span id="pub-methods"></span> Public Member Functions</h2></td>
</tr>
<tr class="even memitem:a519e2b17c0d35c0d1e39bb476801e7a8">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a519e2b17c0d35c0d1e39bb476801e7a8"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>MaterialMatrix2D</strong> (const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_box2_d.md" class="el">OutlinerBox2D</a> &amp;boundingBoxIn, const outlinerreal stepxIn, const outlinerreal stepyIn)</td>
</tr>
<tr class="odd memdesc:a519e2b17c0d35c0d1e39bb476801e7a8">
<td class="mdescLeft"> </td>
<td class="mdescRight">Create a <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_material_matrix2_d.md" class="el">MaterialMatrix2D</a> object.<br />
</td>
</tr>
<tr class="even separator:a519e2b17c0d35c0d1e39bb476801e7a8">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a73554d827f29c3cc544c4f9bf63379d8">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a73554d827f29c3cc544c4f9bf63379d8"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>~MaterialMatrix2D</strong> ()</td>
</tr>
<tr class="even memdesc:a73554d827f29c3cc544c4f9bf63379d8">
<td class="mdescLeft"> </td>
<td class="mdescRight">Destruct the material matrix.<br />
</td>
</tr>
<tr class="odd separator:a73554d827f29c3cc544c4f9bf63379d8">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a13d1437e461a5f98b18e30697b397a44">
<td style="text-align: right;" class="memItemLeft" data-valign="top">void </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_material_matrix2_d.md#a13d1437e461a5f98b18e30697b397a44" class="el">setMaterialMatrix</a> (unsigned int xIndex, unsigned int yIndex)</td>
</tr>
<tr class="odd separator:a13d1437e461a5f98b18e30697b397a44">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:aea7f989dafca2ce42109d765a849a993">
<td style="text-align: right;" class="memItemLeft" data-valign="top">bool </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_material_matrix2_d.md#aea7f989dafca2ce42109d765a849a993" class="el">getMaterialMatrix</a> (unsigned int xIndex, unsigned int yIndex)</td>
</tr>
<tr class="odd separator:aea7f989dafca2ce42109d765a849a993">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a0f3cb172795c299d5a278ff664c2a867">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a0f3cb172795c299d5a278ff664c2a867"></span> bool </td>
<td class="memItemRight" data-valign="bottom"><strong>getMaterialYBounds</strong> (unsigned int xIndex, unsigned int &amp;yIndexFrom, unsigned int &amp;yIndexTo)</td>
</tr>
<tr class="odd memdesc:a0f3cb172795c299d5a278ff664c2a867">
<td class="mdescLeft"> </td>
<td class="mdescRight">Find bounds of material at the y index range, on a given x index.<br />
</td>
</tr>
<tr class="even separator:a0f3cb172795c299d5a278ff664c2a867">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a9b3d867c2b589b8de90efae580e84370">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a9b3d867c2b589b8de90efae580e84370"></span> unsigned int </td>
<td class="memItemRight" data-valign="bottom"><strong>coordinateXToIndex</strong> (outlinerreal x) const</td>
</tr>
<tr class="even memdesc:a9b3d867c2b589b8de90efae580e84370">
<td class="mdescLeft"> </td>
<td class="mdescRight">Map an actual coordinate value to an index.<br />
</td>
</tr>
<tr class="odd separator:a9b3d867c2b589b8de90efae580e84370">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a478449823377ac39577ffc4dcc6881ca">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a478449823377ac39577ffc4dcc6881ca"></span> unsigned int </td>
<td class="memItemRight" data-valign="bottom"><strong>coordinateYToIndex</strong> (outlinerreal y) const</td>
</tr>
<tr class="odd memdesc:a478449823377ac39577ffc4dcc6881ca">
<td class="mdescLeft"> </td>
<td class="mdescRight">Map an actual coordinate value to an index.<br />
</td>
</tr>
<tr class="even separator:a478449823377ac39577ffc4dcc6881ca">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a2b78a7efdeda723a4ad8974c36a44191">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a2b78a7efdeda723a4ad8974c36a44191"></span> outlinerreal </td>
<td class="memItemRight" data-valign="bottom"><strong>indexToCoordinateX</strong> (unsigned int xIndex) const</td>
</tr>
<tr class="even memdesc:a2b78a7efdeda723a4ad8974c36a44191">
<td class="mdescLeft"> </td>
<td class="mdescRight">Map a coordinate index to an actual coordinate value.<br />
</td>
</tr>
<tr class="odd separator:a2b78a7efdeda723a4ad8974c36a44191">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:ae32128e44354e04adb23c285e625bf89">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="ae32128e44354e04adb23c285e625bf89"></span> outlinerreal </td>
<td class="memItemRight" data-valign="bottom"><strong>indexToCoordinateY</strong> (unsigned int yIndex) const</td>
</tr>
<tr class="odd memdesc:ae32128e44354e04adb23c285e625bf89">
<td class="mdescLeft"> </td>
<td class="mdescRight">Map a coordinate index to an actual coordinate value.<br />
</td>
</tr>
<tr class="even separator:ae32128e44354e04adb23c285e625bf89">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a4f20db3ddcef2334fc264a942f7a081f">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a4f20db3ddcef2334fc264a942f7a081f"></span> unsigned int </td>
<td class="memItemRight" data-valign="bottom"><strong>count</strong> (void)</td>
</tr>
<tr class="even memdesc:a4f20db3ddcef2334fc264a942f7a081f">
<td class="mdescLeft"> </td>
<td class="mdescRight">Count the number of matrix elements with flags on.<br />
</td>
</tr>
<tr class="odd separator:a4f20db3ddcef2334fc264a942f7a081f">
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
<tr class="even memitem:ab4718a91ae6995a7fd8de21b792ea457">
<td style="text-align: right;" class="memItemLeft" data-valign="top">static unsigned int </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_material_matrix2_d.md#ab4718a91ae6995a7fd8de21b792ea457" class="el">calculateSize</a> (outlinerreal from, outlinerreal to, outlinerreal step)</td>
</tr>
<tr class="odd separator:ab4718a91ae6995a7fd8de21b792ea457">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a969b402f51ab121dbaafa9033046e676">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a969b402f51ab121dbaafa9033046e676"></span> static void </td>
<td class="memItemRight" data-valign="bottom"><strong>test</strong> (void)</td>
</tr>
<tr class="odd memdesc:a969b402f51ab121dbaafa9033046e676">
<td class="mdescLeft"> </td>
<td class="mdescRight">Run unit tests for this module.<br />
</td>
</tr>
<tr class="even separator:a969b402f51ab121dbaafa9033046e676">
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
<tr class="even memitem:aa145ee6a1bb39c8c7ab5f90d63be0628">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="aa145ee6a1bb39c8c7ab5f90d63be0628"></span> const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_box2_d.md" class="el">OutlinerBox2D</a> </td>
<td class="memItemRight" data-valign="bottom"><strong>boundingBox</strong></td>
</tr>
<tr class="odd separator:aa145ee6a1bb39c8c7ab5f90d63be0628">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a212e7f6d2f4d72ae6eca9723efb4d692">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a212e7f6d2f4d72ae6eca9723efb4d692"></span> const unsigned int </td>
<td class="memItemRight" data-valign="bottom"><strong>xIndexSize</strong></td>
</tr>
<tr class="odd memdesc:a212e7f6d2f4d72ae6eca9723efb4d692">
<td class="mdescLeft"> </td>
<td class="mdescRight">Matrix size in x-coordinate direction.<br />
</td>
</tr>
<tr class="even separator:a212e7f6d2f4d72ae6eca9723efb4d692">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a3d57887a2bc37c6314da40f197024acf">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a3d57887a2bc37c6314da40f197024acf"></span> const unsigned int </td>
<td class="memItemRight" data-valign="bottom"><strong>yIndexSize</strong></td>
</tr>
<tr class="even memdesc:a3d57887a2bc37c6314da40f197024acf">
<td class="mdescLeft"> </td>
<td class="mdescRight">Matrix size in y-coordinate direction.<br />
</td>
</tr>
<tr class="odd separator:a3d57887a2bc37c6314da40f197024acf">
<td colspan="2" class="memSeparator"> </td>
</tr>
</tbody>
</table>

<span id="details"></span>

## Detailed Description

This object represents a quantized 2D view into a 3D cave model. Given a
desired resolution (N x M pixels), it provides a boolean matrix. Each
element (x,y) in the matrix is set to 1 if there's a a face within the
model in that pixel location. The resolution is configurable, so for
instance for a 1 x 1 resolution all faces within the 3D model would be
at the resulting single pixel.

Note that the material matrix resolution is not the same as tile
resolution in the indexed mesh object. Typically, the indexed mesh has
low resolution, just sufficient for fast searches, while the material
matrix needs to support the desired high resolution of the output.

## Member Function Documentation

<span id="ab4718a91ae6995a7fd8de21b792ea457"></span>

## <span class="permalink">[◆ ](#ab4718a91ae6995a7fd8de21b792ea457)</span>calculateSize()

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
<td class="memname">static unsigned int MaterialMatrix2D::calculateSize</td>
<td>(</td>
<td class="paramtype">outlinerreal </td>
<td class="paramname"><em>from</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">outlinerreal </td>
<td class="paramname"><em>to</em>,</td>
</tr>
<tr class="odd">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">outlinerreal </td>
<td class="paramname"><em>step</em> </td>
</tr>
<tr class="even">
<td></td>
<td>)</td>
<td></td>
<td></td>
</tr>
</tbody>
</table></td>
<td class="mlabels-right"><span class="mlabels"><span class="mlabel">static</span></span></td>
</tr>
</tbody>
</table>

Calculate size needed to represent from...to with increments of size
step.

<span id="aea7f989dafca2ce42109d765a849a993"></span>

## <span class="permalink">[◆ ](#aea7f989dafca2ce42109d765a849a993)</span>getMaterialMatrix()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">bool MaterialMatrix2D::getMaterialMatrix</td>
<td>(</td>
<td class="paramtype">unsigned int </td>
<td class="paramname"><em>xIndex</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">unsigned int </td>
<td class="paramname"><em>yIndex</em> </td>
</tr>
<tr class="odd">
<td></td>
<td>)</td>
<td></td>
<td></td>
</tr>
</tbody>
</table>

Get the flag indicating whether there is material in a given (x,y) index
in the matrix.

<span id="a13d1437e461a5f98b18e30697b397a44"></span>

## <span class="permalink">[◆ ](#a13d1437e461a5f98b18e30697b397a44)</span>setMaterialMatrix()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">void MaterialMatrix2D::setMaterialMatrix</td>
<td>(</td>
<td class="paramtype">unsigned int </td>
<td class="paramname"><em>xIndex</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">unsigned int </td>
<td class="paramname"><em>yIndex</em> </td>
</tr>
<tr class="odd">
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

-   src/<a href="outlinermaterialmatrix2d_8hh_source.md" class="el">outlinermaterialmatrix2d.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [Doxygen](https://www.doxygen.org/index.md)
1.9.2</span>
