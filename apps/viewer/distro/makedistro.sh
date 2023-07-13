PKG_DIR=vicbrain
PKG_VERSION=2.0.0
PKG_NAME=$PKG_DIR-$PKG_VERSION
PKG_NAME_TGZ=$PKG_NAME.tgz
PKG_NAME_SH=$PKG_NAME-installer.sh
PKG_LIB=lib
PKG_SHADERS=shaders

declare -a DEPLIST
DEPLIST=(Qt3Support QtSql QtXml QtNetwork QtOpenGL QtGui QtCore z stdc++ png12 freetype )
DEPLIST_COUNT=${#DEPLIST[@]}

delete_tmp () {
  if [ -d $PKG_DIR ]; then
    \rm -fr $PKG_DIR
  fi
  if [ -e $PKG_NAME_TGZ ]; then
    \rm $PKG_NAME_TGZ
  fi
}

echo ""
echo "VIC BRAIN Viewer - Self Extracting Tar File Builder"
echo "by CRS4/ViC - 14/09/2009"
echo ""
echo "*******"
echo "WARNING! This shell script must be run after the BRAIN viewer has been compiled!"
echo "*******"
echo ""


delete_tmp

mkdir -p $PKG_DIR/$PKG_LIB
cp ../vic_brain_viewer-bin $PKG_DIR
cp ./vic_brain_viewer $PKG_DIR

#cp setup.sh $PKG_DIR

mkdir -p $PKG_DIR/$PKG_SHADERS
############## SHADERS ################
#cp ../shaders/*.cg $PKG_DIR/$PKG_SHADERS
#######################################

for (( i=0;i<$DEPLIST_COUNT;i++)); do
  LIBNAME=`ldd $PKG_DIR/vic_brain_viewer-bin | awk '{ print $3 }' | grep ${DEPLIST[$i]}`
  if [[ $LIBNAME ]] 
  then
    cp $LIBNAME $PKG_DIR/$PKG_LIB
  fi
done

tar zcvf $PKG_NAME_TGZ $PKG_DIR
cat extract.sh $PKG_NAME_TGZ > $PKG_NAME_SH
chmod a+x $PKG_NAME_SH
delete_tmp

echo ""
echo "File: $PKG_NAME_SH CREATED!"
echo ""
