#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define CELL_DEAD 0
#define CELL_ALIVE 1
#define NUM_ROWS 500
#define NUM_COL 500

void swap_ptr(int (**p1)[NUM_ROWS][NUM_COL], int (**p2)[NUM_ROWS][NUM_COL]) {
	int (*tmp)[NUM_ROWS][NUM_COL] = *p1;
	*p1 = *p2;
	*p2 = tmp;
	return;
}

int main() {
	srand(time(NULL)); /* seeding rng */

	int max_row,max_col;

	initscr();
	clear();
	curs_set(0);
	
	getmaxyx(stdscr,max_row,max_col);

	int matrix1[NUM_ROWS][NUM_COL];
	int matrix2[NUM_ROWS][NUM_COL];

	/* We work with pointers to our arrays so we can save time swapping them out */
	int (*read_matrix)[NUM_ROWS][NUM_COL] = &matrix1;
	int (*write_matrix)[NUM_ROWS][NUM_COL] = &matrix2;

	/* Calculate where we need our screen to start and end */
	int screen_start_row = (NUM_ROWS / 2) - (max_row / 2);
	int screen_start_col = (NUM_COL / 2) - (max_col / 2);

	int screen_end_row = (NUM_COL / 2) + (max_col / 2);
	int screen_end_col = (NUM_COL / 2) + (max_col / 2);

	/* Initialize our starting matrix */
	for(int r = 0; r < NUM_ROWS; r++) {
		for(int c = 0; c < NUM_COL; c++) {
			if(rand() % 3 == 0) { 
				(*read_matrix)[r][c] = CELL_ALIVE;
			} else {
				(*read_matrix)[r][c] = CELL_DEAD;
			}
		}
	}

	/* Main loop */
	do {
		/* Note that we have to work in 2 different arrays to ensure that all calculations are pure */
		for (int r = 0; r < NUM_ROWS; r++) {
			for(int c = 0; c < NUM_COL; c++) {
				/* Check how many neighbors are alive or dead (while avoiding overflows) */
				int num_neighbors = 0;
				int cstate = (*read_matrix)[r][c];
				for(int nr = r - 1; nr <= r+1; nr++) {
					for(int nc = c - 1; nc <= c + 1; nc++) {
						if(nr == r && nc == c) { /* don't let a cell count itself */
							continue;
						} else if (nr < 0 || nc < 0) { /* Don't allow underflow */
							continue;
						} else if (nr > NUM_ROWS || nc > NUM_COL) { /* Don't allow overflow */
							continue;
						}else if((*read_matrix)[nr][nc] == CELL_ALIVE) {
							num_neighbors++;
						}
					}
				}
				if(cstate == CELL_DEAD && num_neighbors == 3) {
					(*write_matrix)[r][c] = CELL_ALIVE;
				} else if(cstate == CELL_ALIVE && num_neighbors > 3) {
					(*write_matrix)[r][c] = CELL_DEAD;
				} else if(cstate == CELL_ALIVE && num_neighbors < 2) {
					(*write_matrix)[r][c] = CELL_DEAD;
				} else if(cstate == CELL_ALIVE) {
					(*write_matrix)[r][c] = CELL_ALIVE;
				} else {
					(*write_matrix)[r][c] = CELL_DEAD;
				}
			}
		}
		/* Display Loop */
		for(int r = 0; r < max_row; r++) {
			for(int c = 0; c < max_col; c++) {
				int cell = (*read_matrix)[r + screen_start_row][c + screen_start_col];
				if(cell == CELL_ALIVE) {
					mvaddch(r,c,' '|A_REVERSE);
				} else {
					mvaddch(r,c,' ');
				}
			}
		}
		/* Swap our 2d array pointers so that the next cycle can run without having to copy a million values */
		swap_ptr(&read_matrix, &write_matrix);
		refresh();
		usleep(90000);
	} while(1);

}
