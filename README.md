Developing applications most of the times leads developers in places where it is very difficult to understand everything just by reading the code, especially when project evolves faster than the team can keep up. Also it is very possible that knowledge is lost over periods of time because of other priorities or team changes over time.

NOTE: this will mostly focus on linux tools. Other operating systems have similar tools, some of the tools are even available on multiple platforms (cross platform tools).

Please check this repository for example and details on how to use the tools.

# Debugging

Debugging is the first thing to do when you’re in trouble. it is usually easily accessible. It doesn’t matter if we’re talking about Java/C/C++ they usually do the same thing:

* use debug information from the program (if it was compiled with it)
* stop the program at certain points (breakpoints)
* analyze program dumps resulted during a crash (they can also be created on request). In Linux those are called core dumps and are basically the state of a program at a certain moment in time
  * For a Java thread dump you can use `kill -3 <JAVA_PID>` to find out the call stack
  * For any type of program you can use `gcore <PID>` to dump a core
* Attach to a running program
  * that is recommended when reaching the part where debugging is requested takes a long time, because during debug the program can be slower than usually



Example of tools:

* Visual Studio environment (offers also a debugger)
* gdb (with various frontends)
* IntelliJ
* Eclipse
* ...

# Static analysis

No code is perfect. Even if it works, it doesn’t mean it’s perfect and there are many examples of code that doesn’t properly work, yet it is still used, usually without knowing it. Here is the moment where some tools can come handy and help you have “a look” over the entire source code. They go over the source code and report anything unusual/unexpected. Later, developers can analyze those reports and decide how to handle them.

Here is a list of tools in the order you should use:
* compiler
  * you will make sure that warnings are properly treated as some of them can become important during the program’s execution
  * you can even add some flags for extra warnings that can offer more information
* clang (open source)
  * a more robust compiler which is worth mentioning separately because if offers some nice reporting via tools like scan-build
  * it also offers the possibility to create some custom checks
    * just think you want to impose some naming convention and running such custom checker can help you see places where this doesn’t happen
* cppcheck (open source)
  * it provides reports for C++ files analysis 
* sonarqube (open source)
* coverity (commercial but available to integrate for open source under github - at least)
  * very profound analysis as it handles the entire code base going deep into functions, yielding very interesting findings

Extra:

* astyle (NOT A STATIC ANALYZER)
  * a very nice tools that aligns the code according to your specification so that all code is written in the same manner when it come to function parameters, parenthesis, brackets, conditions etc
  * it can help humans (not machines) visualizing the code better

# Runtime analysis

Of course programs need to be executed and compiler, static analysis cannot be trusted to report all the issues that may occur during runtime. That’s why there are tools that can be used at runtime to find out several issues.

## Debugger

One such tools is the debugger but the use is very limited because of the need to always interact with it. It can be used punctually if some specific part of code is suspected but it will fail to have an overview of the program.

## Compiler with specific options

* gcc with -fsanitize
* Visual Studio’s /fsanitize (similar to gcc’s option)

## External libraries 
memleax / libleak for example

## Dedicated tools

Some examples are:

* PurifyPlus (commercial)
* Electric Fence (open source)
* Valgrind (open source)

Valgrind is one tool that can be used at runtime to detect:

* memory leaks
* thread issues like race conditions
* cache misses

See more examples here: https://en.wikipedia.org/wiki/Memory_debugger 

# Logging

Sometimes when dealing with time sensitive applications like timeout in communication protocols for example, debugging might close the other end of the connection, rendering the debugging pretty useless. The same goes for embedded devices where timing is important and a miss of a heartbeat ( see https://en.wikipedia.org/wiki/Heartbeat_(computing)  ) triggers errors in other tasks/processes/threads.



In these particular cases logging is most likely the only feasible option. Depending on the logging tools you want to use you have a large variety of options, depending on what you want.

* log4j / log4j2
* log4c
* zlog
* …

Other libraries come with their own logging systems (Poco, ACE/TAO for example) so special care must be taken in order to make sure multiple logging systems can work together.


Most logs have additional information like:

* date and time (or at least some sort of timestamp)
* source file position (filename and line number)
* thread information (thread id)
* process information (for multi-process logs)
* log count as it can be useful for repetitive bugs happening mostly at the same time of execution for a long running task



Most of the tools have logging levels that should be properly used:

* critical level - logged on exceptional situations - usually program will fail to continue
* error level - something that needs to be logged, usually the program can recover from it, but it is not expected
* warning level - similar to error level, but expected. Sort of saying that a “nice to have” didn’t happen
* info - it is just used to display information about normal application execution
* debug - low level information that helps in understanding certain steps in some code execution


## Considerations

Excessive logging may influence program behavior by adding some performance penalty

Special care must be taken when logging information that is sensitive

Some companies try to avoid sharing logs with customer support since it might contain internal information so some filtering should be possible even after logs have been written

It might be that some logging libraries offer too many options that could increase the likelihood of security breaches. See https://nvd.nist.gov/vuln/detail/CVE-2021-44228 for example

Be sure that you know all capabilities of the logging system since some of them might be exploited

Asses the trends related to that particular solution you’re using since projects with lower effervescence (level of activity) tend to be left behind

# Coverage

Coverage means how much the code is “covered” by program execution. A simple way of explaining it is what lines are “hit” when code is executed. Of course this is more complex but this gives an idea about what this does.

A tool that can measure this is gcov: https://en.wikipedia.org/wiki/Gcov 

Coverage is very important if you want to make sure your code executes all branches during its execution time. 

In some safety critical environments code needs to have a very high coverage - usually more than 90%, the rest requiring clear reasons on why it cannot be covered (critical paths that are hard to reach in normal execution for example).

Such example is the next code that normally doesn’t fail in `stat` function call if the file was successfully opened.
```
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
    int fd = open("example.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("fstat");
        close(fd);
        return 1;
    }

    printf("File size: %ld bytes\n", st.st_size);

    close(fd);
    return 0;
}
```

There are multiple types of code coverage. More details here: https://en.wikipedia.org/wiki/Code_coverage 

# Profiling

This is mostly about the time when your program is slow, too slow for your processing unit to keep up, and you need to optimize.

Examples:

* a function/query takes too long and because of that some request yields a timeout
* a server can not serve too many clients as memory increases for each new connected client
* a game has lower FPS because of some unoptimized processing
* …

If you are in this situation, I’m sorry :). It is definitely challenging and if you’re open to challenges it can be very fun. This is the place where you might need to go deep, very deep, extremely deep. It doesn’t always happen but when it does, you’d better have all the tools you can get by your side because it is sure getting dirty.

The tools helping you with performance testing are called profilers but you can rely on other tools as good indicators for what is consuming a lot of CPU.


You can do:

* poor man’s profiling
* use a debugger and break the program from time to time and see where it ends most of the times
* generate a core dump (see debugging), thread stack etc and spot the places where most of the times you break - just like before, statistic
* check the logs (see logging) to check where is the place where most of the repetitive logs are (usually logs have filename and line number)
* use a profiler like gprof to spot the place where most of the time is spent

Once you found the function responsible for most of the time spent you can start looking at it. If no ideas come into play you will need to look even further, by testing the function in isolation in some cases - maybe with the same profilers.


Here you can think of:

* algorithm optimizations
  * See https://en.wikipedia.org/wiki/Big_O_notation 
* cache mises, where a tool offered by valgrind can be used; the tools is named “cachegrind” ( https://valgrind.org/docs/manual/cg-manual.html  )
* replacing code with lower level instructions
  * For example replacing Java/Python calls to native C/C++ implementations
  * Use instead of C code:
    * libraries optimized for performance like https://google.github.io/highway/en/master/
    * platform specific platform Assembly code (last resort)
* move code from CPU to GPU if possible
  * CUDA is a good example of API that can be used to do this



Examples of optimizations:

* https://en.wikipedia.org/wiki/Binary_search
  * it is often the case this is unnoticed as it is used almost in all databases having an index
* https://en.wikipedia.org/wiki/Lookup_table  / tables of precomputed values
  * think of the problem of calculating the bits from a number
* https://en.wikipedia.org/wiki/Fast_inverse_square_root
  * a very clever to optimize a function where most of the time was spent (no longer relevant as hardware instructions are now available but goes to show that optimizing certain functions consuming most of time makes good results)

