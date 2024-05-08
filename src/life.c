#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define CELL_DEAD 0
#define CELL_ALIVE 1
#define NUM_ROWS 500
#define NUM_COL 500

int main() {
	srand(time(NULL)); /* seeding rng */

	int max_row,max_col;

	initscr();
	clear();
	curs_set(0);
	
	getmaxyx(stdscr,max_row,max_col);

	int smatrix[NUM_ROWS][NUM_COL];
	int work_matrix[NUM_ROWS][NUM_COL];

	/* Calculate where we need our screen to start and end */
	int screen_start_row = (NUM_ROWS / 2) - (max_row / 2);
	int screen_start_col = (NUM_COL / 2) - (max_col / 2);

	int screen_end_row = (NUM_COL / 2) + (max_col / 2);
	int screen_end_col = (NUM_COL / 2) + (max_col / 2);

	/* Variables related to our cells */
	int num_neighbors;
	int cstate;

	/* Initialize our starting matrix */
	for(int r = 0; r < NUM_ROWS; r++) {
		for(int c = 0; c < NUM_COL; c++) {
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
		for (int r = 0; r < NUM_ROWS; r++) {
			for(int c = 0; c < NUM_COL; c++) {
				/* Check how many neighbors are alive or dead (while avoiding overflows) */
				num_neighbors = 0;
				cstate = smatrix[r][c];
				for(int nr = r - 1; nr <= r+1; nr++) {
					for(int nc = c - 1; nc <= c + 1; nc++) {
						if(nr == r && nc == c) { /* don't let a cell count itself */
							continue;
						} else if (nr < 0 || nc < 0) { /* Don't allow underflow */
							continue;
						} else if (nr > NUM_ROWS || nc > NUM_COL) { /* Don't allow overflow */
							continue;
						}else if(smatrix[nr][nc] == CELL_ALIVE) {
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
		/* Display Loop */
		for(int r = 0; r < NUM_ROWS; r++) {
			for(int c = 0; c < NUM_COL; c++) {
				/* Overwrite the old cell state with the new */
				smatrix[r][c] = work_matrix[r][c];
			}
		}
		for(int r = 0; r < max_row; r++) {
			for(int c = 0; c < max_col; c++) {
				int cell = smatrix[r + screen_start_row][c + screen_start_col];
				if(cell == CELL_ALIVE) {
					mvaddch(r,c,' '|A_REVERSE);
				} else {
					mvaddch(r,c,' ');
				}
			}
		}
		refresh();
		usleep(90000);
	} while(1);

}
