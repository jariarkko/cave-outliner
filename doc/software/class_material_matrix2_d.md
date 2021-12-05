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
<td class="memItemRight" data-valign="bottom"><strong>MaterialMatrix2D</strong> (const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box2_d.md" class="el">OutlinerBox2D</a> &amp;boundingBoxIn, const outlinerreal stepxIn, const outlinerreal stepyIn)</td>
</tr>
<tr class="odd memdesc:a519e2b17c0d35c0d1e39bb476801e7a8">
<td class="mdescLeft"> </td>
<td class="mdescRight">Create a <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_material_matrix2_d.md" class="el">MaterialMatrix2D</a> object.<br />
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
<tr class="even memitem:ad749f88cd38c6ee026159c4c25799f6e">
<td style="text-align: right;" class="memItemLeft" data-valign="top">void </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_material_matrix2_d.md#ad749f88cd38c6ee026159c4c25799f6e" class="el">setMaterialMatrix</a> (const unsigned int xIndex, const unsigned int yIndex)</td>
</tr>
<tr class="odd separator:ad749f88cd38c6ee026159c4c25799f6e">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a43b516718ef931b3c9c1b64eedda3585">
<td style="text-align: right;" class="memItemLeft" data-valign="top">void </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_material_matrix2_d.md#a43b516718ef931b3c9c1b64eedda3585" class="el">unsetMaterialMatrix</a> (const unsigned int xIndex, const unsigned int yIndex)</td>
</tr>
<tr class="odd separator:a43b516718ef931b3c9c1b64eedda3585">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:abcadd044a0a3a67371f28e96990c2521">
<td style="text-align: right;" class="memItemLeft" data-valign="top">bool </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_material_matrix2_d.md#abcadd044a0a3a67371f28e96990c2521" class="el">getMaterialMatrix</a> (const unsigned int xIndex, const unsigned int yIndex) const</td>
</tr>
<tr class="odd separator:abcadd044a0a3a67371f28e96990c2521">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:ae2897299ab70201d14a87f01c806d476">
<td style="text-align: right;" class="memItemLeft" data-valign="top">bool </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_material_matrix2_d.md#ae2897299ab70201d14a87f01c806d476" class="el">getMaterialMatrix</a> (const unsigned int xIndexStart, const unsigned int yIndexStart, const unsigned int xIndexEnd, const unsigned int yIndexEnd) const</td>
</tr>
<tr class="odd separator:ae2897299ab70201d14a87f01c806d476">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a95b372e241e38a1db45a4ac0caa89e23">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a95b372e241e38a1db45a4ac0caa89e23"></span> bool </td>
<td class="memItemRight" data-valign="bottom"><strong>getMaterialYBounds</strong> (const unsigned int xIndex, unsigned int &amp;yIndexFrom, unsigned int &amp;yIndexTo) const</td>
</tr>
<tr class="odd memdesc:a95b372e241e38a1db45a4ac0caa89e23">
<td class="mdescLeft"> </td>
<td class="mdescRight">Find bounds of material at the y index range, on a given x index.<br />
</td>
</tr>
<tr class="even separator:a95b372e241e38a1db45a4ac0caa89e23">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:ac7dbeee39b2435ad1bdd1ee377b3f2ac">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="ac7dbeee39b2435ad1bdd1ee377b3f2ac"></span> unsigned int </td>
<td class="memItemRight" data-valign="bottom"><strong>coordinateXToIndex</strong> (const outlinerreal x) const</td>
</tr>
<tr class="even memdesc:ac7dbeee39b2435ad1bdd1ee377b3f2ac">
<td class="mdescLeft"> </td>
<td class="mdescRight">Map an actual coordinate value to an index.<br />
</td>
</tr>
<tr class="odd separator:ac7dbeee39b2435ad1bdd1ee377b3f2ac">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a1d2ab0a414b6f03d0a4ce3b201e3e1dc">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a1d2ab0a414b6f03d0a4ce3b201e3e1dc"></span> unsigned int </td>
<td class="memItemRight" data-valign="bottom"><strong>coordinateYToIndex</strong> (const outlinerreal y) const</td>
</tr>
<tr class="odd memdesc:a1d2ab0a414b6f03d0a4ce3b201e3e1dc">
<td class="mdescLeft"> </td>
<td class="mdescRight">Map an actual coordinate value to an index.<br />
</td>
</tr>
<tr class="even separator:a1d2ab0a414b6f03d0a4ce3b201e3e1dc">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a494b7fa27f1f63eb973ddb18c75710e3">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a494b7fa27f1f63eb973ddb18c75710e3"></span> outlinerreal </td>
<td class="memItemRight" data-valign="bottom"><strong>indexToCoordinateX</strong> (const unsigned int xIndex) const</td>
</tr>
<tr class="even memdesc:a494b7fa27f1f63eb973ddb18c75710e3">
<td class="mdescLeft"> </td>
<td class="mdescRight">Map a coordinate index to an actual coordinate value.<br />
</td>
</tr>
<tr class="odd separator:a494b7fa27f1f63eb973ddb18c75710e3">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a6c152886101975ca8152adc80834a1c2">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a6c152886101975ca8152adc80834a1c2"></span> outlinerreal </td>
<td class="memItemRight" data-valign="bottom"><strong>indexToCoordinateY</strong> (const unsigned int yIndex) const</td>
</tr>
<tr class="odd memdesc:a6c152886101975ca8152adc80834a1c2">
<td class="mdescLeft"> </td>
<td class="mdescRight">Map a coordinate index to an actual coordinate value.<br />
</td>
</tr>
<tr class="even separator:a6c152886101975ca8152adc80834a1c2">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:ac5af09bb1963a5b7c792f601d204916c">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="ac5af09bb1963a5b7c792f601d204916c"></span> unsigned int </td>
<td class="memItemRight" data-valign="bottom"><strong>count</strong> (void) const</td>
</tr>
<tr class="even memdesc:ac5af09bb1963a5b7c792f601d204916c">
<td class="mdescLeft"> </td>
<td class="mdescRight">Count the number of matrix elements with flags on.<br />
</td>
</tr>
<tr class="odd separator:ac5af09bb1963a5b7c792f601d204916c">
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
<tr class="even memitem:a6ae7b380f868c28b048e9f43daacaf1a">
<td style="text-align: right;" class="memItemLeft" data-valign="top">static unsigned int </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_material_matrix2_d.md#a6ae7b380f868c28b048e9f43daacaf1a" class="el">calculateSize</a> (const outlinerreal from, const outlinerreal to, const outlinerreal step)</td>
</tr>
<tr class="odd separator:a6ae7b380f868c28b048e9f43daacaf1a">
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
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="aa145ee6a1bb39c8c7ab5f90d63be0628"></span> const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box2_d.md" class="el">OutlinerBox2D</a> </td>
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

<span id="a6ae7b380f868c28b048e9f43daacaf1a"></span>

## <span class="permalink">[◆ ](#a6ae7b380f868c28b048e9f43daacaf1a)</span>calculateSize()

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
<td class="paramtype">const outlinerreal </td>
<td class="paramname"><em>from</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const outlinerreal </td>
<td class="paramname"><em>to</em>,</td>
</tr>
<tr class="odd">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const outlinerreal </td>
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

<span id="abcadd044a0a3a67371f28e96990c2521"></span>

## <span class="permalink">[◆ ](#abcadd044a0a3a67371f28e96990c2521)</span>getMaterialMatrix() <span class="overload">\[1/2\]</span>

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">bool MaterialMatrix2D::getMaterialMatrix</td>
<td>(</td>
<td class="paramtype">const unsigned int </td>
<td class="paramname"><em>xIndex</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const unsigned int </td>
<td class="paramname"><em>yIndex</em> </td>
</tr>
<tr class="odd">
<td></td>
<td>)</td>
<td></td>
<td>const</td>
</tr>
</tbody>
</table>

Get the flag indicating whether there is material in a given (x,y) index
in the matrix.

<span id="ae2897299ab70201d14a87f01c806d476"></span>

## <span class="permalink">[◆ ](#ae2897299ab70201d14a87f01c806d476)</span>getMaterialMatrix() <span class="overload">\[2/2\]</span>

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">bool MaterialMatrix2D::getMaterialMatrix</td>
<td>(</td>
<td class="paramtype">const unsigned int </td>
<td class="paramname"><em>xIndexStart</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const unsigned int </td>
<td class="paramname"><em>yIndexStart</em>,</td>
</tr>
<tr class="odd">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const unsigned int </td>
<td class="paramname"><em>xIndexEnd</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const unsigned int </td>
<td class="paramname"><em>yIndexEnd</em> </td>
</tr>
<tr class="odd">
<td></td>
<td>)</td>
<td></td>
<td>const</td>
</tr>
</tbody>
</table>

Get the flag indicating whether there is material in a given
(xStart,yStart)..(xEnd,yEnd) rectangle in the matrix.

<span id="ad749f88cd38c6ee026159c4c25799f6e"></span>

## <span class="permalink">[◆ ](#ad749f88cd38c6ee026159c4c25799f6e)</span>setMaterialMatrix()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">void MaterialMatrix2D::setMaterialMatrix</td>
<td>(</td>
<td class="paramtype">const unsigned int </td>
<td class="paramname"><em>xIndex</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const unsigned int </td>
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

<span id="a43b516718ef931b3c9c1b64eedda3585"></span>

## <span class="permalink">[◆ ](#a43b516718ef931b3c9c1b64eedda3585)</span>unsetMaterialMatrix()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">void MaterialMatrix2D::unsetMaterialMatrix</td>
<td>(</td>
<td class="paramtype">const unsigned int </td>
<td class="paramname"><em>xIndex</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const unsigned int </td>
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

Unset (reset) the flag indicating that there is material in a given
(x,y) index in the matrix.

------------------------------------------------------------------------

The documentation for this class was generated from the following file:

-   src/<a href="outlinermaterialmatrix2d_8hh_source.md" class="el">outlinermaterialmatrix2d.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [Doxygen](https://www.doxygen.org/index.md)
1.9.2</span>
