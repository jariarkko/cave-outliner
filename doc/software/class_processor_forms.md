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

[Public Member Functions](#pub-methods) | [Friends](#friends) | [List of
all members](class_processor_forms-members.md)

ProcessorForms Class Reference

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
<tr class="even memitem:a37a3a31f3fbc1f053cbd72e75e15439a">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a37a3a31f3fbc1f053cbd72e75e15439a"></span>
 </td>
<td class="memItemRight"
data-valign="bottom"><strong>ProcessorForms</strong> (const <a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box3_d.md" class="el">OutlinerBox3D</a>
&amp;boundingBoxIn, const enum outlinerdirection directionIn, const <a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box2_d.md" class="el">OutlinerBox2D</a>
&amp;planviewBoundingBoxIn, const outlinerreal stepxIn, const
outlinerreal stepyIn, const outlinerreal stepzIn, const unsigned int
formCondenseIn, <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_material_matrix2_d.md"
class="el">MaterialMatrix2D</a> &amp;matrix2In, class <a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_processor.md" class="el">Processor</a> &amp;procIn)</td>
</tr>
<tr class="odd separator:a37a3a31f3fbc1f053cbd72e75e15439a">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a81c1dc17176f21b7005a612b7b5d5284">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a81c1dc17176f21b7005a612b7b5d5284"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>performFormAnalysis</strong> (const aiScene
*scene)</td>
</tr>
<tr class="odd separator:a81c1dc17176f21b7005a612b7b5d5284">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a86b42a342e91f4cec4209766087a6689">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a86b42a342e91f4cec4209766087a6689"></span>
OutlinerSvgStyle </td>
<td class="memItemRight"
data-valign="bottom"><strong>formToColor</strong> (const unsigned int
xIndex, const unsigned int yIndex) const</td>
</tr>
<tr class="odd separator:a86b42a342e91f4cec4209766087a6689">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a4bf42981f66e05f594cedbfa645a22a6">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a4bf42981f66e05f594cedbfa645a22a6"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>formIsEntrance</strong> (const unsigned int
xIndex, const unsigned int yIndex) const</td>
</tr>
<tr class="odd separator:a4bf42981f66e05f594cedbfa645a22a6">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:adade4e5f314ba4966e92797eafeb7b6d">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="adade4e5f314ba4966e92797eafeb7b6d"></span>
outlinerform </td>
<td class="memItemRight" data-valign="bottom"><strong>getForm</strong>
(const unsigned int xIndex, const unsigned int yIndex) const</td>
</tr>
<tr class="odd separator:adade4e5f314ba4966e92797eafeb7b6d">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:abb6465156d01897bff5977093223e1a3">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="abb6465156d01897bff5977093223e1a3"></span>
void </td>
<td class="memItemRight"
data-valign="bottom"><strong>drawSpines</strong> (<a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_svg_creator.md" class="el">SvgCreator</a> &amp;svg)
const</td>
</tr>
<tr class="odd separator:abb6465156d01897bff5977093223e1a3">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a4a9e2e9732b561472ba23c771c1df25f">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a4a9e2e9732b561472ba23c771c1df25f"></span>
const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_depth_map.md" class="el">DepthMap</a> &amp; </td>
<td class="memItemRight"
data-valign="bottom"><strong>getFloorDepthMap</strong> (void)</td>
</tr>
<tr class="odd separator:a4a9e2e9732b561472ba23c771c1df25f">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a66ae588e104fe8b615db99b227f9816c">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a66ae588e104fe8b615db99b227f9816c"></span>
const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_depth_map.md" class="el">DepthMap</a> &amp; </td>
<td class="memItemRight"
data-valign="bottom"><strong>getRoofDepthMap</strong> (void)</td>
</tr>
<tr class="odd separator:a66ae588e104fe8b615db99b227f9816c">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a7f21b877a30300c5f868e096099df715">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a7f21b877a30300c5f868e096099df715"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>condensedXIndexToIndex</strong> (const
unsigned int matrix3xIndex, unsigned int &amp;matrix2xIndexStart,
unsigned int &amp;matrix2xIndexEnd) const</td>
</tr>
<tr class="odd separator:a7f21b877a30300c5f868e096099df715">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:ae40d58380b6067008e807661980e053f">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="ae40d58380b6067008e807661980e053f"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>condensedYIndexToIndex</strong> (const
unsigned int matrix3yIndex, unsigned int &amp;matrix2yIndexStart,
unsigned int &amp;matrix2yIndexEnd) const</td>
</tr>
<tr class="odd separator:ae40d58380b6067008e807661980e053f">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a8630c40b52080e31b865fba6c084d5bd">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a8630c40b52080e31b865fba6c084d5bd"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>condensedIndexesToIndexes</strong> (const
unsigned int matrix3xIndex, const unsigned int matrix3yIndex, unsigned
int &amp;matrix2xIndexStart, unsigned int &amp;matrix2yIndexStart,
unsigned int &amp;matrix2xIndexEnd, unsigned int &amp;matrix2yIndexEnd)
const</td>
</tr>
<tr class="odd separator:a8630c40b52080e31b865fba6c084d5bd">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a6ea84e783348d35d55726162c544f8ab">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a6ea84e783348d35d55726162c544f8ab"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>condensedXIndexIncrease</strong> (unsigned
int &amp;matrix2xIndex) const</td>
</tr>
<tr class="odd separator:a6ea84e783348d35d55726162c544f8ab">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a5ac532174a88c4593d8067586ac2c95b">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a5ac532174a88c4593d8067586ac2c95b"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>condensedYIndexIncrease</strong> (unsigned
int &amp;matrix2yIndex) const</td>
</tr>
<tr class="odd separator:a5ac532174a88c4593d8067586ac2c95b">
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
<td colspan="2"><h2 id="friends" class="groupheader"><span
id="friends"></span> Friends</h2></td>
</tr>
<tr class="even memitem:a562410625b14f6eb616e03a684b87078">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a562410625b14f6eb616e03a684b87078"></span>
class </td>
<td class="memItemRight"
data-valign="bottom"><strong>OutlineAnalyzer</strong></td>
</tr>
<tr class="odd separator:a562410625b14f6eb616e03a684b87078">
<td colspan="2" class="memSeparator"> </td>
</tr>
</tbody>
</table>

------------------------------------------------------------------------

The documentation for this class was generated from the following file:

-   src/<a href="outlinerprocessorforms_8hh_source.md"
    class="el">outlinerprocessorforms.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [<img src="doxygen.svg" class="footer" width="104" height="31"
alt="doxygen" />](https://www.doxygen.org/index.md) 1.9.4</span>
