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
 members](class_material_matrix-members.md)

MaterialMatrix Class Reference

`#include <outlinermaterialmatrix.hh>`

<table class="memberdecls">
<colgroup>
<col style="width: 50%" />
<col style="width: 50%" />
</colgroup>
<tbody>
<tr class="odd heading">
<td colspan="2"><h2 id="public-member-functions" class="groupheader"><span id="pub-methods"></span> Public Member Functions</h2></td>
</tr>
<tr class="even memitem:a41ea53f42d59a8bfb5f84f90d76a5e47">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a41ea53f42d59a8bfb5f84f90d76a5e47"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>MaterialMatrix</strong> (<a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_box2_d.md" class="el">OutlinerBox2D</a> boundingbox, outlinerreal stepx, outlinerreal stepy)</td>
</tr>
<tr class="odd memdesc:a41ea53f42d59a8bfb5f84f90d76a5e47">
<td class="mdescLeft"> </td>
<td class="mdescRight">Create a <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_material_matrix.md" class="el">MaterialMatrix</a> object.<br />
</td>
</tr>
<tr class="even separator:a41ea53f42d59a8bfb5f84f90d76a5e47">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a92eed74c68437de593c85297e538b73e">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a92eed74c68437de593c85297e538b73e"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>~MaterialMatrix</strong> ()</td>
</tr>
<tr class="even memdesc:a92eed74c68437de593c85297e538b73e">
<td class="mdescLeft"> </td>
<td class="mdescRight">Destruct the material matrix.<br />
</td>
</tr>
<tr class="odd separator:a92eed74c68437de593c85297e538b73e">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:ae66f04264fa9cd60b5cfd43991e0308f">
<td style="text-align: right;" class="memItemLeft" data-valign="top">void </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_material_matrix.md#ae66f04264fa9cd60b5cfd43991e0308f" class="el">setMaterialMatrix</a> (unsigned int xIndex, unsigned int yIndex)</td>
</tr>
<tr class="odd separator:ae66f04264fa9cd60b5cfd43991e0308f">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a2b38c1478297faff7ba33f8939e23041">
<td style="text-align: right;" class="memItemLeft" data-valign="top">bool </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_material_matrix.md#a2b38c1478297faff7ba33f8939e23041" class="el">getMaterialMatrix</a> (unsigned int xIndex, unsigned int yIndex)</td>
</tr>
<tr class="odd separator:a2b38c1478297faff7ba33f8939e23041">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:aa5ff9236765b0441609caef6a7dde927">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="aa5ff9236765b0441609caef6a7dde927"></span> unsigned int </td>
<td class="memItemRight" data-valign="bottom"><strong>count</strong> (void)</td>
</tr>
<tr class="odd memdesc:aa5ff9236765b0441609caef6a7dde927">
<td class="mdescLeft"> </td>
<td class="mdescRight">Count the number of matrix elements with flags on.<br />
</td>
</tr>
<tr class="even separator:aa5ff9236765b0441609caef6a7dde927">
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
<tr class="even memitem:a5a63a005dee88234b9d300ce37267210">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a5a63a005dee88234b9d300ce37267210"></span> static void </td>
<td class="memItemRight" data-valign="bottom"><strong>test</strong> (void)</td>
</tr>
<tr class="odd memdesc:a5a63a005dee88234b9d300ce37267210">
<td class="mdescLeft"> </td>
<td class="mdescRight">Run unit tests for this module.<br />
</td>
</tr>
<tr class="even separator:a5a63a005dee88234b9d300ce37267210">
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
<tr class="even memitem:a641ba46d6b06a0a8046e3814f90117cc">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a641ba46d6b06a0a8046e3814f90117cc"></span> unsigned int </td>
<td class="memItemRight" data-valign="bottom"><strong>xIndexSize</strong></td>
</tr>
<tr class="odd memdesc:a641ba46d6b06a0a8046e3814f90117cc">
<td class="mdescLeft"> </td>
<td class="mdescRight">Matrix size in x-coordinate direction.<br />
</td>
</tr>
<tr class="even separator:a641ba46d6b06a0a8046e3814f90117cc">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:ab48b816340e15a48ffd0d99ea2447c35">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="ab48b816340e15a48ffd0d99ea2447c35"></span> unsigned int </td>
<td class="memItemRight" data-valign="bottom"><strong>yIndexSize</strong></td>
</tr>
<tr class="even memdesc:ab48b816340e15a48ffd0d99ea2447c35">
<td class="mdescLeft"> </td>
<td class="mdescRight">Matrix size in y-coordinate direction.<br />
</td>
</tr>
<tr class="odd separator:ab48b816340e15a48ffd0d99ea2447c35">
<td colspan="2" class="memSeparator"> </td>
</tr>
</tbody>
</table>

<span id="details"></span>

## Detailed Description

This object represents a 2D view into a 3D cave model. Given a desired
resolution (N x M pixels), it provides a boolean matrix. Each element
(x,y) in the matrix is set to 1 if there's a a face within the model in
that pixel location. The resolution is configurable, so for instance for
a 1 x 1 resolution all faces within the 3D model would be at the
resulting single pixel.

Note that the material matrix resolution is not the same as tile
resolution in the indexed mesh object. Typically, the indexed mesh has
low resolution, just sufficient for fast searches, while the material
matrix needs to support the desired high resolution of the output.

## Member Function Documentation

<span id="a2b38c1478297faff7ba33f8939e23041"></span>

## <span class="permalink">[◆ ](#a2b38c1478297faff7ba33f8939e23041)</span>getMaterialMatrix()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">bool MaterialMatrix::getMaterialMatrix</td>
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

<span id="ae66f04264fa9cd60b5cfd43991e0308f"></span>

## <span class="permalink">[◆ ](#ae66f04264fa9cd60b5cfd43991e0308f)</span>setMaterialMatrix()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">void MaterialMatrix::setMaterialMatrix</td>
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

-   src/<a href="outlinermaterialmatrix_8hh_source.md" class="el">outlinermaterialmatrix.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [Doxygen](https://www.doxygen.org/index.md)
1.9.2</span>
