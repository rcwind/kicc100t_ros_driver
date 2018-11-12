.PHONY: all cmake cmake_do install clean distclean tags ut stat prepare mail publish_on_target uninstall_makefile install_makefile cp

# please read:
# ../out/x86/debug/makefile

#SRCS:=$(shell find $(src_root)/kobuki_driver/src  -type f \( -name "*.c" -o -name "*.cpp" -o -name "*.h" \))
#INCS:=$(shell find $(src_root)/kobuki_driver/include  -type f \( -name "*.c" -o -name "*.cpp" -o -name "*.h" \))
#UNITS:=$(shell find $(src_root)/utest  -type f \( -name "*.c" -o -name "*.cpp" -o -name "*.h" \))
#BINS:=$(shell find $(src_root)/bin  -type f \( -name "*" \))

# The execute path
exec_root:=$(build_root)/bin/

all:
#all: $(build_root)
	#@cd $(build_root) && make
	
cp:
	sudo cp -f $(build_root)/devel/lib/libkobuki.so /opt/ros/indigo/lib/libkobuki.so
# below code cause cmake always invoked for $(build_root) is a variable instead 
# of a real path.
# $(build_root): cmake


debug:
	@echo "SRCS=" $(SRCS)
	@echo "INCS=" $(INCS)
	@echo "UNITS=" $(UNITS)
	@echo "BINS=" $(BINS)
	@echo "top=" $(top)
	@echo "build_root=" $(build_root)
	@echo "src_root=" $(src_root)
	@echo "bin_root=" $(bin_root)


$(build_root): $(make_root)/CMakeLists_config.txt $(src_root)/kobuki_driver/CMakeLists.txt
	mkdir -p $(build_root)
cmake_do: $(build_root) install_makefile
	cd $(build_root) && cmake $(src_root)/kobuki_driver
	cd $(build_root) && make 
cmake: cmake_do install 

$(src_root)/kobuki_driver/CMakeLists.txt:
	ifneq ($(bin_root)/../cmake/CMakeLists.txt.templet, $(src_root)/kobuki_driver/CMakeLists.txt)
		cp $(bin_root)/../cmake/CMakeLists.txt.templet $(src_root)/kobuki_driver/CMakeLists.txt
	endif

# $(install_root):
	# mkdir -p $(install_root)
# install: $(install_root)
	# cd $(build_root) && make 

clean:
	cd $(build_root) && make clean -s

install_makefile:
	cd $(bin_root) && ./install_makefile.sh $(src_root)

uninstall_makefile:
	cd $(bin_root) && ./uninstall_makefile.sh $(src_root)

distclean:
	cd $(make_root) && cd .. && sudo rm -rf ./build/
	cd $(make_root) && rm -rf set_env.sh
	#cd $(bin_root) && rm -rf publish_info.txt && ./uninstall_makefile.sh $(src_root)
	cd $(bin_root) && ./uninstall_makefile.sh $(src_root) 


tags: $(src_root)/tags
$(src_root)/tags: $(SRCS) $(INCS) $(UNITS) $(BINS)
	@cd $(src_root) && ctags -R --extra=+q kobuki_driver/ 


$(make_root)/set_env.sh: $(make_root)/set_env.sh.templet
	cp -f $@.templet $@


$(bin_root)/set_env.sh: $(bin_root)/set_env.sh.templet
	cp -f $@.templet $@

$(bin_root)/target.mk : $(bin_root)/target.mk.templet
	cp -f $@.templet $@

#ut: all $(make_root)/set_env.sh $(bin_root)/set_env.sh
	# ref: http://stackoverflow.com/questions/11076350/how-do-you-export-a-variable-through-shell-script
	# below code is error, export do NOT work in child shell process.
	#@cd $(bin_root) && ./set_env.sh
	#@cp -f $(bin_root)/set_env.sh $(make_root)/set_env.sh
	#@cd $(bin_root) && ./set_env.sh
	#@cd $(make_root) && ./new_window_ut.sh

prepare:
	sudo apt-get install cmake
	sudo apt-get install gcc-c++
	sudo apt-get install gitstats
