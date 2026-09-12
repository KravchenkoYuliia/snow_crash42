# Security vulnerability for level01
### Hashed password in `/etc/passwd`
`/etc/passwd` file is the most important file in Linux operating system. This file stores essential information about the users on the system

![/etc/passwd](passwd_file.png)

gfg	-> user name  
x	-> indicates the password that is stored in `/etc/shadow` file in the encrypted format. Only autorized user have access to the `/etc/shadow` so the passwords are secured

user `flag01` has the hash in `/etc/passwd` instead `x`, which is dangerous because every user can see it

## Find password for flag01 user

- check files that belong to flag00 user  
```find / -user flag00 2>/dev/null```

	Result: no files

- check word "flag01" in all the files  
	
	```ls /```  

	check files of every directory  
	```cat /dir_name/* | grep "flag01"```

	or 

	```find / -type f -exec grep -Hn "flag01" {} \; 2>/dev/null```  
	-H shows file name

	Result: suspicious line with `flag01` is in `/etc/passwd`  

	`flag01:hashed_password`
	the other lines has structure like this -> `flag00:x:`  

- find hash type

	[Hash types](https://hashcat.net/wiki/doku.php?id=example_hashes)  
	Result: descrypt, DES (Unix), Traditional DES

- find the tool to decrypt the code

	[Understanding DES Unix](https://www.onlinehashcrack.com/guides/cryptography-algorithms/understanding-des-unix-descrypt-.php#)

	Result: tool `John the Ripper`

- git clone John git repository

	[John repo](https://github.com/openwall/john/blob/bleeding-jumbo)  
	```
	cd src  
	./configure && make  
	cd ../run  
	```
	to execute:   
	```./john file_with_password```

	Result: password to connect as user `flag01`

- getflag command shows password for the next level