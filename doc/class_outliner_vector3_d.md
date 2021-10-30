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
 members](class_outliner_vector3_d-members.md)

OutlinerVector3D Class Reference

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
<tr class="even memitem:a5cf716e1944ca15337614188930d686d">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a5cf716e1944ca15337614188930d686d"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>OutlinerVector3D</strong> ()</td>
</tr>
<tr class="odd memdesc:a5cf716e1944ca15337614188930d686d">
<td class="mdescLeft"> </td>
<td class="mdescRight">Create an empty object, all values initialized to zero.<br />
</td>
</tr>
<tr class="even separator:a5cf716e1944ca15337614188930d686d">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a9eca248971df03a70d0bf4453643206f">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a9eca248971df03a70d0bf4453643206f"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>OutlinerVector3D</strong> (const aiVector3D &amp;in)</td>
</tr>
<tr class="even memdesc:a9eca248971df03a70d0bf4453643206f">
<td class="mdescLeft"> </td>
<td class="mdescRight">Make an exact copy of another similar object. (But with lower precision.)<br />
</td>
</tr>
<tr class="odd separator:a9eca248971df03a70d0bf4453643206f">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a9d5f45589e2eb2b267a0260664cf0b91">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a9d5f45589e2eb2b267a0260664cf0b91"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>OutlinerVector3D</strong> (const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_vector3_d.md" class="el">OutlinerVector3D</a> &amp;in)</td>
</tr>
<tr class="odd memdesc:a9d5f45589e2eb2b267a0260664cf0b91">
<td class="mdescLeft"> </td>
<td class="mdescRight">Make an exact copy of another similar object.<br />
</td>
</tr>
<tr class="even separator:a9d5f45589e2eb2b267a0260664cf0b91">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a2e7ee2d85673f2183a0eec4c5873fd0d">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a2e7ee2d85673f2183a0eec4c5873fd0d"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>OutlinerVector3D</strong> (const outlinerreal xIn, const outlinerreal yIn, const outlinerreal zIn)</td>
</tr>
<tr class="even memdesc:a2e7ee2d85673f2183a0eec4c5873fd0d">
<td class="mdescLeft"> </td>
<td class="mdescRight">Make a vector based on the x,y,z coordinates.<br />
</td>
</tr>
<tr class="odd separator:a2e7ee2d85673f2183a0eec4c5873fd0d">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:acb11ce0e976cd01f6711989e30b0b643">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="acb11ce0e976cd01f6711989e30b0b643"></span> bool </td>
<td class="memItemRight" data-valign="bottom"><strong>equal</strong> (const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_vector3_d.md" class="el">OutlinerVector3D</a> &amp;b) const</td>
</tr>
<tr class="odd separator:acb11ce0e976cd01f6711989e30b0b643">
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
<tr class="even memitem:a17b88bf4d0a6764e086fc1787cc42f0d">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a17b88bf4d0a6764e086fc1787cc42f0d"></span> static void </td>
<td class="memItemRight" data-valign="bottom"><strong>test</strong> (void)</td>
</tr>
<tr class="odd memdesc:a17b88bf4d0a6764e086fc1787cc42f0d">
<td class="mdescLeft"> </td>
<td class="mdescRight">Run unit tests associated with this class.<br />
</td>
</tr>
<tr class="even separator:a17b88bf4d0a6764e086fc1787cc42f0d">
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
<tr class="even memitem:a7025c0bfc1900fb50f4a63bf7eaec44b">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a7025c0bfc1900fb50f4a63bf7eaec44b"></span> outlinerreal </td>
<td class="memItemRight" data-valign="bottom"><strong>x</strong></td>
</tr>
<tr class="odd memdesc:a7025c0bfc1900fb50f4a63bf7eaec44b">
<td class="mdescLeft"> </td>
<td class="mdescRight">The x coordinate of the point.<br />
</td>
</tr>
<tr class="even separator:a7025c0bfc1900fb50f4a63bf7eaec44b">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:afafb83b297f759422392b59e3d381b87">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="afafb83b297f759422392b59e3d381b87"></span> outlinerreal </td>
<td class="memItemRight" data-valign="bottom"><strong>y</strong></td>
</tr>
<tr class="even memdesc:afafb83b297f759422392b59e3d381b87">
<td class="mdescLeft"> </td>
<td class="mdescRight">The y coordinate of the point.<br />
</td>
</tr>
<tr class="odd separator:afafb83b297f759422392b59e3d381b87">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a65c407a22cd55015a0bd7c8cf57def67">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a65c407a22cd55015a0bd7c8cf57def67"></span> outlinerreal </td>
<td class="memItemRight" data-valign="bottom"><strong>z</strong></td>
</tr>
<tr class="odd memdesc:a65c407a22cd55015a0bd7c8cf57def67">
<td class="mdescLeft"> </td>
<td class="mdescRight">The z coordinate of the point.<br />
</td>
</tr>
<tr class="even separator:a65c407a22cd55015a0bd7c8cf57def67">
<td colspan="2" class="memSeparator"> </td>
</tr>
</tbody>
</table>

<span id="details"></span>

## Detailed Description

This object represents a "vector" to a point in 3D space.

------------------------------------------------------------------------

The documentation for this class was generated from the following file:

-   src/<a href="outlinerhighprecision_8hh_source.md" class="el">outlinerhighprecision.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [Doxygen](https://www.doxygen.org/index.md)
1.9.2</span>
