#!/bin/bash

chmod ugo+x ~

ulimit -t 60

make 2> compile_errors.txt
sudo rm -rf testcases_inode 
sudo cp -r ../testcases_inode .
sudo chmod ugo+x .
cd testcases_inode
for i in `ls`;
do
	cp ../filecopy $i;
	sudo chmod ugo+x .
	cd $i
	sudo chmod ugo+x .
	sh $i.sh
	cd ..
done
cd ..



cnt=`find testcases_inode/ -name "*.diff" | wc -l`
for(( c=1;c<=$cnt;c++));
do
        task=`find testcases_inode/ -name "*.diff" | sort | head -n $c | tail -n 1`;
        task=`basename "$task"`;
        task=`echo ${task%%.diff}`;
        echo -n TASK:$task;

        x=`find testcases_inode -name $task".diff" -size 0`;
        y=`find testcases_inode -name $task".stat" -size 0`;
        z=`find testcases_inode -name $task".fsck" | xargs cat |  grep "image.img: clean," `;
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
                echo " "$((($x+$y)*4*1000/20)) | sed 's/..$/.&/';
        else
                echo " "$((($x+$y)*4*700/20)) | sed 's/..$/.&/';
        fi;
done

sudo rm -rf testcases_path
sudo cp -r ../testcases_path .
cd testcases_path
for i in `ls`;
do
        sudo cp ../filecopy $i;
        sudo chmod ugo+x .
        cd $i
        sudo chmod ugo+x .
        sh $i.sh
        cd ..
done
cd ..


cnt=`find testcases_path/ -name "*.diff" | wc -l`
for(( c=1;c<=$cnt;c++)); 
do
	task=`find testcases_path/ -name "*.diff" | sort | head -n $c | tail -n 1`;
	task=`basename "$task"`;
	task=`echo ${task%%.diff}`;
	echo -n TASK:$task;

	x=`find testcases_path -name $task".diff" -size 0`;
	y=`find testcases_path -name $task".stat" -size 0`;
	z=`find testcases_path -name $task".fsck" | xargs cat |  grep "image.img: clean," `;
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
		echo " "$((($x+$y)*5*1000/20)) | sed 's/..$/.&/';
	else
		echo " "$((($x+$y)*5*700/20)) | sed 's/..$/.&/';
	fi;
done


