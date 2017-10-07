#!/bin/sh
# step:
# 1     save path
# 2     copy publish file to <dturls> repo
# 3     create release infomation file
# 4     <dturls> repo add & commit in local
# 5     update <dturls> repo on upgrade file server

here_path="$PWD"

usuage()
{
        echo "usuage: "
        echo "  ./publish.sh <source file path>"
        echo "example: "
        echo "  ./publish.sh /home/hl/ftu/arm/arm/src/out/arm/release/build/bin"
}


src=$1
if [ -z "$src" ]
then
        usuage
        exit
fi



#get src absolute path
cd $src
src_full_path=`pwd`
cd -

dturls_root=~/ftu/arm/dturls
dturls_bin=$dturls_root/bin

echo "publish [$src_full_path] -> [$dturls_root]"

cd $dturls_bin
./arm_dtu_publish.sh $src_full_path
./package_update.sh
./upgrade_repo_update.sh

