[ -d tmp ] || mkdir tmp
for file in $(ls $CUR_DIR)
do 
	if [ "${file##*.}" = "deb" ]
	then
		dpkg-deb --fsys-tarfile $file | tar xvf - -C tmp
	fi
done
