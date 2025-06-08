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

# a runtime that only works with gcc

For the next test we will use the `stack_smasher.cpp` file that can receive some numbers at command line and destroy the stack. For this type of bugs valgrind and gdb are useless as they cannot offer much information. Let's see how they behave:

Compile our code and see how it fails:

```
iulians@tumbleweed:~/diagnose/runtime_analysis> gcc -Wall stack_smasher.cpp -o smasher
iulians@tumbleweed:~/diagnose/runtime_analysis>
iulians@tumbleweed:~/diagnose/runtime_analysis> ./smasher 10
Writing 10 bytes into a stack buffer of 20
iulians@tumbleweed:~/diagnose/runtime_analysis> ./smasher 40
Writing 40 bytes into a stack buffer of 20
Segmentation fault (core dumped)
iulians@tumbleweed:~/diagnose/runtime_analysis>

```

Let's try gdb

```
iulians@tumbleweed:~/diagnose/runtime_analysis> gdb ./smasher
....
Reading symbols from ./smasher...
(gdb) r 40
Starting program: /home/iulians/diagnose/runtime_analysis/smasher 40

.....
Using host libthread_db library "/lib64/libthread_db.so.1".
Writing 40 bytes into a stack buffer of 20

Program received signal SIGSEGV, Segmentation fault.
0x00000000004011d5 in main ()
Missing separate debuginfos, use: zypper install glibc-debuginfo-2.41-2.1.x86_64
(gdb) bt
#0  0x00000000004011d5 in main ()
Backtrace stopped: Cannot access memory at address 0x8
(gdb)

```

Nothing is useful here as the stack is destroyed.

Let's try valgrind.

```
iulians@tumbleweed:~/diagnose/runtime_analysis> valgrind ./smasher 40
==2242804== Memcheck, a memory error detector
==2242804== Copyright (C) 2002-2024, and GNU GPL'd, by Julian Seward et al.
==2242804== Using Valgrind-3.24.0 and LibVEX; rerun with -h for copyright info
==2242804== Command: ./smasher 40
==2242804==
Writing 40 bytes into a stack buffer of 20
==2242804== Warning: client switching stacks?  SP change: 0x1ffefffa30 --> 0x0
==2242804==          to suppress, use: --max-stackframe=137422174768 or greater
==2242804== Invalid read of size 8
==2242804==    at 0x4011D5: main (in /home/iulians/diagnose/runtime_analysis/smasher)
==2242804==  Address 0x0 is not stack'd, malloc'd or (recently) free'd
==2242804==
==2242804==
==2242804== Process terminating with default action of signal 11 (SIGSEGV): dumping core
==2242804==  Access not within mapped region at address 0x0
==2242804==    at 0x4011D5: main (in /home/iulians/diagnose/runtime_analysis/smasher)
==2242804==  If you believe this happened as a result of a stack
==2242804==  overflow in your program's main thread (unlikely but
==2242804==  possible), you can try to increase the size of the
==2242804==  main thread stack using the --main-stacksize= flag.
==2242804==  The main thread stack size used in this run was 8388608.
==2242804==
==2242804== HEAP SUMMARY:
==2242804==     in use at exit: 1,024 bytes in 1 blocks
==2242804==   total heap usage: 1 allocs, 0 frees, 1,024 bytes allocated
==2242804==
==2242804== LEAK SUMMARY:
==2242804==    definitely lost: 0 bytes in 0 blocks
==2242804==    indirectly lost: 0 bytes in 0 blocks
==2242804==      possibly lost: 0 bytes in 0 blocks
==2242804==    still reachable: 1,024 bytes in 1 blocks
==2242804==         suppressed: 0 bytes in 0 blocks
==2242804== Rerun with --leak-check=full to see details of leaked memory
==2242804==
==2242804== For lists of detected and suppressed errors, rerun with: -s
==2242804== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
Segmentation fault (core dumped)
iulians@tumbleweed:~/diagnose/runtime_analysis>
```

It is useless.

But gcc with its instrumentation of code really does make a difference:

```
iulians@tumbleweed:~/diagnose/runtime_analysis> gcc -g -fsanitize=address stack_smasher.cpp -o smasher
iulians@tumbleweed:~/diagnose/runtime_analysis> ./smasher 10
Writing 10 bytes into a stack buffer of 20
iulians@tumbleweed:~/diagnose/runtime_analysis> ./smasher 40
Writing 40 bytes into a stack buffer of 20
=================================================================
==2242867==ERROR: AddressSanitizer: stack-buffer-overflow on address 0x7b2b1ea00034 at pc 0x7f2b2151e6ca bp 0x7ffff4a10cd0 sp 0x7ffff4a104a0
WRITE of size 40 at 0x7b2b1ea00034 thread T0
    #0 0x7f2b2151e6c9 in memset (/lib64/libasan.so.8+0x11e6c9) (BuildId: 9f51e9501f1a177885c12bfa56f55641c3be603e)
    #1 0x00000040126d in smash(int) /home/iulians/diagnose/runtime_analysis/stack_smasher.cpp:9
    #2 0x000000401322 in main /home/iulians/diagnose/runtime_analysis/stack_smasher.cpp:18
    #3 0x7f2b2102b12d in __libc_start_call_main (/lib64/libc.so.6+0x2b12d) (BuildId: 4e306825df357f9b661479a3f9d24a8dbf960c1f)
    #4 0x7f2b2102b1f8 in __libc_start_main_impl (/lib64/libc.so.6+0x2b1f8) (BuildId: 4e306825df357f9b661479a3f9d24a8dbf960c1f)
    #5 0x0000004010e4 in _start ../sysdeps/x86_64/start.S:115

Address 0x7b2b1ea00034 is located in stack of thread T0 at offset 52 in frame
    #0 0x0000004011c5 in smash(int) /home/iulians/diagnose/runtime_analysis/stack_smasher.cpp:6

  This frame has 1 object(s):
    [32, 52) 'buffer' (line 7) <== Memory access at offset 52 overflows this variable
HINT: this may be a false positive if your program uses some custom stack unwind mechanism, swapcontext or vfork
      (longjmp and C++ exceptions *are* supported)
SUMMARY: AddressSanitizer: stack-buffer-overflow /home/iulians/diagnose/runtime_analysis/stack_smasher.cpp:9 in smash(int)
Shadow bytes around the buggy address:
  0x7b2b1e9ffd80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7b2b1e9ffe00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7b2b1e9ffe80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7b2b1e9fff00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7b2b1e9fff80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x7b2b1ea00000: f1 f1 f1 f1 00 00[04]f3 f3 f3 f3 f3 00 00 00 00
  0x7b2b1ea00080: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7b2b1ea00100: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7b2b1ea00180: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7b2b1ea00200: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7b2b1ea00280: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
==2242867==ABORTING
iulians@tumbleweed:~/diagnose/runtime_analysis>
```

Once the error is detected (writing outside the stack's boundaries) - see https://en.wikipedia.org/wiki/Buffer_overflow_protection#Canaries for more details program exits. We can actually see the line where this happens (number 9) which takes us to the place where we need to fix our bug.

```
iulians@tumbleweed:~/diagnose/runtime_analysis> cat -n stack_smasher.cpp | head -n 9 | tail -n 1
     9          memset(buffer,0,count);
iulians@tumbleweed:~/diagnose/runtime_analysis>
```

# Desperation mode
When your out of ideas it is always good to use some tools that give raw access to what the program opens/calls at a certain moment in time. For this we can use on Windows tools like Process Explorer or Process Monitor.

On Linux we have the following commands:
* strace - traces system calls and their parameters
* ltrace - traces library calls and their parameter
* lsof - displays the open files for a certain process ( `lsof -p <PID>` ) or tell you which program has a certain files open ( `lsof <filename>` )


# Conclusion:
* be familiar with all these tools as they can complement each other
