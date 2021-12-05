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
<tr class="even memitem:aa8f1bb7b2ff061aa20926db570fbd5ab">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="aa8f1bb7b2ff061aa20926db570fbd5ab"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>Processor</strong> (const char *fileNameIn, const unsigned int multiplierIn, const bool smoothIn, const bool mergedLinesIn, const float linewidthIn, const bool svgYSwapIn, const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box3_d.md" class="el">OutlinerBox3D</a> &amp;originalBoundingBoxIn, const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_outliner_box3_d.md" class="el">OutlinerBox3D</a> &amp;boundingBoxIn, const outlinerreal stepxIn, const outlinerreal stepyIn, const outlinerreal stepzIn, const enum outlinerdirection directionIn, const enum outlineralgorithm algorithmIn, const unsigned int holethresholdIn, const unsigned int lineHolethresholdIn, const unsigned int dustTresholdIn, const bool labelsIn, const bool formAnalysisIn, const unsigned int formCondenseIn, const bool dimensionsIn, unsigned int nCrossSectionsIn, struct <a href="struct_processor_cross_section_info.md" class="el">ProcessorCrossSectionInfo</a> *crossSectionsIn, <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_indexed_mesh.md" class="el">IndexedMesh</a> &amp;indexedIn)</td>
</tr>
<tr class="odd memdesc:aa8f1bb7b2ff061aa20926db570fbd5ab">
<td class="mdescLeft"> </td>
<td class="mdescRight">Create a <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_processor.md" class="el">Processor</a>.<br />
</td>
</tr>
<tr class="even separator:aa8f1bb7b2ff061aa20926db570fbd5ab">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:acf37952c5b420d4e903a512571678692">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="acf37952c5b420d4e903a512571678692"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>~Processor</strong> ()</td>
</tr>
<tr class="even memdesc:acf37952c5b420d4e903a512571678692">
<td class="mdescLeft"> </td>
<td class="mdescRight">Destruct a <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_processor.md" class="el">Processor</a>.<br />
</td>
</tr>
<tr class="odd separator:acf37952c5b420d4e903a512571678692">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a2db9b603b09bf7de1e7e0976b1dac1bc">
<td style="text-align: right;" class="memItemLeft" data-valign="top">bool </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_processor.md#a2db9b603b09bf7de1e7e0976b1dac1bc" class="el">processScene</a> (const aiScene *scene)</td>
</tr>
<tr class="odd separator:a2db9b603b09bf7de1e7e0976b1dac1bc">
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
<tr class="even memitem:a83e4cc5caa21290f0d1ff4a9441a674e">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a83e4cc5caa21290f0d1ff4a9441a674e"></span> class </td>
<td class="memItemRight" data-valign="bottom"><strong>ProcessorForms</strong></td>
</tr>
<tr class="odd separator:a83e4cc5caa21290f0d1ff4a9441a674e">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a8be311027c613f05eaee0ba171518a59">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a8be311027c613f05eaee0ba171518a59"></span> class </td>
<td class="memItemRight" data-valign="bottom"><strong>DepthMap</strong></td>
</tr>
<tr class="odd separator:a8be311027c613f05eaee0ba171518a59">
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

<span id="a2db9b603b09bf7de1e7e0976b1dac1bc"></span>

## <span class="permalink">[◆ ](#a2db9b603b09bf7de1e7e0976b1dac1bc)</span>processScene()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">bool Processor::processScene</td>
<td>(</td>
<td class="paramtype">const aiScene * </td>
<td class="paramname"><em>scene</em></td>
<td>)</td>
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
