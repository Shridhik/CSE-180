#!/bin/sh

head -c 100 /dev/urandom | od -x > client/d02.c1 &&
head -c 300 /dev/urandom | od -x > client/d02.c2

./httpserver localhost 8080 & sleep 0.5

# (curl -s http://localhost:8009 --request-target 01cdefghijklmnopqrstuvwxyz0 -o client/out01.c1 && curl -s http://localhost:8009 --request-target 01_aefghijklmnopqrstuvwxyz0 -o client/out01.c2 && wait)

(curl -v http://localhost:8080 --request-target 02cdefghijklmnopqrstuvwxyz0 -T client/d02.c1 &&
curl -v http://localhost:8080 --request-target 02_aefghijklmnopqrstuvwxyz0 -T client/d02.c2 && wait)

pkill -P $$

diff -q 02cdefghijklmnopqrstuvwxyz0 client/d02.c1
diff -q 02_aefghijklmnopqrstuvwxyz0 client/d02.c2
