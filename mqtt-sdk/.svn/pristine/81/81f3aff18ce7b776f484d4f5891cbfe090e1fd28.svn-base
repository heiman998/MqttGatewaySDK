# Copyright (c) 2015 Qualcomm Atheros, Inc.
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.
# $ATH_LICENSE_TARGET_MK$
#
default: all

DEMO_LIB = $(LIBDIR)/libcustpatches.a

COPTS := -g3 -Os -Wall -Wpointer-arith -Wundef -Werror -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -ffunction-sections
DEFINES := \
	-DAR6K \
	-DAR6002 \
	-TARG:unaligned_loads=1 \
	-DAR6002_REV76 \
	-DIOT_BUILD_FLAG \
	-DAR6002_REV7 \
	-include $(INTERNALDIR)/include/fwconfig_AR6006.h \
	-Wno-return-type \
	-DATHOS \
	-DATH_TARGET \
        -DSWAT_WMICONFIG_MISC_EXT \
        -DSWAT_SSL \
        -DSWAT_I2C \
        -DSWAT_I2S \
        -DSWAT_WMICONFIG_SNTP \
        -DSWAT_OTA \
        -DENABLE_HTTP_CLIENT  \
        -DENABLE_HTTP_SERVER  \
        -DSWAT_DNS_ENABLED \
        -DHTTP_ENABLED \
        -DDNS_ENABLED \
        -DBRIDGE_ENABLED \
        -DCONFIG_HOSTLESS \
        -DSSL_ENABLED \
        -DSWAT_BENCH_RAW


INCLUDES := \
        -I$(FW)/include/AR6002/hw/include \
        -I. \
        -I../../../SRC-ROM/misc/dset \
        -I../../../SRC-ROM/misc/gpio \
        -I../../../SRC-ROM/wlan/txrx/include \
        -I../../../SRC-ROM/whal/include \
        -I../../../SRC-ROM/wlan/include \
        -I../../../SRC-ROM/wlan/chop \
        -I../../../SRC-ROM/wlan/ap \
        -I../../../SRC-ROM/wlan/wlan_dev \
        -I../../../SRC-ROM/wlan/framegen \
        -I../../../SRC-ROM/wlan/crypto \
        -I../../../SRC-ROM/wlan/com \
        -I../../../SRC-ROM/wlan/wlan_wmi \
        -I../../../SRC-ROM/wlan/wreg \
        -I../../../SRC-ROM/wlan/mgmt \
        -I../../../SRC-ROM/wlan/p2p_fw \
        -I../../../SRC-ROM/wlan/strrcl \
        -I../../../SRC-ROM/wlan/txrx/include \
        -I../../../SRC-ROM/wlan/rfkill/include \
        -I../../../SRC-ROM/wlan/discovery \
        -I../../../SRC-ROM/wlan/wps/include \
        -I../../../SRC-ROM/security/include \
        -I$(FW)/include/qcom \
        -I$(FW)/include \
        -I$(FW)/include/AR6002/ \
        -I$(FW)/include/AR6K/ \
        -I$(THREADXDIR)/include/ \
        -I$(INTERNALDIR)/include \
        -I$(INTERNALDIR)/include/os \
        -I$(INTERNALDIR)/include/whal\
        -I../../../SRC-ROM/whal/AR6003/include \
        -I../../../SRC-ROM/whal/AR6003/reset \
        -I../../../SRC-ROM/whal/AR6003/eeprom \
        -I../../../SRC-ROM/whal/AR6003/xmit \
        -I../../../SRC-ROM/include \
        -I../../../SRC-ROM/include/os \
        -I../../../SRC-ROM/common/include/AR6002/hw7.6 \
        -I../../../SRC-ROM/common/include\
        -I../../../SRC-ROM/common/include/AR6002\
        -I../../../SRC-ROM/wlan/power	


CFLAGS = $(COPTS) $(DEFINES) $(INCLUDES)

CC = xt-xcc
AR = xt-ar
RM = rm -f

CSRCS := $(wildcard *.c) 
OBJS := $(CSRCS:%.c=%.o)
DEPS := $(CSRCS:%.c=%.d)
$(DEMO_LIB): $(OBJS)
	@mkdir -p $(LIBDIR)
	$(AR) ru $@ $^


all: $(OBJS) $(DEMO_LIB) 

clean:
	$(foreach d, $(SUBDIRS), make -C $(d) clean;)
	$(RM) *.o
	$(RM) $(DEMO_LIB)

