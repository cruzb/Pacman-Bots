#include "grid.h"

void grid_init(grid_t* grid, char** occupancy) {
	grid->num_rows = 0;
	grid->num_cols = 0;
	grid->num_cells = 0;
	if(!occupancy) {
		grid->occupancy = NULL;
		return;
	}

	{
	char** row = occupancy;
	while(*row) {
		char* col = *row;
		while(*col) {
			++(grid->num_cells);
			++col;
		}
		++(grid->num_rows);
		++row;
	}
	grid->num_cols = grid->num_cells / grid->num_rows;
	}

	{
	grid->occupancy = (char**)malloc(grid->num_rows * sizeof(char*) + 1);
	char** row_a = occupancy;
	char** row_b = grid->occupancy;
	while(*row_a) {
		char* col_a = *row_a;
		*row_b = (char*)malloc(grid->num_cols * sizeof(char) + 1);
		char* col_b = *row_b;
		while(*col_a) {
			*col_b++ = *col_a++;
		}
		*col_b = '\0';
		++row_a;
		++row_b;
	}
	*row_b = NULL;
	}
}

void grid_destroy(grid_t* grid) {
	if(!grid->occupancy) {
		return;
	}
	char** row = grid->occupancy;
	while(*row) {
		free((*row));
		++row;
	}
	free(grid->occupancy);
}

grid_t grid_copy(grid_t* grid_src) {
	grid_t grid_new;
	grid_init(&grid_new, grid_src->occupancy);
	return grid_new;
}

cell_t grid_cell(size_t row, size_t col) {
	return (cell_t){row, col};
}

uint32_t grid_count(grid_t* grid, char occupant) {
	size_t row, col;
	uint32_t count = 0;
	for(row = 0; row < grid->num_rows; ++row) {
		for(col = 0; col < grid->num_cols; ++col) {
			if(grid_get_occupancy(grid, grid_cell(row, col)) == occupant) {
				++count;
			}
		}
	}
	return count;
}

cell_t grid_find(grid_t* grid, char occupant) {
	size_t row, col;
	for(row = 0; row < grid->num_rows; ++row) {
		for(col = 0; col < grid->num_cols; ++col) {
			cell_t cell = grid_cell(row, col);
			if(grid_get_occupancy(grid, cell) == occupant) {
				return cell;
			}
		}
	}
	return grid_cell(grid->num_rows + 1, grid->num_cols + 1);
}

char grid_get_occupancy(grid_t* grid, cell_t cell) {
	return grid->occupancy[cell.row][cell.col];
    //return grid->occupancy[(cell.row * grid->num_cols) + cell.col];
}

//uint8_t grid_get_occupancy_idx(grid_t* grid, size_t index) {
//    return grid->occupancy[index];
//}

void grid_set_occupancy(grid_t* grid, cell_t cell, char occupancy) {
	grid->occupancy[cell.row][cell.col] = occupancy;
    //grid->occupancy[(cell.row * grid->num_cols) + cell.col] = occupancy;
}

void grid_swap_occupancy(grid_t* grid, cell_t cell_a, cell_t cell_b) {
	char tmp = grid_get_occupancy(grid, cell_a);
	grid_set_occupancy(grid, cell_a, grid_get_occupancy(grid, cell_b));
	grid_set_occupancy(grid, cell_b, tmp);
}

int grid_cell_occupied(grid_t* grid, cell_t cell) {
    return (grid_get_occupancy(grid, cell) == '1');
}

int grid_cell_inbounds(grid_t* grid, cell_t cell) {
    return (cell.row < grid->num_rows && cell.col < grid->num_cols);
}

size_t grid_cell_hash(cell_t cell) {
    return (cell.col*cell.col + cell.row*cell.row);
    //return (size_t)pow((double)(cell.col << 16), (double)cell.row);
};

int grid_cell_match(cell_t a, cell_t b) {
    return (a.row == b.row) && (a.col == b.col);
}

cell_t grid_cell_new(void) {
    return (cell_t){0, 0};
}

double grid_cell_inf_weight(void) {
    return DBL_MAX;
}

void grid_print(grid_t* grid) {
    int row = 0; int col = 0;
    for(; row < grid->num_rows; ++row) {
        for(col = 0; col < grid->num_cols; ++col) {
            printf("%u ", grid_get_occupancy(grid, (cell_t){row, col}));
        }
        printf("\n");
    }
}
