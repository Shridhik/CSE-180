head -c 100 /dev/urandom | od -x > 04_MsaZTliO9XMO9qhNsoSDgGuY
head -c 40000 /dev/urandom | od -x > 04Nz_xFjH_-rgP0YYvf10Auv8UQ
head -c 1000 /dev/urandom | od -x > 04NollxqVcPx9LBUNKOkPbiM86T
head -c 4000 /dev/urandom | od -x > 04xwkSenPIHQl4XukqAsdFrYz43
head -c 10000 /dev/urandom |od -x > client/d04.c2
head -c 400 /dev/urandom | od -x > client/d04.c4
head -c 1000 /dev/urandom | od -x > client/d04.c6
head -c 400 /dev/urandom | od -x > client/d04.c8

./httpserver localhost 8004 -N 4 & sleep 0.5

start=`date +%s`
for i in 1 2 3 4 5 6 7 8 9 10
do
	echo "***********************************************"
	(curl -v http://localhost:8004 --request-target 04_MsaZTliO9XMO9qhNsoSDgGuY -o client/out04.c1 && curl -v http://localhost:8004 --request-target 04oNUC7-y64ZKfw4LzK89-SPPZM -T client/d04.c2 && curl -v http://localhost:8004 --request-target 04NollxqVcPx9LBUNKOkPbiM86T -o client/out04.c3 && curl -v http://localhost:8004 --request-target 04UzND3J0JYh8QqYAh6VLsCQ6nF -T client/d04.c4 && curl -v http://localhost:8004 --request-target 04Nz_xFjH_-rgP0YYvf10Auv8UQ -o client/out04.c5 && curl -v http://localhost:8004 --request-target 04AHy4ro39ZSHI0Px81yFK8zCv1 -T client/d04.c6 && curl -v http://localhost:8004 --request-target 04xwkSenPIHQl4XukqAsdFrYz43 -o client/out04.c7 && curl -v http://localhost:8004 --request-target 04xl_fF0x1hgJSZ-hAPkt163uf_ -T client/d04.c8 && wait)

	 echo "The $i times loop finished."
done

pkill -P $$
end=`date +%s`
        
diff -q 04_MsaZTliO9XMO9qhNsoSDgGuY client/out04.c1
diff -q 04oNUC7-y64ZKfw4LzK89-SPPZM client/d04.c2
diff -q 04NollxqVcPx9LBUNKOkPbiM86T client/out04.c3
diff -q 04UzND3J0JYh8QqYAh6VLsCQ6nF client/d04.c4
diff -q 04Nz_xFjH_-rgP0YYvf10Auv8UQ client/out04.c5
diff -q 04AHy4ro39ZSHI0Px81yFK8zCv1 client/d04.c6
diff -q 04xwkSenPIHQl4XukqAsdFrYz43 client/out04.c7
diff -q 04xl_fF0x1hgJSZ-hAPkt163uf_ client/d04.c8

runtime=$((end-start))
echo "The runtime is $runtime seconds."
