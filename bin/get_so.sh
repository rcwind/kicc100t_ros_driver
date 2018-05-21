[ -d tmp ] || exit
[ -d so ] || mkdir so
cd tmp
find "./" -name "*.so" -exec cp {} -t "../so" \;
