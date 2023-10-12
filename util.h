#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <limits.h>
#include <string> 
#include <charconv>


class pipes {
	public:
		float openingHeight;
		int center;
		int upperGap;
		int lowerGap;
};

class flappy {
	public:
		int height;
	 	int time; //time since last boost
};
