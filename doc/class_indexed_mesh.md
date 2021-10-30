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
members](class_indexed_mesh-members.md)

IndexedMesh Class Reference

`#include <outlinerindexedmesh.hh>`

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
<td class="memItemRight" data-valign="bottom"><strong>IndexedMesh</strong> (unsigned int maxMeshesIn, unsigned int subdivisionsIn, const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_box3_d.md" class="el">OutlinerBox3D</a> &amp;modelBoundingBox, const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_box2_d.md" class="el">OutlinerBox2D</a> &amp;viewBoundingBox, enum outlinerdirection directionIn)</td>
</tr>
<tr class="odd memdesc:ab8c2853a96f3f2d43949575fcb5516b2">
<td class="mdescLeft"> </td>
<td class="mdescRight">Create an <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_indexed_mesh.md" class="el">IndexedMesh</a> object.<br />
</td>
</tr>
<tr class="even separator:ab8c2853a96f3f2d43949575fcb5516b2">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a925eb3e8be2a23948b1a89cf0e84e741">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a925eb3e8be2a23948b1a89cf0e84e741"></span> void </td>
<td class="memItemRight" data-valign="bottom"><strong>addScene</strong> (const aiScene *scene)</td>
</tr>
<tr class="even memdesc:a925eb3e8be2a23948b1a89cf0e84e741">
<td class="mdescLeft"> </td>
<td class="mdescRight">Add a 3D scene to the optimized index.<br />
</td>
</tr>
<tr class="odd separator:a925eb3e8be2a23948b1a89cf0e84e741">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a87bf0125708b666b0e981bd97ecf1b4a">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a87bf0125708b666b0e981bd97ecf1b4a"></span> void </td>
<td class="memItemRight" data-valign="bottom"><strong>addNode</strong> (const aiScene *scene, const aiNode *node)</td>
</tr>
<tr class="odd memdesc:a87bf0125708b666b0e981bd97ecf1b4a">
<td class="mdescLeft"> </td>
<td class="mdescRight">Add a 3D node to the optimized index.<br />
</td>
</tr>
<tr class="even separator:a87bf0125708b666b0e981bd97ecf1b4a">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a62435349e69af952634e805827d56576">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a62435349e69af952634e805827d56576"></span> void </td>
<td class="memItemRight" data-valign="bottom"><strong>addMesh</strong> (const aiScene *scene, const aiMesh *mesh)</td>
</tr>
<tr class="even memdesc:a62435349e69af952634e805827d56576">
<td class="mdescLeft"> </td>
<td class="mdescRight">Add a 3D mesh to the optimized index.<br />
</td>
</tr>
<tr class="odd separator:a62435349e69af952634e805827d56576">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:aea51fdc366f822252850da1f8b6036ea">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="aea51fdc366f822252850da1f8b6036ea"></span> void </td>
<td class="memItemRight" data-valign="bottom"><strong>getFaces</strong> (const aiMesh *mesh, outlinerreal x, outlinerreal y, unsigned int *p_nFaces, const aiFace ***p_faces)</td>
</tr>
<tr class="odd memdesc:aea51fdc366f822252850da1f8b6036ea">
<td class="mdescLeft"> </td>
<td class="mdescRight">Quickly get faces associated with a given (x,y) point in the plan view.<br />
</td>
</tr>
<tr class="even separator:aea51fdc366f822252850da1f8b6036ea">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:af5f57e4b27dc9e76e7fcdcec9edb2bbc">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="af5f57e4b27dc9e76e7fcdcec9edb2bbc"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>~IndexedMesh</strong> ()</td>
</tr>
<tr class="even memdesc:af5f57e4b27dc9e76e7fcdcec9edb2bbc">
<td class="mdescLeft"> </td>
<td class="mdescRight">Release all resources associated with the index.<br />
</td>
</tr>
<tr class="odd separator:af5f57e4b27dc9e76e7fcdcec9edb2bbc">
<td colspan="2" class="memSeparator"> </td>
</tr>
</tbody>
</table>

<span id="details"></span>

## Detailed Description

This object represents an optimized index to the mesh faces contained in
an imported 3D model. The imported model has a large datastructure of
'faces' – typically millions or even tens of millions of faces. There is
no efficient way to search for faces at a given location in the 3D or 2D
space, however. The indexed mesh object sorts the faces into a 2D matrix
of 'tiles'. For instance, a large model could be split into 20x20 or 400
tiles, so that when we are looking for a face within given (x,y)
coordinates, we only need to look at the tile where (x,y) falls into.
The indexing is performed only once, and all searches after the indexing
operation can use the more efficient search.

------------------------------------------------------------------------

The documentation for this class was generated from the following file:

-   src/<a href="outlinerindexedmesh_8hh_source.md" class="el">outlinerindexedmesh.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [Doxygen](https://www.doxygen.org/index.md)
1.9.2</span>
