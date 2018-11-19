#!/bin/bash

image="test4"
ext="png"

#preprocess flowfield kernel 5 10 iterations
#./CLD-cli ./$image $ext 5 10 2 2 .75 .98 .999 1

#run cld on precomputed flowfield
./CLD-cli ./$image $ext 5 10 2 2 .75 .98 .999 0

#use potrace for antialiasing
potrace -g -o $image\_potrace.pgm $image\_result.pgm

#clean potrace image (bug)
convert $image\_potrace.pgm $image\_result.png

#gmic montage
gmic -i $image\.png -to_colormode[0] 3 -i $image\_result.png -to_colormode[1] 3 -i $image\_flowfield.exr div[2] 2 add[2] 1 -n[2] 0,255 -channels[2] 2,1 -montage H -o $image\_montage.png
