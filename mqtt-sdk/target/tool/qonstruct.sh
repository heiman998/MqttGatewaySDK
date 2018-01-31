#!/bin/bash

# Copyright (c) 2014 Qualcomm Atheros Inc.
# All rights reserved.

########################################
# Qonstruct tool, used to guide users through
# construction of firmware images for IoE systems.
########################################

# Qonstruct internal variables
# QONS_DIR: workspace for qonstruct temporary files
# QONS_SDK: software SDK containing target and/or host directories
# QONS_TUNABLE_INPUT_FILE: Tunable input file
# QONS_DATASET_TEXT_FILE: DataSet Text file

QONS_TOOL_NAME=$(basename $0)

dbg()
{
    if [ "$DEBUG" ]; then
        echo $QONS_TOOL_NAME DEBUG: $*
    fi
}

err()
{
    echo $QONS_TOOL_NAME ERROR: $*
}

status()
{
    if [ "$_QONS_STATUS" ]; then
        echo $QONS_TOOL_NAME STATUS: $*
    fi
}

########################################
# Print usage message
########################################
qonstruct_usage()
{
cat << QON_USAGE

Options:

--qons qonstruct_directory
    Specify the path to a working directory for use by qonstruct.
    If this directory doesn't exist, qonstruct creates it.
    qonstruct must be told its working directory either through
    this option or via the QONS_DIR environment variable.  It's
    always safe to remove this directory and start over from
    scratch.  If you do, you might want to preserve your
    tunable_input.txt file.

--help
    Show this help message.

--dset dataset_text_file
    Specify the DataSet text file.  Typically not needed.

--sdk sdk_directory
    Define the software SDK directory, which is the directory
    that contains target and/or host directories.

--status
    Show very brief status of Qonstruct during a build.

--debug
    Show lots of debug info

--tunable
    Define the tunable input file. The default is tunable_input.txt.

QON_USAGE
}

########################################
# Update the qonstruct environment cache
########################################
qonstruct_update_cache()
{
    cd "$QONS_DIR"
    if [ $? -eq 0 ]; then
        if [ "$QONS_SDK" ]; then
            echo QONS_SDK="$QONS_SDK" > .qons_cache
        fi

        if [ "$QONS_TUNABLE_INPUT_FILE" ]; then
            echo QONS_TUNABLE_INPUT_FILE="$QONS_TUNABLE_INPUT_FILE" >> .qons_cache
        fi

        if [ "$QONS_DATASET_TEXT_FILE" ]; then
            echo QONS_DATASET_TEXT_FILE="$QONS_DATASET_TEXT_FILE" >> .qons_cache
        fi

        if [ "$QONS_OVERRIDE_FILE" ]; then
            echo QONS_OVERRIDE_FILE="$QONS_OVERRIDE_FILE" >> .qons_cache
        fi
    fi
}

qonstruct_exit()
{
    qonstruct_update_cache
}

# Where we are when we start executing
_QONS_START_DIR=$(pwd)

# Where this tool comes from
_QONS_TOOL_DIR=$(cd $(dirname $0); pwd)

########################################
# Add a word to file $2
########################################
add_word()
{
        word32=$($PRINTF "%08x\n" $1)

        byte0=$(echo $word32 | cut -b 7-8)
        byte1=$(echo $word32 | cut -b 5-6)
        byte2=$(echo $word32 | cut -b 3-4)
        byte3=$(echo $word32 | cut -b 1-2)

        $PRINTF "\x$byte0" >> $2
        $PRINTF "\x$byte1" >> $2
        $PRINTF "\x$byte2" >> $2
        $PRINTF "\x$byte3" >> $2
}


########################################
# Parse command line arguments
########################################
while [ $# -ne 0 ]
do
    arg=$1
    shift 1
    case $arg in
    --qons)
        if [ $# -ge 1 ]; then
            QONS_DIR="$1"
            shift 1
        else
            echo No directory specified with $arg.
            exit 1
        fi
        ;;
    --qover)
        if [ $# -ge 1 ]; then
            QONS_OVERRIDE_FILE=$(echo $(cd $(dirname "$1"); pwd))
            shift 1
            echo debug: QONS_OVERRIDE_FILE=$QONS_OVERRIDE_FILE
        else
            echo No qonstruct override file specified with $arg.
            exit 1
        fi
        ;;
    --dset)
        if [ $# -ge 1 ]; then
            QONS_DATASET_TEXT_FILE=$(echo $(cd $(dirname "$1"); pwd))
            shift 1
            echo debug: QONS_DATASET_TEXT_FILE=$QONS_DATASET_TEXT_FILE
        else
            echo No DataSet text file specified with $arg.
            exit 1
        fi
        ;;
    -? | --? | -h | --help)
        qonstruct_usage
        exit 1
        ;;
    --status)
        _QONS_STATUS=1
        ;;
    --debug)
        _QONS_STATUS=1
        export DEBUG=1
        ;;
    --tunable)
        if [ $# -ge 1 ]; then
            QONS_TUNABLE_INPUT_FILE=$(echo $(cd $(dirname "$1"); pwd))
            shift 1
            echo debug: QONS_TUNABLE_INPUT_FILE="$QONS_TUNABLE_INPUT_FILE"
        else
            echo "No tunable input file specified with $arg."
            exit 1
        fi
        ;;
     --sdk)
        if [ $# -ge 1 ]; then
            QONS_SDK="$1"
            shift 1
            echo debug: QONS_SDK=$QONS_SDK
        else
            echo "No sdk directory specified with $arg."
            exit 1
        fi
        ;;
    esac
done

if [ -z "$QONS_DIR" ] ; then
    if [ -r .qons_cache ]; then
        QONS_DIR=$(pwd)
    else
        echo 'No qonstruct directory (--qons) specified.  See qonstruct --help.'
        exit 1
    fi
fi

if [ -r "$QONS_DIR"/.qons_cache ]; then
    source "$QONS_DIR"/.qons_cache
fi

export QONS_DIR

########################################
# Establish/Validate Qonstruct Directory
########################################
mkdir -p "$QONS_DIR"
if [ \! -d "$QONS_DIR" ]; then
    echo "Invalid Qonstruct working directory: \'$QONS_DIR\'"
    exit 1;
fi
# Normalize QONS_DIR
QONS_DIR=$(cd "$_QONS_START_DIR"; cd "$QONS_DIR"; pwd)
status Using qonstruct directory at $QONS_DIR

########################################
# Initialize local qonstruct variables.
# These must match locations in the SDK.
# They can be overridden via a QONS_OVERRIDE file.
########################################
_QONS_TUNABLE_DIR=tool/tunable
if [ $AR6002_REV7_VER -ge 5 ]; then
	_QONS_DSET_FILE=image/dsets.patch.hw7_$AR6002_REV7_VER.txt
else 
	_QONS_DSET_FILE=image/dsets.patch.hw4_0.txt
fi

_QONS_MKDSETIMG_DIR=tool/mkdsetimg
_QONS_DSETS_ROM_MAP_FILE=image/dsets.rom.map
_QONS_MAKE_FLASH_DIR=demo/sdk_flash

if [ -r "$QONS_OVERRIDE_FILE" ]; then
    source "$QONS_OVERRIDE_FILE"
else
    # Look for a default override file
    if [ -r "$_QONS_TOOL_DIR"/qons_override.sh ]; then
        source "$_QONS_TOOL_DIR"/qons_override.sh
    fi
fi

########################################
# Locate/Create Master Plan
########################################
trap qonstruct_exit EXIT
# Locate/Create Software Images
cd "$_QONS_START_DIR"
if [ -z "$QONS_SDK" ];
then
    tmp_QONS_SDK=$(cd "$_QONS_START_DIR"; cd "$(dirname $0)"/../..; pwd)
    if [ -d "$tmp_QONS_SDK"/target -o -d "$tmp_QONS_SDK"/host ]; then
	QONS_SDK=$tmp_QONS_SDK
        status Using SDK at $QONS_SDK
    else
        err SDK directory undefined. See qonstruct --help \(--sdk\)
        exit 1
    fi
fi

# Normalize QONS_SDK
cd "$_QONS_START_DIR"
cd "$QONS_SDK"
if [ $? -ne 0 ]; then
    err QONS_SDK is inaccessible: $QONS_SDK
    exit 1
fi
QONS_SDK=$(pwd)
status Using SDK directory at $QONS_SDK

########################################
# Build and execute the tunable application
########################################
if [ -e "$QONS_SDK"/target/$_QONS_TUNABLE_DIR/tunable ]; then
    status Using pre-qonstructed tunable application
else
    cd "$QONS_SDK"/target/$_QONS_TUNABLE_DIR
    make > /dev/null
    if [ $? -ne 0 ]; then
        echo FAILED to build Tunable application.
        exit 1
    fi
    status Built tunable application
fi

if [ -f "$QONS_DIR"/tunable_input.txt ]; then
    status Using pre-qonstructed tunable input
else
    if [ "$QONS_TUNABLE_INPUT_FILE" ]; then
        cp "$QONS_TUNABLE_INPUT_FILE" "$QONS_DIR"/tunable_input.txt
        status Using tunable input from $QONS_TUNABLE_INPUT_FILE
    else
        # If there is no cached copy of tunable input,
        # and no user-chosen tunable input, then
        # initialize with the default version, if one exists.
        if [ -r "$QONS_SDK"/target/$_QONS_TUNABLE_DIR/tunable_input.txt ]; then
            cp "$QONS_SDK"/target/$_QONS_TUNABLE_DIR/tunable_input.txt "$QONS_DIR"/tunable_input.txt
            status Using tunable input from "$QONS_SDK"/target/$_QONS_TUNABLE_DIR/tunable_input.txt
        else
            # Create the initial tunable input from the master XML file
            cd "$QONS_DIR"
            if [ -f input.txt ]; then
                # Odd case. Let's not wipe out input.txt.
                cp input.txt tunable_input.txt
                status Using existing input.txt file.
            else
                "$QONS_SDK"/target/$_QONS_TUNABLE_DIR/tunable "$QONS_SDK"/target/$_QONS_TUNABLE_DIR/master_list.xml > /dev/null
                if [ $? -ne 0 ]; then
                    echo Error: Could not create tunable input file from XML
                    exit 1
                else
                    mv input.txt tunable_input.txt
                    if [ -r "$QONS_SDK"/target/$_QONS_TUNABLE_DIR/master_list_ext.txt ]; then
                        # Append master_list extensions
                        cat "$QONS_SDK"/target/$_QONS_TUNABLE_DIR/master_list_ext.txt >> tunable_input.txt
                    fi
                    echo
                    echo Please modify values in "$QONS_DIR"/tunable_input.txt, if desired.
                    echo You may modify tunable_input.txt later, if you prefer.
                    echo Re-execute $QONS_TOOL_NAME in order to continue qonstruction.
                    exit 1
                fi
            fi
        fi
    fi
fi

if [ -r "$QONS_DIR"/tunable_input.txt ]; then
    if [ "$QONS_DIR"/tunable_input.txt -nt "$QONS_DIR"/tuneenv.sh ]; then
        cd "$QONS_DIR"
        "$QONS_SDK"/target/$_QONS_TUNABLE_DIR/tunable "$QONS_DIR"/tunable_input.txt "$QONS_SDK"/target/$_QONS_TUNABLE_DIR/master_list.xml
        if [ \! -r dset_tunable_1.bin ]; then
            # Base tunables
            echo Error: Missing "$QONS_DIR"/dset_tunable_1.bin
            exit 1
        fi
        if [ \! -r dset_tunable_2.bin ]; then
            # System RAM tunables
            echo Error: Missing "$QONS_DIR"/dset_tunable_2.bin
            exit 1
        fi
        if [ \! -r dset_tunable_3.bin ]; then
            # Customer application tunables
            echo Error: Missing "$QONS_DIR"/dset_tunable_3.bin
            exit 1
        fi
        if [ \! -r tuneenv.sh ]; then
            echo Error: Missing "$QONS_DIR"/tuneenv.sh
            exit 1
        fi
        status Re-qonstructed tunables
    else
        # This allows hand tweaks to the tuneenv.sh (e.g. for experimentation)
        status Using pre-qonstructed tunables.
    fi
else
    echo "Please provide tunable_input file. See qonstruct --help."
    exit 1
fi

if [ -r "$QONS_SDK"/target/sdkenv.sh ]; then
    cd "$QONS_SDK"/target
    source ./sdkenv.sh
    cd "$OLDPWD"
fi


########################################
# Build mkdsetimg tool
########################################
if [ -e "$QONS_DIR"/mkdsetimg ]; then
    status Using pre-qonstructed mkdsetimg application
else
    cd "$QONS_SDK"/target/"$_QONS_MKDSETIMG_DIR"
    if [ $? -ne 0 ]; then
        echo "Error: mkdsetimg directory inaccessible: $QONS_SDK/target/$_QONS_MKDSETIMG_DIR"
        exit 1
    fi
    make WLANCMN_DIR=$QONS_SDK/target/ > /dev/null
    if [ $? -ne 0 ]; then
        echo "Error: Cannot build mkdsetimg"
        exit 1
    fi
    cp mkdsetimg "$QONS_DIR"/mkdsetimg
    status Built mkdsetimg application
fi

# Pull in user's customizations
source "$QONS_DIR"/tuneenv.sh

########################################
# Locate/Create Data Images
########################################
if [ \( -f "$QONS_DIR"/dsets.txt \) -a \( "$QONS_DIR"/dsets.txt -nt "$QONS_DIR"/tunable_input.txt \) ]; then
    status Using pre-qonstructed dsets.txt at $QONS_DIR/dsets.txt
else
    if [ -z "$QONS_DATASET_TEXT_FILE" ]; then
        status Using default dsets.txt file.
        cp "$QONS_SDK"/target/$_QONS_DSET_FILE "$QONS_DIR"/dsets.txt
        if [ $? -eq 0 ]; then
            status Using dsets.txt file from $QONS_SDK/target/$_QONS_DSET_FILE
        else
            echo ERROR: Cannot access dsets.txt file from $QONS_SDK/target/$_QONS_DSET_FILE
            exit 1;
        fi
    else
        cp "$QONS_DATASET_TEXT_FILE" "$QONS_DIR"/dsets.txt
        if [ $? -ne 0 ]; then
            echo "ERROR: Cannot access DataSet text file, $QONS_DATASET_TEXT_FILE."
            exit 1;
        fi
    fi
    # Add dset_tunable DataSets, if not already present
    #
    # Note:
    #     0xa02 == DSETID_TUNABLE_BASE
    #     0xa03 == DSETID_TUNABLE_SYSTEM
    #     0xa04 == DSETID_TUNABLE_CUSTOM
    #
        if [ -s $QONS_DIR/dset_tunable_1.bin ]; then
            echo '0xa02 $QONS_DIR/dset_tunable_1.bin' >> "$QONS_DIR"/dsets.txt
        fi
        if [ -s $QONS_DIR/dset_tunable_2.bin ]; then
            echo '0xa03 $QONS_DIR/dset_tunable_2.bin' >> "$QONS_DIR"/dsets.txt
        fi
        if [ -s $QONS_DIR/dset_tunable_3.bin ]; then
            echo '0xa04 $QONS_DIR/dset_tunable_3.bin' >> "$QONS_DIR"/dsets.txt
    fi
fi

cd "$QONS_DIR"
export INI_OVERRIDE_DIR="$SDK_ROOT"/tool/ini_override

# Calculate the end of IRAM and DRAM based on the number
# switchable banks configured for IRAM.
if [ $AR6002_REV7_VER -ge 5 ]; then
	export NUM_IRAM_BANKS=${qtune_NUM_IRAM_BANKS:-0}
else
	export NUM_IRAM_BANKS=${qtune_NUM_IRAM_BANKS:-1}	
fi
_QONS_IRAM_END=$((0xa18000 + $NUM_IRAM_BANKS * 32768))
_QONS_IRAM_END_hex=$($PRINTF "%08x" $_QONS_IRAM_END)
dbg IRAM range [0xa00000..$_QONS_IRAM_END_hex\)

if [ $AR6002_REV7_VER -ge 5 ]; then
	NUM_DRAM_BANKS=$((16 - $NUM_IRAM_BANKS))
else
	NUM_DRAM_BANKS=$((8 - $NUM_IRAM_BANKS))
fi

_QONS_DRAM_END=$((0x480000 + $NUM_DRAM_BANKS * 32768))
_QONS_DRAM_END_hex=$($PRINTF "%08x" $_QONS_DRAM_END)
dbg DRAM range [0x420000..$_QONS_DRAM_END_hex\)

./mkdsetimg --desc="$QONS_DIR"/dsets.txt \
    --out="$QONS_DIR"/data.patch.bin \
    --mapout="$QONS_DIR"/dsets.patch.map \
    --mapin="$QONS_SDK"/target/$_QONS_DSETS_ROM_MAP_FILE \
    --end=$(($_QONS_DRAM_END-4)) \
    --pad=4 \
    --reloc > /dev/null 2>&1

if [ $? -ne 0 ]; then
    echo "Error: Failed to build DataSet image.  Check $QONS_DIR/dsets.txt."
    exit 1
fi

export DSET_PATCH_FILENAME="$QONS_DIR"/data.patch.bin
export DSET_IMG_LENGTH=$(stat --format %s "$DSET_PATCH_FILENAME")
export DSET_ADDR=$(($_QONS_DRAM_END - $DSET_IMG_LENGTH))

# Verify that Xtensa tools are available
which xt-objdump > /dev/null 2>&1
if [ $? -ne 0 ]; then
    err Xtensa toolchain must be installed and on your PATH in order to proceed.
    err Please fix and then re-execute $QONS_TOOL_NAME.
    exit 1
fi

########################################
# Stuff certain tunables into sdk_proxy
########################################
cd "$QONS_DIR"
if [ \( \! -f sdk_proxy.out \) -o \( "$QONS_SDK"/target/image/sdk_proxy.out -nt sdk_proxy.out \) ]; then
    # Make a local copy of sdk_proxy.out, which will be modified to create sdk_proxy.new.out
    cp "$QONS_SDK"/target/image/sdk_proxy.out sdk_proxy.out
fi

# Find offset into sdk_proxy.out of the start of the rodata section
# which is where sdk_proxy_tune is located
tunable_offset=$((0x$(xt-objdump -hww sdk_proxy.out | grep rodata | sed -e 's/  */,/g' | cut -d ',' -f 7)))
dd if=sdk_proxy.out of=sdk_proxy.new1 bs=1 count=$tunable_offset > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo Problem creating sdk_proxy.new1
    exit 1
fi
dd if=sdk_proxy.out of=sdk_proxy.new2 bs=1 skip=$(($tunable_offset + 132)) > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo Problem creating sdk_proxy.new2
    exit 1
fi

sdk_tune_file=sdk_proxy_tune.bin
rm -f $sdk_tune_file
touch $sdk_tune_file
valid_bits=0

#
# Positions MUST MATCH sdk_proxy.c's SDK_TUNE_*
#

# SDK_TUNE_NUM_SWITCHABLE_IRAM_BANKS
if [ -n "$qtune_NUM_IRAM_BANKS" ]; then
    add_word $qtune_NUM_IRAM_BANKS $sdk_tune_file
    valid_bits=$(($valid_bits | 1))
else
    add_word 0 $sdk_tune_file
fi

# SDK_TUNE_END_RAM_RESERVE_SZ
if [ -n "$qtune_END_RAM_RESERVE_SZ" ]; then
    add_word $qtune_END_RAM_RESERVE_SZ $sdk_tune_file
else
    # Default action is to reserve exactly the right amount of
    # space to hold the DataSet image. 
    # fix me later, + 4 can help reserve the dset memory
    # not sure why
    add_word  $(($DSET_IMG_LENGTH + 4)) $sdk_tune_file
fi
valid_bits=$(($valid_bits | 2))

# SDK_TUNE_ATH_TXBUF
if [ -n "$qtune_ATH_TXBUF" ]; then
    add_word $qtune_ATH_TXBUF $sdk_tune_file
    valid_bits=$(($valid_bits | 4))
else
    add_word 0 $sdk_tune_file
fi

# SDK_TUNE_ATH_RXBUF
if [ -n "$qtune_ATH_RXBUF" ]; then
    add_word $qtune_ATH_RXBUF $sdk_tune_file
    valid_bits=$(($valid_bits | 8))
else
    add_word 0 $sdk_tune_file
fi

# SDK_TUNE_ATH_MGMTBUF
if [ -n "$qtune_ATH_MGMTBUF" ]; then
    add_word $qtune_ATH_MGMTBUF $sdk_tune_file
    valid_bits=$(($valid_bits | 0x10))
else
    add_word 0 $sdk_tune_file
fi

# SDK_TUNE_BOARD_DATA_ADDR
if [ -n "$qtune_BOARD_DATA_ADDR" ]; then
    # Explicitly select location for Board Data -- unusual
    add_word $qtune_BOARD_DATA_ADDR $sdk_tune_file
else
    # Tell sdk_proxy to use 0 rather than some default
    # hard-coded address.  This causes space for Board Data
    # to be allocated dynamically.
    add_word 0 $sdk_tune_file
fi
valid_bits=$(($valid_bits | 0x20))

# SDK_TUNE_DSET_LIST_HEAD
if [ -n "$qtune_DSET_LIST_HEAD" ]; then
    # Use an explicit tuned value -- unusual, but allows
    # DataSet image to be relocated
    add_word $qtune_DSET_LIST_HEAD $sdk_tune_file
else
    # Use derived value at end of DRAM -- typical case
    add_word $DSET_ADDR $sdk_tune_file
fi
valid_bits=$(($valid_bits | 0x40))

# SDK_TUNE_OPTION_FLAG - TBD
if [ -n "$qtune_OPTION_FLAG" ]; then
    add_word $qtune_OPTION_FLAG $sdk_tune_file
    valid_bits=$(($valid_bits | 0x80))
else
    add_word 0 $sdk_tune_file
fi

# SDK_TUNE_OPTION_FLAG2 - TBD
if [ -n "$qtune_OPTION_FLAG2" ]; then
    add_word $qtune_OPTION_FLAG2 $sdk_tune_file
    valid_bits=$(($valid_bits | 0x100))
else
    add_word 0 $sdk_tune_file
fi

# SDK_TUNE_OPTION_FLAG3 - TBD

# SDK_TUNE_DESIRED_BAUD_RATE
if [ -n "$qtune_DESIRED_BAUD_RATE" ]; then
    add_word $qtune_DESIRED_BAUD_RATE $sdk_tune_file
    valid_bits=$(($valid_bits | 0x200))
else
    add_word 0 $sdk_tune_file
fi

# SDK_TUNE_DBG_UART_TXPIN - TBD
if [ -n "$qtune_DBG_UART_TXPIN" ]; then
    add_word $qtune_DBG_UART_TXPIN $sdk_tune_file
    valid_bits=$(($valid_bits | 0x400))
else
    add_word 0 $sdk_tune_file
fi

# SDK_TUNE_HTTPC_BODY_MAXLEN - TBD
if [ -n "$qtune_HTTPC_BODY_MAXLEN" ]; then
    add_word $qtune_HTTPC_BODY_MAXLEN $sdk_tune_file
    valid_bits=$(($valid_bits | 0x800))
else
    add_word 0 $sdk_tune_file
fi

add_word 0 $sdk_tune_file
add_word 0 $sdk_tune_file
add_word 0 $sdk_tune_file
add_word 0 $sdk_tune_file
add_word 0 $sdk_tune_file
add_word 0 $sdk_tune_file
add_word 0 $sdk_tune_file
add_word 0 $sdk_tune_file
add_word 0 $sdk_tune_file
add_word 0 $sdk_tune_file
add_word 0 $sdk_tune_file
add_word 0 $sdk_tune_file
add_word 0 $sdk_tune_file
add_word 0 $sdk_tune_file
add_word 0 $sdk_tune_file
add_word 0 $sdk_tune_file
add_word 0 $sdk_tune_file
add_word 0 $sdk_tune_file
add_word 0 $sdk_tune_file
add_word 0 $sdk_tune_file

add_word $valid_bits $sdk_tune_file

cat sdk_proxy.new1 $sdk_tune_file sdk_proxy.new2 > sdk_proxy.new.out
status Built tuned version of sdk_proxy
export SDK_PROXY_APP="$QONS_DIR"/sdk_proxy.new.out

########################################
# Utility scripts, all under $_QONS_SDK:
#     $_QONS_DSET_INSERT_SCRIPT
#     $_QONS_MAKEIMG_SCRIPT
#     $_QONS_MAKESEG_SCRIPT
########################################

export PARTITION_ENTRY_1=${qtune_PARTITION_ENTRY_1:-""}
export PARTITION_ENTRY_2=${qtune_PARTITION_ENTRY_2:-""}
export PARTITION_ENTRY_3=${qtune_PARTITION_ENTRY_3:-""}
# PARTITION_TABLE_START - TBD
# PARTITION_TABLE_SIZE - TBD
export FLASH_CAPACITY=${qtune_FLASH_CAPACITY:-""}
export FLASH_BLOCKSIZE=${qtune_FLASH_BLOCKSIZE:-""}
export FLASH_SECTORSIZE=${qtune_FLASH_SECTORSIZE:-""}
export FLASH_PAGESIZE=${qtune_FLASH_PAGESIZE:-""}

# May use "none" for any of these:
# TUNE_APP, SDK_PROXY_APP, BD_FILENAME, BOOT_ROUTER_APP,
# OTP_APP
export TUNE_APP=${qtune_TUNE_APP:-""}
if [ -n "$TUNE_APP" -a \( \! -r "$TUNE_APP" \) ]; then
    err Cannot access TUNE_APP: $TUNE_APP
    exit 1
fi

if [ -z "$BD_FILENAME" ] ; then
    export BD_FILENAME=${qtune_BD_FILENAME:-""}
fi

if [ -n "$BD_FILENAME" -a \( \! -r "$BD_FILENAME" \) ]; then
    err Cannot access BD_FILENAME: $BD_FILENAME
    exit 1
fi

if [ -n "$qtune_BAND" ]; then
    case "$qtune_BAND" in
    0) unset OP_BAND
       ;;
    1) export OP_BAND="singleband"
       ;;
    2) export OP_BAND="dualband"
       ;;
    *) err Unknown BAND $qtune_BAND
       exit 1
       ;;
    esac
    dbg OP_BAND=$OP_BAND
fi

if [ -n "$qtune_OP_FLAVOR" ]; then
    case "$qtune_OP_FLAVOR" in
    0) unset OP_FLAVOR
       ;;
    1) export OP_FLAVOR="unidev"
       ;;
    2) export OP_FLAVOR="unidev_p2p"
       ;;
    3) export OP_FLAVOR="p2p+sta_scc"
       ;;
    4) export OP_FLAVOR="p2p+sta_mcc"
       ;;
    5) export OP_FLAVOR="ap+sta_scc"
       ;;
    6) export OP_FLAVOR="ap+sta_mcc"
       ;;
    7) export OP_FLAVOR="ap+sta_scc+bt"
       ;;
    8) export OP_FLAVOR="ap+sta_mcc+bt"
       ;;
    9) export OP_FLAVOR="ap+sta_mcc+aws"
       ;;
    10) export OP_FLAVOR="ap+sta_mcc+mesh"
       ;;
    11) export OP_FLAVOR="ap+sta_mcc+homekit"
       ;;
    *) err Unknown OP_FLAVOR $qtune_OP_FLAVOR
       exit 1
       ;;
    esac
    dbg OP_FLAVOR="$OP_FLAVOR"
fi

# Determine which app to use based on selected OP_FLAVOR
if [ -n "$APPS_OUTFILE" ]; then
    export APPS_OUTFILE_SUFFIX=${APPS_OUTFILE_SUFFIX:-"_$(basename ${APPS_OUTFILE} .out)"}
    status Using custom application at $APPS_OUTFILE
else
    case "$OP_FLAVOR" in
    "unidev")
       export APPS_OUTFILE=${APPS_OUTFILE:-${IMAGEDIR}/iot_demo.out}
       ;;
    "ap+sta_scc")
       export APPS_OUTFILE=${APPS_OUTFILE:-${IMAGEDIR}/iot_demo.out}
       ;;
    "ap+sta_mcc")
       export APPS_OUTFILE=${APPS_OUTFILE:-${IMAGEDIR}/iot_demo.out}
       ;;
    "ap+sta_scc+bt")
       export APPS_OUTFILE=${APPS_OUTFILE:-${IMAGEDIR}/bt_demo.out}
       ;;
    "ap+sta_mcc+bt")
       export APPS_OUTFILE=${APPS_OUTFILE:-${IMAGEDIR}/bt_demo.out}
       ;;
    "unidev_p2p")
       export APPS_OUTFILE=${APPS_OUTFILE:-${IMAGEDIR}/sdk_shell.out}
       ;;
    "p2p+sta_scc")
       export APPS_OUTFILE=${APPS_OUTFILE:-${IMAGEDIR}/sdk_shell.out}
       ;;
    "p2p+sta_mcc")
       export APPS_OUTFILE=${APPS_OUTFILE:-${IMAGEDIR}/sdk_shell.out}
       ;;
    "ap+sta_mcc+aws")
       export APPS_OUTFILE=${APPS_OUTFILE:-${IMAGEDIR}/aws_demo.out}
       ;;
    "ap+sta_mcc+mesh")
       export APPS_OUTFILE=${APPS_OUTFILE:-${IMAGEDIR}/mesh_demo.out}
       ;;
    "ap+sta_mcc+homekit")
       export APPS_OUTFILE=${APPS_OUTFILE:-${IMAGEDIR}/homekit_demo.out}
       ;;
    esac

    # We choose not to reflect the app name for standard demos
    export APPS_OUTFILE_SUFFIX=${APPS_OUTFILE_SUFFIX:-""}
fi

dbg APPS_OUTFILE=$APPS_OUTFILE   APPS_OUTFILE_SUFFIX=$APPS_OUTFILE_SUFFIX

export BOOT_ROUTER_APP=${qtune_BOOT_ROUTER_APP:-""}
if [ -n "$BOOT_ROUTER_APP" -a \( \! -r "$BOOT_ROUTER_APP" \) ]; then
    err Cannot access BOOT_ROUTER_APP: $BOOT_ROUTER_APP
    exit 1
fi

export OTP_APP=${qtune_OTP_APP:-""}
if [ -n "$OTP_APP" -a \( \! -r "$OTP_APP" \) ]; then
    err Cannot access OTP_APP: $OTP_APP
    exit 1
fi

export RODATASET_FILENAME=${qtune_RODATASET_FILENAME:-""}
if [ -n "$RODATASET_FILENAME" -a \( \! -r "$RODATASET_FILENAME" \) ]; then
    err Cannot access RODATASET_FILENAME: $RODATASET_FILENAME
    exit 1
fi

export RODATASET_START=${qtune_RODATASET_START:-""}
export RWDATASET_START=${qtune_RWDATASET_START:-""}
export RWDATASET_MAXSIZE=${qtune_RWDATASET_MAXSIZE:-""}

cd "$QONS_SDK"/target/"$_QONS_MAKE_FLASH_DIR"
./make_flash.sh
if [ $? -ne 0 ]; then
    status make_flash.sh failed
else
    echo Flash images are in $BINDIR
fi

########################################
# Validate images
########################################
# TBD
