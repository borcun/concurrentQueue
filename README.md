# Concurrent Queue
Concurrent Queue is a queue that is circulate head and tail pointers when they reach end of the queue.
Thus, the queue has never filled. However, some implementations may check queue size whether it is full
or not via checking tail pointer that reaches end of the queue. In this implementation, there is no check
buffer fullness.

Concurrent Queue has four functions: init, deinit, enqueue and dequeue. init and deinit are so understandable
function as reading their names, but enqueue and dequeue are thoroughly notions with respect to queue like
push, pop or front. The enqueue pushes an element at index where indicated by the tail, while the dequeue gets
element at index where indicated by the head.

The main function has two different test scenarios. The first one is checking all functions with simple test
procedures. The second one is a bit more sophisticate test step. In that test, two threads, consumer and producer,
are created, and executed concurrently. They compete for enqueuing and dequeuing on the circular buffer. To
prevent access by two threads at same time, a mutex is used to lock and unlock the circular buffer.
