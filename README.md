## SpeedTest

A small repo related to this post on Reddit: https://www.reddit.com/r/csharp/comments/1ejpg73/why_is_this_c_code_so_slow/

### Introduction
Benchmarking code across languages is hard due to the different use cases they support by default.
For example, C#/Java are JIT-compiled languages with longer startup times.
However, they can also compile the IL code to optimize machine code based on the instruction sets available.
C++ is an AOT-compiled language, which makes it much faster at startup, but distributing a pre-optimized binary is a pain, and you can never expect full performance on the end-user's system (unless you own the systems, of course).

### Code changes
The code provided in the Reddit post has a few differences between langauges. I'd like them all to have the exact same code for a more consistent comparison, so I had to make some changes.

* Go has some syntax limitations, as it does not support bracket-less statements. Therefore, I added brackets to all conditions in the other languages.
* Go also does not support increment-before-read syntax (++i), so I changed all the other languages to use increment-after-read (i++) syntax.
* Some languages don't support foreach-loops, so I change all to use for-loops for consistency. Some loops were a reverse-loop variant. I've changed it to a normal for-loop.
* I use Egyptian Bracket syntax (favored by Go/Java) to normalize the condition layout across all the languages.
* In order to avoid scope conflicts between ‘i' in for-loops with the ‘i' variable further down, I renamed the further down variable to ‘ii'.
* I also switch C++ to use auto everywhere to make it type-oblivious.
* The C# code uses `new StopWatch`, but there is a new API `StopWatch.GetTimestamp()` which makes it more consistent with other languages. Underneath the covers it calls `QueryPerformanceCounter`. Java does the same with `system.nanoTime()`.
* I've also implemented an accumulator in all langauges that serves as a way to avoid compiler optimizations that would otherwise remove the unused code.

Now, after doing all this, it is much easier to spot that the C++ variant does not call max() like the other languages do. I've added a call to max() for correctness.

### How to run?
First you need to install the needed runtimes:
* C#: [.NET 8](https://dotnet.microsoft.com/en-us/download/dotnet/thank-you/sdk-8.0.303-windows-x64-installer)
* Java: [Java 22](https://download.oracle.com/java/22/latest/jdk-22_windows-x64_bin.exe)
* Go: [GO 1.22.5](https://go.dev/dl/go1.22.5.windows-amd64.msi)
* C++: [MSVC 19.40.33812](https://go.microsoft.com/fwlink/?linkid=2272610)

1. Open `Developer Command Prompt for VS 2022` if you have Visual Studio 2022 installed. Alterantively, you cna add `cl` to your PATH variable.
2. Execute `Run.bat` to compile and run each benchmark

On my computer I get the following results:

```
### C# ###
Test: 0
n = 1          0.07 ns
n = 10         0.27 ns
n = 100        2.28 ns
n = 1000       23.76 ns
n = 10000      941.24 ns
n = 100000     11522.72 ns
n = 1000000    116770.44 ns
Accumulator: 27683200

### Java ###
Test: 0
n = 1          18.00 ns
n = 10         31.00 ns
n = 100        3669.00 ns
n = 1000       5638.00 ns
n = 10000      120020.00 ns
n = 100000     681503.00 ns
n = 1000000    6949688.00 ns
Accumulator: 27697700

### Go ###
Test: 0
n = 1         0.00 ns
n = 10        0.00 ns
n = 100       0.00 ns
n = 1000      5124.00 ns
n = 10000     30892.00 ns
n = 100000    373672.00 ns
n = 1000000   3839846.00 ns
Accumulator: 27713900

### C++ ###
Test: 0
n = 1          3.00 ns
n = 10         12.00 ns
n = 100        126.00 ns
n = 1000       1270.00 ns
n = 10000      12601.00 ns
n = 100000     127376.00 ns
n = 1000000    1281061.00 ns
Accumulator: 27701000
```