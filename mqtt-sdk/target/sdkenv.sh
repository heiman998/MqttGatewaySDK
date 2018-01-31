
# Set up environment variables in order to build from IoE SDK source
# If SDK_ROOT is not set, this script must be sourced from within
# a script or from a non-login bash shell using:
#
#  . sdkenv.sh

XTENSA_CORE=KF1_prod_rel_2012_4
XTENSA_TOOLS_ROOT=/cad/tensilica/tools/RD-2012.4-linux/XtensaTools
XTENSA_ROOT=/cad/tensilica/chips/kingfisher/RD-2012.4-linux/$XTENSA_CORE

XTENSA_SYSTEM=$XTENSA_ROOT/config
#LM_LICENSE_FILE=/home/helenxu/xtensa/license.dat 
LM_LICENSE_FILE=~/xtensa/license.dat
PATH=$PATH:$XTENSA_TOOLS_ROOT/bin

export LM_LICENSE_FILE XTENSA_TOOLS_ROOT XTENSA_ROOT XTENSA_SYSTEM XTENSA_CORE PATH 

	
export SDK_ROOT=${SDK_ROOT:-$(pwd)}
export FW=$SDK_ROOT
export INTERNALDIR=$FW/Internal
export SRC_IOE=$SDK_ROOT
export IMAGEDIR=$SRC_IOE/image
export LIBDIR=$SRC_IOE/lib
export BINDIR=$SRC_IOE/bin
export TOOLDIR=$SRC_IOE/tool
export PRINTF="/usr/bin/printf"
export SDK_VERSION_IOE=REV76
export AR6002_REV7_VER=6
