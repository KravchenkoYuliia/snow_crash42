# Security Vulnerability for Level13


## Finding the Password for the `level14` User

### 1. Inspect `/home/user/level13` Directory

```bash
ls -l
total 8
-rwsr-sr-x 1 flag13 level13 7303 Aug 30  2015 level13
```

### 2. Inspect `level13` File

`level13` is a binary executable.
Execute the file:
```bash
./level13 
UID 2013 started us but we we expect 4242
```

Check the user ID:
```bash
id level13
uid=2013(level13) gid=2013(level13) groups=2013(level13),100(users)
```
We see that `2013` is our current user's ID.  
The program checks it before proceeding and stops the execution if the UID is not `4242`.

See the readable parts of the binary file:
```bash
getuid
__libc_start_main
GLIBC_2.0
PTRh`
UWVS
[^_]
0123456
UID %d started us but we we expect %d
boe]!ai0FB@.:|L6l@A?>qJ}I
your token is %s
;*2$"$
```

We see that `getuid` is called. The result is checked.

We see `boe]!ai0FB@.:|L6l@A?>qJ}I` in code after the UID checking, it can be the password that only are shown for verified UID.

### 3. Understand `getuid` Function

[Man page](https://man7.org/linux/man-pages/man2/geteuid.2.html):
> getuid() returns the real user ID of the calling process.
> geteuid() returns the effective user ID of the calling process.

The **real UID** identifies the user who started the process, while the **effective UID** determines whose permissions the process actually uses.

It can be the key to the flag getting if we can replace the **real UID**, level13, by **effective UID**, 4242.

### 4. Find the Tool to Intercept During Execution of the `level13` Program

Looking for linux intercept function call during execution.
The result:

> ptrace

> ptrace is the API that debuggers like GDB use to do their debugging. There is a PTRACE_SYSCALL option which will pause execution just before/after syscalls. From there you can do pretty much whatever you like in the same way that GDB can. [Here's an article about how to modify syscall paramters using ptrace.](https://www.alfonsobeato.net/c/modifying-system-call-arguments-with-ptrace/)


GDB !!!
b getuid
r
set rax value in return to 4242