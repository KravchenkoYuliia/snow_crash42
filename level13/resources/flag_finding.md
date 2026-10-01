# Security Vulnerability for Level13

Any user without the required privileges can debug the program which means any user can interfere with the program using GDB.

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
UID 2013 started us but we expect 4242
```

Check the user ID:
```bash
id level13
uid=2013(level13) gid=2013(level13) groups=2013(level13),100(users)
```
We see that `2013` is our current user ID.  
The program checks it before proceeding and stops execution if the UID is not `4242`.

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

We see that `getuid` is called and its result is checked.

We see `boe]!ai0FB@.:|L6l@A?>qJ}I` in the binary after the UID checking, it can be the password that is only displayed for verified UID. We need to find a possibility to interfere with the program during runtime to replace the return value of `getuid` with 4242.

### 3. Interfere with the Program

Open GDB to better see the program's behaviour.

Open gdb:
```bash
gdb -tui ./level13
layout asm
```

Start at `getuid` function and run the program:
```bash
b getuid
r
```

Get into the function:
```bash
si
```

Move to the next line:
```bash
ni
```

Stop at the line
```
0xb7ee4ccc <getuid+12>          ret
```
and check what is in **eax** register:
```bash
(gdb)  p (int) $eax
$1 = 2013
```

Assembly code returns its value in the `eax` register which means we can rewrite the register's value.

Put 4242 into `eax` register:
```bash
set $eax=4242
(gdb) p (int) $eax
$2 = 4242
```

### 4. Get the Flag -> Level 14's Password

Move through the assembly code, we successfully bypass the verification of the UID and reach the `ft_des` function. Print the values in registers that are used in `ft_des` function:
```bash
p (char*) $edx
$3 = 0x8048709 "your token is %s\n"

p (char*) $eax
$4 = 0x804b008 "xxxxxx"
```

The password is found.