#ifndef UTIL_H
#define UTIL_H
#endif

#include "util.h"

#define ROW 24
#define COLUMN 80

#define GRAVITY 0.05
#define UP_VELOCITY -0.5

#define PIPE_RADIUS 3
#define OPENING_WIDTH_MIN 6
#define OPENING_WIDTH_MAX 10

#define FLAPPY_COL 10

#define TARGET_FPS 24

#define START_TIME_SEC 3

#define PROG_BAR_LEN 76
#define PROG_BAR_ROW 22

#define SCORE_START_COL 52

#define PIPE_COLOR 1
#define FLAPPY_COLOR 2
#define DEFAULT_COLOR 3

#define ESCAPE_KEY 27

using namespace std;

int frame = 0;
int currentScore = 0;
int bestScore = 0;

pipes p1, p2, p3;

void drawFloorAndCeiling(int ceiling, int floor, int spacing, int start)
{
	int i;
	attron(COLOR_PAIR(DEFAULT_COLOR));
	for (i = start; i < COLUMN - 1; i += spacing)
	{
		if (i < SCORE_START_COL)
			mvprintw(ceiling, i, "-");
		mvprintw(floor, i, "-");
	}
	attroff(COLOR_PAIR(DEFAULT_COLOR));
}

void refreshPipe(pipes *p)
{
	// If pipe exits screen on the left then wrap it to the right side 
	if (p->center + PIPE_RADIUS < 0)
	{
		int width = OPENING_WIDTH_MIN + rand() % (OPENING_WIDTH_MAX + 1 - OPENING_WIDTH_MIN);
		p->center = COLUMN + PIPE_RADIUS;
		p->openingHeight = rand() / ((float)INT_MAX) * 0.5 + 0.25;
		p->upperGap = p->openingHeight * (ROW - 1) - ( 1 * width / 2);
		p->lowerGap = p->openingHeight * (ROW - 1) - (-1 * width / 2);
		currentScore++;
	}
	p->center--;
}

void drawPipe(pipes p, int ceiling, int floor)
{
	int i, upperTerminus, lowerTerminus;

	// Draw vertical part of upper half of pipe.
	attron(COLOR_PAIR(PIPE_COLOR));
	for (i = ceiling + 1; i < p.upperGap; i++)
	{
		if ((p.center - PIPE_RADIUS) >= 0 && (p.center - PIPE_RADIUS) < COLUMN - 1)
		{
			mvprintw(i, p.center - PIPE_RADIUS, "|");
		}
		if ((p.center + PIPE_RADIUS) >= 0 && (p.center + PIPE_RADIUS) < COLUMN - 1)
		{
			mvprintw(i, p.center + PIPE_RADIUS, "|");
		}
	}
	attroff(COLOR_PAIR(PIPE_COLOR));
	upperTerminus = i;

	// Draw horizontal part of upper part of pipe.
	attron(COLOR_PAIR(4));
	for (i = -PIPE_RADIUS; i <= PIPE_RADIUS; i++)
	{
		if ((p.center + i) >= 0 && (p.center + i) < COLUMN - 1)
		{
			mvprintw(upperTerminus, p.center + i, "=");
		}
	}
	attroff(COLOR_PAIR(4));

	// Draw vertical part of lower half of pipe.
	attron(COLOR_PAIR(PIPE_COLOR));
	for (i = floor - 1; i > p.lowerGap; i--)
	{
		if ((p.center - PIPE_RADIUS) >= 0 && (p.center - PIPE_RADIUS) < COLUMN - 1)
		{
			mvprintw(i, p.center - PIPE_RADIUS, "|");
		}
		if ((p.center + PIPE_RADIUS) >= 0 && (p.center + PIPE_RADIUS) < COLUMN - 1)
		{
			mvprintw(i, p.center + PIPE_RADIUS, "|");
		}
	}
	attroff(COLOR_PAIR(PIPE_COLOR));
	lowerTerminus = i;

	// Draw horizontal part of lower part of pipe.
	attron(COLOR_PAIR(4));
	for (i = -PIPE_RADIUS; i <= PIPE_RADIUS; i++)
	{
		if ((p.center + i) >= 0 && (p.center + i) < COLUMN - 1)
		{
			mvprintw(lowerTerminus, p.center + i, "=");
		}
	}
	attroff(COLOR_PAIR(4));
}
// get the position of flappy using the quadratic formula of ax^2 + bx + c to simulate parabolic arc movement
int getPosition(flappy f) 
{
	return f.height + UP_VELOCITY * f.time + 0.5 * GRAVITY * f.time * f.time;
}

int crashedIntoPipe(flappy f, pipes p)
{
	if (FLAPPY_COL >= p.center - PIPE_RADIUS - 1 && FLAPPY_COL <= p.center + PIPE_RADIUS + 1)
	{

		if (getPosition(f) >= p.upperGap + 1 && getPosition(f) <= p.lowerGap - 1)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	return 0;
}

int deathScreen()
{
	char ch;
	clear();
	if (currentScore > bestScore)
	{
		mvprintw((ROW / 2) - 2, COLUMN / 2 - 21, "Congratulations! You set a new high score!");
		mvprintw((ROW / 2) - 1, COLUMN / 2 - 10, "New High Score: %-4d", currentScore);
		mvprintw(ROW / 2, COLUMN / 2 - 14, "Press <ENTER> to try again.");
		mvprintw((ROW / 2) + 1, COLUMN / 2 - 14, "Press <ESC> or <q> to quit.\n");
	}
	else
	{
		mvprintw((ROW / 2) - 2, COLUMN / 2 - 5, "You died.");
		mvprintw((ROW / 2) - 1, COLUMN / 2 - 5, "Score: %-4d", currentScore);
		mvprintw(ROW / 2, COLUMN / 2 - 7, "High Score: %-4d", bestScore);
		mvprintw((ROW / 2) + 1, COLUMN / 2 - 14, "Press <ENTER> to try again.");
		mvprintw((ROW / 2) + 2, COLUMN / 2 - 14, "Press <ESC> or <q> to quit.\n");
	}
	refresh();
	timeout(-1); // Block until user enters something.
	ch = getch();
	switch (ch)
	{
	case 'q': // Quit.
	case ESCAPE_KEY:
		endwin();
		exit(0);
		break;
	default:
		if (currentScore > bestScore)
			bestScore = currentScore;
		currentScore = 0;
		return 1; // Restart game.
	}
	endwin();
	exit(0);
}

int drawFlappy(flappy f)
{
	int h = getPosition(f);

	attron(COLOR_PAIR(DEFAULT_COLOR));
	// Crashed into ceiling or floor
	if (h <= 0 || h >= ROW - 1)
		return deathScreen();

	// Crashed into a pipe
	if (crashedIntoPipe(f, p1) || crashedIntoPipe(f, p2) || crashedIntoPipe(f, p3))
	{
		return deathScreen();
	}
	attroff(COLOR_PAIR(DEFAULT_COLOR));

	attron(COLOR_PAIR(FLAPPY_COLOR));
	// Going down
	if (GRAVITY * f.time + UP_VELOCITY > 0)
	{
		mvprintw(h, FLAPPY_COL - 1, "\\");
		mvprintw(h, FLAPPY_COL, "O");
		mvprintw(h, FLAPPY_COL + 1, "/");
	}

	// Going up
	else
	{
		// Left wing
		if (frame % 12 > 2)
		{
			mvprintw(h, FLAPPY_COL - 1, "/");
		}
		else
		{
			mvprintw(h, FLAPPY_COL - 1, "\\");
		}

		// Body
		mvprintw(h, FLAPPY_COL, "O");

		// Right wing
		if (frame % 12 > 2)
		{
			mvprintw(h, FLAPPY_COL + 1, "\\");
		}
		else
		{
			mvprintw(h, FLAPPY_COL + 1, "/");
		}
	}

	return 0;
	attroff(COLOR_PAIR(FLAPPY_COLOR));
}

void startScreen()
{
	int i;
	int r = ROW / 2 - 6;
	int c = COLUMN / 2 - 22;

	// Print the title.
	mvprintw(r + 0, c, "  __                       __          _  ");
	mvprintw(r + 1, c, " / _| __  __ ___  ___  _ _|  \\ _  __  | |");
	mvprintw(r + 2, c, "| |  / _|/  |   \\|   \\| | | + |_|/ _|/  |");
	mvprintw(r + 3, c, "| |_| | | | | |  | |  | | | + | | | | | |");
	mvprintw(r + 4, c, " \\__|_|  \\__|  _/|  _/ \\_ |__/|_|_|  \\__|");
	mvprintw(r + 5, c, "            |_|  |_|   |__/      ");
	mvprintw(ROW / 2, COLUMN / 2 - 18, "Press any button to start the game");
	mvprintw(ROW / 2 + 1, COLUMN / 2 - 16, "<up>, <spacebar> or <w> to fly");
	
	// Print the progress bar.
	mvprintw(PROG_BAR_ROW, COLUMN / 2 - PROG_BAR_LEN / 2 - 1, "[");
	mvprintw(PROG_BAR_ROW, COLUMN / 2 + PROG_BAR_LEN / 2, "]");
	refresh();
	for (i = 0; i < PROG_BAR_LEN; i++)
	{
		usleep(1000000 * START_TIME_SEC / (float)PROG_BAR_LEN);
		mvprintw(PROG_BAR_ROW, COLUMN / 2 - PROG_BAR_LEN / 2 + i, "=");
		refresh();
	}

	timeout(-1); // Block until user enters something.
	getch();
}

void initializePipe(pipes *p, float spacing){
	int width = OPENING_WIDTH_MIN + rand() % (OPENING_WIDTH_MAX + 1 - OPENING_WIDTH_MIN);
	p->center = (int)(spacing * (COLUMN - 1));
	p->openingHeight = rand() / ((float)INT_MAX) * 0.5 + 0.25;
	p->upperGap = p->openingHeight * (ROW - 1) - ( 1 * width / 2);
	p->lowerGap = p->openingHeight * (ROW - 1) - (-1 * width / 2);
}

int main()
{
	int leave_loop = 0;
	int ch;
	flappy f;
	int restart = 1;

	srand(time(NULL));

	// Initialize ncurses
	initscr();
	raw(); // Disable line buffering
	keypad(stdscr, TRUE);
	noecho(); // Don't echo() for getch
	curs_set(0);
	timeout(0);

	startScreen();

	// setting up color
	start_color();
	init_pair(PIPE_COLOR, COLOR_GREEN, COLOR_BLACK);
	init_pair(FLAPPY_COLOR, COLOR_YELLOW, COLOR_BLACK);
	init_pair(DEFAULT_COLOR, COLOR_WHITE, COLOR_BLACK);
	init_pair(4, COLOR_BLACK, COLOR_GREEN);

	while (!leave_loop)
	{
		// If we're just starting a game then do some initializations.
		if (restart)
		{
			timeout(0); // Don't block on input.

			// Initialize 3 pipes
			initializePipe(&p1, 1.25);
			initializePipe(&p2, 1.60);
			initializePipe(&p3, 1.95);

			// Initialize flappy
			f.height = ROW / 2;
			f.time = 0;
			restart = 0;
		}

		usleep((unsigned int)(1000000 / TARGET_FPS));

		// Process keystrokes.
		ch = -1;
		ch = getch();
		switch (ch)
		{
		case 'q':		 // Quit.
		case ESCAPE_KEY: // ESC key
			endwin();
			exit(0);
			break;
		case KEY_UP: // Fly up
		case ' ':
		case 'w':
		case '\n':
			f.height = getPosition(f);
			f.time = 0;
			break;
		default: // Fall down
			f.time++;
		}

		clear();

		// print floor and ceiling
		drawFloorAndCeiling(0, ROW - 1, 2, frame % 2);

		// Update pipe locations and draw them.
		drawPipe(p1, 0, ROW - 1);
		drawPipe(p2, 0, ROW - 1);
		drawPipe(p3, 0, ROW - 1);
		refreshPipe(&p1);
		refreshPipe(&p2);
		refreshPipe(&p3);

		if (drawFlappy(f))
		{
			restart = 1;
			continue; // restart game.
		}

		attron(COLOR_PAIR(DEFAULT_COLOR));
		mvprintw(0, SCORE_START_COL, " Score: %-4d  High Score: %-4d", currentScore, bestScore);
		attroff(COLOR_PAIR(DEFAULT_COLOR));
		// Display all the chars for this frame.
		refresh();
		frame++;
	}
	endwin();
	return 0;
}
