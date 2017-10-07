top := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
include $(top)/bin/makefile
