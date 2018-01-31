#!/bin/bash

which wget > /dev/null 2>&1
if [ $? -ne 0 ];
then
    echo Please install "wget" in order to fetch the Xtensa toolchain.
    exit 1
fi

echo
echo Download Xtensa RE-2013.3 toolchain installation packages from Cadence.
echo Note This may take many minutes.  Total size is nearly 2GB.
echo Tool installation packages will be placed at $(pwd)/RE-2013.3-pgblrm
echo

wget --user=ten_dev1 --password=BoxKO0b0 --no-host-directories --no-verbose -r ftp://ftp.cadence.com/RE-2013.3-pgblrm

chmod 755 RE-2013.3-pgblrm/Xplorer-5.0.3-linux-installer.bin
chmod 755 RE-2013.3-pgblrm/xt-ocd-10.0.3-linux-installer

echo
echo Tool installation packages are now at $(pwd)/RE-2013.3-pgblrm
echo
