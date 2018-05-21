ROSVERSION=indigo
cd so
[ -d x86 ] || mkdir x86
#backup
cp /opt/ros/$ROSVERSION/lib/libecl_*.so x86
#replace
sudo cp -rf libecl_*.so /opt/ros/$ROSVERSION/lib/
