#include "path_planning.h"
#include <stdio.h>

int discovered_cell[MAXIMUM_NODE_NUM];

void map_preprocess_init(void) {
    int i = 0;
    for (; i < 900; ++i) {
        discovered_cell[i] = 0;
        processed_map.map[i / 30][i % 30].e_open = 1;
        processed_map.map[i / 30][i % 30].w_open = 1;
        processed_map.map[i / 30][i % 30].n_open = 1;
        processed_map.map[i / 30][i % 30].s_open = 1;
    }
}

void map_iterative_preprocess(maze_map *raw_map) {
    int bot_x = 0; //In MIPS, use memory I/O to get real location of bot
    int bot_y = 0;
    //update center cell, note that we don't need to care about consistency
    processed_map.map[bot_y][bot_x].e_open = raw_map->map[bot_y][bot_x].e_open;
    processed_map.map[bot_y][bot_x].w_open = raw_map->map[bot_y][bot_x].w_open;
    processed_map.map[bot_y][bot_x].n_open = raw_map->map[bot_y][bot_x].n_open;
    processed_map.map[bot_y][bot_x].s_open = raw_map->map[bot_y][bot_x].s_open;

    if (bot_x != 0) {
        //left
        processed_map.map[bot_y][bot_x - 1].e_open = raw_map->map[bot_y][bot_x - 1].e_open;
        processed_map.map[bot_y][bot_x - 1].w_open = raw_map->map[bot_y][bot_x - 1].w_open;
        processed_map.map[bot_y][bot_x - 1].n_open = raw_map->map[bot_y][bot_x - 1].n_open;
        processed_map.map[bot_y][bot_x - 1].s_open = raw_map->map[bot_y][bot_x - 1].s_open;
        //we dont need to update upper left cell and lower left
        if (bot_x != 1) {
            processed_map.map[bot_y][bot_x - 2].e_open = raw_map->map[bot_y][bot_x - 1].w_open;
        }
    }

    if (bot_x != 29) {
        //right
        processed_map.map[bot_y][bot_x + 1].e_open = raw_map->map[bot_y][bot_x + 1].e_open;
        processed_map.map[bot_y][bot_x + 1].w_open = raw_map->map[bot_y][bot_x + 1].w_open;
        processed_map.map[bot_y][bot_x + 1].n_open = raw_map->map[bot_y][bot_x + 1].n_open;
        processed_map.map[bot_y][bot_x + 1].s_open = raw_map->map[bot_y][bot_x + 1].s_open;
        //we dont need to update upper right cell and lower right
        if (bot_x != 28) {
            processed_map.map[bot_y][bot_x + 2].w_open = raw_map->map[bot_y][bot_x + 1].e_open;
        }
    }

    if (bot_y != 0) {
        //upper
        processed_map.map[bot_y - 1][bot_x].e_open = raw_map->map[bot_y - 1][bot_x].e_open;
        processed_map.map[bot_y - 1][bot_x].w_open = raw_map->map[bot_y - 1][bot_x].w_open;
        processed_map.map[bot_y - 1][bot_x].n_open = raw_map->map[bot_y - 1][bot_x].n_open;
        processed_map.map[bot_y - 1][bot_x].s_open = raw_map->map[bot_y - 1][bot_x].s_open;
        if (bot_y != 1) {
            //not 0 or 1
            processed_map.map[bot_y - 2][bot_x].s_open = raw_map->map[bot_y - 1][bot_x].n_open;
        }
        if (bot_x != 0) {
            //upper left
            processed_map.map[bot_y - 1][bot_x - 1].e_open = raw_map->map[bot_y - 1][bot_x - 1].e_open;
            processed_map.map[bot_y - 1][bot_x - 1].w_open = raw_map->map[bot_y - 1][bot_x - 1].w_open;
            processed_map.map[bot_y - 1][bot_x - 1].n_open = raw_map->map[bot_y - 1][bot_x - 1].n_open;
            processed_map.map[bot_y - 1][bot_x - 1].s_open = raw_map->map[bot_y - 1][bot_x - 1].s_open;
            if (bot_x != 1) {
                processed_map.map[bot_y - 1][bot_x - 2].e_open = raw_map->map[bot_y - 1][bot_x - 1].w_open;
            }
            if (bot_y != 1) {
                processed_map.map[bot_y - 2][bot_x - 1].s_open = raw_map->map[bot_y - 1][bot_x - 1].n_open;
            }
        }
        if (bot_x != 29) {
            //upper right
            processed_map.map[bot_y - 1][bot_x + 1].e_open = raw_map->map[bot_y - 1][bot_x + 1].e_open;
            processed_map.map[bot_y - 1][bot_x + 1].w_open = raw_map->map[bot_y - 1][bot_x + 1].w_open;
            processed_map.map[bot_y - 1][bot_x + 1].n_open = raw_map->map[bot_y - 1][bot_x + 1].n_open;
            processed_map.map[bot_y - 1][bot_x + 1].s_open = raw_map->map[bot_y - 1][bot_x + 1].s_open;
            if (bot_x != 28) {
                processed_map.map[bot_y - 1][bot_x + 2].w_open = raw_map->map[bot_y - 1][bot_x + 1].e_open;
            }
            if (bot_y != 1) {
                processed_map.map[bot_y - 2][bot_x + 1].s_open = raw_map->map[bot_y - 1][bot_x + 1].n_open;
            }
        }
    }
    if (bot_y != 29) {
        //lower
        processed_map.map[bot_y + 1][bot_x].e_open = raw_map->map[bot_y + 1][bot_x].e_open;
        processed_map.map[bot_y + 1][bot_x].w_open = raw_map->map[bot_y + 1][bot_x].w_open;
        processed_map.map[bot_y + 1][bot_x].n_open = raw_map->map[bot_y + 1][bot_x].n_open;
        processed_map.map[bot_y + 1][bot_x].s_open = raw_map->map[bot_y + 1][bot_x].s_open;
        if (bot_y != 28) {
            processed_map.map[bot_y + 2][bot_x].n_open = raw_map->map[bot_y + 1][bot_x].s_open;
        }
        if (bot_x != 0) {
            //lower left
            processed_map.map[bot_y + 1][bot_x - 1].e_open = raw_map->map[bot_y + 1][bot_x - 1].e_open;
            processed_map.map[bot_y + 1][bot_x - 1].w_open = raw_map->map[bot_y + 1][bot_x - 1].w_open;
            processed_map.map[bot_y + 1][bot_x - 1].n_open = raw_map->map[bot_y + 1][bot_x - 1].n_open;
            processed_map.map[bot_y + 1][bot_x - 1].s_open = raw_map->map[bot_y + 1][bot_x - 1].s_open;
            if (bot_x != 1) {
                processed_map.map[bot_y + 1][bot_x - 2].e_open = raw_map->map[bot_y + 1][bot_x - 1].w_open;
            }
            if (bot_y != 28) {
                processed_map.map[bot_y + 2][bot_x - 1].n_open = raw_map->map[bot_y + 1][bot_x - 1].s_open;
            }
        }
        if (bot_x != 29) {
            //lower right
            processed_map.map[bot_y + 1][bot_x + 1].e_open = raw_map->map[bot_y + 1][bot_x + 1].e_open;
            processed_map.map[bot_y + 1][bot_x + 1].w_open = raw_map->map[bot_y + 1][bot_x + 1].w_open;
            processed_map.map[bot_y + 1][bot_x + 1].n_open = raw_map->map[bot_y + 1][bot_x + 1].n_open;
            processed_map.map[bot_y + 1][bot_x + 1].s_open = raw_map->map[bot_y + 1][bot_x + 1].s_open;
            if (bot_x != 28) {
                processed_map.map[bot_y + 1][bot_x + 2].w_open = raw_map->map[bot_y + 1][bot_x + 1].e_open;
            }
            if (bot_y != 28) {
                processed_map.map[bot_y + 2][bot_x + 1].n_open = raw_map->map[bot_y + 1][bot_x + 1].s_open;
            }
        }
    }
}

/*
void map_preprocess(maze_map* raw_map, maze_map* processed_map) {
    int i = 0;
    for (; i < 900; ++i) {
        discovered_cell[i] = 0;
        // In MIPS, this reduces to raw_map->[i] (why?)
        char e_open = raw_map->map[i / 30][i % 30].e_open;
        char w_open = raw_map->map[i / 30][i % 30].w_open;
        char n_open = raw_map->map[i / 30][i % 30].n_open;
        char s_open = raw_map->map[i / 30][i % 30].s_open;
        if (e_open | w_open | n_open | s_open) {
            //this is a discovered cell
            discovered_cell[i] = 1;
        } else {
            processed_map->map[i / 30][i % 30].e_open = 1;
            processed_map->map[i / 30][i % 30].w_open = 1;
            processed_map->map[i / 30][i % 30].n_open = 1;
            processed_map->map[i / 30][i % 30].s_open = 1;
        }
    }
    i = 0;
    for (; i < 900; ++i) {
        if (i % 30 != 29) {
            if (discovered_cell[i + 1]) {
                processed_map->map[i / 30][i % 30].e_open = processed_map->map[i / 30][i % 30 + 1].w_open;
            }
        }
        if (i % 30 != 0) {
            if (discovered_cell[i - 1]) {
                processed_map->map[i / 30][i % 30].w_open = processed_map->map[i / 30][i % 30 - 1].e_open;
            }
        }
        if (i / 30 != 0) {
            if (discovered_cell[i - 30]) {
                processed_map->map[i / 30][i % 30].n_open = processed_map->map[i / 30 - 1][i % 30].s_open;
            }
        }
        if (i / 30 != 29) {
            if (discovered_cell[i + 30]) {
                processed_map->map[i / 30][i % 30].s_open = processed_map->map[i / 30 + 1][i % 30].n_open;
            }
        }
    }
    return;
}
*/

void refresh_pp(void) {
    int i = 0;
    for (; i < MAXIMUM_NODE_NUM; ++i) {
        prv_pos[i] = -1;
        visited_mark[i] = 0;
        command_buffer[i] = -1;
    }
}

void bfs_with_weight_update(maze_map* current_map, int* target_point) {
    int target_x = target_point[0];
    int target_y = target_point[1];

    int visited_node_count = 0;

    refresh_pp();

    int queue[MAXIMUM_NODE_NUM * 2]; //at most 900!
    int queue_ptr = -2;
    queue[0] = target_y * 30 + target_x;
    queue[1] = 0; //weight
    int ava_spot_ptr = 2;
    visited_mark[queue[0]] = 1;

    while (visited_node_count < 900) {
        queue_ptr = queue_ptr + 2;
        // if queue > end_ptr, we should end. but technically that should not happen.
        int current_node = queue[queue_ptr];
        int current_node_weight = queue[queue_ptr + 1];
        weight_mat[current_node] = current_node_weight;
        ++visited_node_count;
        int node_x = current_node % 30;
        int node_y = current_node / 30;
        if (current_map->map[node_y][node_x].e_open) {
            int next_pos = node_y * 30 + node_x + 1;
            if (next_pos / 30 == node_y && !(visited_mark[next_pos])) {
                visited_mark[next_pos] = 1; //mark
                queue[ava_spot_ptr++] = next_pos;
                queue[ava_spot_ptr++] = current_node_weight + 1;
            }
        }
        if (current_map->map[node_y][node_x].w_open) {
            int next_pos = node_y* 30 + node_x - 1;
            if (next_pos / 30 == node_y && !(visited_mark[next_pos])) {
                visited_mark[next_pos] = 1;
                queue[ava_spot_ptr++] = next_pos;
                queue[ava_spot_ptr++] = current_node_weight + 1;
            }
        }
        if (current_map->map[node_y][node_x].n_open) {
            int next_pos = node_y * 30 + node_x - 30;
            if (next_pos >= 0 && !(visited_mark[next_pos])) {
                visited_mark[next_pos] = 1;
                queue[ava_spot_ptr++] = next_pos;
                queue[ava_spot_ptr++] = current_node_weight + 1;
            }
        }
        if (current_map->map[node_y][node_x].s_open) {
            int next_pos = node_y * 30 + node_x + 30;
            if (next_pos < MAXIMUM_NODE_NUM && !(visited_mark[next_pos])) {
                visited_mark[next_pos] = 1;
                queue[ava_spot_ptr++] = next_pos;
                queue[ava_spot_ptr++] = current_node_weight + 1;
            }
        }
    }
}
