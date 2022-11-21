## 2D-Heat-Equation_OpenMP

## Introduction
2D Heat Equation for a Square Plate. Solution found by Iterating through the System Algebraic Equations via the OpenMP API.

## Scenario

The north (top) plate has a temperature of 0 deg. celsius
The rest of the plate-sides have temperatures of 100 deg. celsius

                            I = 0
               [0][0]------------------[0][N-1]
                    |                  |
             J = 0  |                  |  J = N-1
                    |                  |
             [M-1][0]------------------[M-1][N-1]
              
                            I = M-1
                            
this diagram will prove very useful so refer to it.
solution to the heat-equation:

      `W(Central) = (1/4)*[W(North] + w[South] + w[East] + w[West])`

## Operating System
`Ubuntu 20.04`

## Text Editor
`Visual Studio Code`

## Compiler
`gcc/g++ 9.4.0`

## OpenMP API
OpenMP is typically used for loop-level parallelism, but it also supports function-level parallelism.
In this project, OpenMP is used to parallelise the iterator. By using multithreading, we are able to achive
faster speeds while iterating through the loop structures, thus obtain a solution/heat distribution of a plate
more quickly than we would via a sequential programming.
OpenMP comes with `gcc/g++` and `gfortran` so there is no need to install anything.

## Running the Application

  * `$ git clone http://github/MRLintern/2D-Heat-Equation_OpenMP.git`
  * `$ make`
  * `$ ./2D_Heat`

## TODO

This application is very much in its infancy. It needs to sorted into a "proper project structure", including things like unit tests, a better Makefile for building the whole project and reorganising the source code into header files where appropriate. e.g., separate functions into header files thus increasing readability. It will also prove interesting to allow the user to set the required number of threads. Once this has been done, we could make plots of performance of the application (e.g. time and speed) against the number of iterations perfomed.
  
  
  
  

