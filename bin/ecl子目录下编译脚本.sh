#!/bin/sh
# $PWD $(pwd) $cur_dir $CUR_DIR都是正确写法，区别?
for file in $(ls $cur_dir)
do
	[ -d $file ] && cd "$PWD/$file" || continue
	[ -f CMakeLists.txt ] || continue
	#echo $PWD
	[ -d build ] || mkdir build
	cd build
	#采用外部编译，所有中间文件放在build目录下
	cmake .. && make
	cd ../..
	#echo $PWD
done

