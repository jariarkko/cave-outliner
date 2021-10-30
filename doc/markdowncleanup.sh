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
    cat  > $OUTPUT
