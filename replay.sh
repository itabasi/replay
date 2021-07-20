#! /bin/sh


init=111206
end=111220

i=`expr $init`
j=`expr $end`
    echo start run: $i
    echo end run: $j



    while [ $i -le $j ]
do
	echo run number: $i

	analyzer="analyzer -l \"replay_coinc_new.C($i,-1)\""
	echo $analyzer
	eval $analyzer


	i=$((i+1));
	echo $i
    done
    	    

    
