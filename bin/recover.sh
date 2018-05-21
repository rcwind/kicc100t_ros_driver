ROSVERSION=indigo
cd so
[ -d x86 ] || exit
sudo cp -rf x86/libecl_*.so /opt/ros/$ROSVERSION/lib/
