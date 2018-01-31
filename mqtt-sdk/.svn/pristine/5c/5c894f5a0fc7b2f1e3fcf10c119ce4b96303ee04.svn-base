#!/bin/bash

# Add a DataSet to the ReadOnly DataSet Image.
# Parameters:
#  $1 = dset_id
#  $2 = name of file containing DataSet bytes
#  $3 = name of RO DataSet Image to append to

dset_id=$1
filename=$2
out_file=$3
if [ \! -r "$filename" ]; then
    echo $0 ERROR: Cannot access $filename
    exit 1
fi
filesz=$(stat --format=%s $filename)

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


# DataSet ID
add_word $dset_id

# DataSet size in bytes
add_word $filesz

# DataSet
cat $filename >> $out_file

# Pad to 4 byte boundary
pad_bytes=$(($filesz % 4))
if [ $pad_bytes -ne 0 ]; then
    pad_bytes=$((4 - $pad_bytes))
    dd if=/dev/zero bs=1 count=$pad_bytes >> $out_file 2>/dev/null
fi

exit 0
