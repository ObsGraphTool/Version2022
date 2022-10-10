#!/bin/bash
no_trains=$1
# write alphabet file
if [ -f alphatrain ]
	then rm -f alphatrain
fi
echo "lower" > alphatrain
echo "raise" >> alphatrain
i=1
while (($i <= $no_trains)); do
	echo "app($i)" >> alphatrain
	echo "exit($i)" >> alphatrain
	i=`expr $i + 1`
done
# write controller file
if [ -f trainctrl ]
	then rm -f trainctrl
fi
cp trainctrl.tmpl trainctrl
i=1
while (($i <= $no_trains)); do
	echo "(21, app($i) ,22)" >> trainctrl
	echo "(21, exit($i) ,23)" >> trainctrl
	i=`expr $i + 1`
done
# write trains files
i=1
while (($i <= $no_trains)); do
	if [ -f traintrain$i ]
		then rm -f traintrain$i
	fi
	cpp -Dstate1=10$i -Dstate2=20$i -Dstate3=30$i -DI=$i \
	-P train.cpp traintrain$i
	i=`expr $i + 1`
done

