#!/bin/sh


dst=~/tmp/ftu_src.tgz

rm -f $dst
cd .. && tar --exclude='./out' --exclude='./lib' --exclude='./ref' --exclude='./to_del' --exclude='./tags' -czf $dst . && cd - 1>/dev/null
echo "ftu arm src backup from huanglin" | mail -s "ftu arm src backup" -a $dst husthl@gmail.com
echo "ftu arm src backup ok"
