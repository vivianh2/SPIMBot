from __future__ import print_function
import sys
import numpy as np
from IPython import embed

def help():
    print("This program takes debugging map output and format it in C")
    print("Example Usage: python {0} map.txt".format(sys.argv[0]))

# struct maze_cell {
#     char s_open;
#     char w_open;
#     char n_open;
#     char e_open;
# };
#
# struct maze_map {
#     maze_cell map[30][30]; // hold the maze map
# };

def get_struct_str(printed_map, cell_x, cell_y):
    #1 is open; 0 is closed
    ret_str = ''
    lut = {True: 0, False: 1}
    upper_wall = printed_map[cell_y - 1][cell_x] == '-'
    lower_wall = printed_map[cell_y + 1][cell_x] == '-'
    left_wall = printed_map[cell_y][cell_x - 1] == '|'
    right_wall = printed_map[cell_y][cell_x + 1] == '|'
    ret_str += "    cur_cell.s_open = {0};\r\n".format(lut[lower_wall])
    ret_str += "    cur_cell.w_open = {0};\r\n".format(lut[left_wall])
    ret_str += "    cur_cell.n_open = {0};\r\n".format(lut[upper_wall])
    ret_str += "    cur_cell.e_open = {0};\r\n".format(lut[right_wall])
    return ret_str

def main(file_path = "weight.txt"):
    print_str = '#include "path_planning.h"\r\n\r\n'
    print_str += "void make_weight(void) {\r\n    //start\r\n    "
    #cell_coordinates = [(i, j) for i in range(60) for j in range(60) if i % 2 == 1 and j % 2 == 1]
    with open(file_path) as f:
        printed_weight = f.readlines()

    #embed()
    for y in range(30):
        row = printed_weight[y].replace('\n', '').split(' ')
        for x in range(30):
            id = y * 30 + x
            weight = int(row[x])
            #print_str += '\r\n'
            #print_str += get_struct_str(printed_map, x, y)
            print_str += "    weight_mat[{0}] = {1};\r\n".format(id, weight) #ensure it works correctly in python3

    print_str += '}'
    print(print_str)

if __name__ == '__main__':
    if len(sys.argv) == 2:
        main(sys.argv[1])
    else:
        main()
