# Security Vulnerability for Level00

## File Permissions

Files owned by the `flag00` user are readable by other users. 

This means that anyone can access the files and potentially find sensitive information, such as the user's password.

## Finding the Password for the `flag00` User

### 1. Connect as the `level00` User

### 2. Find Files Belonging to the `flag00` User

```bash
find / -user flag00 2>/dev/null
```

Result: two files are found:
```bash
`/usr/sbin/john`
`/rofs/usr/sbin/john`
```

### 3. Inspect the Files

Both files have the same permissions: `----r--r--`
It means that other users have the right to read this file.

Both files have the same content: an encrypted string.

### 4. Identify the Cipher

Try different offsets using Cesar cipher untill readable text is found.

The correct answer is obtained with an offset of -11 using [Cesar cipher](https://cryptii.com/pipes/caesar-cipher/).

### 5. Connect as the `flag00` User

Use the obtained password to connect as the flag00 user.

### 6. Get the Password for the Next Level

Use the getflag command to retrieve the password for the next level.