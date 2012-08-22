set term png size 2000,1200
set output "ten_years.png"
set title "Ten years values - output from BEST"
plot '../results/ten_year.result' using 1:2 with lines
