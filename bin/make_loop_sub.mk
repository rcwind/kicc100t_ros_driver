#This file include by directory under <out>, It calls all sub dirs' makefile 
#targets.

# please read:
# ../out/makefile

.PHONY: all cmake install clean distclean tags ut stat mail 


exclude_dirs := include bin
dirs := $(shell find . -maxdepth 1 -type d)
dirs := $(basename $(patsubst ./%,%,$(dirs)))
dirs := $(filter-out $(exclude_dirs),$(dirs))

cmake_dirs:=$(addprefix cmake_,$(dirs))
install_dirs:=$(addprefix install_,$(dirs))
clean_dirs:=$(addprefix clean_,$(dirs))
distclean_dirs:=$(addprefix distclean_,$(dirs))
tags_dirs:=$(addprefix tags_,$(dirs))
ut_dirs:=$(addprefix ut_,$(dirs))
stat_dirs:=$(addprefix stat_,$(dirs))
mail_dirs:=$(addprefix mail_,$(dirs))

all: $(dirs)
cmake: $(cmake_dirs)
install: $(install_dirs)
clean: $(clean_dirs)
distclean: $(distclean_dirs)
tags: $(tags_dirs)
ut: $(ut_dirs)
stat: $(stat_dirs)
mail: $(mail_dirs)

.PHONY: force
$(dirs): force
	make -C $@
$(cmake_dirs): force
	make -C $(patsubst cmake_%,%,$@) cmake
$(install_dirs): force
	make -C $(patsubst install_%,%,$@) install
$(clean_dirs): force
	make -C $(patsubst clean_%,%,$@) clean
$(distclean_dirs): force
	make -C $(patsubst distclean_%,%,$@) distclean
$(tags_dirs): force
	make -C $(patsubst tags_%,%,$@) tags
$(ut_dirs): force
	make -C $(patsubst ut_%,%,$@) ut
$(stat_dirs): force
	make -C $(patsubst stat_%,%,$@) stat
$(mail_dirs): force
	make -C $(patsubst mail_%,%,$@) mail



