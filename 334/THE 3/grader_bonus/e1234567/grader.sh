#!/bin/bash

chmod ugo+x ~

ulimit -t 60

make 2> compile_errors.txt

rm -rf testcases_indirect
cp -r ../testcases_indirect .




chmod ugo+x .
cd testcases_indirect
for i in `ls`;
do
        cp ../filecopy $i;
        chmod ugo+x .
        cd $i
        chmod ugo+x .
        sh $i.sh
        cd ..
done
cd ..



cnt=`find testcases_indirect/ -name "*.diff" | wc -l`
for(( c=1;c<=$cnt;c++));
do
        task=`find testcases_indirect/ -name "*.diff" | sort | head -n $c | tail -n 1`;
        task=`basename "$task"`;
        task=`echo ${task%%.diff}`;
        echo
        echo -n TASK:$task;

        x=`find testcases_indirect -name $task".diff" -size 0`;
        y=`find testcases_indirect -name $task".stat" -size 0`;
        z=`find testcases_indirect -name $task".fsck" | xargs cat |  grep "image.img: clean," `;
        if [ "$x" ];
        then
                x=1;
        else
                x=0;
        fi;
        if [ "$y" ];
        then
                y=1;
        else
                y=0;
        fi;

        if [ "$z" ];
        then
                echo " "$((($x)*5*1000/10)) | sed 's/..$/.&/';
        else
                echo " "$((($x)*5*700/10)) | sed 's/..$/.&/';
        fi;
	
	echo
done

