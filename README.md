#### Lab6_Vector_Lab
## Vector Lab Calculator
# @Author *Matthew Schiemann*

This lab is a culmination of Lab 5,6,7, and 8 in which we are using the git repo to demonstrate how we may use and manipulate files in collaboration with others.

<For this program, we have built a vector calculator which allows the input of vectors in the form a = 1 2 3 and allows us to perform multiple operations between vectors and such. Upgrades have been added along the way such as the addition of storage and loading of vectors to CSV files.>

To Build, we use our Make file with the command Make All
Otherwise to use the basic compiler
gcc -o main -Wall -lm storage.c vect.c ui.c main.c

# The list of available operations is as follows:
    - minimat - interactive 3D vector calculator 
    - a = 1 2 3        store a vector commas or spaces; z optional
    - a                display a vector
    - a + b            add
    - a - b            subtract
    - a * 2 | 2 * a    scalar multiply
    - a . b            dot product
    - a x b            cross product
    - c = a + b        operate and store the result in c
    - list             show all stored vectors
    - clear            erase all stored vectors
    - quit             exit
    - save             save vectors to a csv file
    - load             load vectors from a csv file
    - 'quit' to exit.

    Our program dynamically allocates memory based on how many values we have stored of vectors. 

