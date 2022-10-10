#!/bin/bash
no_phils=$1
# write alphabet file
if [ -f alphaphil ]
	then rm -f alphaphil
fi
i=1
while (($i <= $no_phils)); do
	echo "take($i)" >> alphaphil
	echo "leave($i)" >> alphaphil
	i=`expr $i + 1`
done
# write chopsticks files
if [ -f chop1 ]
	then rm -f chop1
fi
cpp -DI=1 -DJ=$no_phils -Dfree=11 -Doccupied=21 -P Chop.cpp chop1
i=2
while (($i <= $no_phils)); do
	if [ -f chop$i ]
		then rm -f chop$i
	fi
	cpp -DI=$i -DJ=`expr $i - 1` -Dfree=1$i -Doccupied=2$i \
	-P Chop.cpp chop$i
	i=`expr $i + 1`
done
# write philosophers files
i=1
while (($i <= $no_phils)); do
	if [ -f phil$i ]
		then rm -f phil$i
	fi
	cpp -DI=$i -Dthinking=1$i -Deating=2$i -P Phil.cpp phil$i
	i=`expr $i + 1`
done

