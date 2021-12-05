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

[Public Member Functions](#pub-methods) | 
members](class_bounding_boxer-members.md)

BoundingBoxer Class Reference

`#include <outlinerboundingboxer.hh>`

<table class="memberdecls">
<colgroup>
<col style="width: 50%" />
<col style="width: 50%" />
</colgroup>
<tbody>
<tr class="odd heading">
<td colspan="2"><h2 id="public-member-functions" class="groupheader"><span id="pub-methods"></span> Public Member Functions</h2></td>
</tr>
<tr class="even memitem:a13a038e1f02a7d71f4ab80512c76fd86">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a13a038e1f02a7d71f4ab80512c76fd86"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>BoundingBoxer</strong> (const aiScene *scene, outlinerreal xIncrease, outlinerreal yIncrease, outlinerreal zIncrease)</td>
</tr>
<tr class="odd memdesc:a13a038e1f02a7d71f4ab80512c76fd86">
<td class="mdescLeft"> </td>
<td class="mdescRight">Construct an object can calculate the bounding box.<br />
</td>
</tr>
<tr class="even separator:a13a038e1f02a7d71f4ab80512c76fd86">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a417c235e7404f240cf490fbb3b83fa3b">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a417c235e7404f240cf490fbb3b83fa3b"></span> void </td>
<td class="memItemRight" data-valign="bottom"><strong>getOriginalBoundingBox</strong> (<a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box3_d.md" class="el">OutlinerBox3D</a> &amp;boundingBoxOut)</td>
</tr>
<tr class="even memdesc:a417c235e7404f240cf490fbb3b83fa3b">
<td class="mdescLeft"> </td>
<td class="mdescRight">Calculate the bounding box.<br />
</td>
</tr>
<tr class="odd separator:a417c235e7404f240cf490fbb3b83fa3b">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a20a1b874da8feb076dfe973decd442bc">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a20a1b874da8feb076dfe973decd442bc"></span> void </td>
<td class="memItemRight" data-valign="bottom"><strong>getIncreasedBoundingBox</strong> (<a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box3_d.md" class="el">OutlinerBox3D</a> &amp;boundingBoxOut)</td>
</tr>
<tr class="odd memdesc:a20a1b874da8feb076dfe973decd442bc">
<td class="mdescLeft"> </td>
<td class="mdescRight">Calculate the bounding box, with a given extra space around.<br />
</td>
</tr>
<tr class="even separator:a20a1b874da8feb076dfe973decd442bc">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:ad456ba22bf7d1b739add85445314847b">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="ad456ba22bf7d1b739add85445314847b"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>~BoundingBoxer</strong> ()</td>
</tr>
<tr class="even memdesc:ad456ba22bf7d1b739add85445314847b">
<td class="mdescLeft"> </td>
<td class="mdescRight">Release all resources associated with the bounding box calculation.<br />
</td>
</tr>
<tr class="odd separator:ad456ba22bf7d1b739add85445314847b">
<td colspan="2" class="memSeparator"> </td>
</tr>
</tbody>
</table>

<span id="details"></span>

## Detailed Description

This class enables the calculation to determine the size of a given
model, i.e., the bounding box the 3D model fits in.

------------------------------------------------------------------------

The documentation for this class was generated from the following file:

-   src/<a href="outlinerboundingboxer_8hh_source.md" class="el">outlinerboundingboxer.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [Doxygen](https://www.doxygen.org/index.md)
1.9.2</span>
