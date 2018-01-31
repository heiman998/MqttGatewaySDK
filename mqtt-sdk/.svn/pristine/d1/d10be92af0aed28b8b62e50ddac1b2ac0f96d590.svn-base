#!/bin/bash

# Create specific software variants based on input environment variables including:
#     OP_FLAVOR=single    --> 1 device  + !p2p + SCC mode
#               singlep2p --> 1 device  +  p2p + SCC mode
#               p2p+sta_mcc       --> 2 devices +  p2p + sta + MCC mode
#               p2p+sta_scc       --> 2 devices +  p2p + sta + SCC mode
#               ap+sta_mcc       --> 2 devices +  ap + sta + MCC mode
#               ap+sta_scc       --> 2 devices +  ap + sta + SCC mode
#     OP_BAND=singleband  --> 2.4GHz only
#             dualband    --> 2.4GHz + 5GHz
#     APPS_OUTFILE: sdk_shell.out, iot_demo.out, ecc_demo.out, uart_at.out, <custom.out>
#
# Output from this script includes:
#     raw_flash* image, which is programmed into flash (see gdb.sdk_flash)
#     flash_* image, which may be executed in order to program flash
#     ota_* image, which may be used with Over The Air upgrades
#

export AR6002_REV=7
export AR6002_REV7_VER=${AR6002_REV7_VER:-6}
export FLASH_FLAVOR=${FLASH_FLAVOR:-"_IOT"}
if [[ "$FPGA_FLAG" -eq 1 ]]; then
		export FPGAorHW=${FPGAorHW:-"fpga"}
else
        export FPGAorHW=${FPGAorHW:-"hw"}
fi
#export DSET_PATCH_FILENAME=${DSET_PATCH_FILENAME:-"$SRC_IOE/bin/data.patch.hw4_0_hostless.bin"}
#export DSET_ADDR=${DSET_ADDR:-0x477eb4}
export DSET_PATCH_FILENAME=${DSET_PATCH_FILENAME:-"$SRC_IOE/bddata/data.patch.hw7_6.bin"}
export DSET_ADDR=${DSET_ADDR:-0x4dded4}
export TUNE_PARAM=${TUNE_PARAM:-0}
export HTMLTOFLASH_SCRIPT=${HTMLTOFLASH_SCRIPT:-htmltoflash.sh}
export BD_FILENAME_TEMPLATE=${BD_FILENAME_TEMPLATE:-"boardData_SP144"}

# tmp names
export MAKE_FLASH_APPS_ONLY=0
export APPS_FILENAME="flashimg.bin"
export INC_FILENAME="flashapps.bin"

dbg()
{
    if [ "$DEBUG" ]; then
        echo $(basename $0) DEBUG: $*
    fi
}

dbg Start $*

export OP_FLAVOR=${OP_FLAVOR:-"unidev"}
dbg OP_FLAVOR=$OP_FLAVOR

export OP_BAND=${OP_BAND:-"dualband"}
dbg OP_BAND=$OP_BAND
export DEMO_BAND="$OP_BAND"

if [ $AR6002_REV7_VER -eq 6 ]; then
	if [[ "$FPGA_FLAG" -eq 1 ]]; then
		BD_FILENAME="../../bddata/fakeBoardData_AR6004.bin"
	fi
#   BD_FILENAME="../../bddata/boardData_2_1_SP241.bin"
if [ "$DEMO_BAND" = "singleband" ]; then
    export BD_FILENAME=${BD_FILENAME:-"$SRC_IOE/bddata/boardData_2_1_SP241.bin"}
else
    export BD_FILENAME=${BD_FILENAME:-"$SRC_IOE/bddata/boardData_2_1_SP242.bin"}
fi
else
if [ "$DEMO_BAND" = "singleband" ]; then
    export BD_FILENAME=${BD_FILENAME:-"$SRC_IOE/bddata/${BD_FILENAME_TEMPLATE}_Single_band.bin"}
else
    export BD_FILENAME=${BD_FILENAME:-"$SRC_IOE/bddata/${BD_FILENAME_TEMPLATE}.bin"}
fi
fi
dbg BD_FILENAME=$BD_FILENAME

# PROXY_PARAM_DEV_MODE is used to configurate device mode during system boot up.
# There are 8 bits can be used to pass boot-up parameters to sdk_proxy.
# now we just use 4 bits to set some boot-up parameter:
# bit1~0:
#        device number
#        00 : single device
#        01 : 2 devices
#        10 : 3 devices
#        11 : 4 devices
# bit2:
#       concurrency mode
#       0 : SCC mode
#       1 : MCC mode
# bit3:
#       P2P enable
#       0 : P2P disable
#       1 : P2P enable      

case $OP_FLAVOR in
#    "p2p+sta_mcc")
#        export PROXY_PARAM_DEV_MODE=${PROXY_PARAM_DEV_MODE:-0x0D}
#	export APPS_OUTFILE=${APPS_OUTFILE:-$IMAGEDIR/"sdk_shell.out"}
#        ;;
#    "p2p+sta_scc") 
#        export PROXY_PARAM_DEV_MODE=${PROXY_PARAM_DEV_MODE:-0x09}
#	export APPS_OUTFILE=${APPS_OUTFILE:-$IMAGEDIR/"sdk_shell.out"}
#        ;;
#    "unidev_p2p")
#        export PROXY_PARAM_DEV_MODE=${PROXY_PARAM_DEV_MODE:-0x08}
#	export APPS_OUTFILE=${APPS_OUTFILE:-$IMAGEDIR/"sdk_shell.out"}
#        ;;
    "ap+sta_mcc")
        export PROXY_PARAM_DEV_MODE=${PROXY_PARAM_DEV_MODE:-0x05}
	export APPS_OUTFILE=${APPS_OUTFILE:-$IMAGEDIR/"iot_demo.out"}
        ;;
    "ap+sta_scc") 
        export PROXY_PARAM_DEV_MODE=${PROXY_PARAM_DEV_MODE:-0x01}
	export APPS_OUTFILE=${APPS_OUTFILE:-$IMAGEDIR/"iot_demo.out"}
        ;;
    "ap+sta_mcc+bt")
        export PROXY_PARAM_DEV_MODE=${PROXY_PARAM_DEV_MODE:-0x05}
	export APPS_OUTFILE=${APPS_OUTFILE:-$IMAGEDIR/"bt_demo.out"}
        ;;
    "ap+sta_scc+bt") 
        export PROXY_PARAM_DEV_MODE=${PROXY_PARAM_DEV_MODE:-0x01}
	export APPS_OUTFILE=${APPS_OUTFILE:-$IMAGEDIR/"bt_demo.out"}
        ;;
    "ap+sta_mcc+aws")
        export PROXY_PARAM_DEV_MODE=${PROXY_PARAM_DEV_MODE:-0x05}
        export APPS_OUTFILE=${APPS_OUTFILE:-$IMAGEDIR/"aws_demo.out"}
        ;;
    "ap+sta_mcc+mesh")
        export PROXY_PARAM_DEV_MODE=${PROXY_PARAM_DEV_MODE:-0x05}
        export APPS_OUTFILE=${APPS_OUTFILE:-$IMAGEDIR/"mesh_demo.out"}
        ;;
    "ap+sta_mcc+homekit")
        export PROXY_PARAM_DEV_MODE=${PROXY_PARAM_DEV_MODE:-0x05}
        export APPS_OUTFILE=${APPS_OUTFILE:-$IMAGEDIR/"homekit_demo.out"}
        ;;
    "unidev")
        export PROXY_PARAM_DEV_MODE=${PROXY_PARAM_DEV_MODE:-0x00}
	export APPS_OUTFILE=${APPS_OUTFILE:-$IMAGEDIR/"iot_demo.out"}
        ;;
    *)
        echo $0: ERROR: Unknown OP_FLAVOR, \'$OP_FLAVOR\'
        exit 1
        ;;
esac
dbg APPS_OUTFILE=$APPS_OUTFILE
dbg PROXY_PARAM_DEV_MODE=$PROXY_PARAM_DEV_MODE

export FLASH_FILE_SUFFIX="_AR401X_REV${AR6002_REV7_VER}${FLASH_FLAVOR}_hostless_${OP_FLAVOR}_${DEMO_BAND}${APPS_OUTFILE_SUFFIX}.bin"
dbg FLASH_FILE_SUFFIX=$FLASH_FILE_SUFFIX


./make_flash_hostless.sh
if [ $? -ne 0 ]; then
    dbg make_flash_hostless.sh failed
    exit 1
fi

# sh $HTMLTOFLASH_SCRIPT
# if [ $? -ne 0 ]; then
#     dbg $HTMLTOFLASH_SCRIPT failed
#     exit 1
# fi

dbg End
exit 0
