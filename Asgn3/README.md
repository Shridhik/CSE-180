# Assignment 3

The goal for Assignment 3 is to modify the HTTP server that you already implemented to have one additional feature: caching. Caching means that I am going to maintain a buffer in my server that contains a subset of the pages. When a request is received, if the requested page is in the cache, then it is read from the cache (if it is a GET request) or updated in the cache (if it is a PUT request). Otherwise, the page is first read from disk into the cache. In the log record of each request, I will indicate whether the page was in the cache at the time the request was received. 


Everything in my program works as expected except the put's log conversion to hex. Besides this my program has a working Log and cacheing system. 


## Installation

You can either use "make" or compile it manually

```bash

$ make

OR

$ clang++ -std=gnu++11 -lpthread -Wall -Wextra -Wpedantic -Wshadow httpserver.cpp -o httpserver
```

## Usage

Make sure you open two terminal windows, on one window execute the server, and on the other open the curl (client - different directory)
SERVER: 
```c++
EXAMPLE OF A GET COMMAND BEING USED WITH SERVER
$ ./httpserver -N 8 -l my_log.txt -c localhost 8888
> HTTP/1.1 200 OK
>
> THIS IS A TEST FILE FROM THE CLIENT


EXAMPLE OF A PUT COMMAND BEING USED WITH SERVER
$ ./httpserver -N 8 -l my_log.txt -c localhost 8888
> HTTP/1.1 200 OK
>
> THIS IS A TEST FILE FROM THE CLIENT

```

CLIENT: 
```c++
EXAMPLE OF A GET COMMAND BEING USED WITH CLIENT
$ curl http://localhost:8080/ABCDEFabcdef012345XYZxyz-mm
> THIS IS A TEST FILE FROM THE CLIENT

TO RUN THE SCRIPT JUST RUN:
$ ./shell.sh

EXAMPLE OF A PUT COMMAND BEING USED WITH CLIENT
$ curl --upload-file ABCDEFabcdef012345XYZxyz-mm localhost:8080
>   % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100    36    0     0  100    36      0     35  0:00:01  0:00:01 --:--:--    35  
  
```



