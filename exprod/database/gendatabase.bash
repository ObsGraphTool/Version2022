#!/bin/bash
no_db=$1
# write alphabet file
if [ -f alphadb ]
	then rm -f alphadb
fi
i=1
while (($i <= $no_db)); do
	echo "sendmess($i)" >> alphadb
	echo "recvack($i)" >> alphadb
	i=`expr $i + 1`
done
# write system file
if [ -f system ]
	then rm -f system
fi
cpp -Dactive=1 -Dpassive=2 -P System.cpp system
i=1
while (($i <= $no_db)); do
	echo "(2, sendmess($i) ,1)" >> system
	echo "(1, recvack($i) ,2)" >> system
	i=`expr $i + 1`
done
# write databases files
i=1
while (($i <= $no_db)); do
	if [ -f db$i ]
		then rm -f db$i
	fi
	cpp -DI=$i -Dinactive=1$i -Dwaiting=2$i \
		-Dprocessing=3$i -Dsent=4$i -Dacks=5$i -P DB.cpp db$i
	j=1
	while (($j <= $no_db)); do
	  	if (($i != $j))
			then echo "(1$i, sendmess($j) ,4$i)" >> db$i
			     echo "(5$i, recvack($j) ,1$i)" >> db$i
		fi
		j=`expr $j + 1`
	done
	i=`expr $i + 1`
done

