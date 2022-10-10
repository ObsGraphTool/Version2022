#!/bin/bash
# check the parameters
if [ $# != 2 ]
	then echo "usage: $0 executable number_of_trains"
	exit -1
fi
program=$1
no_trains=$2
./gentrain.bash $no_trains
$program alphatrain traingate trainctrl traintrain*
rm -f alphatrain trainctrl traintrain*
