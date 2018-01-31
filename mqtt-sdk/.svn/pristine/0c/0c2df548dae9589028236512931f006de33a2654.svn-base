#!/bin/bash

# Copyright (c) 2014 Qualcomm Atheros Inc.
# All rights reserved.

# Create an image to be used with an Over The Air (OTA) flash firmware upgrade.
# Works by prepending an OTA header to a partition flash image.
# Usage ./ota.sh input_image ota_output_image

#PRINTF="/usr/bin/printf"

dbg()
{
    if [ "$DEBUG" ]; then
        echo $0: $*
    fi
}

# Append a 32-bit word to the output file.
print_32bit()
{
	word32=$($PRINTF "%08x\n" $1)

	byte0=$(echo $word32 | cut -b 7-8)
	byte1=$(echo $word32 | cut -b 5-6)
	byte2=$(echo $word32 | cut -b 3-4)
	byte3=$(echo $word32 | cut -b 1-2)

	$PRINTF "\x$byte3" >> $out_file
	$PRINTF "\x$byte2" >> $out_file
	$PRINTF "\x$byte1" >> $out_file
	$PRINTF "\x$byte0" >> $out_file
}


# Add a file (optionally compressing it) at a specified address to the output file.
add_ota_header() {
	in_file=$1
	out_file=$2
	LENGTH=$(stat --format=%s $in_file)
	dbg original image length=$LENGTH

	# Remove the leading 6 words which are always in the raw_flashimage:
	#   pointer to partition table
	#   3 TBD words
	#   1 Magic number word
	#   1 partition age word
	# These last two words will be added back by OTA Upgrade firmware
	# when the data is committed to flash.
	let LENGTH=LENGTH-24

	tail -c $LENGTH $in_file > tmp.bin

	MD5=$(md5sum tmp.bin | cut -f1 -d " ")
	dbg md5=$MD5
	
	rm -f $out_file

	print_32bit 0x1234
	print_32bit $LENGTH
	print_32bit 0x$(echo $MD5 | cut -b 1-8)
	print_32bit 0x$(echo $MD5 | cut -b 9-16)
	print_32bit 0x$(echo $MD5 | cut -b 17-24)
	print_32bit 0x$(echo $MD5 | cut -b 25-32)
	cat tmp.bin >> $out_file
	rm -f tmp.bin
}

dbg Start $*

if [ \! -r $1 ]; then
    echo $0 Error: Cannot access $1
    exit 1
fi

add_ota_header $1 $2

if [ \! -f "$2" ]; then
    echo $0 Error: failed to create $2
    exit 1
else
    echo Creating image: $(basename "$2")
fi

dbg End
exit 0
