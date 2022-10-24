# Elevator scheduling optimization

This program solves the following problem: 

Imagine a building with an infinite number of floors. On the ground level there are some people who intend to use the elevator to get to their destination floor. However there is only one elevator that can make a limited number of stops. Every person will disembark at the stop that is closer to his destination floor and will then go up or down the stairs to reach his destination. If every transition from a floor to a neighboring floor costs 1, find which stops the elevator should make, to minimaze the ultimate cost, as well as the cost.

## Concepts Used

The problem is solved with three different approaches:

-a recursive method

-a recursive method with memorization

-dynamic programming


## Compilation and Execution

Depending on the method you want to use, give the corresponding command for compilation:

-For the recursive method: ~$make -f Makefile_rec  (run as: ~$./elevator_rec)

-For the recursive method with memorization: ~$make -f Makefile_mem  (run as: ~$./elevator_mem)

-For the dynamic programming method: ~$make -f Makefile_dp  (run as: ~$./elevator_dp)
