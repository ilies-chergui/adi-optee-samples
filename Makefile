# Copyright 2023, Analog Devices, Inc.
#
# SPDX-License-Identifier: BSD-3-Clause

OPTEE_SAMPLE_APPS := $(subst /,,$(dir $(wildcard */Makefile)))

.PHONY: all
all:
	@for apps in $(OPTEE_SAMPLE_APPS); do \
		$(MAKE) -C $$apps \
			CROSS_COMPILE=$(CROSS_COMPILE) \
			TA_DEV_KIT_DIR=$(TA_DEV_KIT_DIR) \
			OPTEE_CLIENT_EXPORT=$(OPTEE_CLIENT_EXPORT) \
			O=$(O) || exit 1; \
	done

.PHONY: clean
clean:
	@for apps in $(OPTEE_SAMPLE_APPS); do \
		$(MAKE) -C $$apps \
			TA_DEV_KIT_DIR=$(TA_DEV_KIT_DIR) \
			OPTEE_CLIENT_EXPORT=$(OPTEE_CLIENT_EXPORT) \
			O=$(O) \
			clean || exit 1; \
	done
