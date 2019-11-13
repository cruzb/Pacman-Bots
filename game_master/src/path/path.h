#ifndef PATH_H
#define PATH_H

#include <inttypes.h>
#include <math.h>
#include "../grid/grid.h"
#include "array_cell_t.h"
#include "map_cell_t_double.h"
#include "map_cell_t_cell_t.h"
#include "link_list_cell_t.h"
#include <stdio.h>
#include "CMSIS/mss_assert.h"

typedef array_cell_t path_t;

void path_print(path_t* path);
path_t path_get(grid_t* grid, cell_t start_cell, cell_t end_cell);

void path_print(path_t* path) {
    cell_t* it = array_cell_t_front(path);
    cell_t* end = array_cell_t_back(path) + 1;
    while(it != end) {
        printf("{%u, %u}\n", it->row, it->col);
        ++it;
    }
}

map_cell_t_double g_score; // cost pose->(x,y)
map_cell_t_double f_score; // estimated cost pose->(x,y)->goal
map_cell_t_cell_t came_from;
link_list_cell_t open_set;

int f_score_lowest_cost(cell_t* a, cell_t* b) {
    return map_cell_t_double_get(&f_score, *a) > map_cell_t_double_get(&f_score, *b);
}

double f_score_heuristic(cell_t a, cell_t b) {
    int dx = abs(a.col - b.col);
    int dy = abs(a.row - b.row);
	return sqrt((double)(dx*dx + dy*dy));
}

path_t path() {
	path_t path;
	memset(&path, 0, sizeof(path));
	return path;
}

path_t path_get(grid_t* grid, cell_t start_cell, cell_t end_cell) {
	path_t path;
    array_cell_t_init(&path, 12);
    ASSERT(grid_cell_inbounds(grid, start_cell) && grid_cell_inbounds(grid, end_cell));

    map_cell_t_cell_t_init(&came_from, 200, grid_cell_hash, grid_cell_match, grid_cell_new, grid_cell_new);
    map_cell_t_double_init(&g_score, 200, grid_cell_hash, grid_cell_match, grid_cell_new, grid_cell_inf_weight);
    map_cell_t_double_init(&f_score, 200, grid_cell_hash, grid_cell_match, grid_cell_new, grid_cell_inf_weight);

    *map_cell_t_double_get(&g_score, start_cell) = 0.0;
    *map_cell_t_double_get(&f_score, start_cell) = f_score_heuristic(start_cell, end_cell);

    link_list_cell_t_init(&open_set, f_score_lowest_cost);
    link_list_cell_t_push(&open_set, &start_cell);

	while (!link_list_cell_t_empty(&open_set)) {
		cell_t current = link_list_cell_t_remove_front(&open_set);

		if (grid_cell_match(current, end_cell)) { //successful end
            while(!grid_cell_match(current, start_cell)) {
                array_cell_t_push(&path, &current);
                current = *map_cell_t_cell_t_get(&came_from, current);
            }
            //array_cell_t_push(&path, &current);
            break;
		}

		cell_t neighbors[4];
		neighbors[0] = (cell_t){current.row + 1, current.col};
		neighbors[1] = (cell_t){current.row, current.col + 1};
		neighbors[2] = (cell_t){current.row - 1, current.col};
		neighbors[3] = (cell_t){current.row, current.col - 1};

		size_t i;
		for (i = 0; i < 4; ++i) {
			if (!grid_cell_inbounds(grid, neighbors[i])) {
				continue;
            }
			if (grid_cell_occupied(grid, neighbors[i])) {
				continue;
            }

            double tent_g_score = *map_cell_t_double_get(&g_score, current) + 1.0;
			if (!map_cell_t_double_find(&g_score, neighbors[i])) { //Not in open set. This path is new
                link_list_cell_t_push(&open_set, &(neighbors[i]));
            }
            if(tent_g_score < *map_cell_t_double_get(&g_score, neighbors[i])) { //This path is new or better than before
                *map_cell_t_cell_t_get(&came_from, neighbors[i]) = current;
				*map_cell_t_double_get(&g_score, neighbors[i]) = tent_g_score;
                *map_cell_t_double_get(&f_score, neighbors[i]) = tent_g_score + f_score_heuristic(neighbors[i], end_cell);
            }
		}
	}

    map_cell_t_cell_t_destroy(&came_from);
    map_cell_t_double_destroy(&g_score);
    map_cell_t_double_destroy(&f_score);
    link_list_cell_t_destroy(&open_set);

	return path;
}

#endif
