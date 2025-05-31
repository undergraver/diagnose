# Steps to verify
Execute these commands on a linux machine having gcc and gdb installed.

`gcc -Wall buggy.cpp -o buggy`

`./buggy`

As you can see it will crash with a message similar to this one:
```
iulians@tumbleweed:~/diagnose/debugging> gcc -Wall buggy.cpp -o buggy
buggy.cpp: In function ‘int func1(int)’:
buggy.cpp:8:16: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
    8 |         memset((char*)param,0,sizeof(param));
      |                ^~~~~~~~~~~~
iulians@tumbleweed:~/diagnose/debugging> ls -l
total 28
-rwxr-xr-x 1 iulians iulians 20144 May 31 18:14 buggy
-rw-r--r-- 1 iulians iulians   356 May 31 18:09 buggy.cpp
-rw-r--r-- 1 iulians iulians   162 May 31 18:13 README.md
iulians@tumbleweed:~/diagnose/debugging> ./buggy
before calling func1()
Called function with param: 44
Segmentation fault (core dumped)
iulians@tumbleweed:~/diagnose/debugging>

```

While it is clear why it crashes it is a good example on how to use a debugger like gdb.

We run the code with a debugger like described next (output was truncated):

```
iulians@tumbleweed:~/diagnose/debugging> gdb ./buggy
GNU gdb (GDB; openSUSE Tumbleweed) 16.3
...
For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from ./buggy...
(gdb) run
Starting program: /home/iulians/diagnose/debugging/buggy

...
before calling func1()
Called function with param: 44

Program received signal SIGSEGV, Segmentation fault.
__memset_avx2_unaligned_erms () at ../sysdeps/x86_64/multiarch/memset-vec-unaligned-erms.S:407
407             MOVD    %VMM_128(0), (%rdi)
(gdb) bt
#0  __memset_avx2_unaligned_erms () at ../sysdeps/x86_64/multiarch/memset-vec-unaligned-erms.S:407
#1  0x000000000040118c in func1(int) ()
#2  0x00000000004011b2 in func0(int) ()
#3  0x00000000004011d1 in main ()
(gdb) quit
A debugging session is active.

        Inferior 1 [process 2204070] will be killed.

Quit anyway? (y or n) y
iulians@tumbleweed:~/diagnose/debugging>

```
As you can see, inside the debugger we have some commands that can help us see where the problem is. One such command is `bt` which is the same as `backtrace` showing the backtrace of the point where the program is stopped (in this case where it crashes).

We can see the function where the crash happens and also the function calling it and so on.

But we don't get any information about where the problem might be. This is because we didn't compile using the `-g` flag which generates debug information.

By doing this we can see that the obtained executable has larger size (because of extra debug information added). Let's try again but compiling with `-g`.

```
iulians@tumbleweed:~/diagnose/debugging> gcc -g -Wall buggy.cpp -o buggy
buggy.cpp: In function ‘int func1(int)’:
buggy.cpp:8:16: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
    8 |         memset((char*)param,0,sizeof(param));
      |                ^~~~~~~~~~~~
iulians@tumbleweed:~/diagnose/debugging> gdb ./buggy
GNU gdb (GDB; openSUSE Tumbleweed) 16.3
....
(gdb) run
....
Program received signal SIGSEGV, Segmentation fault.
0x00007ffff7d733b0 in __memset_avx2_unaligned_erms () from /lib64/libc.so.6
Missing separate debuginfos, use: zypper install glibc-debuginfo-2.41-2.1.x86_64
(gdb) bt
#0  0x00007ffff7d733b0 in __memset_avx2_unaligned_erms () from /lib64/libc.so.6
#1  0x000000000040118c in func1 (param=44) at buggy.cpp:8
#2  0x00000000004011b2 in func0 (param=44) at buggy.cpp:15
#3  0x00000000004011d1 in main () at buggy.cpp:23
(gdb)



```

As you can see debug information shows the file name, line numbers where the problem happens so this makes it easier to identify where the problem resides which is at line 8.

A quick look shows us the problem:

```
iulians@tumbleweed:~/diagnose/debugging> cat -n buggy.cpp  | grep -E "^[[:space:]]+8[[:space:]]"
     8          memset((char*)param,0,sizeof(param));
iulians@tumbleweed:~/diagnose/debugging>
```

We are simply wriging to an address obtained from an integer which is not a valid memory address, therefore we crash out program.

Another simple example is to set some breakpoints. Those can be at a certain file line, or even functions. An example is here:
```
iulians@tumbleweed:~/diagnose/debugging> gdb ./buggy
GNU gdb (GDB; openSUSE Tumbleweed) 16.3
........
For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from ./buggy...
(gdb) break buggy.cpp:7
Breakpoint 1 at 0x401161: file buggy.cpp, line 7.
(gdb) run
Starting program: /home/iulians/diagnose/debugging/buggy
........
Using host libthread_db library "/lib64/libthread_db.so.1".
before calling func1()

Breakpoint 1, func1 (param=44) at buggy.cpp:7
7               printf("Called function with param: %d\n", param);
Missing separate debuginfos, use: zypper install glibc-debuginfo-2.41-2.1.x86_64
(gdb) bt
#0  func1 (param=44) at buggy.cpp:7
#1  0x00000000004011b2 in func0 (param=44) at buggy.cpp:15
#2  0x00000000004011d1 in main () at buggy.cpp:23
(gdb) next
Called function with param: 44
8               memset((char*)param,0,sizeof(param));
(gdb) next

Program received signal SIGSEGV, Segmentation fault.
0x00007ffff7d733b0 in __memset_avx2_unaligned_erms () from /lib64/libc.so.6
(gdb)

```

So you can pretty much do a lot of things with a debugger. This information is generally available for all debuggers, the ideas remains the same.

Key takeaways:
* debug information provides better debugging experience
* not optimizing code is easier to debug (you can test for yourself with -O options - see gcc help for more details)


Further work:
* attach to a running program.
  * Make a simple program that sleeps for some seconds (enough to run the gdb command to attach commands) - see https://man7.org/linux/man-pages/man3/sleep.3.html
  * run the program
* in a different terminal attach to it by PID. Program's pid can be found with `ps aux | grep my_program`
* attaching the debugger can be done via `gdb ./my_program -p <PID>`
* Remote debugging:
  * install gdbserver on a machine
  * follow the instructions on how to use it: https://gcc.gnu.org/onlinedocs/gcc-4.8.5/gnat_ugn_unw/Remote-Debugging-using-gdbserver.html
