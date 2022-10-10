#!/bin/bash
# check the parameters
if [ $# != 2 ]
#	then echo "usage: $0 executable"
	then echo "usage: $0 executable number_of_databases"
	exit -1
fi
program=$1
no_db=$2
echo "[1;36m Processing $no_db databases [0m"
./gendatabase.bash $no_db
$program alphadb system db*
rm -f alphadb system
i=1
while (($i <= $no_db)); do
  rm db$i
  i=`expr $i + 1`
done
