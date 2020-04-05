# Assignment 2

The goals for Assignment 2 are to modify the HTTP server that I  already have. I will be adding two additional features: multi-threading and logging. Multi-threading means your server must be able to handle multiple requests simultaneously, each in its own thread. Logging means that your server must write out a record of each request, including both header information and data (dumped as hex). I’ll need to use synchronization techniques to service multiple requests at once, and to ensure that entries in the log aren’t intermixed from multiple threads. 


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
$ ./httpserver -N 8 -l my_log.txt localhost 8888
> HTTP/1.1 200 OK
>
> THIS IS A TEST FILE FROM THE CLIENT


EXAMPLE OF A PUT COMMAND BEING USED WITH SERVER
$ ./httpserver -N 8 -l my_log.txt localhost 8888
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



