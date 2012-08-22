set term png size 2000,1200
set output "monthly.png"
set title "Monthly values - output from BEST"
plot '../results/monthly.result' using 1:2 with lines
