#!/bin/bash


init=111200
end=111220

i=`expr $init`
j=`expr $end`
    echo start run: $i
    echo end run: $j

    analyzer="analyzer"
    echo $analyzer
    eval ${analyzer}
