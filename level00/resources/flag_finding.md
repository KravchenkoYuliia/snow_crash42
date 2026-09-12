# Security vulnerability for level00
### File permission  
Files owned by user `flag00` are visible to other users, anybody can see user's password inside these files

## Find password for flag00 user

- connect as user level00
- check files that belong to flag00 user  
`find / -user flag00 2>/dev/null`

	there are 2 results:  
	`/usr/sbin/john`
	`/rofs/usr/sbin/john`

- both files has the same content - some ciphed code

- try different offsets with Cesar cipher
	till we have smth readable  
	
	Found the answer with offset -11 on [Cesar cipher](https://cryptii.com/pipes/caesar-cipher/)

- now we can connect as flag00 user

- getflag command shows password for the next level