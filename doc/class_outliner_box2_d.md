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
 members](class_outliner_box2_d-members.md)

OutlinerBox2D Class Reference

<table class="memberdecls">
<colgroup>
<col style="width: 50%" />
<col style="width: 50%" />
</colgroup>
<tbody>
<tr class="odd heading">
<td colspan="2"><h2 id="public-member-functions" class="groupheader"><span id="pub-methods"></span> Public Member Functions</h2></td>
</tr>
<tr class="even memitem:a77672a7d5994f41c4b8d4c56c06725f1">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a77672a7d5994f41c4b8d4c56c06725f1"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>OutlinerBox2D</strong> (const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_line2_d.md" class="el">OutlinerLine2D</a> &amp;in)</td>
</tr>
<tr class="odd separator:a77672a7d5994f41c4b8d4c56c06725f1">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:acbb553458d1bae36f7a920d4bacd0a16">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="acbb553458d1bae36f7a920d4bacd0a16"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>OutlinerBox2D</strong> (const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_vector2_d.md" class="el">OutlinerVector2D</a> &amp;startIn, const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_vector2_d.md" class="el">OutlinerVector2D</a> &amp;endIn)</td>
</tr>
<tr class="odd separator:acbb553458d1bae36f7a920d4bacd0a16">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:ae7909e320812130d353ad424607e12a7">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="ae7909e320812130d353ad424607e12a7"></span> bool </td>
<td class="memItemRight" data-valign="bottom"><strong>equal</strong> (const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_box2_d.md" class="el">OutlinerBox2D</a> &amp;box2) const</td>
</tr>
<tr class="odd separator:ae7909e320812130d353ad424607e12a7">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a3a8e08ddb95dd75a40d0607c64d70d33">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a3a8e08ddb95dd75a40d0607c64d70d33"></span> void </td>
<td class="memItemRight" data-valign="bottom"><strong>intersection</strong> (const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_box2_d.md" class="el">OutlinerBox2D</a> &amp;box2, <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_box2_d.md" class="el">OutlinerBox2D</a> &amp;resultBox) const</td>
</tr>
<tr class="odd separator:a3a8e08ddb95dd75a40d0607c64d70d33">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:aacd004948e4007f3f0d30370b63655bf">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="aacd004948e4007f3f0d30370b63655bf"></span> void </td>
<td class="memItemRight" data-valign="bottom"><strong>boxUnion</strong> (const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_box2_d.md" class="el">OutlinerBox2D</a> &amp;box2, <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_box2_d.md" class="el">OutlinerBox2D</a> &amp;resultBox) const</td>
</tr>
<tr class="odd separator:aacd004948e4007f3f0d30370b63655bf">
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
<tr class="even memitem:ae30c2b3cc74f2dd6d577f3605593ef1f">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="ae30c2b3cc74f2dd6d577f3605593ef1f"></span> static void </td>
<td class="memItemRight" data-valign="bottom"><strong>test</strong> (void)</td>
</tr>
<tr class="odd separator:ae30c2b3cc74f2dd6d577f3605593ef1f">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:ad1f12c29e2c3e2b9d8565eee83208e1b">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="ad1f12c29e2c3e2b9d8565eee83208e1b"></span> static void </td>
<td class="memItemRight" data-valign="bottom"><strong>testEqual</strong> (void)</td>
</tr>
<tr class="odd separator:ad1f12c29e2c3e2b9d8565eee83208e1b">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:acf2f05b35039a0dc27b659dc5d68accb">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="acf2f05b35039a0dc27b659dc5d68accb"></span> static void </td>
<td class="memItemRight" data-valign="bottom"><strong>testUnion</strong> (void)</td>
</tr>
<tr class="odd separator:acf2f05b35039a0dc27b659dc5d68accb">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a600251fd14c2ac740891de465103b39e">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a600251fd14c2ac740891de465103b39e"></span> static void </td>
<td class="memItemRight" data-valign="bottom"><strong>testIntersection</strong> (void)</td>
</tr>
<tr class="odd separator:a600251fd14c2ac740891de465103b39e">
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
<tr class="even memitem:abee81ccc4fac6434dfd0b369375ff21c">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="abee81ccc4fac6434dfd0b369375ff21c"></span> <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_vector2_d.md" class="el">OutlinerVector2D</a> </td>
<td class="memItemRight" data-valign="bottom"><strong>start</strong></td>
</tr>
<tr class="odd separator:abee81ccc4fac6434dfd0b369375ff21c">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a4152c9f6a809fa7f707d09251b0928eb">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a4152c9f6a809fa7f707d09251b0928eb"></span> <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_vector2_d.md" class="el">OutlinerVector2D</a> </td>
<td class="memItemRight" data-valign="bottom"><strong>end</strong></td>
</tr>
<tr class="odd separator:a4152c9f6a809fa7f707d09251b0928eb">
<td colspan="2" class="memSeparator"> </td>
</tr>
</tbody>
</table>

------------------------------------------------------------------------

The documentation for this class was generated from the following file:

-   src/<a href="outlinerhighprecision_8hh_source.md" class="el">outlinerhighprecision.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [Doxygen](https://www.doxygen.org/index.md)
1.9.2</span>
