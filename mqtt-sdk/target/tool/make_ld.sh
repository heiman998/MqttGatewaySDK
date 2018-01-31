#!/bin/sh
# Copyright (c) 2008 Atheros Communications Inc.
# All rights reserved.
# $ATH_LICENSE_TARGET_MK$

# This script is intended for use with separately linked ROM and RAM.
# It creates EITHER a linker script that satisfies references made by RAM
# applications to ROM symbols OR a linker script that forces symbols to
# be included in a ROM image.
#
# The option "--addrs" causes the RAM linkage script to be generated
# and the option "--externs" causes the ROM linkage script to be generated.
#
# Example usage:
#     make_ld.sh --addrs sw.rom.out rom.symbols > rom.addrs.ld
#     make_ld.sh --externs rom.symbols > rom.externs.ld

AR6002_NM=xt-nm
eval NM=\$"$TARGET"_NM

Usage() {
    echo Usage:
    echo $progname '{--addrs ROM_ELF_Image | --externs} symbol_file'
    exit 1
}

Provide() {
    addr=0x`grep " $1$" $nm_file | cut -d ' ' -f 1`
    if [ "$addr" != "0x" ]
    then
    	echo PROVIDE \( $1 = $addr \)\;
    fi
}

Extern() {
    echo EXTERN \( $1 \)\;
}

progname=$0
script_choice=$1

if [ "$script_choice" = "--addrs" ]
then
    action=Provide
    image_file=$2
    if [ ! -r "$image_file" ]
    then
        echo "Cannot read ELF image: $image_file"
        Usage
    fi
    symbol_file=$3
    nm_file=/tmp/make_ld.nm.$$
    $NM $image_file > $nm_file
elif [ "$script_choice" = "--externs" ]
then
    action=Extern
    symbol_file=$2
else
    Usage
fi

if [ ! -r "$symbol_file" ]
then
    echo "Cannot read symbol list from: $symbol_file"
    Usage
fi

for i in `cat $symbol_file`
do
    $action $i
done
rm -rf $nm_file
