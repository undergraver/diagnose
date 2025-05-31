We will show the log of an apache server while in a browser we access a page - even if it's invalid.

Waiting for log to be output is done via `tail -f` command.

```
tumbleweed:/var/log/apache2 # ls -l
total 552
-rw-r--r-- 1 root root 361054 May 31 11:18 access_log
-rw-r--r-- 1 root root 196727 May 31 19:29 error_log
tumbleweed:/var/log/apache2 # tail -f access_log
10.255.100.21 - - [31/May/2025:11:17:48 +0300] "GET /icons/blank.gif HTTP/1.1" 200 148 "http://10.100.82.222/~iulians/" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/136.0.0.0 Safari/537.36 Edg/136.0.0.0"
10.255.100.21 - - [31/May/2025:11:17:48 +0300] "GET /icons/back.gif HTTP/1.1" 200 216 "http://10.100.82.222/~iulians/" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/136.0.0.0 Safari/537.36 Edg/136.0.0.0"
10.255.100.21 - - [31/May/2025:11:17:48 +0300] "GET /icons/movie.gif HTTP/1.1" 200 243 "http://10.100.82.222/~iulians/" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/136.0.0.0 Safari/537.36 Edg/136.0.0.0"
10.255.100.21 - - [31/May/2025:11:17:48 +0300] "GET /icons/unknown.gif HTTP/1.1" 200 245 "http://10.100.82.222/~iulians/" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/136.0.0.0 Safari/537.36 Edg/136.0.0.0"
10.255.100.21 - - [31/May/2025:11:17:48 +0300] "GET /icons/folder.gif HTTP/1.1" 200 225 "http://10.100.82.222/~iulians/" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/136.0.0.0 Safari/537.36 Edg/136.0.0.0"
10.255.100.21 - - [31/May/2025:11:17:58 +0300] "GET /~iulians/a HTTP/1.1" 404 985 "-" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/136.0.0.0 Safari/537.36 Edg/136.0.0.0"
10.255.100.21 - - [31/May/2025:11:18:04 +0300] "GET / HTTP/1.1" 403 989 "-" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/136.0.0.0 Safari/537.36 Edg/136.0.0.0"
10.255.100.21 - - [31/May/2025:11:18:17 +0300] "GET / HTTP/1.1" 403 989 "-" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/136.0.0.0 Safari/537.36 Edg/136.0.0.0"
10.255.100.21 - - [31/May/2025:11:18:22 +0300] "GET /~iulians/ HTTP/1.1" 200 951 "-" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/136.0.0.0 Safari/537.36 Edg/136.0.0.0"
10.255.100.21 - - [31/May/2025:11:18:30 +0300] "-" 408 - "-" "-"


<PRESS ENTER HERE TO GET A SEPARATION IN THE TERMINAL>


10.255.100.21 - - [31/May/2025:20:09:14 +0300] "GET /~iulians/wrong_address HTTP/1.1" 404 985 "-" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/136.0.0.0 Safari/537.36 Edg/136.0.0.0"

```

Accessing other pages on that apache server will write extra lines in this log.
