set term png size 2000,1200
set output "annual.png"
set title "Annual values - output from BEST"
plot '../results/annual.result' using 1:2 with lines
