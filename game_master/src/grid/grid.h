#ifndef GRID_H
#define GRID_H

#include <inttypes.h>
#include <float.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct grid {
	size_t num_rows;
	size_t num_cols;
    size_t num_cells;
	char** occupancy;
} grid_t;

typedef struct cell {
    size_t row;
    size_t col;
} cell_t;

void grid_init(grid_t* grid, char** occupancy);

void grid_destroy(grid_t* grid);

grid_t grid_copy(grid_t* grid_src);

char grid_get_occupancy(grid_t* grid, cell_t cell);

//uint8_t grid_get_occupancy_idx(grid_t* grid, size_t index);

void grid_set_occupancy(grid_t* grid, cell_t cell, char occupancy);

void grid_swap_occupancy(grid_t* grid, cell_t cell_a, cell_t cell_b);

cell_t grid_cell(size_t row, size_t col);

int grid_cell_occupied(grid_t* grid, cell_t cell);

int grid_cell_inbounds(grid_t* grid, cell_t cell);

size_t grid_cell_hash(cell_t cell);

int grid_cell_match(cell_t a, cell_t b);

cell_t grid_find(grid_t* grid, char occupant);

uint32_t grid_count(grid_t* grid, char occupant);

cell_t grid_cell_new(void);

double grid_cell_inf_weight(void);

void grid_print(grid_t* grid);


#endif
