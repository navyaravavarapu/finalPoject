# CS377 FInal Project
For my final project I decided to extend Project 3 which was the scheduling project in order to implement the Multi-Level Feedback Queue (MLFQ) scheduling algorithm and the Last In, First Out (LIFO) scheduling algorithm as was one of the project ideas. The MLFQ algorithm is a scheduling algorithm that assigns priorities to processes based on their characteristics and adjusts their priorities over time. The LIFO algorithm is a scheduling algorithm that prioritizes the most recently arrived processes over the older ones. I had planned on creating my own custom scheduling algorithm however, I did not get around to it.

## Features
### MLFQ
* **Four priority levels**: The MLFQ algorithm supports four priority levels (queues), t0 prioritize difference processes.
* **Time slicing**: Each queue is assigned a different time, with higher priority queues receiving shorter time. This allows for fair scheduling and ensures that processes in lower priority queues get a chance to execute.
* **Round-Robin within levels**: Each priority level uses the Round-Robin scheduling policy to allocate CPU time among processes.
* **Boosting**: The MLFQ algorithm performs a boost operation to raises the priority of all processes to a higher priority level. This helps to ensure fairness among processes and improve the overall system performance.
* **Workload input**: This implementation accepts a workload file as input, containing arrival time and duration information for each process. This allows for simulating the scheduling algorithm with different workloads and evaluating its performance. This is similar to the other scheduling algorithms as they all accept the same input to create consistency when comparing the different algorithms.
### LIFO
* **Non-Preemptive**: The LIFO algorithm is a non-preemptive scheduling algorithm which means that once a process begins to execute, it will continue until it is completed without any interruptions in between.
* **Workload input**: This implementation accepts a workload file as input, containing arrival time and duration information for each process. This allows for simulating the scheduling algorithm with different workloads and evaluating its performance. This is similar to the other scheduling algorithms as they all accept the same input to create consistency when comparing the different algorithms.

## Video
[Final Project Video](https://drive.google.com/file/d/1Nz7vXwnYn158YUSqhiZKuNItBxxX7CiS/view?usp=sharing)
