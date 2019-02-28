#!/bin/bash

inputfolder="polys/"

for i in $inputfolder*.poly; do
	echo $i
	../../karambola $i --labels
done
