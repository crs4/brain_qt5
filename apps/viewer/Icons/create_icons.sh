#!/bin/bash
logo_img=vic-brain.svg
logo_img_png=${logo_img%%.svg}.png
for i in 16 22 24 32 48 64 96
do
    icon_sz=${i}x${i}
    echo "Creating  ${icon_sz}/${logo_img} ..."
    [ -d ${icon_sz} ] || mkdir ${icon_sz}
    convert -resize ${icon_sz} ${logo_img}  ${icon_sz}/${logo_img_png}
done
