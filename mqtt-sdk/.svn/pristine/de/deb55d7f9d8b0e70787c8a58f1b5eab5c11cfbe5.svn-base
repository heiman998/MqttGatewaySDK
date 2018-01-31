#!/bin/bash

# Copyright (c) 2010, 2013 Qualcomm Atheros Inc.
# All rights reserved.

# Construct an image consisting of files, sections from
# files, specific words, padding, etc.  This is used, for
# instance, to construct an NVRAM image.

#PRINTF="/usr/bin/printf"
OBJDUMP=${OBJDUMP:-xt-objdump} # Can use any "objdump" if xt-objdump is not available
OBJCOPY=${OBJCOPY:-xt-objcopy} # Must use an Xtensa-capable version of objcopy

EXEC_FLAG="0"
COMPRESS_FLAG="0"
STOP_FLAG="0"
NOLOAD_FLAG="0"
INDIRECT_FLAG="0"
ENTRY_ADDR="0"
PARAM="0"
FILL_PATTERN=0xffffffff

if [ $AR6002_REV -eq 4 ]; then
        ADDR_ADJUSTMENT=0x400000
        BDDATA_ADDR="0x540654"
        EXTBDDATA_ADDR="0x5406ac"
elif [ $AR6002_REV -eq 6 ]; then
        ADDR_ADJUSTMENT=0
        BDDATA_ADDR="0x400854"
        EXTBDDATA_ADDR="0x4008ac"
elif [ $AR6002_REV -eq 7 ]; then
        ADDR_ADJUSTMENT=0
        BDDATA_ADDR="0x420854"
        EXTBDDATA_ADDR="0x4208ac"
else
        echo "$0: Error: Unexpected TARGET"
        #BDDATA_ADDR = HOST_INTEREST->hi_board_data
        #EXTBDDATA_ADDR = HOST_INTEREST->hi_board_ext_data
        exit -1
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
add_byte()
{
        word32=$($PRINTF "%08x\n" $1)
        byte0=$(echo $word32 | cut -b 7-8)
        $PRINTF "\x$byte0" >> $out_file
}
#Append 2 bytes to output file
add_short()
{
        word32=$($PRINTF "%08x\n" $1)
        byte0=$(echo $word32 | cut -b 7-8)
        byte1=$(echo $word32 | cut -b 5-6)
        $PRINTF "\x$byte0" >> $out_file
        $PRINTF "\x$byte1" >> $out_file
}
#Add image hash to secure image header. Hash is 32 bytes since SHA256 is used
add_hash()
{
    for (( i=1; i <= 64; i=$i+2 ))
        do
          j=$(($i+1))
          byte=$(echo $1 | cut -b $i-$j)
          $PRINTF "\x$byte" >> $out_file
        done
}
#Add singature to secure image header
add_signature()
{
   echo adding signature to the image
   value=`cat $1`
   echo $value
   for (( i=1; i <= 512; i=$i+2 ))
        do
          j=$(($i+1))
          byte=$(echo $value | cut -b $i-$j)
          echo $byte
          $PRINTF "\x$byte" >> $out_file
        done
}
#Add modulus of public key to secure image footer. Exponent is assumed to be 3
add_public_key_mod()
{
	if [ -f modulus_script.txt ];then
        	rm modulus_script.txt
	fi
        echo adding public key mod
        #Extract modulo of public key
        openssl pkey -in $1 -pubin -text | sed '12,29!d' > modulo
        j=0
        for line in `cat modulo`;do
        #echo $line
            for (( i=1; i <= 15; i++ ))
            do
            j=$(($j+1))
            byte=$(echo $line | cut -d ":" -f $i)
            #Skip first byte if it is zero. Zero is added by openssl to indicate
            #that next byte is signed 
            if [ "$j" -eq 1 ] && [ "$i" -eq 1 ] && [ "$byte" -eq 0 ]
                then continue
            fi
            if [[ -z "$byte" ]]
                then break
            fi
            $PRINTF "\x$byte" >> modulus_script.txt
            $PRINTF "\x$byte" >> $out_file
            done
        done
        rm -f modulo
}
#Secure image header format:
#Field Name: Field_size(bytes)
#Reserved: 4
#Reserved: 4
#Image type: 4
#Image length: 4
#Sign alg: 1
#Reserved words length: 1
#Image version: 2
#OEM ID: 2
#Product ID: 2
#Img hash: 32
#Reserved words: none
add_secure_img_hdr()
{
    imgtype=$1
    imglen=$2
    imgver=$3
    oemid=$4
    prdid=$5
    imghash=$6
    add_word 0 #Reserved
    add_word 0 #Reserved
    add_word $imgtype #Img type
    add_word $imglen #Img length
    alg=2 #Signature algorithm. Currently support only
               # SHA256_2048_RSA_PSS_TAG
    add_byte $alg
    add_byte 2 #Reserved words length
    add_short $imgver 
    add_short $oemid
    add_short $prdid
    add_hash $imghash
    add_word 0
    add_word 0
}

add_fill()
{
        fill_word=$1
        word_count=$2
        while [ $word_count -gt 0 ]; do
                add_word $fill_word
                word_count=$(($word_count - 1))
        done
}

add_name()
{
        ($PRINTF "%s" $NAME; dd if=/dev/zero bs=16 count=1) > $out_file.tmp.an 2> /dev/null
        dd if=$out_file.tmp.an bs=16 count=1 >> $out_file 2> /dev/null
        rm -f $out_file.tmp.an
}

add_nvram_magic()
{
        add_word 0x4d52564e # "NVRM"
}

add_meta()
{
        addr=$1

        flags="1"               # NVRAM_ENTRY_FLAG_VALID
        if [ $COMPRESS_FLAG -eq 1 ]; then
                flags=$(($flags | 0x4))
        fi
        if [ $STOP_FLAG -eq 1 ]; then
                flags=$(($flags | 0x10))
        fi
        if [ $INDIRECT_FLAG -eq 1 ]; then
                flags=$(($flags | 0x20))
        fi
        if [ $NOLOAD_FLAG -eq 1 ]; then
                flags=$(($flags | 0x40))
        fi

        add_word $flags
        add_word $addr
        add_word $SEGMENT_LENGTH
        add_word $ENTRY_ADDR
        add_word $PARAM
        add_name
        add_word 0 # checksum
}

# Add a file (optionally compressing it) at a specified address to the output file.
add_segment() {
        file_name=$1
        addr=$2
        
        SEGMENT_LENGTH=$(stat --format=%s $file_name) # before compression

        if [ $SEGMENT_LENGTH -eq 0 ]; then
            return
        fi

        if [ "$COMPRESS_FLAG" = "1" ]; then
            $TOOLDIR/lz77/lz77 $file_name
            file_name=$file_name.z77
        fi

        SEGMENT_LENGTH=$(stat --format=%s $file_name) # after compression

        add_meta $addr
        cat $file_name >> $out_file

        pad_bytes=$(($SEGMENT_LENGTH % 4))
        if [ $pad_bytes -ne 0 ]; then
            pad_bytes=$((4 - $pad_bytes))
            dd if=/dev/zero bs=1 count=$pad_bytes >> $out_file 2>/dev/null
        fi

        if [ "$COMPRESS_FLAG" = "1" ]; then
            rm -f $file_name
        fi
}

# Append a section from an ELF file to the output file.
add_elf_segment() {
        section_name=$1
        elf_file_name=$2
        addr_adjust=$3

        addr=0x$($OBJDUMP -hww $elf_file_name | grep " $section_name " | sed -e 's/  */ /g' | cut -d " " -f 5)
        addr=$(($addr - $addr_adjust))
        $OBJCOPY --only-section $section_name -O binary $elf_file_name $out_file.tmp.aes
        add_segment $out_file.tmp.aes $addr

        rm -f $out_file.tmp.aes
}

add_elf_file() {
        elf_file=$1

        SAVE_STOP_FLAG=$STOP_FLAG
        STOP_FLAG=0
        add_elf_segment ".literals" $elf_file 0
        add_elf_segment ".rodata" $elf_file 0
        add_elf_segment ".data" $elf_file 0
        STOP_FLAG=$SAVE_STOP_FLAG

        PARAM=$2
        if [ $EXEC_FLAG -eq 1 ]; then
                ENTRY_ADDR=$($OBJDUMP -f $elf_file | grep "start address" | sed -e 's/  */ /g' | cut -d " " -f 3)
        else
                ENTRY_ADDR=0x00000000
        fi
    
        add_elf_segment ".text" $elf_file $ADDR_ADJUSTMENT

        ENTRY_ADDR="0"
        PARAM="0"
}

add_named_elf_segment() {
        elf_file=$1
        seg_name=$2
        addr_offset=$3    
        ENTRY_ADDR=$4
        PARAM=$5
    
        add_elf_segment $seg_name $elf_file $addr_offset

        ENTRY_ADDR="0"
        PARAM="0"
}

# Reset flags to defaults
reset_flags() {
        COMPRESS_FLAG="0"
        STOP_FLAG="0"
        NAME=""
        NOLOAD_FLAG="0"
        INDIRECT_FLAG="0"
}


while [ "$#" -ne 0 ]
do
        case $1 in
        -bddata|-bd)
            INDIRECT_FLAG="1"
            add_segment $2 $BDDATA_ADDR
            reset_flags
            shift 2
            ;;
        -extbddata|-ebd)
            INDIRECT_FLAG="1"
            add_segment $2 $EXTBDDATA_ADDR
            reset_flags
            shift 2
            ;;
        -compress|-c)
            COMPRESS_FLAG="1"
            shift
            ;;
        -data|-d)
            add_segment $2 $3
            reset_flags
            shift 3
            ;;
        -exec|-e)
            EXEC_FLAG="1"
            add_elf_file $2 $3
            EXEC_FLAG="0"
            reset_flags
            shift 3
            ;;
        -fill|-f)
            add_fill $2 $3
            FILL_PATTERN=$2
            shift 3
            ;;
        -fillto)
            add_fill $2 $((($3 - $(stat --format=%s $out_file)) / 4 ))
            FILL_PATTERN=$2
            shift 3
            ;;
        -fillpat | -fp)
            FILL_PATTERN=$2
            shift 2
            ;;
        -load|-l)
            add_elf_file $2 0
            reset_flags
            shift 3
            ;;
        -segload| -sl)
            add_named_elf_segment $2 $3 $4 $5
            reset_flags
            shift 5
            ;;
        -magic|-m)
            add_nvram_magic
            shift 1
            ;;
        -name|n)
            NAME=$2
            shift 2
            ;;
        -new)
            rm -f $out_file
            shift 1
            ;;
        -noload)
            NOLOAD_FLAG="1"
            add_segment $2 0
            reset_flags
            shift 2
            ;;
        -out|-o)
            out_file=$2
            shift 2
            ;;
        -pad0to)
            num_bytes=$(( $3 - $(stat --format=%s $out_file) ))
            dd if=/dev/zero bs=$num_bytes count=1 >> $out_file
            shift 3
            ;;
        -stop|-s)
            STOP_FLAG="1"
            shift 1
            ;;
        -word|-w)
            add_word $2
            shift 2
            ;;
        -secureimghdr)
            add_secure_img_hdr $2 $3 $4 $5 $6 $7
            shift 7
            ;;
        -addsignature)
            add_signature $2
            shift 2
            ;; 
        -publickeymod)
            add_public_key_mod $2
            shift 2
            ;;
        *)
            echo "$0: Unsupported argument: " "$1"
            exit -1
            shift
        esac
done
