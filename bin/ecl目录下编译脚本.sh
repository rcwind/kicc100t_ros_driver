#!/bin/sh
# ecl_core ecl_lite ecl_manipulation ecl_navigation 全部放在ecl目录下，运行脚本即可
for file in $(ls $CUR_DIR)
do
	[ -d $file ] && cd "$PWD/$file" || continue
	#echo $PWD
	./compile.sh
	cd ..
	#echo $PWD
done

