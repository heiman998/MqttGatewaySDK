#!/bin/bash
# @file
#################################################################################
# Copyright 2015 Qualcomm Connected Experiences, Inc.
#################################################################################
#
# Add AllJoyn DataSets to the ReadOnly DataSet Image.
# Parameters:
#  $1 = Directory containing AllJoyn NVM files
#  $2 = name of RO DataSet Image to append to
PRINTF="/usr/bin/printf"

if [ $# -eq 2 ] ; then
    aj_data=$1
    out_file=$2
else
    aj_data=${HTMLTOFLASH_LIST:-"aj_dsets"}
    out_file=${RODATASET_FILENAME:-"rodsetimage.bin"}
fi


# These DSET ID's come from the following definitions
# DSETID_ALLJOYN_START == 0x00000800 in .../wlanfw_cmn.ioe/include/dsetid.h
# AJ_FACTORY_DATA_ID == DSETID_ALLJOYN_START+1 in .../aj.ioe/alljoyn/aj_qca4004/src/aj_target_nvram.h
# AJ_RO_DATA_ID == DSETID_ALLJOYN_START+2 in .../aj.ioe/alljoyn/aj_qca4004/src/aj_target_nvram.h
# AJ_JS_SCRIPT == DSETID_ALLJOYN_START+3 in .../aj.ioe/alljoyn/aj_qca4004/src/aj_target_nvram.h
aj_factory_dset_id=0x801
aj_ro_dset_id=0x802
aj_js_dset_id=0x803

if [ \! -e "$aj_data" ]; then
    echo no AllJoyn dset data found
    exit 0
fi

# Append a 32-bit word to the output file.
add_word()
{
	word32=$($PRINTF "%08x\n" $1)

	byte0=$(echo $word32 | cut -b 7-8)
	byte1=$(echo $word32 | cut -b 5-6)
	byte2=$(echo $word32 | cut -b 3-4)
	byte3=$(echo $word32 | cut -b 1-2)

	$PRINTF "\x$byte0" >> $out_file
	$PRINTF "\x$byte1" >> $out_file
	$PRINTF "\x$byte2" >> $out_file
	$PRINTF "\x$byte3" >> $out_file
}

add_short()
{
	word16=$($PRINTF "%04x\n" $1)

	byte0=$(echo $word16 | cut -b 3-4)
	byte1=$(echo $word16 | cut -b 1-2)

	$PRINTF "\x$byte0" >> $out_file
	$PRINTF "\x$byte1" >> $out_file
}

if [ -n "$(ls $aj_data/*js.nvm 2>/dev/null)" ]; then
	# DSET for allJoyn-js Script
	add_word $aj_js_dset_id

	# Only one script is allowed
	script=$(ls -1 $aj_data/*js.nvm | head -1)
	filesz=$(stat --format=%s $script)

	# Calculated size of Script DSET
	dset_total=$(($filesz+4))
	add_word $dset_total

	echo AJ-JS Script Data: $script, size $filesz

	# Add Sentinel
	$PRINTF "AJNV" >> $out_file

	# Add actual Script
	cat $script >> $out_file
	pad_bytes=$(($dset_total % 4))
	if [ $pad_bytes -ne 0 ]; then
		pad_bytes=$((4 - $pad_bytes))
		echo AllJoyn Script DSET Pad Bytes: $pad_bytes
		dd if=/dev/zero bs=1 count=$pad_bytes >> $out_file 2>/dev/null
	fi
fi

# AllJoyn data needs to be padded to 16 bit boundaries
# DSETs need to be padded to 32 bit boundaries
if [ -n "$(ls $aj_data/*ro_*.nvm 2>/dev/null)" ]; then
	# DSET for AllJoyn Read-Only data
	add_word $aj_ro_dset_id
	dset_total=4
	# Calculate size of DSET
	for i in $(ls $aj_data/*ro_*.nvm); do
		filesz=$(stat --format=%s $i)
		id=$(stat --format=%n $i | sed -e"s/.*ro_/0x/" -e"s/.nvm$//")
		echo AJ RO Data: $i, size $filesz AJ_ID: $id
		pad_bytes=$(($filesz % 2))
		dset_total=$(($dset_total+$filesz+$pad_bytes+4))
	done

	# Calculated size of Read only DSET
	add_word $dset_total
	echo AllJoyn Read-Only DSET Size: $dset_total

	# Add Sentinel
	$PRINTF "AJNV" >> $out_file

	for i in $(ls $aj_data/*ro_*.nvm); do
		filesz=$(stat --format=%s $i)
		id=$(stat --format=%n $i | sed -e"s/.*ro_/0x/" -e"s/.nvm$//")
		pad_bytes=$(($filesz % 2))
		filesz=$(($filesz+$pad_bytes))
		add_short $id
		add_short $filesz
		cat $i >> $out_file
		if [ $pad_bytes -ne 0 ]; then
			dd if=/dev/zero bs=1 count=1 >> $out_file 2>/dev/null
		fi
	done
	pad_bytes=$(($dset_total % 4))
	if [ $pad_bytes -ne 0 ]; then
		pad_bytes=$((4 - $pad_bytes))
		echo AllJoyn Read-Only DSET Pad Bytes: $pad_bytes
		dd if=/dev/zero bs=1 count=$pad_bytes >> $out_file 2>/dev/null
	fi
fi

echo

if [ -n "$(ls $aj_data/*rw_*.nvm 2>/dev/null)" ]; then
	# DSET for AllJoyn Factory Defaults of R/W data
	add_word $aj_factory_dset_id
	dset_total=4
	# Calculate size of DSET
	for i in $(ls $aj_data/*rw_*.nvm); do
		filesz=$(stat --format=%s $i)
		id=$(stat --format=%n $i | sed -e"s/.*rw_/0x/" -e"s/.nvm$//")
		echo AJ RW Data: $i, size $filesz AJ_ID: $id
		pad_bytes=$(($filesz % 2))
		dset_total=$(($dset_total+$filesz+$pad_bytes+4))
	done

	# Calculated size of Factory Default DSET
	add_word $dset_total
	echo AllJoyn Factory Default DSET Size: $dset_total

	# Add Sentinel
	$PRINTF "AJNV" >> $out_file

	for i in $(ls $aj_data/*rw_*.nvm); do
		filesz=$(stat --format=%s $i)
		id=$(stat --format=%n $i | sed -e"s/.*rw_/0x/" -e"s/.nvm$//")
		pad_bytes=$(($filesz % 2))
		filesz=$(($filesz+$pad_bytes))
		add_short $id
		add_short $filesz
		cat $i >> $out_file
		if [ $pad_bytes -ne 0 ]; then
			dd if=/dev/zero bs=1 count=1 >> $out_file 2>/dev/null
		fi
	done
	pad_bytes=$(($dset_total % 4))
	if [ $pad_bytes -ne 0 ]; then
		pad_bytes=$((4 - $pad_bytes))
		echo AllJoyn Factory Default DSET Pad Bytes: $pad_bytes
		dd if=/dev/zero bs=1 count=$pad_bytes >> $out_file 2>/dev/null
	fi
fi

exit 0
