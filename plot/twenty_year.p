set term png size 2000,1200
set output "twenty_years.png"
set title "Twenty years values - output from BEST"
plot '../results/twenty_year.result' using 1:2 with lines
