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
members](class_processor_cross_section-members.md)

ProcessorCrossSection Class Reference

`#include <outlinerprocessorcrosssection.hh>`

<table class="memberdecls">
<colgroup>
<col style="width: 50%" />
<col style="width: 50%" />
</colgroup>
<tbody>
<tr class="odd heading">
<td colspan="2"><h2 id="public-member-functions" class="groupheader"><span id="pub-methods"></span> Public Member Functions</h2></td>
</tr>
<tr class="even memitem:ab5b03c7067518b02e549e4051634e3c1">
<td style="text-align: right;" class="memItemLeft" data-valign="top"> </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_processor_cross_section.md#ab5b03c7067518b02e549e4051634e3c1" class="el">ProcessorCrossSection</a> (const char *fileNameIn, const char *labelIn, enum outlinerdirection sliceDirectionIn, const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_vector2_d.md" class="el">OutlinerVector2D</a> &amp;lineStartIn, const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_vector2_d.md" class="el">OutlinerVector2D</a> &amp;lineEndIn, outlinerreal stepzIn, <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_processor.md" class="el">Processor</a> &amp;procIn)</td>
</tr>
<tr class="odd separator:ab5b03c7067518b02e549e4051634e3c1">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a18dee48dd788c2c74b40d88d75b0417c">
<td style="text-align: right;" class="memItemLeft" data-valign="top"> </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_processor_cross_section.md#a18dee48dd788c2c74b40d88d75b0417c" class="el">~ProcessorCrossSection</a> ()</td>
</tr>
<tr class="odd separator:a18dee48dd788c2c74b40d88d75b0417c">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a2454d1a516440eb265832a87f40be255">
<td style="text-align: right;" class="memItemLeft" data-valign="top">bool </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_processor_cross_section.md#a2454d1a516440eb265832a87f40be255" class="el">processSceneCrossSection</a> (const aiScene *scene)</td>
</tr>
<tr class="odd separator:a2454d1a516440eb265832a87f40be255">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:ae29053fb50a8ac8e7a640807697c7ccc">
<td style="text-align: right;" class="memItemLeft" data-valign="top">void </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_processor_cross_section.md#ae29053fb50a8ac8e7a640807697c7ccc" class="el">getLineActualEndPoints</a> (<a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_line2_d.md" class="el">OutlinerLine2D</a> &amp;actualLine, outlinerreal extralineatends)</td>
</tr>
<tr class="odd separator:ae29053fb50a8ac8e7a640807697c7ccc">
<td colspan="2" class="memSeparator"> </td>
</tr>
</tbody>
</table>

<span id="details"></span>

## Detailed Description

This is the module inside of the Core module that performs the the
drawing of cross sections.

## Constructor & Destructor Documentation

<span id="ab5b03c7067518b02e549e4051634e3c1"></span>

## <span class="permalink">[◆ ](#ab5b03c7067518b02e549e4051634e3c1)</span>ProcessorCrossSection()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">ProcessorCrossSection::ProcessorCrossSection</td>
<td>(</td>
<td class="paramtype">const char * </td>
<td class="paramname"><em>fileNameIn</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const char * </td>
<td class="paramname"><em>labelIn</em>,</td>
</tr>
<tr class="odd">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">enum outlinerdirection </td>
<td class="paramname"><em>sliceDirectionIn</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_vector2_d.md" class="el">OutlinerVector2D</a> &amp; </td>
<td class="paramname"><em>lineStartIn</em>,</td>
</tr>
<tr class="odd">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_vector2_d.md" class="el">OutlinerVector2D</a> &amp; </td>
<td class="paramname"><em>lineEndIn</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">outlinerreal </td>
<td class="paramname"><em>stepzIn</em>,</td>
</tr>
<tr class="odd">
<td class="paramkey"></td>
<td></td>
<td class="paramtype"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_processor.md" class="el">Processor</a> &amp; </td>
<td class="paramname"><em>procIn</em> </td>
</tr>
<tr class="even">
<td></td>
<td>)</td>
<td></td>
<td></td>
</tr>
</tbody>
</table>

Create an object to prepare for the drawing of a cross section view of a
cave tunnel.

<span id="a18dee48dd788c2c74b40d88d75b0417c"></span>

## <span class="permalink">[◆ ](#a18dee48dd788c2c74b40d88d75b0417c)</span>~ProcessorCrossSection()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">ProcessorCrossSection::~ProcessorCrossSection</td>
<td>(</td>
<td class="paramname"></td>
<td>)</td>
<td></td>
</tr>
</tbody>
</table>

Release all resources associated with this cross section view, close the
image file, etc.

## Member Function Documentation

<span id="ae29053fb50a8ac8e7a640807697c7ccc"></span>

## <span class="permalink">[◆ ](#ae29053fb50a8ac8e7a640807697c7ccc)</span>getLineActualEndPoints()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">void ProcessorCrossSection::getLineActualEndPoints</td>
<td>(</td>
<td class="paramtype"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_line2_d.md" class="el">OutlinerLine2D</a> &amp; </td>
<td class="paramname"><em>actualLine</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">outlinerreal </td>
<td class="paramname"><em>extralineatends</em> </td>
</tr>
<tr class="odd">
<td></td>
<td>)</td>
<td></td>
<td></td>
</tr>
</tbody>
</table>

Once a cross section has been draw, we have information exactly how
wide/high the cross section is. On the plan view this can be represented
by a line drawn across the cave tunnel, with each end of the line ending
exactly at the tunnel walls. This function returns the coordinates for
this line.

<span id="a2454d1a516440eb265832a87f40be255"></span>

## <span class="permalink">[◆ ](#a2454d1a516440eb265832a87f40be255)</span>processSceneCrossSection()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">bool ProcessorCrossSection::processSceneCrossSection</td>
<td>(</td>
<td class="paramtype">const aiScene * </td>
<td class="paramname"><em>scene</em></td>
<td>)</td>
<td></td>
</tr>
</tbody>
</table>

Draw an actual cross section of the given scene. The coordinates
associated with the line representing the cross section were specified
at the time the
<a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_processor_cross_section.md" class="el">ProcessorCrossSection</a>
object was created.

------------------------------------------------------------------------

The documentation for this class was generated from the following file:

-   src/<a href="outlinerprocessorcrosssection_8hh_source.md" class="el">outlinerprocessorcrosssection.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [Doxygen](https://www.doxygen.org/index.md)
1.9.2</span>
