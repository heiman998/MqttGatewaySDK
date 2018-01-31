#!/bin/bash

# The task of this script is to create a ReadOnly DataSet
# Image -- including meta-data and data -- in $RODATASET_FILENAME.

# Remove old RO DataSet image, if any, and start fresh
rm -f $RODATASET_FILENAME

# Below are two examples of how to create a DataSet image.
# There are many other ways, as well.

# The first example simply places contents of three files
# into DataSets 1125, 1126, and 1127.
# The second example uses a script to create a mapping
# file that works with the embedded HTML server. The 
# mapping file contains filename-->dsetID mappings and it
# is stored in the RO DataSet Area of flash along with
# a DataSet for each of the HTML files.

# Example: Create a DataSet image with these ReadOnly DataSets
# ./mkrodset.sh 0x1127 rodset.1127.bin $RODATASET_FILENAME
# ./mkrodset.sh 0x1125 rodset.1125.bin $RODATASET_FILENAME
# ./mkrodset.sh 0x1126 rodset.1126.bin $RODATASET_FILENAME

# Example: Create a DataSet image with HTML files
# if [ -f $HTMLTOFLASH_SCRIPT ]; then
#     sh $HTMLTOFLASH_SCRIPT
# else
#     sh ./htmltoflash.sh
# fi
