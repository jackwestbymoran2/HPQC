1. This report looks at how MPI communication works by doing a series of tests. These tests focus on things like point-to-point communication, latency, bandwidth, collective operations and custom reductions. The goal is to compare ways MPI communicates and see how efficient they are in terms of how long they take to run and how well they scale.

2. MPI Communication Basics

I started by testing point-to-point communication between processes using MPI_Send and MPI_Recv. The main process got messages from all the processes in a loop and the other processes sent integers based on their rank.

The results showed that the order of the messages was not always the same and the output was different each time I ran the test. This showed that MPI works correctly. It does not guarantee that messages will arrive in a certain order.

3. Ping-Pong Latency Measurement

I made a program that used two processes to send a counter back and forth. I used MPI_Wtime to measure how long this took.

Here is what was found:

* When I did not do iterations the results were all over the place and not very useful.

* As I did iterations the average time it took became more stable.

* When I did many iterations the time it took became very consistent.

So to get a measure of MPI latency I needed to do many iterations to get rid of noise and other issues.

4. Bandwidth Measurement

I changed the program to send arrays of just numbers. I sent arrays of sizes to see how well MPI could handle them.

Here is what was found:

* When I sent arrays it took about the same amount of time no matter what.

* When I sent arrays it took longer to send them and the time it took was related to the size of the array.

* The time it took to send an array is related to the size of the array and the bandwidth of the connection.

So MPI communication changes from being slow because of latency to being slow because of bandwidth as the size of the message increases.

5. Vector Addition: Communication Strategies

I tested three ways to add vectors using MPI:

MPI_Bcast: the main process sends the array to all the other processes.

* This is easy to do. It sends more data than it needs to.

* It works okay. Not great.

MPI_Scatter: the main process breaks the array into pieces. Sends them to the other processes.

* This is a way to send data and it works very well.

* It is the way to do this and it scales well.

Manual Send/Recv: I use point-to-point communication to send the array pieces.

* This takes a lot of time. Is slow.

* It does not scale well.

So MPI_Scatter is the way to do this and manual communication is the worst.

6. Reduction Methods

I compared three ways to reduce data:

Send/Recv Reduction: each process sends its part of the result to the process, which adds them up.

* This takes a lot of time. Is slow.

MPI_Gather: the processes send their parts to the process, which adds them up.

* This works okay. It takes some extra time.

MPI_Reduce (MPI_SUM): MPI adds up the results for us.

* This is the way to do this.

* It takes the amount of time to communicate.

So MPI_Reduce is the way to reduce data.

7. Custom Reduction Operation

I made a custom reduction operation using MPI_Op_create. Compared it to the built-in MPI_SUM.

Here is what I found:

* Both ways gave the results so my custom operation works.

* My custom operation was slower than MPI_SUM.

* I had some problems at first because I were not handling arrays and numbers correctly. I fixed it by reducing numbers instead of arrays.

So custom reductions work,. The built-in MPI operations are much faster.

8. Overall In all my tests MPI collective operations worked better than point-to-point communication. The performance depended on the size of the message and the communication pattern.

Here is a summary of how each method worked:

* The fastest methods were MPI_Reduce and MPI_Scatter.

* The moderate methods were MPI_Bcast and MPI_Gather.

* The slowest methods were the manual Send/Recv implementations.

MPI performance changes from being slow because of latency to being slow because of bandwidth, as the size of the message increases.
