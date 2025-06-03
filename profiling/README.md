Compile code with:

`gcc -Wall -pg -g -Wall linear.c -o linear`

Run:

`./linear`

Result:
```
iulians@tumbleweed:~/diagnose/profiling> gcc -pg -g -Wall linear.c -o linear
iulians@tumbleweed:~/diagnose/profiling> ./linear
Vector size:6000
Search count:10
Found=5
Not found=5
iulians@tumbleweed:~/diagnose/profiling> ls -ltr
total 36
-rw-r--r-- 1 iulians iulians     0 Jun  3 11:44 README.md
-rw-r--r-- 1 iulians iulians  1398 Jun  3 12:29 linear.c
-rwxr-xr-x 1 iulians iulians 25760 Jun  3 12:39 linear
-rw-r--r-- 1 iulians iulians  2794 Jun  3 12:39 gmon.out
iulians@tumbleweed:~/diagnose/profiling>

```
See the culprit:

`gprof ./linear gmon.out | less`

Exercise:

* Try optimizations. See improvements
* Change algorithm
* ...
