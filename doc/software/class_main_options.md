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
members](class_main_options-members.md)

MainOptions Class Reference

`#include <mainoptions.hh>`

<table class="memberdecls">
<colgroup>
<col style="width: 50%" />
<col style="width: 50%" />
</colgroup>
<tbody>
<tr class="odd heading">
<td colspan="2"><h2 id="public-member-functions" class="groupheader"><span id="pub-methods"></span> Public Member Functions</h2></td>
</tr>
<tr class="even memitem:af3c0f3afe360cabab115c2a239555b9c">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="af3c0f3afe360cabab115c2a239555b9c"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>MainOptions</strong> (<a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_main_config.md" class="el">MainConfig</a> &amp;configIn)</td>
</tr>
<tr class="odd memdesc:af3c0f3afe360cabab115c2a239555b9c">
<td class="mdescLeft"> </td>
<td class="mdescRight">Create an options processor.<br />
</td>
</tr>
<tr class="even separator:af3c0f3afe360cabab115c2a239555b9c">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a012c2b3ec96c28f973d4a6db4f77b54c">
<td style="text-align: right;" class="memItemLeft" data-valign="top"><span id="a012c2b3ec96c28f973d4a6db4f77b54c"></span>  </td>
<td class="memItemRight" data-valign="bottom"><strong>~MainOptions</strong> ()</td>
</tr>
<tr class="even memdesc:a012c2b3ec96c28f973d4a6db4f77b54c">
<td class="mdescLeft"> </td>
<td class="mdescRight">Release all resources associated with the options processor.<br />
</td>
</tr>
<tr class="odd separator:a012c2b3ec96c28f973d4a6db4f77b54c">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a19b9958f21a641e84d004cc0a6f495fb">
<td style="text-align: right;" class="memItemLeft" data-valign="top">bool </td>
<td class="memItemRight" data-valign="bottom"><a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_main_options.md#a19b9958f21a641e84d004cc0a6f495fb" class="el">processCommandLine</a> (int argc, char **argv)</td>
</tr>
<tr class="odd separator:a19b9958f21a641e84d004cc0a6f495fb">
<td colspan="2" class="memSeparator"> </td>
</tr>
</tbody>
</table>

<span id="details"></span>

## Detailed Description

The <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_main_options.md" class="el">MainOptions</a> class
represents the functionalit to process all end-user configuration
information, from the command line. It updates the
<a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_main_config.md" class="el">MainConfig</a> object as it
processes.

## Member Function Documentation

<span id="a19b9958f21a641e84d004cc0a6f495fb"></span>

## <span class="permalink">[◆ ](#a19b9958f21a641e84d004cc0a6f495fb)</span>processCommandLine()

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">bool MainOptions::processCommandLine</td>
<td>(</td>
<td class="paramtype">int </td>
<td class="paramname"><em>argc</em>,</td>
</tr>
<tr class="even">
<td class="paramkey"></td>
<td></td>
<td class="paramtype">char ** </td>
<td class="paramname"><em>argv</em> </td>
</tr>
<tr class="odd">
<td></td>
<td>)</td>
<td></td>
<td></td>
</tr>
</tbody>
</table>

Process options and arguments. Return 1 if successful, 0 upon error.

------------------------------------------------------------------------

The documentation for this class was generated from the following file:

-   src/<a href="mainoptions_8hh_source.md" class="el">mainoptions.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [Doxygen](https://www.doxygen.org/index.md)
1.9.2</span>
