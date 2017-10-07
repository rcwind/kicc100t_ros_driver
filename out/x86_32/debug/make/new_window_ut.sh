#!/bin/sh

pushd $(dirname $(readlink -f "$BASH_SOURCE")) > /dev/null
SCRIPT_DIR="$PWD"
popd > /dev/null

cd $PWD
bin_path=$PWD/../build/bin

source ./set_env.sh

#tmux -2 attach-session -t arm
tmux kill-window -t :0 
tmux new-window  -k -t arm:0 -n 'board'
tmux select-window -t arm:0
tmux send-keys -t arm:0 "cd $bin_path" Enter
#tmux send-keys -t arm:0 "sudo rm /tmp/dtu.pid; sudo ./ftu -u" Enter
tmux send-keys -t arm:0 "sudo rm -f /tmp/dtu.pid; sudo $ARM_EXEC_NAME" Enter

