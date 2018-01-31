#!/bin/bash

HTMLTOFLASH_LIST=${HTMLTOFLASH_LIST:-"filelist"}
RODATASET_FILENAME=${RODATASET_FILENAME:-"rodsetimage.bin"}

export AR6002_REV=${AR6002_REV:-7}

dbg()
{
    if [ "$DEBUG" ]; then
        echo $(basename $0) DEBUG: $*
    fi
}

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

add_string()
{
    $PRINTF "%-32s" $1 >> $out_file
}

dbg Start $0 $*

if [ \! -f $HTMLTOFLASH_LIST ]; then
    echo $0 ERROR: "File \"${HTMLTOFLASH_LIST}\" not found"
    exit 1
fi

out_file=mapfile
filesz=0
dset_id=0x401    # Start Dataset ID for Static HTML pages 
dbdset_id=0x421  # Start Dataset ID for DB for HTML pages

nooffiles=$(wc -l $HTMLTOFLASH_LIST | awk '{print $1}')
dbg $0: No of files:$nooffiles
if [ $nooffiles -ge 8 ]; then
    echo $0 Error: Max 7 html files are supported
    exit 1
fi

# Delete the existing file
rm -f $out_file

# Create a mapfile
cat $HTMLTOFLASH_LIST | while read a; do \
                        add_string $a; \
                        add_word $dset_id; \
                        dset_id=$(( $dset_id + 1 )); \
                        add_word $dbdset_id; \
                        dbdset_id=$(( $dbdset_id + 1 ));\
                        done

# Now create a Dset for files
dset_id=0x401

# Delete the previous RODSET
rm -f $RODATASET_FILENAME

# Creating dataset for map file
./mkrodset.sh 0x400 $out_file $RODATASET_FILENAME
cat $HTMLTOFLASH_LIST | while read a; do \
                        ./mkrodset.sh $dset_id $a $RODATASET_FILENAME; \
                        if [ $? -ne 0 ]; then \
                            echo $0 ERROR: Missing HTML file, $a; \
                            exit 1; \
                        fi; \
                        dset_id=$(( $dset_id + 1 )); \
                        done

rm -f $out_file

dbg End $0

exit 0
