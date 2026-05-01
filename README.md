# game-of-life
Implementation of the Game of Life using CUDA programming in Raylib.
This is just a simple game of life implementation that uses a square grid of size `GRID_SIZE` and spawns a number of random [R-pentominos](https://www.youtube.com/watch?v=bTPN3spiq1I) and [Acorns](https://www.youtube.com/watch?v=dUq5SWXs0bc)

## Demonstration (real-time)

> GRID_SIZE = 3000 & 5000 random spawns (2 patterns each spawn so 10000, but this only makes it visually pleasing, no change in processing as it processes the whole grid everytime)

- GPU processing with CUDA
<img width="720" height="427" alt="CUDA" src="https://github.com/user-attachments/assets/ce098486-2729-4459-9941-d6a3e178b3a9" />


- CPU processing
<img width="720" height="427" alt="NONCUDA" src="https://github.com/user-attachments/assets/d220f844-9b6b-4394-988e-d281df19cbe2" />


## Prerequisites

- nvcc
- CUDA Toolkit
- raylib

## Running

- Run normal c file:
```
make run
```

- Run cuda file:
```
make run-cuda
```

## Sources

- https://pt.wikipedia.org/wiki/Jogo_da_vida
- https://www.raylib.com/cheatsheet/cheatsheet.html
- [Inventing Game of Life (John Conway) - Numberphile](https://www.youtube.com/watch?v=R9Plq-D1gEk)
- https://copy.sh/life/examples/
