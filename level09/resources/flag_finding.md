# Security Vulnerability for Level09

The vulnerability is to store the sensitive data in the directory accessible by other users, even if this sensitive data is encoded.

## Finding the Password for the `level10` User

### 1. Inspect the `/home/user/level09` Directory

There are 2 files in the directory:
```bash
-rwsr-sr-x 1 flag09 level09 7640 Mar  5  2016 level09
----r--r-- 1 flag09 level09   26 Mar  5  2016 token
```

### 2. Inspect "token" File

```bash
cat token
```
Output:
```bash
f4kmm6p|=�p�n��DB�Du{��
```
There is some encoded text.

### 3. Inspect "level09" File

This is a binary executable that needs 1 argument.
Give a file "token" to this program to see if it could decode it.
```bash
level09@SnowCrash:~$ ./level09 token
tpmhr
level09@SnowCrash:~$ ./level09 test
tfuw
level09@SnowCrash:~$ ./level09 123
135
level09@SnowCrash:~$ ./level09 abcd
aceg
```
The program does not open the file from the argument as on the previous level 08. It takes a text argument and encodes it.

### 4. Identify the Patterns in Encoding

First character always is the same after encoding.

The other characters are moving to the next characters `x` times.  
Looks like `x` is a position of the character in the string, first characher is never changed because its position is `0`.

|string  |"12345"|
|--------|-------|
|position| 01234 |

'1' is '1' + 0 -> 1  
'2' is '2' + 1 -> 3  
'3' is '3' + 2 -> 5  
'4' is '4' + 3 -> 7  
'5' is '5' + 4 -> 9  

Result: 13579

### 5. Write a Code to Decode the Password

Copy `token` file from Virtual Machine to host:
```bash
scp -P 4242 levelXX@localhost:/home/user/levelXX/file_name directory_on_host
```

Change file's permissions to be able to read its contents with `read` function.

It's not necessary but we start by writting `encode.c` to test that we understand correctly the pattern of this encoding by comparing the results with the `level09` executable.

Encoding is working by simply adding a position of the current character to the character itsef:

```c
result[position] = input[position] + position
```

Read `token` contents and take it as the string to decode.  
Decoding is working by simply changing `+` sign to `-` sign.

```c
result[position] = input[position] - position;
```

Result: get the password to connect as `flag09` user.

### 6. Get the Password for the Next Level

Use the getflag command to retrieve the password for the next level.