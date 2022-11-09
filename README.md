# Readers-Writers-using-POSIX-API
There are two ways to solve this Readers - writers problem. 
One is with strict alternating between readers and writers and second one is without strict alternating.
_____
**rw_part1.cc** is without strict alternating.
```
Program takes in two command line arguments: number of reader threads, number of writer threads.
Program creates several reader threads and several writer threads based on the arguments. Each
thread loops for several iterations for writing or reading.
The readers read a shared string and print the contents to the standard output. The writers write to the
same string. Only one writer can write at any time. Readers can access concurrently, 
but a writer needs mutual exclusion with other writers and
all readers.
The main() function initializes the semaphores (which are also shared by all
threads). It also creates the separate reader and writer threads, with number of threads based on command
line arguments. Once it has created the threads, the main() function will wait for the threads to finish.
```
_____
**rw_part2.cc** is with strict alternating.
```
Some updates of the writers may be missed by readers in part 1 which is without strict alternating. And
some contents may be read more than once. Part 2’s setup, on the other hand, will result in strict
alternating between writers and reader.
In Part 2, we assume that we will have many writers, and one reader.
The one reader reads from the shared string and prints the contents to the standard output. Several writers
write to the same shared string. Only one writer can write to the string at any time. And once any writer
writes, the reader should display it before other writers change the contents. In some sense, the reader is
like a display server that displays the contents of the buffer.
Using two semaphores, we can achieve both the mutual exclusion as well as the synchronization between
the reader and writers.
____
What if there are more readers than writers? well, it is not covered here.
```
