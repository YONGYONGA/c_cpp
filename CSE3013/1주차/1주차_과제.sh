#!/bin/bash
echo 'working directory:'
read dirname
if [ -n "$dirname" ] 
then
	echo >dummy
	rm dummy
else
	echo 'no input'
	exit 1

fi
if [ -d "$dirname" ]
then
	cd $dirname
	if [ $? -eq 0 ]
	then
		echo >dummy
		rm dummy
	else
		echo "please 접근권한"
		exit 1
	fi
else
	echo 'wrong directory'
	exit 1
fi	
for dir in `ls $dirname`
do
	chmod 777 $dir 
	newname=`echo $dir | tr "[a-z] [A-Z]" "[A-Z] [a-z]"`
	mv $dir $newname  
done
