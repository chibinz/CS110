#!/bin/bash

RISC_FILE="heap.S"

minCommentRatio=2

commentCount=`grep -c "[#]" $RISC_FILE`
lineCount=`cat $RISC_FILE | sed '/^\s*$/d' | wc -l`
let ratio=lineCount/\(commentCount+1\)
echo  "linecount: $lineCount"
echo  "commentcount: $commentCount"

if [ "$ratio" -gt "$minCommentRatio" ]; then
        echo " Not enough comments in $RISC_FILE !"
        exit 2
fi

exit 0