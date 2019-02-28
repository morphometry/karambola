#!/bin/bash

inputfolder="polys/"
outputfolder="output_minkowski/"
if [ -d $outputfolder ]; then rm -r $outputfolder; fi
mkdir $outputfolder

outputfilenamestart="result"

for w in "w020" "w120" "w220" "w320" "w102" "w202"; do
	echo "#density #beta_"$w > $outputfolder$outputfilenamestart"_"$w".txt"
done

for i in $inputfolder*.poly; do


	tmp=${i#*hs-*_}
	Phi=${tmp%.poly}
	echo "Phi = " $Phi

	minkfilefolder=${i%.poly}_mink_val/

	for w in "w020" "w120" "w220" "w320" "w102" "w202"; do

			grep -v "^\#" $minkfilefolder$w"_eigsys" | awk '
			{sum+=$2/$10; sumsq+=$2/$10*$2/$10}
			END{	anzahl=$1;
					var=(sumsq/anzahl-sum*sum/(anzahl*anzahl))*anzahl/(anzahl-1);
					printf "'$Phi' %.12f %.12f \n" , sum/anzahl, sqrt(var) >> "'$outputfolder$outputfilenamestart'_'$w'.txt"}
			';

	done

done

gnuplot gnuplot.plt
