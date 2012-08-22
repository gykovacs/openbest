set term png size 2000,1200
set output "five_years.png"
set title "Five years values - output from BEST"
plot '../results/five_year.result' using 1:2 with lines
