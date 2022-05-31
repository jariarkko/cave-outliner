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

[Public Member Functions](#pub-methods) | [Public
Attributes](#pub-attribs) | 
members](class_processor_options-members.md)

ProcessorOptions Class Reference

`#include <outlinerprocessoroptions.hh>`

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
<tr class="even memitem:a32bb47a9aa2cb95cd9766cc23b004c10">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a32bb47a9aa2cb95cd9766cc23b004c10"></span>
 </td>
<td class="memItemRight"
data-valign="bottom"><strong>ProcessorOptions</strong> ()</td>
</tr>
<tr class="odd memdesc:a32bb47a9aa2cb95cd9766cc23b004c10">
<td class="mdescLeft"> </td>
<td class="mdescRight">Create the configuration object, with default
values.<br />
</td>
</tr>
<tr class="even separator:a32bb47a9aa2cb95cd9766cc23b004c10">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a5dfd5980cc58fd67d7a24015bbd0a1f1">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a5dfd5980cc58fd67d7a24015bbd0a1f1"></span>
 </td>
<td class="memItemRight"
data-valign="bottom"><strong>ProcessorOptions</strong> (const unsigned
int holeThresholdIn, const unsigned int lineHoleThresholdIn, const
unsigned int dustThresholdIn, const char *floorDepthMapIn, const char
*roofDepthMapIn, const bool floorStyleDiffIn, const bool tunnelSpineIn,
const bool labelsIn, const bool formAnalysisIn, const unsigned int
formCondenseIn, const bool dimensionsIn)</td>
</tr>
<tr class="even memdesc:a5dfd5980cc58fd67d7a24015bbd0a1f1">
<td class="mdescLeft"> </td>
<td class="mdescRight">Create the configuration object, with given
values.<br />
</td>
</tr>
<tr class="odd separator:a5dfd5980cc58fd67d7a24015bbd0a1f1">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a12c1562dffdc6b3a5f4f567ce1414324">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a12c1562dffdc6b3a5f4f567ce1414324"></span>
<a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_processor_options.md" class="el">ProcessorOptions</a>
&amp; </td>
<td class="memItemRight" data-valign="bottom"><strong>operator=</strong>
(const <a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_processor_options.md"
class="el">ProcessorOptions</a> &amp;input)</td>
</tr>
<tr class="odd memdesc:a12c1562dffdc6b3a5f4f567ce1414324">
<td class="mdescLeft"> </td>
<td class="mdescRight">Assign the configuration object, as a copy from
another object.<br />
</td>
</tr>
<tr class="even separator:a12c1562dffdc6b3a5f4f567ce1414324">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a82ce27792582011234132976f363f427">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a82ce27792582011234132976f363f427"></span>
 </td>
<td class="memItemRight"
data-valign="bottom"><strong>~ProcessorOptions</strong> ()</td>
</tr>
<tr class="even memdesc:a82ce27792582011234132976f363f427">
<td class="mdescLeft"> </td>
<td class="mdescRight">Release all resources for the configuration
object.<br />
</td>
</tr>
<tr class="odd separator:a82ce27792582011234132976f363f427">
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
<td colspan="2"><h2 id="public-attributes" class="groupheader"><span
id="pub-attribs"></span> Public Attributes</h2></td>
</tr>
<tr class="even memitem:aafd03ad9431287809675e24b49b4f75f">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="aafd03ad9431287809675e24b49b4f75f"></span>
unsigned int </td>
<td class="memItemRight"
data-valign="bottom"><strong>holeThreshold</strong></td>
</tr>
<tr class="odd memdesc:aafd03ad9431287809675e24b49b4f75f">
<td class="mdescLeft"> </td>
<td class="mdescRight">If the model has holes, e.g., missing faces for
various reasons, should the software try to fill them?<br />
</td>
</tr>
<tr class="even separator:aafd03ad9431287809675e24b49b4f75f">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a73052ae5ed8d1886b159b76ff6ffd4e2">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a73052ae5ed8d1886b159b76ff6ffd4e2"></span>
unsigned int </td>
<td class="memItemRight"
data-valign="bottom"><strong>lineHoleThreshold</strong></td>
</tr>
<tr class="even memdesc:a73052ae5ed8d1886b159b76ff6ffd4e2">
<td class="mdescLeft"> </td>
<td class="mdescRight">If the cross section lines have holes, e.g.,
missing faces for various reasons, should the software try to fill
them?<br />
</td>
</tr>
<tr class="odd separator:a73052ae5ed8d1886b159b76ff6ffd4e2">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a5c61aad37f8e5743037b9711d81c5114">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a5c61aad37f8e5743037b9711d81c5114"></span>
unsigned int </td>
<td class="memItemRight"
data-valign="bottom"><strong>dustThreshold</strong></td>
</tr>
<tr class="odd memdesc:a5c61aad37f8e5743037b9711d81c5114">
<td class="mdescLeft"> </td>
<td class="mdescRight">Can remove small isolated pieces of model?<br />
</td>
</tr>
<tr class="even separator:a5c61aad37f8e5743037b9711d81c5114">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a9f41876800fbcc56085feeaaea6f86ab">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a9f41876800fbcc56085feeaaea6f86ab"></span>
const char * </td>
<td class="memItemRight"
data-valign="bottom"><strong>floorDepthMap</strong></td>
</tr>
<tr class="even memdesc:a9f41876800fbcc56085feeaaea6f86ab">
<td class="mdescLeft"> </td>
<td class="mdescRight">Should we output the cave floor depth map to a
separate file?<br />
</td>
</tr>
<tr class="odd separator:a9f41876800fbcc56085feeaaea6f86ab">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a1aba013f22a735ad83f87400bb576fe7">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a1aba013f22a735ad83f87400bb576fe7"></span>
const char * </td>
<td class="memItemRight"
data-valign="bottom"><strong>roofDepthMap</strong></td>
</tr>
<tr class="odd memdesc:a1aba013f22a735ad83f87400bb576fe7">
<td class="mdescLeft"> </td>
<td class="mdescRight">Should we output the cave roof depth map to a
separate file?<br />
</td>
</tr>
<tr class="even separator:a1aba013f22a735ad83f87400bb576fe7">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:a45078a28942fd21aa7ce680c0b265f6a">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a45078a28942fd21aa7ce680c0b265f6a"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>floorStyleDiff</strong></td>
</tr>
<tr class="even memdesc:a45078a28942fd21aa7ce680c0b265f6a">
<td class="mdescLeft"> </td>
<td class="mdescRight">Floor style, either depth map (0) or differential
depth map (1).<br />
</td>
</tr>
<tr class="odd separator:a45078a28942fd21aa7ce680c0b265f6a">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:abea6c296eb28c109bd8f9f10985b75b0">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="abea6c296eb28c109bd8f9f10985b75b0"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>tunnelSpine</strong></td>
</tr>
<tr class="odd memdesc:abea6c296eb28c109bd8f9f10985b75b0">
<td class="mdescLeft"> </td>
<td class="mdescRight">Should we show tunnel 'spines', i.e., tunnel
midpoints?<br />
</td>
</tr>
<tr class="even separator:abea6c296eb28c109bd8f9f10985b75b0">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="odd memitem:aeab34c66c3ba608c6e9320fc59f306c0">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="aeab34c66c3ba608c6e9320fc59f306c0"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>labels</strong></td>
</tr>
<tr class="even memdesc:aeab34c66c3ba608c6e9320fc59f306c0">
<td class="mdescLeft"> </td>
<td class="mdescRight">Should we label cross sections in the
images?<br />
</td>
</tr>
<tr class="odd separator:aeab34c66c3ba608c6e9320fc59f306c0">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:aaf749520dec0cb651a9ce45d211bd557">
<td class="memItemLeft" style="text-align: right;"
data-valign="top">bool </td>
<td class="memItemRight" data-valign="bottom"><a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_processor_options.md#aaf749520dec0cb651a9ce45d211bd557"
class="el">formAnalysis</a></td>
</tr>
<tr class="odd separator:aaf749520dec0cb651a9ce45d211bd557">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:ad8c180ce58fae624a526d7f0b36c828c">
<td class="memItemLeft" style="text-align: right;"
data-valign="top">unsigned int </td>
<td class="memItemRight" data-valign="bottom"><a
href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_processor_options.md#ad8c180ce58fae624a526d7f0b36c828c"
class="el">formCondense</a></td>
</tr>
<tr class="odd separator:ad8c180ce58fae624a526d7f0b36c828c">
<td colspan="2" class="memSeparator"> </td>
</tr>
<tr class="even memitem:a9f6eff1a85bd2d052ff338aa7402d765">
<td class="memItemLeft" style="text-align: right;"
data-valign="top"><span id="a9f6eff1a85bd2d052ff338aa7402d765"></span>
bool </td>
<td class="memItemRight"
data-valign="bottom"><strong>dimensions</strong></td>
</tr>
<tr class="odd memdesc:a9f6eff1a85bd2d052ff338aa7402d765">
<td class="mdescLeft"> </td>
<td class="mdescRight">Should we show dimensions?<br />
</td>
</tr>
<tr class="even separator:a9f6eff1a85bd2d052ff338aa7402d765">
<td colspan="2" class="memSeparator"> </td>
</tr>
</tbody>
</table>

<span id="details"></span>

## Detailed Description

The
<a href="https://github.com/jariarkko/cave-outliner/blob/master/doc/software/class_processor_options.md" class="el">ProcessorOptions</a>
class represents an object of all configuration information related to
the cave-outliner program. It is initialized to default values, and the
main program can read options and other command line arguments to set
specific values in the configuration. Once the processing of the actual
function in cave-outliner begins, the configuration data is read from
this object to decide how to process.

## Member Data Documentation

<span id="aaf749520dec0cb651a9ce45d211bd557"></span>

## <span class="permalink">[◆ ](#aaf749520dec0cb651a9ce45d211bd557)</span>formAnalysis

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">bool ProcessorOptions::formAnalysis</td>
</tr>
</tbody>
</table>

Should cave outliner perform form analysis of cave entrances, stones,
drops, stalactites, stalagmites, etc?

<span id="ad8c180ce58fae624a526d7f0b36c828c"></span>

## <span class="permalink">[◆ ](#ad8c180ce58fae624a526d7f0b36c828c)</span>formCondense

<table class="memname">
<tbody>
<tr class="odd">
<td class="memname">unsigned int ProcessorOptions::formCondense</td>
</tr>
</tbody>
</table>

If forms are analysed, what's the compression factor with respect to the
picture size?

------------------------------------------------------------------------

The documentation for this class was generated from the following file:

-   src/<a href="outlinerprocessoroptions_8hh_source.md"
    class="el">outlinerprocessoroptions.hh</a>

------------------------------------------------------------------------

<span class="small">Generated
by [<img src="doxygen.svg" class="footer" width="104" height="31"
alt="doxygen" />](https://www.doxygen.org/index.md) 1.9.4</span>
