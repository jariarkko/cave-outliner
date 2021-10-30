#!/bin/sh

INPUT="$1"
OUTPUT="$2"

#
# Sanity checks
#

if [ "$INPUT" = "" -o "$OUTPUT" = "" ]
then
    echo "markdowncleanup: need two non-empty file names as inputs -- exit"
    exit 1
fi

if [ ! -f "$INPUT" ]
then
    echo "markdowncleanup: cannot read file $INPUT -- exit"
    exit 1
fi

#
# Process
#

cat $INPUT |
    sed 's/.img src="doxygen.svg" class="footer" width="104" height="31" alt="doxygen" ../Doxygen/g' |
    sed 's/.List of all//g' |
    sed 's/members..class_outliner_math-members.html.//g' |
    sed 's@<a href="class_outliner_triangle3_d.html" class="el">OutlinerTriangle3D</a>@[OutlinerTriangle3D](https://github.com/jariarkko/cave-outliner/blob/master/doc/class_outliner_triangle3_d.md)@g' |
    cat  > $OUTPUT
