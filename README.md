# ECE 252 Assignment 1: Monte Carlo 

### Intended to be completed after lecture: 5


You will be  be parallelizing the Monte Carlo estimation for pi. It's a crude estimation that works by generating a bunch of random numbers
between [0, 1]. One can estimate pi by picking a collection of random points and counting how many are contained within a quarter circle. The more points one uses, the more precise the answer. See the algorithm description below.

### Primary Objective
The primary objective of this exercise is to practice using `fork()` in C as a mechanism for creating multiple processes and having these processes work together to solve a problem.

### Description of Behaviour

The goal is to implement the program so that the following behaviour occurs:


Your program will generate _i_ random points, determine _c_ by calculating how many are enclosed in the quarter
circle, and finally output the estimate for pi.

A more detailed explanation: for each iteration, generate two random numbers _x_ and _y_, and check if 
x<sup>2</sup> + y<sup>2</sup> <= 1. If so, then add 1 to the accumulator. Repeat this for
_i_ pairs of random points. Of those points, _c_ pairs fall within the quarter circle. Then, we can estimate pi by calculating 4 * c/i (since the
ratio of the quarter circle's area to that of the square is pi/4). 


Using the starter code, create a multiple-process version of this program. When you run it (`bin/montecarlo`), it takes two arguments: `-p` for the number of child processes to create, and `-i` for the number of iterations (points to generate). All of the work of generating points and checking them should be done by the child process(es); when this is all complete then the estimate for pi is printed by the parent process.

### General Guidance 

If you're having trouble, try checking these things, and it may resolve your problem.

* Check the documentation for how functions work if you are unfamiliar with them (google is your friend!)
* Check return values (and possibly `errno`) for network functions if things are going wrong.
* Have you initialized all variables? It is easy to forget; `malloc` does not initialize the value...
* Pay attention to compiler warnings. Warnings can sometimes be ignored, but may be a hint about a semantic error that you will want to resolve. If the compiler is trying to tell you something, listen.
* You need a buffer for receiving the data, and the receive function tells you how many bytes the server sent.
* Don't forget null terminators for strings!
* Is there a missing or extra `*` (dereference) on a pointer somewhere?
* Does every memory allocation have a matching deallocation?
* It may be helpful to put `printf()` statements to follow along what the program is doing and it may help you narrow down where the issue is.

### Submitting Your Code. 
When you're done, use the command `git commit -a` to add all files to your commit and enter a commit message. Then `git push` to upload your changes to your repository. You can commit and push as many times as you like; we'll look at whatever was last pushed.
