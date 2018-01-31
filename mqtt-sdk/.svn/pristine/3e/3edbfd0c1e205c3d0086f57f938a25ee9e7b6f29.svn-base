#!/bin/bash

# Create segments in a downloadable "segmented file".
# Such a file is easily downloaded from Host to Target.
# Segments are APPENDED to the output file.
#
# An "exec" file adds segments for literals, rodata, data, and text
# and it triggers execution immediately after the download.
#
# A "begin" file adds segments for literals, rodata, data, and text
# and it sets the app_start address appropriately.
#
# A "bddata" file loads from a board data file to the correct place
# in Target RAM.
#
# A "data" file is placed at a specific address, specified on the
# command line
#
# Examples:
#	makeseg.sh -o /tmp/fw.bin -new
# 	makeseg.sh -o /tmp/fw.bin -exec targsetup.out
#	makeseg.sh -o /tmp/fw.bin -bddata board_data.bin
#	makeseg.sh -o /tmp/fw.bin -exec otp.out
#	makeseg.sh -o /tmp/fw.bin -begin athwlan.out
#	makeseg.sh -o /tmp/fw.bin -done
#
# Example like above, but rolled into a single command line:
#	$targ/support/makeseg.sh -o /tmp/fw.bin -new -exec targsetup.out -bddata board_data.bin -exec otp.out -begin athwlan.out -done
#
#	makeseg.sh -o /tmp/fw.bin -data file.bin 0x?????????

# Default output filename
out_file="fw.bin"
OBJDUMP=${OBJDUMP:-objdump}
OBJCOPY=${OBJCOPY:-objcopy}
COMPRESS_FLAG="1"
EX_INIT_TEXT_FLAG="0"
RAW_BIN_FLAG="0"
raw_out_file="athwlan_ext.bin"

# Append a 32-bit word to the output file.
print_32bit()
{
	word32=$(printf "%08x\n" $1)

	byte0=$(echo $word32 | cut -b 7-8)
	byte1=$(echo $word32 | cut -b 5-6)
	byte2=$(echo $word32 | cut -b 3-4)
	byte3=$(echo $word32 | cut -b 1-2)

	printf "\x$byte0" >> $out_file
	printf "\x$byte1" >> $out_file
	printf "\x$byte2" >> $out_file
	printf "\x$byte3" >> $out_file
}

# Add a file (optionally compressing it) at a specified address to the output file.
add_segment() {
	file_name=$1
	addr=$2

	length=$(stat --format=%s $file_name)

	if [ $length -eq 0 ]; then
	    return
	fi

	if [ "$COMPRESS_FLAG" = "1" ]; then
	    $TOOLDIR/lz77/lz77 $file_name
	    file_name=$file_name.z77
	fi

	print_32bit 0x$addr
	print_32bit $length
	cat $file_name >> $out_file

	if [ "$COMPRESS_FLAG" = "1" ]; then
	    rm -f $file_name
	fi
}


# Append a section from an ELF file to the output file.
add_elf_segment() {
	section_name=$1
	elf_file_name=$2

	addr=$($OBJDUMP -hww $elf_file_name | grep " $section_name " | sed -e 's/  */ /g' | cut -d ' ' -f 5)
        if [ -n "$addr" ]; then
	    tmp_name=${TMPDIR:-/tmp}/segmented.$$
	    $OBJCOPY --target=elf32-little --only-section $section_name -O binary $elf_file_name $tmp_name
	    add_segment $tmp_name $addr
	    rm -f $tmp_name
	fi
}

# Append a section from an ELF file to the output file.
print_32bit_ex()
{
	word32=$(printf "%08x\n" $1)
	
	byte0=$(echo $word32 | cut -b 7-8)
	byte1=$(echo $word32 | cut -b 5-6)
	byte2=$(echo $word32 | cut -b 3-4)
	byte3=$(echo $word32 | cut -b 1-2)

	printf "\x$byte0" >> $raw_out_file
	printf "\x$byte1" >> $raw_out_file
	printf "\x$byte2" >> $raw_out_file
	printf "\x$byte3" >> $raw_out_file
}

copy_elf_segment() {
	section_name=$1
	elf_file_name=$2

	addr=$($OBJDUMP -hww $elf_file_name | grep " $section_name " | sed -e 's/  */ /g' | cut -d ' ' -f 5)
        if [ -n "$addr" ]; then
	    tmp_name=${TMPDIR:-/tmp}/segmented_ext.$$
	    $OBJCOPY --target=elf32-little --only-section $section_name -O binary $elf_file_name $tmp_name
		
		length=$(stat --format=%s $tmp_name)
		print_32bit_ex 0x$addr
		print_32bit_ex $length
		echo $addr
		echo $length
		cat $tmp_name >> $raw_out_file
		rm -f tmp_name
	fi
}

add_elf_file() {
	elf_file=$1

    	add_elf_segment ".literals" $elf_file
if [ "$RAW_BIN_FLAG" = "1" ]; then
    	copy_elf_segment ".literals" $elf_file
fi
    	add_elf_segment ".rodata" $elf_file
if [ "$RAW_BIN_FLAG" = "1" ]; then
    	copy_elf_segment ".rodata" $elf_file
fi
    	add_elf_segment ".data" $elf_file
if [ "$RAW_BIN_FLAG" = "1" ]; then
    	copy_elf_segment ".data" $elf_file
fi
    	add_elf_segment ".text" $elf_file
if [ "$EX_INIT_TEXT_FLAG" = "1" ]; then
    	add_elf_segment ".ex_init_text" $elf_file
fi
if [ "$RAW_BIN_FLAG" = "1" ]; then
    	copy_elf_segment ".text" $elf_file
fi
}

# Write a segmented file header to the output file.
add_segmented_header() {
	print_32bit "0x544D4753" # "SGMT"
	print_32bit $COMPRESS_FLAG
}

add_segment_fileaddr() {
	elf_file_name=$1
	trail_value=$2
	addr=$($OBJDUMP -f $elf_file_name | grep "start address" | cut -b 17-24)

	print_32bit 0x$addr
	print_32bit 0x$trail_value
}

# Finish a segmented file.
add_segment_done() {
	print_32bit 0x00000000
	print_32bit 0xffffffff # BMI_SGMTFILE_DONE

}

add_bddata_segment() {
	file_name=$1
	
	length=$(stat --format=%s $file_name)
	if [ $length -eq 0 ]; then
	    return
	fi

	if [ "$COMPRESS_FLAG" = "1" ]; then
	    $TOOLDIR/lz77/lz77 $file_name
	    file_name=$file_name.z77
	fi

	print_32bit $length
	print_32bit 0xfffffffe # BMI_SGMTFILE_BDDATA
	cat $file_name >> $out_file

	if [ "$COMPRESS_FLAG" = "1" ]; then
	    rm -f $file_name
	fi
}


# Append a "begin" segment based on a specified output file.
add_segment_begin() {
	add_segment_fileaddr $1 fffffffd # BMI_SGMTFILE_BEGINADDR

}

# Append an "exec" segment based on a specified output file.
add_segment_exec() {
	add_segment_fileaddr $1 fffffffc # BMI_SGMTFILE_EXEC

}

while [ "$#" -ne 0 ]
do
    case $1 in
    -bddata|-bd)
	add_bddata_segment $2
	shift 2
	;;
    -begin|-i)
	add_elf_file $2
    	add_segment_begin $2
        shift 2
        ;;
    -setbegin)
    	add_segment_begin $2
        shift 2
        ;;
    -data)
    	add_segment $2 $3
	shift 3
	;;
    -done|-end)
    	add_segment_done
	shift 1
	;;
    -exec|-e)
	add_elf_file $2
    	add_segment_exec $2
        shift 2
        ;;
    -load)
	add_elf_file $2
        shift 2
        ;;
    -nc|-nocompress)
        COMPRESS_FLAG="0"
        shift 1
        ;;
	-ex_init_text)
        EX_INIT_TEXT_FLAG="1"
        shift
	;;
	-raw)
        RAW_BIN_FLAG="1"
		raw_out_file=$2
		rm -f $raw_out_file
        shift 2
	;;
    -new)
    	rm -f $out_file
    	add_segmented_header
        shift 1
        ;;
    -out|-o)
        out_file=$2
        shift 2
        ;;
    * )
        echo "Unsupported argument"
        exit -1
        shift
    esac
done