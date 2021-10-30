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

[Public Member Functions](#pub-methods) | [List of all
members](class_indexed_mesh-members.html)

IndexedMesh Class Reference

<table class="memberdecls">
<colgroup>
<col style="width: 50%" />
<col style="width: 50%" />
</colgroup>
<tbody>
<tr class="odd heading">
<td colspan="2"><h2 id="public-member-functions" class="groupheader"><span id="pub-methods"></span> Public Member Functions</h2></td>
</tr>
<tr class="even memitem:ab8c2853a96f3f2d43949575fcb5516b2">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="ab8c2853a96f3f2d43949575fcb5516b2"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>IndexedMesh</strong> (unsigned int maxMeshesIn, unsigned int subdivisionsIn, const <a href="class_outliner_box3_d.html" class="el">OutlinerBox3D</a> &amp;modelBoundingBox, const <a href="class_outliner_box2_d.html" class="el">OutlinerBox2D</a> &amp;viewBoundingBox, enum outlinerdirection directionIn)</td>
</tr>
<tr class="odd separator:ab8c2853a96f3f2d43949575fcb5516b2">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a925eb3e8be2a23948b1a89cf0e84e741">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a925eb3e8be2a23948b1a89cf0e84e741"></span> void </td>
<td class="memItemRight" data-valign="bottom"><strong>addScene</strong> (const aiScene *scene)</td>
</tr>
<tr class="odd separator:a925eb3e8be2a23948b1a89cf0e84e741">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a87bf0125708b666b0e981bd97ecf1b4a">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a87bf0125708b666b0e981bd97ecf1b4a"></span> void </td>
<td class="memItemRight" data-valign="bottom"><strong>addNode</strong> (const aiScene *scene, const aiNode *node)</td>
</tr>
<tr class="odd separator:a87bf0125708b666b0e981bd97ecf1b4a">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a62435349e69af952634e805827d56576">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a62435349e69af952634e805827d56576"></span> void </td>
<td class="memItemRight" data-valign="bottom"><strong>addMesh</strong> (const aiScene *scene, const aiMesh *mesh)</td>
</tr>
<tr class="odd separator:a62435349e69af952634e805827d56576">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:aea51fdc366f822252850da1f8b6036ea">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="aea51fdc366f822252850da1f8b6036ea"></span> void </td>
<td class="memItemRight" data-valign="bottom"><strong>getFaces</strong> (const aiMesh *mesh, outlinerreal x, outlinerreal y, unsigned int *p_nFaces, const aiFace ***p_faces)</td>
</tr>
<tr class="odd separator:aea51fdc366f822252850da1f8b6036ea">
<td colspan="2" class="memSeparator"> </td>
</tr>
</tbody>
</table>

------------------------------------------------------------------------

The documentation for this class was generated from the following file:

-   src/<a href="outlinerindexedmesh_8hh_source.html" class="el">outlinerindexedmesh.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [<img src="doxygen.svg" class="footer" width="104" height="31" alt="doxygen" />](https://www.doxygen.org/index.html)
1.9.2</span>
