/*
--------------------------------------------------------
MyAlloc: Reimplementation of malloc
CONTRIBUTORS: Abdullah Azeem <abdullahazeem770@gmail.com
--------------------------------------------------------

Functionalities:
 - Coalescing
 - Allocation using sbrk for smaller requests and mmap for larger requests
 - exploring various fitting algorithms (first fit, best fit, worst fit, binning strategies...)
*/

