# Compiler with extra warning flags

The difference is quite clear, with the `-Wall` flag we get more warnings that prove to be errors.

```
iulians@tumbleweed:~/diagnose/static_analysis> gcc buggy.cpp
iulians@tumbleweed:~/diagnose/static_analysis> gcc -Wall buggy.cpp
buggy.cpp: In function ‘int func1(long int)’:
buggy.cpp:8:46: warning: format ‘%d’ expects argument of type ‘int’, but argument 2 has type ‘long int’ [-Wformat=]
    8 |         printf("Called function with param: %d\n", param);
      |                                             ~^     ~~~~~
      |                                              |     |
      |                                              int   long int
      |                                             %ld
buggy.cpp:7:15: warning: unused variable ‘ptr’ [-Wunused-variable]
    7 |         void *ptr = malloc(param);
      |               ^~~
iulians@tumbleweed:~/diagnose/static_analysis>
```

# Use clang's static analysis

The command used here is scan-build which comes from clangs tools so make sure you install the clang related tools. The packages may vary depending on your Linux distribution.

```
iulians@tumbleweed:~/diagnose/static_analysis> scan-build gcc buggy.cpp
scan-build: Using '/usr/bin/clang-19' for static analysis
buggy.cpp:7:8: warning: Value stored to 'ptr' during its initialization is never read [deadcode.DeadStores]
    7 |         void *ptr = malloc(param);
      |               ^~~   ~~~~~~~~~~~~~
buggy.cpp:8:2: warning: Potential leak of memory pointed to by 'ptr' [unix.Malloc]
    8 |         printf("Called function with param: %d\n", param);
      |         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
2 warnings generated.
scan-build: Analysis run complete.
scan-build: 2 bugs found.
scan-build: Run 'scan-view /tmp/scan-build-2025-05-31-190212-2206162-1' to examine bug reports.
iulians@tumbleweed:~/diagnose/static_analysis>

```

What we see is an unused variable (also detected by gcc) but also an extra memory leak. So it's always good to run multiple analyzers on your code.

# Using cppcheck

Using cppcheck we can see that by default it detects the memory leak.

```
iulians@tumbleweed:~/diagnose/static_analysis> cppcheck buggy.cpp
Checking buggy.cpp ...
buggy.cpp:9:2: error: Memory leak: ptr [memleak]
 return 0;
 ^
iulians@tumbleweed:~/diagnose/static_analysis>

```

If we want to go further and see all that cppcheck has to offer we can add `--enable=all` option and we will get much more errors:
```
iulians@tumbleweed:~/diagnose/static_analysis> cppcheck --enable=all buggy.cpp
Checking buggy.cpp ...
buggy.cpp:1:0: information: Include file: <stdio.h> not found. Please note: Cppcheck does not need standard library headers to get proper results. [missingIncludeSystem]
#include <stdio.h>
^
buggy.cpp:2:0: information: Include file: <stdlib.h> not found. Please note: Cppcheck does not need standard library headers to get proper results. [missingIncludeSystem]
#include <stdlib.h>
^
buggy.cpp:3:0: information: Include file: <string.h> not found. Please note: Cppcheck does not need standard library headers to get proper results. [missingIncludeSystem]
#include <string.h>
^
buggy.cpp:8:2: warning: %d in format string (no. 1) requires 'int' but the argument type is 'signed long'. [invalidPrintfArgType_sint]
 printf("Called function with param: %d\n", param);
 ^
buggy.cpp:9:2: error: Memory leak: ptr [memleak]
 return 0;
 ^
buggy.cpp:7:8: style: Variable 'ptr' can be declared as pointer to const [constVariablePointer]
 void *ptr = malloc(param);
       ^
buggy.cpp:7:12: style: Variable 'ptr' is assigned a value that is never used. [unreadVariable]
 void *ptr = malloc(param);
           ^
buggy.cpp:7:14: style: Variable 'ptr' is allocated memory that is never used. [unusedAllocatedMemory]
 void *ptr = malloc(param);
             ^
nofile:0:0: information: Active checkers: 167/856 (use --checkers-report=<filename> to see details) [checkersReport]

iulians@tumbleweed:~/diagnose/static_analysis>

```
In conclusion it is good practice to have the static analysis enabled on your code, you can even use multiple tools in order to get different "opinions".

Important note: NOT EVERYTHING IS CORRECT, please analyze each report and decide if the reports are valid or not.

# Symbols (low level work)

Sometimes the executables obtained might not be as expected and some functions might not have the desired name in the binary (especially when we're talking about DLLs/Shared objects). In such cases we can use tools like `nm`, `objdump`, 'strings` to help us figure out if some binary symbols are what they are supposed to be.

Please read more about this tools.
Related topic: https://en.wikipedia.org/wiki/Name_mangling

# Further work
* Integrate coverity from github
* Add other static analysis tools in the examples
