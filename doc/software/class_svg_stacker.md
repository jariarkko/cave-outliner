<table data-cellspacing="0" data-cellpadding="0">
<colgroup>
<col style="width: 100%" />
</colgroup>
<tbody>
<tr id="projectrow" class="odd">
<td id="projectalign"><div id="projectname">
Cave Outliner
</div></td>
</tr>
</tbody>
</table>

[Public Member Functions](#pub-methods) | [Static Public Member
Functions](#pub-static-methods) | 
members](class_svg_stacker-members.md)

SvgStacker Class Reference

`#include <outlinersvgstacker.hh>`

<table class="memberdecls">
<colgroup>
<col style="width: 50%" />
<col style="width: 50%" />
</colgroup>
<tbody>
<tr class="odd heading">
<td colspan="2"><h2 id="public-member-functions"
class="groupheader"><span id="pub-methods"></span> Public Member
Functions</h2></td>
</tr>
<tr class="even memitem:a97354a854f9332c3e4d48dab276dcb3c">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"> </td>
<td class="memItemRight" data-valign="bottom"><a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_svg_stacker.md#a97354a854f9332c3e4d48dab276dcb3c"
class="el">SvgStacker</a> (const char *fileNameIn, const unsigned int
nImageFileNames, const char **imageFileNames, const <a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_svg_options.md" class="el">SvgOptions</a> &amp;optionsIn,
const unsigned int verticalSpacing)</td>
</tr>
<tr class="odd separator:a97354a854f9332c3e4d48dab276dcb3c">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:ac062d25ce9b78d15f40fdd52432c8e31">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="ac062d25ce9b78d15f40fdd52432c8e31"></span>
 </td>
<td class="memItemRight"
data-valign="bottom"><strong>~SvgStacker</strong> ()</td>
</tr>
<tr class="odd memdesc:ac062d25ce9b78d15f40fdd52432c8e31">
<td class="mdescLeft"> </td>
<td class="mdescRight">Release all resources associated with the SVG
stacker object. And close the file, if still open.<br />
</td>
</tr>
<tr class="even separator:ac062d25ce9b78d15f40fdd52432c8e31">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a210263e21e7be99648484a7c0fb821fd">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a210263e21e7be99648484a7c0fb821fd"></span>
bool </td>
<td class="memItemRight" data-valign="bottom"><strong>ok</strong>
()</td>
</tr>
<tr class="even memdesc:a210263e21e7be99648484a7c0fb821fd">
<td class="mdescLeft"> </td>
<td class="mdescRight">Verify that image creation and file write was
successful.<br />
</td>
</tr>
<tr class="odd separator:a210263e21e7be99648484a7c0fb821fd">
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
<td colspan="2"><h2 id="static-public-member-functions"
class="groupheader"><span id="pub-static-methods"></span> Static Public
Member Functions</h2></td>
</tr>
<tr class="even memitem:a4898b3eb1450485145872b3a3008c89b">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a4898b3eb1450485145872b3a3008c89b"></span>
static void </td>
<td class="memItemRight" data-valign="bottom"><strong>test</strong>
(void)</td>
</tr>
<tr class="odd memdesc:a4898b3eb1450485145872b3a3008c89b">
<td class="mdescLeft"> </td>
<td class="mdescRight">Unit tests.<br />
</td>
</tr>
<tr class="even separator:a4898b3eb1450485145872b3a3008c89b">
<td colspan="2" class="memSeparator"> </td>
</tr>
</tbody>
</table>

<span id="details"></span>

## Detailed Description

This object can be used to read SVG images from a file.

## Constructor & Destructor Documentation

<span id="a97354a854f9332c3e4d48dab276dcb3c"></span>

## <span class="permalink">[◆ ](#a97354a854f9332c3e4d48dab276dcb3c)</span>SvgStacker()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">SvgStacker::SvgStacker</td>
<td>(</td>
<td class="paramtype">const char * </td>
<td class="paramname"><em>fileNameIn</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const unsigned int </td>
<td class="paramname"><em>nImageFileNames</em>,</td>
</tr>
<tr class="odd">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const char ** </td>
<td class="paramname"><em>imageFileNames</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_svg_options.md"
class="el">SvgOptions</a> &amp; </td>
<td class="paramname"><em>optionsIn</em>,</td>
</tr>
<tr class="odd">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const unsigned int </td>
<td class="paramname"><em>verticalSpacing</em> </td>
</tr>
<tr class="even">
<td></td>
<td>)</td>
<td></td>
<td></td>
</tr>
</tbody>
</table>

Create an SVG file with given file name, by tiling a number of input SVG
images on top of each other (vertically).

------------------------------------------------------------------------

The documentation for this class was generated from the following file:

-   src/<a href="outlinersvgstacker_8hh_source.md"
    class="el">outlinersvgstacker.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [<img src="doxygen.svg" class="footer" width="104" height="31"
alt="doxygen" />](https://www.doxygen.org/index.md) 1.9.4</span>
