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
members](class_svg_reader-members.md)

SvgReader Class Reference

`#include <outlinersvgreader.hh>`

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
<tr class="even memitem:a6963b7372a4aa4d291037e43a778e9aa">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a6963b7372a4aa4d291037e43a778e9aa"></span>
 </td>
<td class="memItemRight" data-valign="bottom"><strong>SvgReader</strong>
(const char *fileNameIn)</td>
</tr>
<tr class="odd memdesc:a6963b7372a4aa4d291037e43a778e9aa">
<td class="mdescLeft"> </td>
<td class="mdescRight">Create an SVG file with given file name and
size.<br />
</td>
</tr>
<tr class="even separator:a6963b7372a4aa4d291037e43a778e9aa">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:aed7dca5eb2e450e5500adfea2d26de71">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="aed7dca5eb2e450e5500adfea2d26de71"></span>
 </td>
<td class="memItemRight"
data-valign="bottom"><strong>~SvgReader</strong> ()</td>
</tr>
<tr class="even memdesc:aed7dca5eb2e450e5500adfea2d26de71">
<td class="mdescLeft"> </td>
<td class="mdescRight">Release all resources associated with the SVG
reader object. And close the file, if still open.<br />
</td>
</tr>
<tr class="odd separator:aed7dca5eb2e450e5500adfea2d26de71">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a7c65fa6d864c20227090343ac935c803">
<td class="memItemLeft" style="text-align: right;"
data-valign="top">bool </td>
<td class="memItemRight" data-valign="bottom"><a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_svg_reader.md#a7c65fa6d864c20227090343ac935c803"
class="el">getSize</a> (unsigned int &amp;xSize, unsigned int
&amp;ySize)</td>
</tr>
<tr class="odd separator:a7c65fa6d864c20227090343ac935c803">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:adb855015a634ba3c142a9e1d7c1d276c">
<td class="memItemLeft" style="text-align: right;"
data-valign="top">bool </td>
<td class="memItemRight" data-valign="bottom"><a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_svg_reader.md#adb855015a634ba3c142a9e1d7c1d276c"
class="el">getStatement</a> (enum SvgStatement &amp;statementType,
unsigned int &amp;lineno, const char *&amp;statement)</td>
</tr>
<tr class="odd separator:adb855015a634ba3c142a9e1d7c1d276c">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a52eaa05ba9ad598e624b91b9d9644302">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a52eaa05ba9ad598e624b91b9d9644302"></span>
unsigned int </td>
<td class="memItemRight" data-valign="bottom"><strong>getLine</strong>
() const</td>
</tr>
<tr class="odd memdesc:a52eaa05ba9ad598e624b91b9d9644302">
<td class="mdescLeft"> </td>
<td class="mdescRight">Get the current line we are on.<br />
</td>
</tr>
<tr class="even separator:a52eaa05ba9ad598e624b91b9d9644302">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:abee5d3fc5242dabc8d95c0ea1c16e255">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="abee5d3fc5242dabc8d95c0ea1c16e255"></span>
const char * </td>
<td class="memItemRight"
data-valign="bottom"><strong>getFileName</strong> () const</td>
</tr>
<tr class="even memdesc:abee5d3fc5242dabc8d95c0ea1c16e255">
<td class="mdescLeft"> </td>
<td class="mdescRight">Get the file name we are reading.<br />
</td>
</tr>
<tr class="odd separator:abee5d3fc5242dabc8d95c0ea1c16e255">
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
<tr class="even memitem:aa278c910c5f2ed0e24522355ef0a76e8">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="aa278c910c5f2ed0e24522355ef0a76e8"></span>
static bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>iterateStatementOptionsInit</strong> (const
char *statement, const char *fileName, const unsigned int lineNo, struct
<a href="struct_svg_reader_option_parser.md"
class="el">SvgReaderOptionParser</a> &amp;iter)</td>
</tr>
<tr class="odd memdesc:aa278c910c5f2ed0e24522355ef0a76e8">
<td class="mdescLeft"> </td>
<td class="mdescRight">Initialize the parsing of a statement and its
options.<br />
</td>
</tr>
<tr class="even separator:aa278c910c5f2ed0e24522355ef0a76e8">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:ac1d42eea821bdb906e8fdc6ea11622a1">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="ac1d42eea821bdb906e8fdc6ea11622a1"></span>
static bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>iterateStatementOptionsNext</strong>
(struct <a href="struct_svg_reader_option_parser.md"
class="el">SvgReaderOptionParser</a> &amp;iter, bool &amp;end, const
char *&amp;optionName, unsigned int &amp;optionNameLength, const char
*&amp;optionValue, unsigned int &amp;optionValueLength)</td>
</tr>
<tr class="even memdesc:ac1d42eea821bdb906e8fdc6ea11622a1">
<td class="mdescLeft"> </td>
<td class="mdescRight">Move to the next iterated option.<br />
</td>
</tr>
<tr class="odd separator:ac1d42eea821bdb906e8fdc6ea11622a1">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:aec907bb964fe5c90c6538b1c247fc5a0">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="aec907bb964fe5c90c6538b1c247fc5a0"></span>
static void </td>
<td class="memItemRight" data-valign="bottom"><strong>test</strong>
(void)</td>
</tr>
<tr class="odd memdesc:aec907bb964fe5c90c6538b1c247fc5a0">
<td class="mdescLeft"> </td>
<td class="mdescRight">Unit tests.<br />
</td>
</tr>
<tr class="even separator:aec907bb964fe5c90c6538b1c247fc5a0">
<td colspan="2" class="memSeparator"> </td>
</tr>
</tbody>
</table>

<span id="details"></span>

## Detailed Description

This object can be used to read SVG images from a file.

## Member Function Documentation

<span id="a7c65fa6d864c20227090343ac935c803"></span>

## <span class="permalink">[◆ ](#a7c65fa6d864c20227090343ac935c803)</span>getSize()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">bool SvgReader::getSize</td>
<td>(</td>
<td class="paramtype">unsigned int &amp; </td>
<td class="paramname"><em>xSize</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">unsigned int &amp; </td>
<td class="paramname"><em>ySize</em> </td>
</tr>
<tr class="odd">
<td></td>
<td>)</td>
<td></td>
<td></td>
</tr>
</tbody>
</table>

Find out the size of the SVG image. Return 1 upon success and set output
parameters, otherwise return 0.

<span id="adb855015a634ba3c142a9e1d7c1d276c"></span>

## <span class="permalink">[◆ ](#adb855015a634ba3c142a9e1d7c1d276c)</span>getStatement()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">bool SvgReader::getStatement</td>
<td>(</td>
<td class="paramtype">enum SvgStatement &amp; </td>
<td class="paramname"><em>statementType</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">unsigned int &amp; </td>
<td class="paramname"><em>lineno</em>,</td>
</tr>
<tr class="odd">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const char *&amp; </td>
<td class="paramname"><em>statement</em> </td>
</tr>
<tr class="even">
<td></td>
<td>)</td>
<td></td>
<td></td>
</tr>
</tbody>
</table>

Read the SVG file statement-by-statement. Return 1 upon success, and set
the output parameters, otherwise return 0. End of file is reported as
output parameter statementType set to svgstatement\_eof.

------------------------------------------------------------------------

The documentation for this class was generated from the following file:

-   src/<a href="outlinersvgreader_8hh_source.md"
    class="el">outlinersvgreader.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [<img src="doxygen.svg" class="footer" width="104" height="31"
alt="doxygen" />](https://www.doxygen.org/index.md) 1.9.4</span>
