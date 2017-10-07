#!/bin/sh
#if you want to debug this script, append -ex at first line.

pushd $(dirname $(readlink -f "$BASH_SOURCE")) > /dev/null
SCRIPT_DIR="$PWD"
popd > /dev/null

source ./set_env.sh
cmd="./logging_ut.expect $ARM_BOARD_IP $ARM_EXEC_PATH $ARM_EXEC_NAME"


cd $PWD
#tmux -2 attach-session -t arm
tmux kill-window -t :0 &>/dev/null || true
#tmux new-window -k -t :0 -n 'board' 'export $ARM_BOARD_IP'
tmux new-window -k -t :0 -n 'board' "$cmd"
#tmux new-window -k -t :0 -n 'board' 
tmux select-window -t :0

