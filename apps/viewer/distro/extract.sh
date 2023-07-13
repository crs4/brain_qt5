#!/bin/bash

DEFAULT_DIR=.
PKG_DIR=vicbrain

echo ""
echo "CRS4 ViC BRAIN Viewer - Self Extracting Tar File"
echo ""
echo "by CRS4/ViC - 14/09/2009"
echo ""
echo ""

echo "Please enter the installation path of the CRS4 ViC BRAIN Viewer"
printf "(default ${DEFAULT_DIR}/[${PKG_DIR}]): "
read TMP_DIR
INSTALL_DIR=`eval "echo $TMP_DIR"`

if [ -z "$INSTALL_DIR" ]; then
  INSTALL_DIR=${DEFAULT_DIR}
fi

if [ ! -d "$INSTALL_DIR" ]; then
  echo "${INSTALL_DIR} is not a directory!"
  exit 1
fi

if [ ! -w "$INSTALL_DIR" ]; then
  echo "${INSTALL_DIR} is not writable!"
  exit 1
fi

if [ -x ${INSTALL_DIR}/${PKG_DIR} ]; then
  echo ""
  echo "${INSTALL_DIR}/${PKG_DIR} already exist!"
  printf "Delete it? (default no) yes/no "
  read answer
  if [ -z answer ]; then
    answer="no"
  fi
  if [[ $answer = "yes" ]]; then
    \rm -fr ${INSTALL_DIR}/${PKG_DIR}
  else
    echo "Installation aborted!"
    exit 0
  fi
fi

echo "Extracting file into ${INSTALL_DIR}"
SKIP=`awk '/^__TARFILE_FOLLOWS__/ { print NR + 1; exit 0; }' $0`

#remember our file name

THIS=$0

# take the tarfile and pipe it into tar
tail -n +$SKIP $THIS | tar -xz -C ${INSTALL_DIR}/

#
# place any bash script here you need.
# Any script here will happen after the tar file extract.
echo ""
echo "VIC BRAIN Viewer installed!"
echo ""
#cd $TMP_DIR/$PKG_DIR
#echo ""
#sh setup.sh
#cd ..
#\rm -fr $PKG_DIR
exit 0

 # NOTE: Don't place any newline characters after the last line below.
__TARFILE_FOLLOWS__
