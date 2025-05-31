# compiling program for coverage

Coverage actually instruments the code so that it is counting/marking how many times the code is executed.
Compilation is done with the following switches:

```
gcc -fprofile-arcs -ftest-coverage -o main main.cpp
```
This will generate and executable and coverage nodes file.

```
iulians@tumbleweed:~/diagnose/coverage> ls -l
total 4
-rw-r--r-- 1 iulians iulians 198 May 31 20:27 main.cpp
-rw-r--r-- 1 iulians iulians   0 May 31 20:24 README.md
iulians@tumbleweed:~/diagnose/coverage> gcc -fprofile-arcs -ftest-coverage -o main main.cpp
iulians@tumbleweed:~/diagnose/coverage> ls -ltr
total 92
-rw-r--r-- 1 iulians iulians     0 May 31 20:24 README.md
-rw-r--r-- 1 iulians iulians   198 May 31 20:27 main.cpp
-rw-r--r-- 1 iulians iulians   690 May 31 20:28 main.gcno
-rwxr-xr-x 1 iulians iulians 84976 May 31 20:28 main
iulians@tumbleweed:~/diagnose/coverage>
```

Running the program generates a .gcda file which is coverage data.

```
iulians@tumbleweed:~/diagnose/coverage> ./main
Max is 10
iulians@tumbleweed:~/diagnose/coverage> ls -ltr
total 96
-rw-r--r-- 1 iulians iulians     0 May 31 20:24 README.md
-rw-r--r-- 1 iulians iulians   198 May 31 20:27 main.cpp
-rw-r--r-- 1 iulians iulians   690 May 31 20:28 main.gcno
-rwxr-xr-x 1 iulians iulians 84976 May 31 20:28 main
-rw-r--r-- 1 iulians iulians   132 May 31 20:30 main.gcda
iulians@tumbleweed:~/diagnose/coverage>
```

This data can be interpretted with `gcov` which generates a report:
```
iulians@tumbleweed:~/diagnose/coverage> gcov main.cpp
File 'main.cpp'
Lines executed:87.50% of 8
Creating 'main.cpp.gcov'

Lines executed:87.50% of 8
iulians@tumbleweed:~/diagnose/coverage> ls -ltr
total 100
-rw-r--r-- 1 iulians iulians     0 May 31 20:24 README.md
-rw-r--r-- 1 iulians iulians   198 May 31 20:27 main.cpp
-rw-r--r-- 1 iulians iulians   690 May 31 20:28 main.gcno
-rwxr-xr-x 1 iulians iulians 84976 May 31 20:28 main
-rw-r--r-- 1 iulians iulians   132 May 31 20:30 main.gcda
-rw-r--r-- 1 iulians iulians   556 May 31 20:32 main.cpp.gcov
iulians@tumbleweed:~/diagnose/coverage>
```

Viewing the main.cpp.gcov we can actually see which lines were no executed (with #####).
Lines with "-" have no executable code generated. 
The other lines have the count of times invoked mentioned as number.

```
iulians@tumbleweed:~/diagnose/coverage> cat main.cpp.gcov
        -:    0:Source:main.cpp
        -:    0:Graph:main.gcno
        -:    0:Data:main.gcda
        -:    0:Runs:1
        -:    1:#include <stdio.h>
        -:    2:
        1:    3:int max(int a, int b) {
        1:    4:    if (a > b)
    #####:    5:        return a;
        -:    6:    else
        1:    7:        return b;
        -:    8:
        -:    9:
        -:   10:
        -:   11:
        -:   12:}
        -:   13:
        1:   14:int main() {
        1:   15:    int x = 5, y = 10;
        -:   16:
        1:   17:    printf("Max is %d\n", max(x, y));
        -:   18:
        1:   19:    return 0;
        -:   20:}
        -:   21:
iulians@tumbleweed:~/diagnose/coverage>

```
