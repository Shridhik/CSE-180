# Assignment 1

The goal for Assignment 1 is to implement a simple single-threaded HTTP server. The server will respond to simple GET and PUT commands to read and write (respectively) “files” named by 27-character ASCII names. The server will persistently store files in a directory on the server, so it can be restarted or otherwise run on a directory that already has files. As usual, you must have a design document and writeup along with your README.md in your git. My code must build httpserver using make.

LIMITATIONS: Could not access content length, but predefine content length/filze size to 50

## Installation

You can either use "make" or compile it manually

```bash

$ make

OR

$ clang++ -std=gnu++11 -Wall -Wextra -Wpedantic -Wshadow httpserver.cpp -o httpserver
```

## Usage

Make sure you open two terminal windows, on one window execute the server, and on the other open the curl (client - different directory)
SERVER: 
```c++
EXAMPLE OF A GET COMMAND BEING USED WITH SERVER
$ ./httpserver localhost 8080
> ^[[AHTTP/1.1 200 OK
>
> THIS IS A TEST FILE FROM THE CLIENT


EXAMPLE OF A PUT COMMAND BEING USED WITH SERVER
$ ./httpserver localhost 8080
> HTTP/1.1 200 OK
>
> THIS IS A TEST FILE FROM THE CLIENT

```

CLIENT: 
```c++
EXAMPLE OF A GET COMMAND BEING USED WITH CLIENT
$ curl http://localhost:8080/ABCDEFabcdef012345XYZxyz-mm
> THIS IS A TEST FILE FROM THE CLIENT



EXAMPLE OF A PUT COMMAND BEING USED WITH CLIENT
$ curl --upload-file ABCDEFabcdef012345XYZxyz-mm localhost:8080
>   % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100    36    0     0  100    36      0     35  0:00:01  0:00:01 --:--:--    35  
  
```

What happens in your implementation if, during a PUT with a Content-Length, the connection was closed, ending the communication early? This extra concern was not present in your implementation of dog. Why not? Hint: this is an example of complexity being added by an extension of requirements (in this case, data transfer over a network).


ANSWER: I believe that if the connection is ended early, only what had time to be sent would be sent and everything else would be cut off. This was not a concern in our implementation of Dog because we had to manually enter in input ourselves on the command prompt rather than having it in a file or by sending it through a client/server.

