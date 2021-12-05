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
 members](class_outliner_vector2_d-members.md)

OutlinerVector2D Class Reference

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
<tr class="even memitem:a46400e63d11411fbf9aca32c9a95643b">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a46400e63d11411fbf9aca32c9a95643b"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>OutlinerVector2D</strong> ()</td>
</tr>
<tr class="odd memdesc:a46400e63d11411fbf9aca32c9a95643b">
<td class="mdescLeft"> </td>
<td class="mdescRight">Create an empty object, all values initialized to zero.<br />
</td>
</tr>
<tr class="even separator:a46400e63d11411fbf9aca32c9a95643b">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a7bbea017cfe65831322f22d8709c7e17">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a7bbea017cfe65831322f22d8709c7e17"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>OutlinerVector2D</strong> (const aiVector2D &amp;in)</td>
</tr>
<tr class="even memdesc:a7bbea017cfe65831322f22d8709c7e17">
<td class="mdescLeft"> </td>
<td class="mdescRight">Make an exact copy of another similar object. (But with lower precision.)<br />
</td>
</tr>
<tr class="odd separator:a7bbea017cfe65831322f22d8709c7e17">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:aefd197feb0e736f53488e2f06e0394f9">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="aefd197feb0e736f53488e2f06e0394f9"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>OutlinerVector2D</strong> (const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_vector2_d.md" class="el">OutlinerVector2D</a> &amp;in)</td>
</tr>
<tr class="odd memdesc:aefd197feb0e736f53488e2f06e0394f9">
<td class="mdescLeft"> </td>
<td class="mdescRight">Make an exact copy of another similar object.<br />
</td>
</tr>
<tr class="even separator:aefd197feb0e736f53488e2f06e0394f9">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a17a0d7512dcf1da4cef420a21716c211">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a17a0d7512dcf1da4cef420a21716c211"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>OutlinerVector2D</strong> (const outlinerreal xIn, const outlinerreal yIn)</td>
</tr>
<tr class="even memdesc:a17a0d7512dcf1da4cef420a21716c211">
<td class="mdescLeft"> </td>
<td class="mdescRight">Make a vector based on the x,y coordinates.<br />
</td>
</tr>
<tr class="odd separator:a17a0d7512dcf1da4cef420a21716c211">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a754eb4bf3312a92de26517e3cf8a1354">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a754eb4bf3312a92de26517e3cf8a1354"></span> <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_vector2_d.md" class="el">OutlinerVector2D</a> &amp; </td>
<td class="memItemRight" data-valign="bottom"><strong>operator=</strong> (const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_vector2_d.md" class="el">OutlinerVector2D</a> &amp;in)</td>
</tr>
<tr class="odd memdesc:a754eb4bf3312a92de26517e3cf8a1354">
<td class="mdescLeft"> </td>
<td class="mdescRight">Assignment.<br />
</td>
</tr>
<tr class="even separator:a754eb4bf3312a92de26517e3cf8a1354">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:aa21f5f0786ba884bda75223d4f8099cc">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="aa21f5f0786ba884bda75223d4f8099cc"></span> bool </td>
<td class="memItemRight" data-valign="bottom"><strong>equal</strong> (const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_vector2_d.md" class="el">OutlinerVector2D</a> &amp;b) const</td>
</tr>
<tr class="even memdesc:aa21f5f0786ba884bda75223d4f8099cc">
<td class="mdescLeft"> </td>
<td class="mdescRight">Test for equality.<br />
</td>
</tr>
<tr class="odd separator:aa21f5f0786ba884bda75223d4f8099cc">
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
<tr class="even memitem:a37be3f76ed2e583ba72f0c8c405cb0e9">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a37be3f76ed2e583ba72f0c8c405cb0e9"></span> static void </td>
<td class="memItemRight" data-valign="bottom"><strong>test</strong> (void)</td>
</tr>
<tr class="odd memdesc:a37be3f76ed2e583ba72f0c8c405cb0e9">
<td class="mdescLeft"> </td>
<td class="mdescRight">Run unit tests associated with this class.<br />
</td>
</tr>
<tr class="even separator:a37be3f76ed2e583ba72f0c8c405cb0e9">
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
<tr class="even memitem:a3947a056922d4ff8046f38ec366c57de">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a3947a056922d4ff8046f38ec366c57de"></span> outlinerreal </td>
<td class="memItemRight" data-valign="bottom"><strong>x</strong></td>
</tr>
<tr class="odd memdesc:a3947a056922d4ff8046f38ec366c57de">
<td class="mdescLeft"> </td>
<td class="mdescRight">The x coordinate of the point.<br />
</td>
</tr>
<tr class="even separator:a3947a056922d4ff8046f38ec366c57de">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a786083ebecf5558b495239745816eac4">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a786083ebecf5558b495239745816eac4"></span> outlinerreal </td>
<td class="memItemRight" data-valign="bottom"><strong>y</strong></td>
</tr>
<tr class="even memdesc:a786083ebecf5558b495239745816eac4">
<td class="mdescLeft"> </td>
<td class="mdescRight">The y coordinate of the point.<br />
</td>
</tr>
<tr class="odd separator:a786083ebecf5558b495239745816eac4">
<td colspan="2" class="memSeparator"> </td>
</tr>
</tbody>
</table>

<span id="details"></span>

## Detailed Description

This object represents a "vector" to a point in 2D space.

------------------------------------------------------------------------

The documentation for this class was generated from the following file:

-   src/<a href="outlinerhighprecision_8hh_source.md" class="el">outlinerhighprecision.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [Doxygen](https://www.doxygen.org/index.md)
1.9.2</span>
