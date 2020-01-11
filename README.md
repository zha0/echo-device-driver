# echo-device-driver
My First Linux Module


# Usage

```
#  cat /dev/echo.dev 
[*] Hello from Kernel!
#  cat /dev/echo.dev
[*] Hello from Kernel!
#  echo -e "Message-1\nMessage-2" > /dev/echo.dev 
#  cat /dev/echo.dev                              
Message-1
Message-2
#  cat /dev/echo.dev
Message-1
Message-2
```
