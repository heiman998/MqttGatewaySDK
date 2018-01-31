#!/bin/bash

# Script to allow a RO DataSet image to be dropped
# into a pre-formed flashotp*.bin image for flashing.
#
# inputs: FLASHBIN_ORIG, RODATASET_FILENAME
# output: FLASHBIN_NEW
# uses: SUPPORT_DIR/makeimg.sh, SUPPORT_DIR/makeseg.sh
#

# check the make_flash_sh.sdk for default PARTITION_ENTRY_1 size
PARTITION_ENTRY_1=${PARTITION_ENTRY_1:-0x000F8000}
PARTITION_TABLE_SIZE=${PARTITION_TABLE_SIZE:-12}
PARTITION_TABLE_START=${PARTITION_TABLE_START:-$(($PARTITION_ENTRY_1 - $PARTITION_TABLE_SIZE))}
RODATASET_MAXSIZE=${RODATASET_MAXSIZE:-$((0xf000))}
RWDATASET_MAXSIZE=${RWDATASET_MAXSIZE:-$((0xf000))}
RWDATASET_START=${RWDATASET_START:-$(($PARTITION_TABLE_START - $RWDATASET_MAXSIZE))}
RODATASET_START=${RODATASET_START:-$(($RWDATASET_START - $RODATASET_MAXSIZE))}
RAW_FILENAME=${RAW_FILENAME:-"${BINDIR}/raw_flashimage${FLASH_FILE_SUFFIX}"}
FLASHBIN_ORIG=${FLASHBIN_ORIG:-$RAW_FILENAME}
FLASHBIN_ORIG_SZ=$(stat --format=%s $FLASHBIN_ORIG)
FLASHBIN_NEW=${FLASHBIN_NEW:-${FLASHBIN_ORIG}.new}
RODATASET_FILENAME=${RODATASET_FILENAME:-rodsetimage.bin}

dbg()
{
    if [ "$DEBUG" ]; then
        echo $(basename $0) DEBUG: $*
    fi
}

if [ $# -eq 1 ]; then
    RODATASET_FILENAME=$1
else
    echo Usage: $0 ReadOnlyDataSetImage.bin
    exit 1
fi

if [ \! -f "$RODATASET_FILENAME" ]; then
    echo Error: $RODATASET_FILENAME file not found
    exit 1
fi

if [ \! -r "$RODATASET_FILENAME" ]; then
    echo Error: Cannot access $RODATASET_FILENAME
    exit 1
fi

if [ \! -f "${FLASHBIN_ORIG}" ]; then
    echo $0 ERROR: $FLASHBIN_ORIG not found
    exit 1
fi
if [ \! -r "${FLASHBIN_ORIG}" ]; then
    echo $0 ERROR: Cannot access $FLASHBIN_ORIG
    exit 1
fi


FLASHBIN_ORIG_SZ=$(stat --format=%s $FLASHBIN_ORIG)


# Read a particular word from a file
read_word()
{
  filename=$1
  offset=$2

  echo 0x$(dd if=$1 bs=1 count=4 skip=$offset 2> /dev/null | od -t x4 | head -1 | cut -d " " -f 2)
}

# Replace a particular word in a file with a specified word
write_word()
{
  filename=$1
  offset=$2
  word=$3

  dd if=$filename bs=1 count=$offset > ${filename}.tmp 2> /dev/null
  word32=$(printf "%08x\n" $word)

  byte0=$(echo $word32 | cut -b 7-8)
  byte1=$(echo $word32 | cut -b 5-6)
  byte2=$(echo $word32 | cut -b 3-4)
  byte3=$(echo $word32 | cut -b 1-2)

  printf "\x$byte0" >> ${filename}.tmp
  printf "\x$byte1" >> ${filename}.tmp
  printf "\x$byte2" >> ${filename}.tmp
  printf "\x$byte3" >> ${filename}.tmp

  dd if=$filename bs=1 skip=$(($offset + 4)) >> ${filename}.tmp 2> /dev/null
  cp ${filename}.tmp $filename

  rm -f ${filename}.tmp
}

replace_section()
{
    org_filename=$1
    tgt_filename=$2
    offset=$3
    length=$(($(stat --format=%s $tgt_filename)))

    dd if=$org_filename bs=1 count=$offset > ${org_filename}.tmp 2> /dev/null
    dd if=$tgt_filename bs=1 count=$length >> ${org_filename}.tmp 2> /dev/null
	offset=$(($offset + $length)) 
    dd if=$org_filename bs=1 skip=$(($offset)) >> ${org_filename}.tmp 2> /dev/null
    mv ${org_filename}.tmp $org_filename
}

dbg Start $*

sizeof_flash_segment_hdr=12
flash_desc=$(read_word $FLASHBIN_ORIG  $((8*4)))
# echo flash_desc=$flash_desc
flash_desc_sz=$((8*4)) # 8 words in flash_descriptor
cfg_hdr_sz=$((9*4))
mem_addr=$(read_word $FLASHBIN_ORIG  $(($FLASHBIN_ORIG_SZ - 28)))
mem_addr=$(printf "%x" $mem_addr)
# echo mem_addr=$mem_addr

# Strip out termination
dd if=$FLASHBIN_ORIG of=$FLASHBIN_NEW bs=1 count=$((FLASHBIN_ORIG_SZ - 28)) 2> /dev/null

# Append ReadOnly DataSets
seg_length=$(stat --format=%s $RODATASET_FILENAME)
if [ $seg_length -gt 65536 ]; then
    echo Error: $RODATASET_FILENAME is too large
    rm $FLASHBIN_NEW
    exit
fi

# echo ROdset length is $seg_length
flash_addr=0x20000
${TOOLDIR}/makeimg.sh \
        -out seg_hdr \
        -new \
        -word 0x48534c46 \
        -word $flash_addr \
        -word $seg_length
cat seg_hdr $RODATASET_FILENAME > seg
${TOOLDIR}/makeseg.sh \
        -out $FLASHBIN_NEW \
        -nc \
        -data seg $mem_addr
mem_addr=$(printf "%x" $((0x$mem_addr + $seg_length + $sizeof_flash_segment_hdr)))

# Append termination
flash_addr=0
seg_length=0
${TOOLDIR}/makeimg.sh \
        -out seg_hdr \
        -new \
        -word 0x48534c46 \
        -word $flash_addr \
        -word $seg_length
${TOOLDIR}/makeseg.sh \
        -o $FLASHBIN_NEW \
        -nc \
        -data seg_hdr $mem_addr \
        -done
rm -f seg_hdr seg

# Adjust total length, first word of cfg_hdr
FLASHBIN_NEW_SZ=$(($(stat --format=%s $FLASHBIN_NEW) - $cfg_hdr_sz))
write_word $FLASHBIN_NEW  0  $FLASHBIN_NEW_SZ
# Just use repalce_section function
rm -f $FLASHBIN_NEW
replace_section $FLASHBIN_ORIG $RODATASET_FILENAME $RODATASET_START
dbg New binary created: $FLASHBIN_ORIG
dbg End
exit 0
