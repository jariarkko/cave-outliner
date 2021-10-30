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
<tr class="even memitem:a7c84522a5270caa2425e12a2cca8ba2b">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a7c84522a5270caa2425e12a2cca8ba2b"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>BoundingBoxer</strong> (const aiScene *scene)</td>
</tr>
<tr class="odd memdesc:a7c84522a5270caa2425e12a2cca8ba2b">
<td class="mdescLeft"> </td>
<td class="mdescRight">Construct an object can calculate the bounding box.<br />
</td>
</tr>
<tr class="even separator:a7c84522a5270caa2425e12a2cca8ba2b">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:ab062fdf2e2697814b08b247d4be3fc16">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="ab062fdf2e2697814b08b247d4be3fc16"></span> void </td>
<td class="memItemRight" data-valign="bottom"><strong>getBoundingBox</strong> (<a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_box3_d.md" class="el">OutlinerBox3D</a> &amp;boundingBoxOut)</td>
</tr>
<tr class="even memdesc:ab062fdf2e2697814b08b247d4be3fc16">
<td class="mdescLeft"> </td>
<td class="mdescRight">Calculate the bounding box.<br />
</td>
</tr>
<tr class="odd separator:ab062fdf2e2697814b08b247d4be3fc16">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:ad456ba22bf7d1b739add85445314847b">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="ad456ba22bf7d1b739add85445314847b"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>~BoundingBoxer</strong> ()</td>
</tr>
<tr class="odd memdesc:ad456ba22bf7d1b739add85445314847b">
<td class="mdescLeft"> </td>
<td class="mdescRight">Release all resources associated with the bounding box calculation.<br />
</td>
</tr>
<tr class="even separator:ad456ba22bf7d1b739add85445314847b">
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
