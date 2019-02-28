#!/bin/bash

inputfolder="input_xyz/"
outputfolder="polys/"

if [ -d $outputfolder ]; then rm -r $outputfolder; fi
mkdir $outputfolder

for i in $inputfolder*.xyz; do
    outputfilename=$outputfolder${i#$inputfolder}
    outputfilename=${outputfilename%xyz}"poly"
    ../pointpattern2voronoi3d/pointpattern2voronoi3d -i $i -o $outputfilename
done
