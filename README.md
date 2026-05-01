# game-of-life
Implementation of the Game of Life using CUDA programming in Raylib.
This is just a simple game of life implementation that uses a square grid of size `GRID_SIZE` and spawns a number of random [R-pentominos](https://www.youtube.com/watch?v=bTPN3spiq1I) and [Acorns](https://www.youtube.com/watch?v=dUq5SWXs0bc)

## Demonstration (real-time)

> GRID_SIZE = 2000 & 5000 random spawns

- GPU processing with CUDA
<img width="720" height="432" alt="CUDA" src="https://github.com/user-attachments/assets/3dad9107-2a45-4404-8c29-b8b2d37a9a96" />

- CPU processing
<img width="720" height="432" alt="NONCUDA" src="https://github.com/user-attachments/assets/ab37ca50-dd51-48bf-a76e-dbe2e9c57b08" />


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
