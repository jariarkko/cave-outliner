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

[Public Member Functions](#pub-methods) | [Friends](#friends) | [List of
all members](class_processor-members.md)

Processor Class Reference

`#include <outlinerprocessor.hh>`

<table class="memberdecls">
<colgroup>
<col style="width: 50%" />
<col style="width: 50%" />
</colgroup>
<tbody>
<tr class="odd heading">
<td colspan="2"><h2 id="public-member-functions" class="groupheader"><span id="pub-methods"></span> Public Member Functions</h2></td>
</tr>
<tr class="even memitem:a29c77668a4dd09c43c88ad02ba6cc34b">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a29c77668a4dd09c43c88ad02ba6cc34b"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>Processor</strong> (const char *fileNameIn, unsigned int multiplierIn, bool smoothIn, bool mergedLinesIn, float linewidthIn, <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_box3_d.md" class="el">OutlinerBox3D</a> boundingBoxIn, outlinerreal stepxIn, outlinerreal stepyIn, outlinerreal stepzIn, enum outlinerdirection directionIn, enum outlineralgorithm algorithmIn, unsigned int holethresholdIn, bool labelsIn, <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_indexed_mesh.md" class="el">IndexedMesh</a> &amp;indexedIn)</td>
</tr>
<tr class="odd memdesc:a29c77668a4dd09c43c88ad02ba6cc34b">
<td class="mdescLeft"> </td>
<td class="mdescRight">Create a <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_processor.md" class="el">Processor</a>.<br />
</td>
</tr>
<tr class="even separator:a29c77668a4dd09c43c88ad02ba6cc34b">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:acf37952c5b420d4e903a512571678692">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="acf37952c5b420d4e903a512571678692"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>~Processor</strong> ()</td>
</tr>
<tr class="even memdesc:acf37952c5b420d4e903a512571678692">
<td class="mdescLeft"> </td>
<td class="mdescRight">Destruct a <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_processor.md" class="el">Processor</a>.<br />
</td>
</tr>
<tr class="odd separator:acf37952c5b420d4e903a512571678692">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:ac9ea1cec3c065afad46bb6ef7c21a934">
<td style="text-align: right;" class="memItemLeft" data-valign="top">bool </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_processor.md#ac9ea1cec3c065afad46bb6ef7c21a934" class="el">processScene</a> (const aiScene *scene, unsigned int nCrossSections, struct <a href="struct_processor_cross_section_info.md" class="el">ProcessorCrossSectionInfo</a> *crossSections)</td>
</tr>
<tr class="odd separator:ac9ea1cec3c065afad46bb6ef7c21a934">
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
<td colspan="2"><h2 id="friends" class="groupheader"><span id="friends"></span> Friends</h2></td>
</tr>
<tr class="even memitem:ae10b7fc1984e6fb27901b0cd87838a72">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="ae10b7fc1984e6fb27901b0cd87838a72"></span> class </td>
<td class="memItemRight" data-valign="bottom"><strong>ProcessorCrossSection</strong></td>
</tr>
<tr class="odd separator:ae10b7fc1984e6fb27901b0cd87838a72">
<td colspan="2" class="memSeparator"> </td>
</tr>
</tbody>
</table>

<span id="details"></span>

## Detailed Description

This is the main program of the Core module, it performs the actual
mapping from a given model and indexed mesh to an SVG image. It creates
a material matrix as part of the process.

## Member Function Documentation

<span id="ac9ea1cec3c065afad46bb6ef7c21a934"></span>

## <span class="permalink">[◆ ](#ac9ea1cec3c065afad46bb6ef7c21a934)</span>processScene()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">bool Processor::processScene</td>
<td>(</td>
<td class="paramtype">const aiScene * </td>
<td class="paramname"><em>scene</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">unsigned int </td>
<td class="paramname"><em>nCrossSections</em>,</td>
</tr>
<tr class="odd">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">struct <a href="struct_processor_cross_section_info.md" class="el">ProcessorCrossSectionInfo</a> * </td>
<td class="paramname"><em>crossSections</em> </td>
</tr>
<tr class="even">
<td></td>
<td>)</td>
<td></td>
<td></td>
</tr>
</tbody>
</table>

Process a plan fiew fora given scene in the processor, with a given set
of (optional) cross sections.

------------------------------------------------------------------------

The documentation for this class was generated from the following file:

-   src/<a href="outlinerprocessor_8hh_source.md" class="el">outlinerprocessor.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [Doxygen](https://www.doxygen.org/index.md)
1.9.2</span>
