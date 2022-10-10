#!/bin/bash
no_resps=$1
# write alphabet file
if [ -f alphaprot ]
	then rm -f alphaprot
fi
echo "" >> alphaprot
i=1
while (($i <= $no_resps)); do
	echo "" >> alphaprot
	i=`expr $i + 1`
done
# write chopsticks files
if [ -f chop1 ]
	then rm -f chop1
fi
cpp -DI=1 -DJ=$no_resps -Dfree=11 -Doccupied=21 -P Chop.cpp chop1
i=2
while (($i <= $no_resps)); do
	if [ -f chop$i ]
		then rm -f chop$i
	fi
	cpp -DI=$i -DJ=`expr $i - 1` -Dfree=1$i -Doccupied=2$i \
	-P Chop.cpp chop$i
	i=`expr $i + 1`
done
# write philosophers files
i=1
while (($i <= $no_resps)); do
	if [ -f phil$i ]
		then rm -f phil$i
	fi
	cpp -DI=$i -DJ=`expr \( $i % $no_resps \) + 1` -Dunborn=1$i \
	-Dthinking=2$i -Dtakingchops=3$i -Deating=4$i -Dleavingchops=5$i \
	-Ddead=6$i -P Phil.cpp phil$i
	i=`expr $i + 1`
done

