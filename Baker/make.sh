#!/bin/bash
gcc Baker.c Semaphore.h Semaphore.c -o Baker -lpthread;
gcc Customer.c Semaphore.h Semaphore.c -o Customer -lpthread;
g++ reset.c Semaphore.c Semaphore.h -o reset;
