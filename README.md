# echo-device-driver
My First Linux Module


## Usage

```bash
[root]$ cat /dev/echo.dev 
[*] Hello from Kernel!
[root]$ cat /dev/echo.dev
[*] Hello from Kernel!
[root]$ echo -e "Message-1\nMessage-2" > /dev/echo.dev 
[root]$ cat /dev/echo.dev                              
Message-1
Message-2
[root]$ cat /dev/echo.dev
Message-1
Message-2
```
