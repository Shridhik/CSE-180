#!/bin/sh

head -c 100 /dev/urandom | od -x > 01cdefghijklmnopqrstuvwxyz0 &&
head -c 300 /dev/urandom | od -x > 01_aefghijklmnopqrstuvwxyz0

./httpserver -N 8 -l my_log.txt localhost 8080 &sleep 0.3

# (curl -s http://localhost:8009 --request-target 01cdefghijklmnopqrstuvwxyz0 -o client/out01.c1 && curl -s http://localhost:8009 --request-target 01_aefghijklmnopqrstuvwxyz0 -o client/out01.c2 && wait)

(curl -v http://localhost:8080 --request-target 01cdefghijklmnopqrstuvwxyz0 -o client/out01.c1 &&
curl -v http://localhost:8080 --request-target 01_aefghijklmnopqrstuvwxyz0 -o client/out01.c2 && wait)

pkill -P $$

diff -q 01cdefghijklmnopqrstuvwxyz0 client/out01.c1
diff -q 01_aefghijklmnopqrstuvwxyz0 client/out01.c2
