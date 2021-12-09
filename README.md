# multiProcessAdditionGame

Project Name: Multi-Process addition game to learn Interprocess synchronization and concurrency using various locking mechanisms like blocking, non blocking and spin locks.

Project Descrition: The objective of this project is to demonstrate interprocess synchronization and concurrency by playing a game of addition in which multiple process modify a variable in the shared memory region until one process wins. 
The game: A process enters the game and adds two numbers present in the shared memory. If the sum of addition is greater than a predefined variable M then the process declares itself the winner and exits. Else the process swaps the lesser of two numbers with the value of sum and exits to play the game again. All processes play the game concurrently until a winner is declared.

Installation:
Compile the project using command: 'make'
Run the project by passing command line arguments for number of process(P) and winning value(M) using command: './additiongame P M'

Usage:
The project can be used to learn interprocess synchronization and concurrency. The game employees different locking mechanisms for the user to exploit. You can test the performance of each locking mechanism employed within the game.
