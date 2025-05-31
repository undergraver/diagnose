# gcc compilation with -fsanitize=address

Using the `-fsanitize=address` command line option offers a report at program exit which shows us the place where leak is present. 

Debug is important as it can show details about the exact location where the leak started.

```
iulians@tumbleweed:~/diagnose/runtime_analysis> gcc -g -fsanitize=address buggy.cpp -o buggy
iulians@tumbleweed:~/diagnose/runtime_analysis> ./buggy
before calling func1()
Called function with param: 44
after calling func1()

=================================================================
==2241080==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 44 byte(s) in 1 object(s) allocated from:
    #0 0x7f32fe121c0b in malloc (/lib64/libasan.so.8+0x121c0b) (BuildId: 9f51e9501f1a177885c12bfa56f55641c3be603e)
    #1 0x0000004011dd in func1(long) /home/iulians/diagnose/runtime_analysis/buggy.cpp:8
    #2 0x000000401220 in func0(long) /home/iulians/diagnose/runtime_analysis/buggy.cpp:16
    #3 0x0000004012b1 in main /home/iulians/diagnose/runtime_analysis/buggy.cpp:35
    #4 0x7f32fdc2b12d in __libc_start_call_main (/lib64/libc.so.6+0x2b12d) (BuildId: 4e306825df357f9b661479a3f9d24a8dbf960c1f)

SUMMARY: AddressSanitizer: 44 byte(s) leaked in 1 allocation(s).
iulians@tumbleweed:~/diagnose/runtime_analysis> cat buggy.cpp | head -n 8
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int func1(long param)
{
        void *ptr = malloc(param);
iulians@tumbleweed:~/diagnose/runtime_analysis>
```

# valgrind

Another option is to use valgrind. By default it will only inform you about the memory problems (not only leaks, also corruption).

```
iulians@tumbleweed:~/diagnose/runtime_analysis> gcc -g buggy.cpp -o buggy
iulians@tumbleweed:~/diagnose/runtime_analysis> valgrind ./buggy
==2241992== Memcheck, a memory error detector
==2241992== Copyright (C) 2002-2024, and GNU GPL'd, by Julian Seward et al.
==2241992== Using Valgrind-3.24.0 and LibVEX; rerun with -h for copyright info
==2241992== Command: ./buggy
==2241992==
before calling func1()
Called function with param: 44
after calling func1()
==2241992==
==2241992== HEAP SUMMARY:
==2241992==     in use at exit: 44 bytes in 1 blocks
==2241992==   total heap usage: 2 allocs, 1 frees, 1,068 bytes allocated
==2241992==
==2241992== LEAK SUMMARY:
==2241992==    definitely lost: 44 bytes in 1 blocks
==2241992==    indirectly lost: 0 bytes in 0 blocks
==2241992==      possibly lost: 0 bytes in 0 blocks
==2241992==    still reachable: 0 bytes in 0 blocks
==2241992==         suppressed: 0 bytes in 0 blocks
==2241992== Rerun with --leak-check=full to see details of leaked memory
==2241992==
==2241992== For lists of detected and suppressed errors, rerun with: -s
==2241992== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
iulians@tumbleweed:~/diagnose/runtime_analysis>

```

If following the suggestions we can use `--leak-check=full` to get a better information displayed:

```
iulians@tumbleweed:~/diagnose/runtime_analysis> valgrind --leak-check=full ./buggy
==2242059== Memcheck, a memory error detector
==2242059== Copyright (C) 2002-2024, and GNU GPL'd, by Julian Seward et al.
==2242059== Using Valgrind-3.24.0 and LibVEX; rerun with -h for copyright info
==2242059== Command: ./buggy
==2242059==
before calling func1()
Called function with param: 44
after calling func1()
==2242059==
==2242059== HEAP SUMMARY:
==2242059==     in use at exit: 44 bytes in 1 blocks
==2242059==   total heap usage: 2 allocs, 1 frees, 1,068 bytes allocated
==2242059==
==2242059== 44 bytes in 1 blocks are definitely lost in loss record 1 of 1
==2242059==    at 0x48477C4: malloc (vg_replace_malloc.c:446)
==2242059==    by 0x40118D: func1(long) (buggy.cpp:8)
==2242059==    by 0x4011D0: func0(long) (buggy.cpp:16)
==2242059==    by 0x401241: main (buggy.cpp:35)
==2242059==
==2242059== LEAK SUMMARY:
==2242059==    definitely lost: 44 bytes in 1 blocks
==2242059==    indirectly lost: 0 bytes in 0 blocks
==2242059==      possibly lost: 0 bytes in 0 blocks
==2242059==    still reachable: 0 bytes in 0 blocks
==2242059==         suppressed: 0 bytes in 0 blocks
==2242059==
==2242059== For lists of detected and suppressed errors, rerun with: -s
==2242059== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
iulians@tumbleweed:~/diagnose/runtime_analysis>

```

This can prove very useful. One thing about valgrind is the fact that it slows the program quite considerably, so you need to be aware of this.
