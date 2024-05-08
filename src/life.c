#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define CELL_DEAD 0
#define CELL_ALIVE 1

int main() {
	srand(time(NULL)); /* seeding rng */

	int max_row,max_col;

	initscr();
	clear();
	curs_set(0);

	getmaxyx(stdscr,max_row,max_col);

	int smatrix[max_row][max_col];
	int work_matrix[max_row][max_col];
	
	/* Variables related to our cells */
	int num_neighbors;
	int cstate;

	/* Initialize our starting matrix */
	for(int r = 0; r < max_row; r++) {
		for(int c = 0; c < max_col; c++) {
			if(rand() % 3 == 0) { 
				smatrix[r][c] = CELL_ALIVE;
			} else {
				smatrix[r][c] = CELL_DEAD;
			}
		}
	}

	/* Main loop */
	do {
		/* Note that all logic is run on work_matrix and then later copied over to smatrix, so that all operations are pure */
		for (int r = 0; r < max_row; r++) {
			for(int c = 0; c < max_col; c++) {
				/* Check how many neighbors are alive or dead (while avoiding overflows) */
				if(r != 0 && c != 0) {
					num_neighbors = 0;
					cstate = smatrix[r][c];
					for(int nr = r - 1; nr < r+2; nr++) {
						for(int nc = c - 1; nc < c + 2; nc++) {
							if(nr == r && nc == c) { /* don't let a cell count itself */
								continue;
							} else if(smatrix[nr][nc] == CELL_ALIVE) {
								num_neighbors++;
							}
						}
					}
					if(cstate == CELL_DEAD && num_neighbors == 3) {
						work_matrix[r][c] = CELL_ALIVE;
					} else if(cstate == CELL_ALIVE && num_neighbors > 3) {
						work_matrix[r][c] = CELL_DEAD;
					} else if(cstate == CELL_ALIVE && num_neighbors < 2) {
						work_matrix[r][c] = CELL_DEAD;
					} else if(cstate == CELL_ALIVE) {
						work_matrix[r][c] = CELL_ALIVE;
					} else {
						work_matrix[r][c] = CELL_DEAD;
					}

				}
			}
		}
		/* Display Loop */
		for(int r = 0; r < max_row; r++) {
			for(int c = 0; c < max_col; c++) {
				/* Overwrite the old cell state with the new */
				smatrix[r][c] = work_matrix[r][c];
				if(smatrix[r][c] == CELL_ALIVE) {
					mvaddch(r,c,' '|A_REVERSE);
				} else {
					mvaddch(r,c,' ');
				}
			}
		}
		refresh();
	} while(1);

}
