#!/bin/bash
# check the parameters
if [ $# != 2 ]
	then echo "usage: $0 executable number_of_responders"
	exit -1
fi
program=$1
no_resps=$2
./genresponders.bash $no_resps
$program alphaprot broadcast_reactive  broadcast_stages initiator responder*
# rm -f alphaprot broadcast_reactive  broadcast_stages initiator responder*
