#This file execute default build target's makefile commands.


top := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
# default build target
include $(top)/target.mk
include $(top)/../out/$(target)/makefile
# ../out/arm/debug/makefile
