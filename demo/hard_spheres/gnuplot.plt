set terminal postscript enhanced solid color eps
set output "output_minkowski/result.eps"

set key box
set key left

set xlabel "global packing fraction {/Symbol h}"
set ylabel "<{/Symbol m}> (anisotropy)"

set xrange [0:0.74]
set yrange [0.3:1]

plot "output_minkowski/result_w020.txt" using 1:2 title "w020",\
"output_minkowski/result_w120.txt" using 1:2 title "w120",\
"output_minkowski/result_w220.txt" using 1:2 title "w220",\
"output_minkowski/result_w320.txt" using 1:2 title "w320",\
"output_minkowski/result_w102.txt" using 1:2 title "w102",\
"output_minkowski/result_w202.txt" using 1:2 title "w202"

!epstopdf output_minkowski/result.eps

