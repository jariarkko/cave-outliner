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
 members](class_form_matrix2_d-members.md)

FormMatrix2D Class Reference

`#include <outlinerformmatrix2d.hh>`

<table class="memberdecls">
<colgroup>
<col style="width: 50%" />
<col style="width: 50%" />
</colgroup>
<tbody>
<tr class="odd heading">
<td colspan="2"><h2 id="public-member-functions" class="groupheader"><span id="pub-methods"></span> Public Member Functions</h2></td>
</tr>
<tr class="even memitem:ac31aa4a7b5bcfb4f99514ac0a01b272b">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="ac31aa4a7b5bcfb4f99514ac0a01b272b"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>FormMatrix2D</strong> (const unsigned int xIndexSizeIn, const unsigned int yIndexSizeIn)</td>
</tr>
<tr class="odd memdesc:ac31aa4a7b5bcfb4f99514ac0a01b272b">
<td class="mdescLeft"> </td>
<td class="mdescRight">Create a Formmatrix2d object.<br />
</td>
</tr>
<tr class="even separator:ac31aa4a7b5bcfb4f99514ac0a01b272b">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a3fd9dc4f2b12e4e40329c15f458482de">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a3fd9dc4f2b12e4e40329c15f458482de"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>~FormMatrix2D</strong> ()</td>
</tr>
<tr class="even memdesc:a3fd9dc4f2b12e4e40329c15f458482de">
<td class="mdescLeft"> </td>
<td class="mdescRight">Destruct the material matrix.<br />
</td>
</tr>
<tr class="odd separator:a3fd9dc4f2b12e4e40329c15f458482de">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:aed9f868ba8bdf4ddf19f3d50135a1345">
<td style="text-align: right;" class="memItemLeft" data-valign="top">void </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_form_matrix2_d.md#aed9f868ba8bdf4ddf19f3d50135a1345" class="el">setForm</a> (const unsigned int xIndex, const unsigned int yIndex, const outlinerform form)</td>
</tr>
<tr class="odd separator:aed9f868ba8bdf4ddf19f3d50135a1345">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:aab13d5d58e30a64fe84386d722e8692f">
<td style="text-align: right;" class="memItemLeft" data-valign="top">void </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_form_matrix2_d.md#aab13d5d58e30a64fe84386d722e8692f" class="el">setForm</a> (const unsigned int xIndexStart, const unsigned int yIndexStart, const unsigned int xIndexEnd, const unsigned int yIndexEnd, const outlinerform form)</td>
</tr>
<tr class="odd separator:aab13d5d58e30a64fe84386d722e8692f">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a03f462b59bb37cad777ac21fde3a0555">
<td style="text-align: right;" class="memItemLeft" data-valign="top">outlinerform </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_form_matrix2_d.md#a03f462b59bb37cad777ac21fde3a0555" class="el">getForm</a> (const unsigned int xIndex, const unsigned int yIndex) const</td>
</tr>
<tr class="odd separator:a03f462b59bb37cad777ac21fde3a0555">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:af3078e216048b5792c6e2bb38a856a2f">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="af3078e216048b5792c6e2bb38a856a2f"></span> char </td>
<td class="memItemRight" data-valign="bottom"><strong>getFormChar</strong> (const outlinerform form) const</td>
</tr>
<tr class="odd memdesc:af3078e216048b5792c6e2bb38a856a2f">
<td class="mdescLeft"> </td>
<td class="mdescRight">Convert a form to a descriptive character.<br />
</td>
</tr>
<tr class="even separator:af3078e216048b5792c6e2bb38a856a2f">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a87ebf8d776785f8ae105579b36406673">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a87ebf8d776785f8ae105579b36406673"></span> void </td>
<td class="memItemRight" data-valign="bottom"><strong>print</strong> (unsigned int step=1) const</td>
</tr>
<tr class="even memdesc:a87ebf8d776785f8ae105579b36406673">
<td class="mdescLeft"> </td>
<td class="mdescRight">Print the entire matrix.<br />
</td>
</tr>
<tr class="odd separator:a87ebf8d776785f8ae105579b36406673">
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
<tr class="even memitem:a34e6a90c6fabe20bf69473aa00d9ad19">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a34e6a90c6fabe20bf69473aa00d9ad19"></span> static void </td>
<td class="memItemRight" data-valign="bottom"><strong>test</strong> (void)</td>
</tr>
<tr class="odd memdesc:a34e6a90c6fabe20bf69473aa00d9ad19">
<td class="mdescLeft"> </td>
<td class="mdescRight">Run unit tests for this module.<br />
</td>
</tr>
<tr class="even separator:a34e6a90c6fabe20bf69473aa00d9ad19">
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
<tr class="even memitem:a48419b75066a56cd806a880b9ed17d51">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a48419b75066a56cd806a880b9ed17d51"></span> const unsigned int </td>
<td class="memItemRight" data-valign="bottom"><strong>xIndexSize</strong></td>
</tr>
<tr class="odd memdesc:a48419b75066a56cd806a880b9ed17d51">
<td class="mdescLeft"> </td>
<td class="mdescRight">Matrix size in x-coordinate direction.<br />
</td>
</tr>
<tr class="even separator:a48419b75066a56cd806a880b9ed17d51">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a619677cc03f8a61b6b7a2c2dd9701aaf">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a619677cc03f8a61b6b7a2c2dd9701aaf"></span> const unsigned int </td>
<td class="memItemRight" data-valign="bottom"><strong>yIndexSize</strong></td>
</tr>
<tr class="even memdesc:a619677cc03f8a61b6b7a2c2dd9701aaf">
<td class="mdescLeft"> </td>
<td class="mdescRight">Matrix size in y-coordinate direction.<br />
</td>
</tr>
<tr class="odd separator:a619677cc03f8a61b6b7a2c2dd9701aaf">
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

<span id="a03f462b59bb37cad777ac21fde3a0555"></span>

## <span class="permalink">[◆ ](#a03f462b59bb37cad777ac21fde3a0555)</span>getForm()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">outlinerform FormMatrix2D::getForm</td>
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

<span id="aed9f868ba8bdf4ddf19f3d50135a1345"></span>

## <span class="permalink">[◆ ](#aed9f868ba8bdf4ddf19f3d50135a1345)</span>setForm() <span class="overload">\[1/2\]</span>

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">void FormMatrix2D::setForm</td>
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
<td class="paramtype">const outlinerform </td>
<td class="paramname"><em>form</em> </td>
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

<span id="aab13d5d58e30a64fe84386d722e8692f"></span>

## <span class="permalink">[◆ ](#aab13d5d58e30a64fe84386d722e8692f)</span>setForm() <span class="overload">\[2/2\]</span>

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">void FormMatrix2D::setForm</td>
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
<td class="paramname"><em>yIndexEnd</em>,</td>
</tr>
<tr class="odd">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const outlinerform </td>
<td class="paramname"><em>form</em> </td>
</tr>
<tr class="even">
<td></td>
<td>)</td>
<td></td>
<td></td>
</tr>
</tbody>
</table>

Set the flag indicating that there is material in a given
(xStart,yStart) - (xEnd,yEnd) rectangle/range in the matrix.

------------------------------------------------------------------------

The documentation for this class was generated from the following file:

-   src/<a href="outlinerformmatrix2d_8hh_source.md" class="el">outlinerformmatrix2d.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [Doxygen](https://www.doxygen.org/index.md)
1.9.2</span>
