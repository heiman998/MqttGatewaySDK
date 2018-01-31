#!/bin/bash

# Customizable variables
# Golden image size : 992KB
# OTA Partition size : 1056KB
PARTITION_ENTRY_1=${PARTITION_ENTRY_1:-0x000F8000}
PARTITION_ENTRY_2=${PARTITION_ENTRY_2:-0xFFFFFFFF}
PARTITION_ENTRY_3=${PARTITION_ENTRY_3:-0xFFFFFFFF}
if [ $AR6002_REV7_VER -ge 5 ]; then
   PARTITION_ENTRY_4=${PARTITION_ENTRY_4:-0xFFFFFFFF}
   PARTITION_ENTRY_5=${PARTITION_ENTRY_5:-0xFFFFFFFF}
   PARTITION_ENTRY_6=${PARTITION_ENTRY_6:-0xFFFFFFFF}
   PARTITION_ENTRY_7=${PARTITION_ENTRY_7:-0xFFFFFFFF}

   PARTITION_TABLE_SIZE=${PARTITION_TABLE_SIZE:-28}   
else
   PARTITION_TABLE_SIZE=${PARTITION_TABLE_SIZE:-12}
fi
#PARTITION_TABLE_SIZE=${PARTITION_TABLE_SIZE:-12}
# By default, the partition table is located at the end of the first partition
PARTITION_TABLE_START=${PARTITION_TABLE_START:-$(($PARTITION_ENTRY_1 - $PARTITION_TABLE_SIZE))}
# Sample values for ~64KB RO dsets and ~64KB RW dsets
# positioned at the end of the flash partition
# (For NO datasets, use 0xffffffff for all four R*DATASET_* values)
FLASH_SECTORSIZE=${FLASH_SECTORSIZE:-0x00001000}
RODATASET_MAXSIZE=${RODATASET_MAXSIZE:-$((0xf000))}
RWDATASET_MAXSIZE=${RWDATASET_MAXSIZE:-$((0xf000))}
RWDATASET_START=${RWDATASET_START:-$(($PARTITION_TABLE_START - $RWDATASET_MAXSIZE - $FLASH_SECTORSIZE + $PARTITION_TABLE_SIZE))}
RODATASET_START=${RODATASET_START:-$(($RWDATASET_START - $RODATASET_MAXSIZE))}
FLASH_CAPACITY=${FLASH_CAPACITY:-0x00200000}
FLASH_BLOCKSIZE=${FLASH_BLOCKSIZE:-0x00010000}
FLASH_PAGESIZE=${FLASH_PAGESIZE:-0x00000100}
FLASH_HDR=${FLASH_HDR:-""}

# SBL_CFG_FILENAME - Secondary boot loader configuration
SBL_CFG_FILENAME="sbl_cfg_hostless.bin"
# SBL_CFG_NAME - is the flag name
SBL_CFG_NAME="sbl_config"
SBL_TRIGGER_METHOD=${SBL_TRIGGER_METHOD:-0x00000000}
METHOD1_ASSERT_PIN=${METHOD1_ASSERT_PIN:-0x0000001E}
METHOD2_DELAY_TIME=${METHOD2_DELAY_TIME:-0x00000005}
METHOD2_RCV_CHAR=${METHOD2_RCV_CHAR:-0x00000078}
METHOD2_RCV_TIMES=${METHOD2_RCV_TIMES:-0x00000005}
MENU_CMD_UPGRADE_NAME=${MENU_CMD_UPGRADE_NAME:-0x00000075}
MENU_CMD_RESET_NAME=${MENU_CMD_RESET_NAME:-0x00000072}
DBG_UART_RXPIN=${DBG_UART_RXPIN:-0x0000001C}
DBG_UART_TXPIN=${DBG_UART_TXPIN:-0x0000001D}

TUNE_APP=${TUNE_APP:-$IMAGEDIR/tune.out}
SDK_PROXY_APP=${SDK_PROXY_APP:-$IMAGEDIR/sdk_proxy.out}
BOOT_ROUTER_APP=${BOOT_ROUTER_APP:-$IMAGEDIR/boot_router.out}
OTP_APP=${OTP_APP:-$IMAGEDIR/otp_iot.out}
SBL_APP=${SBL_APP:-$IMAGEDIR/sbl.out}
EZXML_APP=${EZXML_APP:-$IMAGEDIR/ezxml.out}
MAKE_FLASH_APPS_ONLY=${MAKE_FLASH_APPS_ONLY:-0}
#Create secure boot image for Ruby
if [ $AR6002_REV7_VER -ge 5 ]; then
    MAKE_SECURE_BOOT_IMAGE=${MAKE_SECURE_BOOT_IMAGE:-1}
else
    MAKE_SECURE_BOOT_IMAGE=${MAKE_SECURE_BOOT_IMAGE:-0}
fi


dbg()
{
    if [ "$DEBUG" ]; then
        echo $(basename $0) DEBUG: $*
    fi
}

# Switch endianness of a 32-bit word
swizzle_word()
{
	word32=$($PRINTF "%08x\n" $1)

	byte0=$(echo $word32 | cut -b 7-8)
	byte1=$(echo $word32 | cut -b 5-6)
	byte2=$(echo $word32 | cut -b 3-4)
	byte3=$(echo $word32 | cut -b 1-2)

        echo 0x$byte0$byte1$byte2$byte3
}

to_hex()
{
	echo $($PRINTF "0x%08x\n" $1)
}

dbg Start $*

if [ -z "$APPS_OUTFILE" ]; then
    echo $0 ERROR: No APPS_OUTFILE specified
    exit 1
fi
if [ \! -r "$APPS_OUTFILE" ]; then
    echo $0 ERROR: Cannot access APPS_OUTFILE, $APPS_OUTFILE
    exit 1
fi
dbg APPS_OUTFILE=$APPS_OUTFILE

OBJDUMP=${OBJDUMP:-objdump}
NM=${NM:-nm}
PATTERN_FIND_TOOL="extract.out"

OTA_FILENAME="${BINDIR}/ota_image${FLASH_FILE_SUFFIX}"
FLASHER_FILENAME="${BINDIR}/flash${FLASH_FILE_SUFFIX}"
RAW_FILENAME="${BINDIR}/raw_flashimage${FLASH_FILE_SUFFIX}"


# Lots of internal variables
INCLUDEDIR=${FW}/include
BIN_FILENAME="flashimage_hostless.bin"
CFG_FILENAME="config_header_hostless.bin"
FLASH_APP_BIN="sdk_flash.bin"
FLASH_APP_OUT="sdk_flash.out"
PART_TABLE_FILENAME="parttable_hostless.bin"
FLASH_DESC_PATTERN_OFFSET=20
INC_LENGTH=0
CURR_DIR=$((pwd))
# FLASH_DESC_FILENAME - is a file that describes the flash chip
FLASH_DESC_FILENAME="flash_desc_hostless.bin"
# FLASH_DESC_NAME - is the input name for nvram_seek()
FLASH_DESC_NAME="flash_descriptor"


#
# Hostproxy param is athwlan_start_addr<<8 + EXT_BD_LENGTH
# this results in the start address and the BD_LENGTH overlapping by 4 bits which is ok as
# it is assumed the lower 4-bits of the start address will always be 0.
#

START_ADDR=0x$($OBJDUMP -f $APPS_OUTFILE | grep "start address" | cut -b 17-24)
dbg START_ADDR=$(to_hex $START_ADDR)
START_ADDR=$($PRINTF "%d\n" $START_ADDR)
START_ADDR=$(($START_ADDR*256))
PROXY_PARAM=$(($START_ADDR+$PROXY_PARAM_DEV_MODE))
dbg proxy param =$(to_hex $PROXY_PARAM)

#
# If we did not specify an APPS_FILENAME, supply
# a default and build it from scratch.  If we DID
# supply such a name, then only build it if it
# doesn't already exist.
#
if [ -z "$APPS_FILENAME" ]; then
    APPS_FILENAME=flashapps.bin
    rm -f $APPS_FILENAME
else
    dbg Using pre-built flashapps.bin
fi

dbg DSET_ADDR=$(to_hex $DSET_ADDR)
dbg VER=$AR6002_REV7_VER
dbg TUNE_PARAM=$(to_hex "$TUNE_PARAM")
########################################
# Create the applications segment of the flash image from ELF files
# NVRAM format:
#     32-bit pointer to partition table
#     32-bit unused
#     32-bit unused
#     32-bit unused
#     (Start of partition 0)
#     32-bit partition magic value (validates partition)
# START post-REV72 addition {
#     32-bit partition age (used when selecting which partition to use)
#     32-bit unused
#     32-bit unused
#     32-bit unused
#     32-bit unused
#     32-bit unused
#     32-bit unused
# END post_rev72 addition }
#     first meta-data entry
#     first data
#     second meta-data entry
#     second data
#      ....
########################################

# Construct ReadOnly DataSet Image
# File to contain Read-Only DataSet image
export RODATASET_FILENAME=${RODATASET_FILENAME:-"rodsetimage.bin"}
./makedsets.sh

dbg RWDATASET START=$(to_hex $RWDATASET_START)  SIZE=$RWDATASET_MAXSIZE
dbg RODATASET START=$(to_hex $RODATASET_START)  SIZE=$RODATASET_MAXSIZE

if [ -r "$RODATASET_FILENAME" ]; then
    RODATASET_LENGTH=$(stat --format=%s $RODATASET_FILENAME)
    if [ $RODATASET_LENGTH -gt $RODATASET_MAXSIZE ]; then
        echo "Error: ReadOnly DataSet image is too large."
        exit
    fi
else
    dbg Not using RODATASET
    RODATASET_FILENAME=""
    RODATASET_LENGTH=0
fi

FLASH_UNUSED=0xffffffff 
FLASH_SECURE_BOOT_DSET_MASK=0xff000000 # Secure Boot Mask for secure boot and no secure boot
#FLASH_SECURE_BOOT_DSET_MASK=0x00000000 # No Secure Boot only
FLASH_RODSET_START=${FLASH_RODSET_START:-$(($RODATASET_START | $FLASH_SECURE_BOOT_DSET_MASK))}
FLASH_RWDSET_START=${FLASH_RWDSET_START:-$(($RWDATASET_START | $FLASH_SECURE_BOOT_DSET_MASK))}
FLASH_RWDSET_SZ=$RWDATASET_MAXSIZE



# Flash descriptor, stored in flash
${TOOLDIR}/makeimg.sh \
        -out $FLASH_DESC_FILENAME \
        -new \
        -word  $FLASH_CAPACITY \
        -word  $FLASH_BLOCKSIZE \
        -word  $FLASH_SECTORSIZE \
        -word  $FLASH_PAGESIZE \
	-word  $FLASH_UNUSED \
	-word  $FLASH_RODSET_START \
	-word  $FLASH_RWDSET_START \
	-word  $FLASH_RWDSET_SZ

# SBL configuration
${TOOLDIR}/makeimg.sh \
        -out $SBL_CFG_FILENAME \
        -new \
        -word  $SBL_TRIGGER_METHOD \
        -word  $METHOD1_ASSERT_PIN \
        -word  $METHOD2_DELAY_TIME \
        -word  $METHOD2_RCV_CHAR \
        -word  $METHOD2_RCV_TIMES \
        -word  $MENU_CMD_UPGRADE_NAME \
        -word  $MENU_CMD_RESET_NAME \
        -word  $DBG_UART_RXPIN \
        -word  $DBG_UART_TXPIN       

# Partition header preceded by the 4-word flash header
# for partition 0.  This will be stripped out later if
# the image is not intended for use as partition 0.
if [ $AR6002_REV7_VER -ge 5 ] #{
    then
        #Split the flash image so that secure boot header and footer
        #can be inserted
        ${TOOLDIR}/makeimg.sh \
            -out flash_start \
            -new \
            -word $PARTITION_TABLE_START \
            -fill 0xffffffff 3 \
            -magic \
            -word 0\
            -fill 0xffffffff 6


    if [ "$TUNE_APP" != "none" ]; then
    	  dbg ${TOOLDIR}/makeimg.sh -out apps -name tune -exec "$TUNE_APP" $TUNE_PARAM
    	  ${TOOLDIR}/makeimg.sh -out apps -name tune -exec "$TUNE_APP" $TUNE_PARAM
    fi

    if [ "$SDK_PROXY_APP" != "none" ]; then
         dbg ${TOOLDIR}/makeimg.sh -out apps -name hostproxy -exec "$SDK_PROXY_APP" $PROXY_PARAM
         ${TOOLDIR}/makeimg.sh -out apps -name hostproxy -exec "$SDK_PROXY_APP" $PROXY_PARAM
    fi
    echo "board data is $BD_FILENAME"
    if [ "$BD_FILENAME" != "none" ]; then
        dbg ${TOOLDIR}/makeimg.sh -out apps -name boarddata -bddata "$BD_FILENAME"
        ${TOOLDIR}/makeimg.sh -out apps -name boarddata -bddata "$BD_FILENAME"
    fi

    if [ "$BOOT_ROUTER_APP" != "none" ]; then
       dbg ${TOOLDIR}/makeimg.sh -out apps -name bootrouter -exec "$BOOT_ROUTER_APP" 0x0
       ${TOOLDIR}/makeimg.sh -out apps -name bootrouter -exec "$BOOT_ROUTER_APP" 0x0
    fi

    if [ "$OTP_APP" != "none" ]; then
        dbg ${TOOLDIR}/makeimg.sh -out apps -name otp -exec "$OTP_APP" 0x2
        ${TOOLDIR}/makeimg.sh -out apps -name otp -exec "$OTP_APP" 0x2
    fi

    #if [ "$EZXML_APP" != "none" ]; then
    #   dbg ${TOOLDIR}/makeimg.sh -out apps -name ezxml -exec "$EZXML_APP" 0x0
    #   ${TOOLDIR}/makeimg.sh -out apps -name ezxml -exec "$EZXML_APP" 0x0
    #fi
    
    if [ "$SBL_APP" != "none" ]; then
    dbg ${TOOLDIR}/makeimg.sh -out apps -name sbl -exec "$SBL_APP" 0x0
    ${TOOLDIR}/makeimg.sh -out apps -name sbl -exec "$SBL_APP" 0x0
    fi
    
    echo "DSET_PATCH_FILENAME is $DSET_PATCH_FILENAME and DSET_ADDR is $DSET_ADDR" 
    if [ "$DSET_PATCH_FILENAME" != "none" ]; then
         dbg ${TOOLDIR}/makeimg.sh -out apps -name dsetpatch -data "$DSET_PATCH_FILENAME" $DSET_ADDR
        ${TOOLDIR}/makeimg.sh -out apps -name dsetpatch -data "$DSET_PATCH_FILENAME" $DSET_ADDR
    fi
   
    if [ "$FLASH_DESC_NAME" != "none" ]; then
        dbg ${TOOLDIR}/makeimg.sh -out apps -name $FLASH_DESC_NAME -noload "$FLASH_DESC_FILENAME"
        ${TOOLDIR}/makeimg.sh -out apps -name $FLASH_DESC_NAME -noload "$FLASH_DESC_FILENAME"
    fi
    
    if [ "$SBL_CFG_NAME" != "none" ]; then
    dbg ${TOOLDIR}/makeimg.sh -out apps -name $SBL_CFG_NAME -noload "$SBL_CFG_FILENAME"
    ${TOOLDIR}/makeimg.sh -out apps -name $SBL_CFG_NAME -noload "$SBL_CFG_FILENAME"
    fi

    
    # Finally, add the main application to the flash image
         dbg ${TOOLDIR}/makeimg.sh -out apps -name main_app -stop -load $APPS_OUTFILE 0x0
         ${TOOLDIR}/makeimg.sh -out apps -name main_app -stop -load $APPS_OUTFILE 0x0

        if [ "$MAKE_SECURE_BOOT_IMAGE" -eq 1 ]; then #{

             #openssl commands to generate key pair
             #openssl genpkey -algorithm RSA -out testkey.private -pkeyopt rsa_keygen_bits:2048 -pkeyopt rsa_keygen_pubexp:3
             #openssl rsa -in testkey.private -pubout -out testkey.public

            if [ ! -f testkey.private ] || [ ! -f testkey.public ]; then #{
                echo "Key files not found! Exiting..."
                exit
            fi #}

            #Calculate image hash
            imghash=$(sha256sum apps | sed 's/ .*//')

            #Length of unsigned image
            imglen=$(stat --format=%s apps)
  
            echo img length $imglen

            #changeme: Values used for testing
            imgtype=0x12345678
            img_security_version=0
            oemid=0x5678
            prdid=0xabcd

            #Create secure image header
            ${TOOLDIR}/makeimg.sh \
                 -out secure_img_hdr \
                 -new \
                 -secureimghdr $imgtype $imglen $img_security_version $oemid $prdid $imghash

             #Calculate signature of image header using SHA256_2048_RSA_PSS
             openssl dgst -sha256 -sign testkey.private -out signature.sign -sigopt rsa_padding_mode:pss -sigopt rsa_pss_saltlen:32 secure_img_hdr

             #OpenSSL command to verify
             #openssl dgst -sha256 -verify testkey.public -signature signature.sign -sigopt rsa_padding_mode:pss -sigopt rsa_pss_saltlen:32 secure_img_hdr 

             #Extract modulo of public key
             ${TOOLDIR}/makeimg.sh \
                 -out public_key_modulo \
                 -new \
                 -publickeymod testkey.public

             cat flash_start secure_img_hdr apps signature.sign public_key_modulo > $APPS_FILENAME

             rm -f flash_start
             rm -f secure_img_hdr
             rm -f public_key_modulo
             rm -f apps
             rm -f signature.sign
         #}
         else  #{
             #Secure boot image not needed
             cat flash_start apps > $APPS_FILENAME
         fi #}
    fi #}
    
    if [ "$MAKE_FLASH_APPS_ONLY" -eq 1 ]; then
         exit
    fi
#fi


########################################
# Create RAW flash image that can be written to flash chips directly.
# (e.g. via JTAG using gdb.sdk_flash)
########################################
echo "starting with apps section"
# Prepare to construct the remainder of the flash image, starting
# with the "apps section" that we just made.
cp $APPS_FILENAME $BIN_FILENAME




# Get current length of image so we know how much padding to add
FLASH_LENGTH=$(stat --format=%s $BIN_FILENAME)

rm -f sub_fill1.bin sub_fill2.bin
if [ $RODATASET_LENGTH -gt 0 ]; then
    # pad1 covers from end of flash image to start of RO dsets
    PAD1_LENGTH=$(($RODATASET_START - $FLASH_LENGTH))

    # pad2 covers from end of RO dsets to start of partition table
    PAD2_LENGTH=$(($PARTITION_TABLE_START - $RODATASET_START - $RODATASET_LENGTH))
else
    # Pad the whole area from end of flash to start of partition table
    # This works regardless of whether or a RW dset area is defined.
    PAD1_LENGTH=0
    PAD2_LENGTH=$(($PARTITION_TABLE_START - $FLASH_LENGTH))
fi

if [ $PAD1_LENGTH -gt 0 ]; then
    tail -c $PAD1_LENGTH fill.bin >> sub_fill1.bin
else
    touch sub_fill1.bin
fi

if [ $PAD2_LENGTH -gt 0 ]; then
    tail -c $PAD2_LENGTH fill.bin >> sub_fill2.bin
else
    touch sub_fill2.bin
fi

# Create the partition table, to be appended to the flash image
# (If not partition 0, will be stripped out later.)
if [ $AR6002_REV7_VER -ge 5 ]; then
${TOOLDIR}/makeimg.sh \
        -out $PART_TABLE_FILENAME \
        -new \
        -word $PARTITION_ENTRY_1 \
        -word $PARTITION_ENTRY_2 \
        -word $PARTITION_ENTRY_3 \
        -word $PARTITION_ENTRY_4 \
        -word $PARTITION_ENTRY_5 \
        -word $PARTITION_ENTRY_6 \
        -word $PARTITION_ENTRY_7 
else
${TOOLDIR}/makeimg.sh \
        -out $PART_TABLE_FILENAME \
        -new \
        -word $PARTITION_ENTRY_1 \
        -word $PARTITION_ENTRY_2 \
        -word $PARTITION_ENTRY_3
fi

echo Creating image: $(basename "$RAW_FILENAME")
cat $BIN_FILENAME sub_fill1.bin $RODATASET_FILENAME sub_fill2.bin $PART_TABLE_FILENAME > $RAW_FILENAME
# Note: At this point, the image does not yet contain RO DataSets (e.g. HTML)

rm -f sub_fill1.bin
rm -f sub_fill2.bin

# Done creating RAW image


########################################
# At this point we have created a raw flash image that can be
# committed to flash (RAW_FILENAME). We use that raw flash image
# to create an image that can be used for OTA upgrades.
########################################

#
# TBD: Should use a *segmented* image for OTA upgrade
#

${TOOLDIR}/ota.sh $RAW_FILENAME  $OTA_FILENAME
if [ $? -ne 0 ]; then
    echo $0 Error: ota.sh failed to create OTA image
    exit 1
fi

dbg End
exit 0
