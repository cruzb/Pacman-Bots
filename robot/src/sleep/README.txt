Library to control SmartFusion MSS hardware timers

Blocking sleep for the specified number of microsenconds using usleep.
Blocking sleep for the specified number of nanoseconds using nsleep.

Set a non-blocking timer for the specified number of microseconds with timer_set.
Optionally, supply a function to be called on completion of the timer.

Get the current count of the non-blocking timer with timer_get.

Check the status of the non-blocking timer with timer_done