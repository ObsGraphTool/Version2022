#!/bin/bash
# check the parameters
if [ $# != 2 ]
#	then echo "usage: $0 executable"
	then echo "usage: $0 executable number_of_philosophers"
	exit -1
fi
program=$1
no_phils=$2
#for no_phils in 2 3 4 5 6 7 8 9 10; do
echo "[1;36m Processing $no_phils philosophers [0m"
./genphil.bash $no_phils
$program alphaphil chop* phil*
rm -f alphaphil chop* phil*
#done
