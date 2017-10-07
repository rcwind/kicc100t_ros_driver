#!/bin/bash

# directory of shell script, not equal `pwd`
#CDIR=$(pwd)/${0}

# some global config for program running.
# It's source in 
#       ../out/arm/debug/make/set_env.sh
# or
#       ../out/arm/release/make/set_env.sh

export ARM_BOARD_IP=10.0.1.19
export ARM_USER=`whoami`
#export ARM_EXEC_NAME="nice -n 19 ./ftu -u"
#export ARM_EXEC_NAME='./ftu -u -f="dp_app_rxs_exec@"'
export ARM_EXEC_NAME='./ftu -u -f="@qport_server"'
#export ARM_EXEC_NAME="./ftu -u"

