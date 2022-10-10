#!/bin/bash
no_procs=$1
# write alphabet file
if [ -f alphaproc ]
	then rm -f alphaproc
fi
i=1
while (($i < $no_procs)); do
	echo "Sync($i)" >> alphaproc
	i=`expr $i + 1`
done
# write processes files
if [ -f proc1 ]
	then rm -f proc1
fi
sed -e "s/state\([0-9]\)/\11/g" outside |
sed -e "s/I/1/g" | sed -e "s/J/1/g" > proc1
if [ -f proc$no_procs ]
	then rm -f proc$no_procs
fi
sed -e "s/state\([0-9]\)/\1$no_procs/g" outside |
sed -e "s/I/$no_procs/g" | sed -e "s/J/`expr $no_procs - 1`/g" > proc$no_procs
i=2
while (($i < $no_procs)); do
	if [ -f proc$i ]
		then rm -f proc$i
	fi
	sed -e "s/state\([0-9]\)/\1$i/g" inside |
	sed -e "s/I/$i/g" | sed -e "s/PREV/`expr $i - 1`/g" > proc$i
	i=`expr $i + 1`
done

