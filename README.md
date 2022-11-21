## 2D-Heat-Equation_OpenMP

## Introduction
2D Heat Equation for a Square Plate. Solution found by Iterating through the System Algebraic Equations via the OpenMP API

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

  *`$ git clone https://github.com/MRLintern/2D-Heat-Equation_OpenMP.git`
  *`$ make`
  *`./2D_Heat
  
  
  
  

