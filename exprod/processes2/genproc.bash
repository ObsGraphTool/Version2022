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
cpp -Dstate1=11 -Dstate2=21 -Dstate3=31 -Dstate4=41 -Dstate5=51 -Dstate6=61 \
	-Dstate7=71 -DI=1 -DJ=1 -P outside.cpp proc1
if [ -f proc$no_procs ]
	then rm -f proc$no_procs
fi
cpp -Dstate1=1$no_procs -Dstate2=2$no_procs -Dstate3=3$no_procs \
	-Dstate4=4$no_procs -Dstate5=5$no_procs -Dstate6=6$no_procs \
	-Dstate7=7$no_procs -DI=$no_procs -DJ=`expr $no_procs - 1` -P \
	outside.cpp proc$no_procs
i=2
while (($i < $no_procs)); do
	if [ -f proc$i ]
		then rm -f proc$i
	fi
	cpp -DPREV=`expr $i - 1` -Dstate1=1$i -Dstate2=2$i \
	-Dstate3=3$i -Dstate4=4$i -Dstate5=5$i -Dstate6=6$i -Dstate7=7$i \
	-DI=$i -P inside.cpp proc$i
	i=`expr $i + 1`
done

