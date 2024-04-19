### 4.8: Provide two programming examples in which multithreading does not provide better performance than a single-threaded solution.

Answer:
Multi-threading does not provide better performance than a single-threaded solution if the tasks are not parallelizable. 
For example, if the tasks are dependent on each other, then multi-threading will not provide better performance. 
```
public class Main {
    public static void main(String[] args) {
        int a = 0;
        int b = 0;
        for (int i = 0; i < 1000000; i++) {
            a++;
            b++;
        }
        System.out.println("a: " + a + ", b: " + b);
    }
}
```
In the above example, the tasks are dependent on each other, so multi-threading will not provide better performance. Multi-threading introduces overhead in terms of context switching, synchronization, and communication between threads.

Another example is when the tasks are I/O bound, multi-threading will not provide better performance.

```
public class Main {
    public static void main(String[] args) {
        String url = "https://www.example.com";
        String data = readDataFromURL(url);
        System.out.println(data);
    }
}
```
In the above example, the task is I/O bound as it involves reading data from a URL. Multi-threading will not provide better performance in this case as the bottleneck is the I/O operation, not the computation.

### 4.10: Which of the following components of
program state are shared across threads in a
multithreaded process?
(a) Register values
(b) Heap memory
(c) Global variables
(d) Stack memory

Answer:
(b) Heap memory
(c) Global variables

Because register values and stack memory are thread-specific, they are not shared across threads in a multithreaded process. However, heap memory and global variables are shared across threads.

### 4.16: A system with two dual-core processors has four processors available for scheduling
• A CPU-intensive application is running on this system
• All input is performed at program start-up, when a single file
must be opened
• Similarly, all output is performed just before the program
terminates, when the program results must be written to a single
file
• Between start-up and termination, the program is entirely CPUbound
• Your task is to improve the performance of this application by
multithreading it
• The application runs on a system that uses the one-to-one
threading model (each user thread maps to a kernel thread)

• How many threads will you create to
perform the input and output? Explain.
• How many threads will you create for the
CPU-intensive portion of the application?
Explain.

Answer:
For the input and output portions of the application, I would create two threads. One thread would be responsible for opening the file and reading the input, while the other thread would be responsible for writing the output to the file. This would allow these I/O operations to be performed concurrently, improving the overall performance of the application.
For the CPU-intensive portion of the application, I would create four threads, one for each processor available for scheduling. This would allow the CPU-bound tasks to be distributed across the available processors, maximizing the utilization of the system's resources and improving the performance of the application.