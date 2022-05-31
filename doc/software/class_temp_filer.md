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

[Public Member Functions](#pub-methods) | 
members](class_temp_filer-members.md)

TempFiler Class Reference

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
<tr class="even memitem:a05367122bed43ff5f72781ac43df1d2a">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a05367122bed43ff5f72781ac43df1d2a"></span>
 </td>
<td class="memItemRight" data-valign="bottom"><strong>TempFiler</strong>
()</td>
</tr>
<tr class="odd memdesc:a05367122bed43ff5f72781ac43df1d2a">
<td class="mdescLeft"> </td>
<td class="mdescRight">Construct a <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_temp_filer.md"
class="el">TempFiler</a> object.<br />
</td>
</tr>
<tr class="even separator:a05367122bed43ff5f72781ac43df1d2a">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a900b98186a99fa565a679d23ebba8f0d">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a900b98186a99fa565a679d23ebba8f0d"></span>
 </td>
<td class="memItemRight"
data-valign="bottom"><strong>~TempFiler</strong> ()</td>
</tr>
<tr class="even memdesc:a900b98186a99fa565a679d23ebba8f0d">
<td class="mdescLeft"> </td>
<td class="mdescRight">Destructor.<br />
</td>
</tr>
<tr class="odd separator:a900b98186a99fa565a679d23ebba8f0d">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a732b75420290b883018a253b6ec59731">
<td class="memItemLeft" style="text-align: right;"
data-valign="top">char * </td>
<td class="memItemRight" data-valign="bottom"><a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_temp_filer.md#a732b75420290b883018a253b6ec59731"
class="el">createTempFile</a> (const char *base, const char
*extension)</td>
</tr>
<tr class="odd separator:a732b75420290b883018a253b6ec59731">
<td colspan="2" class="memSeparator"> </td>
</tr>
</tbody>
</table>

## Member Function Documentation

<span id="a732b75420290b883018a253b6ec59731"></span>

## <span class="permalink">[◆ ](#a732b75420290b883018a253b6ec59731)</span>createTempFile()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">char * TempFiler::createTempFile</td>
<td>(</td>
<td class="paramtype">const char * </td>
<td class="paramname"><em>base</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">const char * </td>
<td class="paramname"><em>extension</em> </td>
</tr>
<tr class="odd">
<td></td>
<td>)</td>
<td></td>
<td></td>
</tr>
</tbody>
</table>

Create a new temporary file name. The return value is a newly allocated
string, and the responsibility for freeing it is on the caller. Base is
a descriptive part of the file name, extension is the file extension
(e.g., "svg"). All directory information etc. is filled in by the
createTempFile routine.

------------------------------------------------------------------------

The documentation for this class was generated from the following file:

-   src/<a href="outlinertemp_8hh_source.md" class="el">outlinertemp.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [<img src="doxygen.svg" class="footer" width="104" height="31"
alt="doxygen" />](https://www.doxygen.org/index.md) 1.9.4</span>
