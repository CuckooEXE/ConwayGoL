/**
 * @Author Axel Persinger (me@axelp.io)
 * @date   September, 2017
 * @brief  Conway's Game of Life
 *
 * Implement Conway's Game of Life in C to a text file
 *
 * Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
 * Any live cell with two or three live neighbours lives on to the next generation.
 * Any live cell with more than three live neighbours dies, as if by overpopulation.
 * Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
 */
#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "main.h"

#define ALIVE 1
#define DEAD  0
#define mod %

int** board;
int sq;
int g;

/**
 * @name    kill_cell
 * @brief   Kills cell
 *
 * Kills a cell at a specified point
 *
 * @param [int] x  X coordinate of cell
 * @param [int] y  Y coordinate of cell
 *
 * @retval void
 *
 * Example Usage:
 * @code
 *    kill_cell(5, 2);
 * @code
**/
void kill_cell(int i, int j) {
	board[i][j] = DEAD;
}

/**
 * @name    produce_cell
 * @brief   Produces cell
 *
 * Produces a cell at a specified point
 *
 * @param [int] x  X coordinate of cell
 * @param [int] y  Y coordinate of cell
 *
 * @retval void
 *
 * Example Usage:
 * @code
 *    produce_cell(5, 2);
 * @code
**/
void produce_cell(int i, int j) {
	board[i][j] = ALIVE;
}

/**
 * @name    print_board
 * @brief   Prints board
 *
 * Prints all cells in a board
 *
 * @retval void
 *
 * Example Usage:
 * @code
 *    free_board();
 * @print_board
**/
void print_board() {
	for(int i = 0; i < sq; i++) {
		for(int j = 0; j < sq; j++) {
			printf("%d", board[i][j]);
		}
		printf("\n");
	}
}

/**
 * @name    write_board
 * @brief   Writes board to file
 *
 * Writes the board to a file using '#' for alive cells and '-' for dead cells
 *
 * @param [char*] fname  Filename to write to
 *
 * @retval void
 *
 * Example Usage:
 * @code
 *    write_board();
 * @endcode
**/
void write_board(char* fname) {
	//TODO: Find a better way to store dead cells
	FILE* fp;
	if((fp = fopen(fname, "w")) == NULL) {
		printf("Error opening %s for writing.", fname);
		return;
	}
	char symbol;
	for(int i = 0; i < sq; i++) {
		for(int j = 0; j < sq; j++) {
			if(board[i][j] == ALIVE) {
				symbol = '#';
			} else if(board[i][j] == DEAD) {
				symbol = '-';
			}
			fputc(symbol, fp);
		}
		fputc('\n', fp);
	}
	fclose(fp);
}

/**
 * @name    count_neighbors
 * @brief   Counts the neighbors of a given cell
 *
 * Counts the neighbors of a given cell by finding the status of adjacent cells.
 *
 * @param [int] x  X coordinate of cell
 * @param [int] y  Y coordinate of cell
 *
 * @retval int either ALIVE or DEAD
 *
 * Example Usage:
 * @code
 *    count_neighbors(3, 5); //Returns either ALIVE or DEAD (1 or 0)
 * @endcode
**/
int count_neighbors(int i, int j) {
	int up = (i==0)       ? board[sq-1][j] : board[i-1][j];
	int down = (i==sq-1)  ? board[0][j]    : board[i+1][j];
	int left = (j==0)     ? board[i][sq-1] : board[i][j-1];
	int right = (j==sq-1) ? board[i][0]    : board[i][j+1];

	//Count Diagonals
	int ll, lr, ul, ur; //Lower left, lower right, upper left, upper right.
	if(i==0&&j==sq-1) {
		ll = board[0][sq-1];
	} else {
		ll = board[i+1][j-1];
	}

	if(i==sq-1&&j==sq-1) {
		//ll
		lr = board[0][0];
	} else {
		lr = board[i+1][j+1];
	}

	if(i==0&&j==0) {
		ul = board[sq-1][sq-1];
	} else {
		ul = board[i-1][j-1];
	}

	if(i==sq-1&&j==0) {
		ur = board[sq-1][0];
	} else {
		ur = board[i-1][j+1];
	}


	return up + down + left + right + ll + lr + ul + ur;
}

/**
 * @name    loop_board
 * @brief   Loops through a board
 *
 * Loops through board and updates cells based on Conway's rules
 *
 * @retval void
 *
 * Example Usage:
 * @code
 *    loop_board();
 * @endcode
**/
void loop_board() {
	int n; // Number of neighbors when looping through board
	for(int gcounter = 0; gcounter < g; gcounter++) {
		for(int i = 0; i < sq; i++){
			for(int j = 0; j < sq; j++) {
				n = count_neighbors(i, j);
				printf("i=%d j=%d n=%d\n", i, j, n);
				if(n < 2) {
					// Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
					kill_cell(i, j);
				} else if(board[i][j] == DEAD && n == 3) {
					// Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
					produce_cell(i, j);
				} else if(n < 4 && n > 1) {
					// Any live cell with two or three live neighbours lives on to the next generation.
					continue;
				} else if(n < 3) {
					// Any live cell with more than three live neighbours dies, as if by overpopulation.
					kill_cell(i, j);
				}
			}
		}
		sleep(1);
		write_board("board");
	}
}

/**
 * @name    read_board
 * @brief   Reads a board from file
 *
 * Reads a board from a file to board global variable
 *
 * @param [char*] fname  Filename to read in from
 *
 * @retval void
 *
 * Example Usage:
 * @code
 *    read_board();
 * @endcode
**/
void read_board(const char* fname) {
	// Open the file for reading, loop through each char from each line
	FILE* fp; int x = 0; int y = 0;
	fp = fopen(fname, "r");
	char ch;

	while((ch = fgetc(fp)) != EOF) {
		if(ch == '\n') {
			x = 0; y++;
			continue;
		}
		if(ch == '-') {
			board[x][y] = DEAD;
		} else if(ch == '#') {
			board[x][y] = ALIVE;
		}
		x++;
	}
	fclose(fp);
}

/**
 * @name    free_board
 * @brief   Frees memory from board
 *
 * Frees all pointers in board
 *
 * @retval void
 *
 * Example Usage:
 * @code
 *    free_board();
 * @endcode
**/
void free_board() {
	for(int i = 0; i < sq; i++)
    free(board[i]);
	free(board);
}


int main(int argc, char const *argv[]) {
	if(argc == 3) {
		sq = atoi(argv[1]);
		g = atoi(argv[2]);
	} else {
		printf("Usage:\n\t ./main.o (int)SQUARE (int)GENERATIONS\n");
		return 1;
	}

	//Init board global variable
	board = malloc(sq*sizeof(int*));
	for(int i = 0; i < sq; i++) {
		board[i] = malloc(sq*sizeof(int));
		memset(board[i], 0, sq*sizeof(board[i][0]));
	}



	read_board("board");
	loop_board();
	write_board("board");
	free_board();

	return 0;
}
