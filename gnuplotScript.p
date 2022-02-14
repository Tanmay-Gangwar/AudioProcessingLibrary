set xlabel "Dimension of matrix (n by n)"
set ylabel "Time (in sec)"
plot "normalData.txt" using 1:2:3 title "No library" linetype 7 linecolor 1 with errorbars, '' title "No library" linecolor 1 with lines, "pthreadData.txt" using 1:2:3 title "Pthread" linetype 7 linecolor 2 with errorbars,'' title "Pthread" linecolor 2 with lines, "openBlasData.txt" using 1:2:3 title "OpenBlas" linetype 7 linecolor 3 with errorbars,'' title "OpenBlas" linecolor 3 with lines, "mklData.txt" using 1:2:3 title "MKL" linetype 7 linecolor 4 with errorbars, '' title "MKL" linecolor 4 with lines
set terminal png size 800, 600
set output "gnuplotImage.png"
replot
set terminal epslatex color colortext
set output "gnuplotPdf.tex"
replot
