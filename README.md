# kicc100t_ros_driver
有两种方式更改kobuki的驱动
第一种：
1. cd ~
2. git clone https://github.com/rcwind/kicc100t_ros_driver.git
3. cd kicc100t_ros_driver
4. chmod a+x ./**/*.sh
5. make cmake		  
第二种：
拷贝src/kobuki_driver目录到catkin_ws/src/目录下使用catkin_make编译也可以，
然后手动拷贝编译出来的libkobuki.so到/opt/ros/indigo/lib目录下覆盖原来的，如有必要备份一下原来的库
