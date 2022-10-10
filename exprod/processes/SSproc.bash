#!/bin/bash
# check the parameters
if [ $# != 2 ]
	then echo "usage: $0 executable number_of_processes"
	exit -1
fi
program=$1
no_procs=$2
./genproc.bash $no_procs
$program alphaproc proc*
rm -f alphaproc proc*
