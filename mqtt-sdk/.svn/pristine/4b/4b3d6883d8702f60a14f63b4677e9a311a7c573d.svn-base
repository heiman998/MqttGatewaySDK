# Copyright (c) 2015 Qualcomm Atheros, Inc.
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.
# $ATH_LICENSE_TARGET_MK$
#
default: all

IMAGEDIR := $(SRC_IOE)/image
DEMO_OUT = $(IMAGEDIR)/iot_demo.out
DEMO_LIB = $(LIBDIR)/libiot_demo.a

COPTS := -g3 -Os -Wall -Wpointer-arith -Wundef -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -ffunction-sections
DEFINES := \
	-DAR6K \
	-DAR6002 \
	-TARG:unaligned_loads=1 \
	-DIOT_BUILD_FLAG \
	-DAR6002_REV7 \
	-include $(INTERNALDIR)/include/fwconfig_AR6006.h \
	-Wno-return-type \
	-DATHOS \
	-DP2P_ENABLED \
	-DSWAT_WMICONFIG_P2P \
	-DAPP_P2P_SUPPORT \
	-DATH_TARGET \
        -DSWAT_DNS_ENABLED \
        -DHTTP_ENABLED \
        -DDNS_ENABLED \
        -DBRIDGE_ENABLED \
        -DCONFIG_HOSTLESS \
        -DSSL_ENABLED \
        -DSWAT_CRYPTO \
        -DSWAT_BENCH_RAW \
        -DENABLE_JSON \
        -DSSDP_ENABLED

ifeq ($(AR6002_REV7_VER), 5) # {
DEFINES += -DNUM_OF_VDEVS=2 \
		-DAR6002_REV75
endif # }

ifeq ($(AR6002_REV7_VER), 6) # {
DEFINES += -DNUM_OF_VDEVS=2 \
		-DAR6002_REV76
endif # }

ifeq ($(FPGA_FLAG),1) # {
DEFINES += -DFPGA
endif # }

INCLUDES := \
	-I$(FW)/include/AR6002/hw/include \
	-I. \
	-I$(FW)/include/qcom \
	-I$(FW)/include \
	-I$(FW)/include/AR6002/ \
	-I$(FW)/include/AR6K/ \
	-I$(THREADXDIR)/include/ \
	-I$(INTERNALDIR)/include \
	-I$(INTERNALDIR)/include/os \
	-I$(INTERNALDIR)/include/whal \
        -I$(FW)/demo/ezxml \
		-I$(FW)/include/qca


CFLAGS = $(COPTS) $(DEFINES) $(INCLUDES)

CC = xt-xcc
AR = xt-ar
RM = rm -f

CSRCS := $(wildcard *.c)
OBJS := $(CSRCS:%.c=%.o)
DEPS := $(CSRCS:%.c=%.d)
LIBS := $(LIBDIR)/libiot_demo.a \
	$(LIBDIR)/libadc.a \
	$(LIBDIR)/libramcust.a \
	$(LIBDIR)/libserport.a \
	$(LIBDIR)/libuartserp.a \
	$(LIBDIR)/libconsole.a\
	$(LIBDIR)/libpatches_iot.a\
	$(LIBDIR)/libpatches.a \
	$(LIBDIR)/libhost_dset_api.a \
        $(LIBDIR)/libhttppatches.a \
        $(LIBDIR)/libsntppatches.a \
        $(LIBDIR)/libdnspatches.a \
        $(LIBDIR)/libotapatches.a \
        $(LIBDIR)/libstrrclpatches.a \
        $(LIBDIR)/libipv6patches.a \
        $(LIBDIR)/libsslpatches.a \
        $(LIBDIR)/libqcomcrypto.a \
        $(LIBDIR)/libcryptopatches.a \
        $(LIBDIR)/libcrypto_mgmt.a \
        $(LIBDIR)/libcrypto_aes.a \
        $(LIBDIR)/libcrypto_chacha20_poly1305.a \
        $(LIBDIR)/libcrypto_des.a \
        $(LIBDIR)/libcrypto_srp.a \
        $(LIBDIR)/libcrypto_hmacsha384.a \
        $(LIBDIR)/libcrypto_hmacsha256.a \
        $(LIBDIR)/libcrypto_hmacsha1.a \
        $(LIBDIR)/libwhalpatches.a \
        $(LIBDIR)/lib_ezxml.a \
        $(LIBDIR)/libssdp.a \
	$(LIBDIR)/libcustpatches.a \
	$(LIBDIR)/libstackghost_stub.a \
        $(LIBDIR)/ltlib.a \
		$(LIBDIR)/libltlink.a \
		$(LIBDIR)/libsniffer.a

LDFLAGS = -g -nostdlib -Wl,-EL -Wl,--gc-sections \
	-L$(IMAGEDIR)  \
	-L.\
	-Tltm3210.ld \
	-u app_start \
	-Wl,-static \
	-Wl,--start-group \
	$(LIBS)	\
	-Wl,--end-group\
	$(IMAGEDIR)/rom.addrs.ld \
	-lgcc


ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),mkdbg)
ifdef DEPS
sinclude $(DEPS)
endif
endif
endif

$(DEMO_LIB): $(OBJS)
	@mkdir -p $(LIBDIR)
	$(AR) ru $@ $^

$(DEMO_OUT): $(LIBS) $(LDS)
	@mkdir -p $(IMAGEDIR)
	$(CC) $(LDFLAGS) -o $@

all: $(OBJS) $(DEMO_LIB) $(DEMO_OUT)

clean:
	$(foreach d, $(SUBDIRS), make -C $(d) clean;)
	$(RM) *.o
	$(RM) $(DEMO_LIB)
	$(RM) $(DEMO_OUT)


mkdbg:
	@echo pwd=`pwd`
	@echo TARGET = $(TARGET)
	@echo AR6002_REV = $(AR6002_REV)
	@echo ROMDIR = $(ROMDIR)
	@echo DEMO_OUT = $(DEMO_OUT)
	@echo DEMO_LIB = $(DEMO_LIB)
	@echo SUBDIRS = $(SUBDIRS)
	@echo CSRCS = $(CSRCS)
	@echo OBJS = $(OBJS)
	@echo DEPS = $(DEPS)
	@echo LIBS = $(LIBS)
	@echo CC = $(CC) at `which $(CC)`
	@echo CFLAGS = $(CFLAGS)
	@echo LDFLAGS = $(LDFLAGS)
