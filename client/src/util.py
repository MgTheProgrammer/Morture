from string import ascii_lowercase
import itertools
from protocol import (MOON_ROWS,MOON_COLS)

def iter_all_strings():
    for size in itertools.count(1):
        for s in itertools.product(ascii_lowercase, repeat=size):
            yield "".join(s)

def moonboard_1d_to_2d(board_1d):
    board = [[0]*MOON_COLS for i in range(MOON_ROWS)]
    i = 0
    for column in range(MOON_COLS):
        for row in range(MOON_ROWS):
            if column % 2 == 0:
                row = MOON_ROWS-row-1
            board[row][column] = board_1d[i]
            i+=1
    return board