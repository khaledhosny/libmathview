
set xdata time
set format x "%m/%d"
set terminal png small

day(x)=(int(x/(3600*24)))*3600*24 - ((30 * 365 + 7) * 24 * 3600)
#day(x) = int(x / (3600 * 24))
plot 'stats.data' using (day($1)):(1) smooth frequency title 'Number of accesses' with lines

