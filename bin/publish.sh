#!/bin/sh
# because make_rule.mk publish base on target.mk, so use this shell script 
# change target.mk before run "make publish_on_target"
# 
# step:
# 1     backup old target.mk
# 2     set target.mk for release version
# 3     make publish
# 4     restore target.mk

here_path="$PWD"


git_is_clean=`git status | grep -c "nothing to commit, working directory clean"`
#echo $git_is_clean
if [[ $git_is_clean -ne 1 ]] 
then
        echo "The reposite is NOT clean. please COMMIT before publish."
        exit 1
fi

echo "The reposite is clean. Push reposite & publish."
git push

mv target.mk target.mk.publish_bak
cp target.mk.publish target.mk

make publish_on_target

mv target.mk.publish_bak target.mk

