# Maze

A simple 2D maze solver based on breadth-first search.
(This program is a good example of interview problem.)

To simplify the problem, the maze is assumed to be given by a vector of the 
vector of integers, where 0 and 1 represent blocked and free region, respectively.

When this program runs, it prints the path to reach the goal given from the 
initial start position, in reverse, i.e., from the goal to the initial.

The circumference of the maze is assumed to be blocked cells.  Hence, the matrix
which represents the maze has zeroes in the first and last rows and columns.
```c++
using maze_t = vector<vector<int>>;

maze_t maze = {
    {0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 1, 1, 1, 0},
    {0, 1, 0, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 0, 1, 0, 0},
    {0, 1, 1, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0},
};
```

The coordinates are all 0 based and corresponds to the convention of matrix
index.  That is (0, 0) is the upper left corner cell, and (2, 3) is the cell 
in the 3rd row and 4th column.

The program solves the route from (1, 1) to (5, 5) as an example.
