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
members](class_svg_creator-members.md)

SvgCreator Class Reference

`#include <outlinersvg.hh>`

<table class="memberdecls">
<colgroup>
<col style="width: 50%" />
<col style="width: 50%" />
</colgroup>
<tbody>
<tr class="odd heading">
<td colspan="2"><h2 id="public-member-functions" class="groupheader"><span id="pub-methods"></span> Public Member Functions</h2></td>
</tr>
<tr class="even memitem:a73ce0fd293a4cfb52e9b5c2fe7ae1c9a">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a73ce0fd293a4cfb52e9b5c2fe7ae1c9a"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>SvgCreator</strong> (const char *fileName, unsigned int xSize, unsigned int ySize, unsigned int multiplier, outlinerreal xStart, outlinerreal yStart, outlinerreal xFactor, outlinerreal yFactor, bool smooth, bool mergedLines, outlinerreal linewidth)</td>
</tr>
<tr class="odd memdesc:a73ce0fd293a4cfb52e9b5c2fe7ae1c9a">
<td class="mdescLeft"> </td>
<td class="mdescRight">Create an SVG file with given file name and size.<br />
</td>
</tr>
<tr class="even separator:a73ce0fd293a4cfb52e9b5c2fe7ae1c9a">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a5287bc3ef08517a8dda80238cdbb2079">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a5287bc3ef08517a8dda80238cdbb2079"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>~SvgCreator</strong> ()</td>
</tr>
<tr class="even memdesc:a5287bc3ef08517a8dda80238cdbb2079">
<td class="mdescLeft"> </td>
<td class="mdescRight">Release all resources associated with the SVG creation object. And close the file, if still open.<br />
</td>
</tr>
<tr class="odd separator:a5287bc3ef08517a8dda80238cdbb2079">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:ae3491ce32bc9594ea3b170a4c8325ed3">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="ae3491ce32bc9594ea3b170a4c8325ed3"></span> void </td>
<td class="memItemRight" data-valign="bottom"><strong>line</strong> (outlinerreal fromX, outlinerreal fromY, outlinerreal toX, outlinerreal toY, bool dashed=0)</td>
</tr>
<tr class="odd memdesc:ae3491ce32bc9594ea3b170a4c8325ed3">
<td class="mdescLeft"> </td>
<td class="mdescRight">Draw a line.<br />
</td>
</tr>
<tr class="even separator:ae3491ce32bc9594ea3b170a4c8325ed3">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:ad58b8ea3bb3de693b6bfd3e97a6fd3ba">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="ad58b8ea3bb3de693b6bfd3e97a6fd3ba"></span> void </td>
<td class="memItemRight" data-valign="bottom"><strong>pixel</strong> (outlinerreal x, outlinerreal y)</td>
</tr>
<tr class="even memdesc:ad58b8ea3bb3de693b6bfd3e97a6fd3ba">
<td class="mdescLeft"> </td>
<td class="mdescRight">Draw a pixel.<br />
</td>
</tr>
<tr class="odd separator:ad58b8ea3bb3de693b6bfd3e97a6fd3ba">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:ae6d7398ad209735b829a3d3b6580f3eb">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="ae6d7398ad209735b829a3d3b6580f3eb"></span> void </td>
<td class="memItemRight" data-valign="bottom"><strong>text</strong> (outlinerreal x, outlinerreal y, const char *string)</td>
</tr>
<tr class="odd memdesc:ae6d7398ad209735b829a3d3b6580f3eb">
<td class="mdescLeft"> </td>
<td class="mdescRight">Write text to the image.<br />
</td>
</tr>
<tr class="even separator:ae6d7398ad209735b829a3d3b6580f3eb">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a7f0841b5c58a366f842ad4c62feafe44">
<td style="text-align: right;" class="memItemLeft" data-valign="top">outlinerreal </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_svg_creator.md#a7f0841b5c58a366f842ad4c62feafe44" class="el">getPixelXSize</a> (void)</td>
</tr>
<tr class="even separator:a7f0841b5c58a366f842ad4c62feafe44">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a36c891f38cea26cb371dfa183d9724c1">
<td style="text-align: right;" class="memItemLeft" data-valign="top">outlinerreal </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/class_svg_creator.md#a36c891f38cea26cb371dfa183d9724c1" class="el">getPixelYSize</a> (void)</td>
</tr>
<tr class="even separator:a36c891f38cea26cb371dfa183d9724c1">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a41d4849c73c8a2b794c0e67035c2efe3">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a41d4849c73c8a2b794c0e67035c2efe3"></span> bool </td>
<td class="memItemRight" data-valign="bottom"><strong>ok</strong> ()</td>
</tr>
<tr class="even memdesc:a41d4849c73c8a2b794c0e67035c2efe3">
<td class="mdescLeft"> </td>
<td class="mdescRight">Verify that image creation and file write was successful.<br />
</td>
</tr>
<tr class="odd separator:a41d4849c73c8a2b794c0e67035c2efe3">
<td colspan="2" class="memSeparator"> </td>
</tr>
</tbody>
</table>

<span id="details"></span>

## Detailed Description

This object can be used to output SVG images to a file. For a
description of what an SVG image format is, see
<https://www.w3schools.com/graphics/svg_intro.asp>.

## Member Function Documentation

<span id="a7f0841b5c58a366f842ad4c62feafe44"></span>

## <span class="permalink">[◆ ](#a7f0841b5c58a366f842ad4c62feafe44)</span>getPixelXSize()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">outlinerreal SvgCreator::getPixelXSize</td>
<td>(</td>
<td class="paramtype">void </td>
<td class="paramname"></td>
<td>)</td>
<td></td>
</tr>
</tbody>
</table>

Get the x-direction size of each individual pixel, expressed in the 3D
model coordinate system. For instance, if each pixel represents a 1
meter by 1 meter square, then return 1.0.

<span id="a36c891f38cea26cb371dfa183d9724c1"></span>

## <span class="permalink">[◆ ](#a36c891f38cea26cb371dfa183d9724c1)</span>getPixelYSize()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">outlinerreal SvgCreator::getPixelYSize</td>
<td>(</td>
<td class="paramtype">void </td>
<td class="paramname"></td>
<td>)</td>
<td></td>
</tr>
</tbody>
</table>

Get the y-direction size of each individual pixel, expressed in the 3D
model coordinate system. For instance, if each pixel represents a 1
meter by 1 meter square, then return 1.0.

------------------------------------------------------------------------

The documentation for this class was generated from the following file:

-   src/<a href="outlinersvg_8hh_source.md" class="el">outlinersvg.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [Doxygen](https://www.doxygen.org/index.md)
1.9.2</span>
