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

#define ALIVE 1
#define DEAD  0

int** board;
int sq;

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
int count_neighbors(int x, int y) {
	return 0;
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

	for(int i = 0; i < sq; i ++) {
		for(int j = 0; j < sq; j++) {
			fputc((char) board[i][j], fp);
		}
	}

	fclose(fp);
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
		printf("Current ch: %c | %d | %d\t", ch, x, y);
		if(ch == '\n') {
			x = 0; y++;
			printf("Skipping to next line, %d %d\n", x, y);
			continue;
		}
		if(ch == '-') {
			printf("DEAD\n");
			board[x][y] = DEAD;
		} else if(ch == '#') {
			printf("ALIVE\n");
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
 * Frees all pointerss in board
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

int main(int argc, char const *argv[]) {
	if(argc == 2) {
		sq = atoi(argv[1]);
	} else {
		printf("Usage:\n\t ./main.o (int)SQUARE\n");
		return 1;
	}

	//Init board global variable
	board = malloc(sq*sizeof(int*));
	for(int i = 0; i < sq; i++) {
		board[i] = malloc(sq*sizeof(int));
		memset(board[i], 0, sq*sizeof(board[i][0]));
	}



	read_board("board");
	print_board();
	write_board("board");
	// free_board();

	return 0;
}
