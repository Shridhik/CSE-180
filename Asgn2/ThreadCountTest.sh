./httpserver -N 8 -l my_log.txt localhost 8080 & sleep 0.3
ps -o thcount $! > client/out0
pkill -P $$
grep "5" client/out0

