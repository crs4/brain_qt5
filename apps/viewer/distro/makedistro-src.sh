PKG_NAME=brain-crs4
PKG_VERSION=2.0.1
PKG_NAME_VERSION=$PKG_NAME-$PKG_VERSION
PKG_NAME_TGZ=$PKG_NAME_VERSION.tgz

PKG_ROOT_DIR=/home/lfabio/svnarea/vic/trunk
TMP_DIR=/usr/local/tmp/tmp12345


DEPLIST=(Qt3Support QtSql QtXml QtNetwork QtOpenGL QtGui QtCore z stdc++ png12 freetype )
DEPLIST_COUNT=${#DEPLIST[@]}

delete_tmp () {
  if [ -d $TMP_DIR ]; then
    \rm -fr $TMP_DIR
  fi
}

echo ""
echo "CRS4 VIC BRAIN - Creating src distro"
echo "by CRS4/ViC - 30/09/2009"

delete_tmp

LOCAL_DIR=`pwd`

mkdir -p $TMP_DIR
cp -r $PKG_ROOT_DIR/software/qmakeconfig $TMP_DIR
cp -r $PKG_ROOT_DIR/playground/brain $TMP_DIR

pushd $TMP_DIR/qmakeconfig
make distclean
popd
pushd $TMP_DIR/brain
make distclean
popd

find $TMP_DIR -name ".svn" -exec rm -fr {} \;
find $TMP_DIR -name "Release" -exec rm -fr {} \;
find $TMP_DIR -name "Debug" -exec rm -fr {} \;
autopurge $TMP_DIR

rm -fr $TMP_DIR/brain/apps/viewer/distro
rm -fr $TMP_DIR/brain/apps/viewer/distro
rm -fr $TMP_DIR/brain/data
rm $TMP_DIR/brain/addheader.sh
rm $TMP_DIR/brain/copyright.txt

pushd $TMP_DIR
tar zcvf $LOCAL_DIR/$PKG_NAME_TGZ qmakeconfig brain
popd

delete_tmp
