#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>

#define N beep(); usleep(100000);
#define P usleep(100000);

int main(){
	initscr();
                noecho();
                curs_set(0);
                cbreak();
                keypad(stdscr, 1);
                box(stdscr, ACS_VLINE, ACS_HLINE);
                refresh();
		N P P N N P N P N N P
		N N N N N N N N N N N N N N N P
		N N N N N N N N P N P N N N N N N P P P P
	while(getch()=='\0');
	return 0;
}
