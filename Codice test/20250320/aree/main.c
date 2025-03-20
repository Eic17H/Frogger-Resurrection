#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>

typedef enum{
	BASE,
	MARCIAPIEDE,
	ACQUA,
	SPONDA,
	COCCODRILLO,
	RANA_SU_COCCODRILLO,
	NERO
}colori;

int altezzaSponda(){
	return 6;
}

int altezzaMarciapiede(){
	return 6;
}

#define RANA "<00>"

#define COCCODRILLO_SU "______^^"
#define COCCODRILLO_GIU "uu__uu_<"
#define COCCODRILLO_NO "        "

#define W_RANA 4
#define W_COCCODRILLO 8

int main(){
	initscr();
        noecho();
        curs_set(0);
        cbreak();
        keypad(stdscr, 1);
        //box(stdscr, ACS_VLINE, ACS_HLINE);
        refresh();
        start_color();
        init_pair(MARCIAPIEDE, COLOR_BLACK, COLOR_WHITE);
        init_pair(ACQUA, COLOR_WHITE, COLOR_BLUE);
        init_pair(SPONDA, COLOR_BLACK, COLOR_GREEN);
        init_pair(COCCODRILLO, COLOR_GREEN, COLOR_BLUE);
        init_pair(RANA_SU_COCCODRILLO, COLOR_WHITE, COLOR_GREEN);
        init_pair(NERO, COLOR_WHITE, COLOR_BLACK);
        attron(COLOR_PAIR(ACQUA));
        bkgd(COLOR_PAIR(ACQUA));
        attron(COLOR_PAIR(NERO));
        for(int i=0; i<COLS+1; i++){
        	mvaddch(0, i, ' ');
        }
        attron(COLOR_PAIR(SPONDA));
        for(int i=0; i<COLS+1; i++){
        	for(int j=0; j<altezzaSponda(); j++){
        		mvaddch(j+1, i, ' ');
        		refresh();
        	}
        }
        attron(COLOR_PAIR(MARCIAPIEDE));
        for(int i=0; i<COLS+1; i++){
        	for(int j=0; j<altezzaMarciapiede(); j++){
        		mvaddch(-j+LINES-1, i, ' ');
        		refresh();
        	}
        }
        refresh();
        
        int x=0, y=10;
        int X=30, Y=10;
        
        attron(COLOR_PAIR(COCCODRILLO));
        
        int vivo;
        int tempo=1000;
        
        while(1){
        // TODO: usare time.h eccetera con start stop clock eccetera eccetera
        tempo -= 2;
        attron(COLOR_PAIR(NERO));
        mvprintw(0, 0, "Tempo: %d", tempo/10);
        mvaddch(1, 1, vivo+'0');
        attron(COLOR_PAIR(COCCODRILLO));
        mvaddstr(y, x, COCCODRILLO_NO);
        mvaddstr(y+1, x, COCCODRILLO_NO);
        x++;
        vivo = X-x>-W_RANA&&X-x<W_COCCODRILLO;
        mvaddstr(y, x, COCCODRILLO_SU);
        mvaddstr(y+1, x, COCCODRILLO_GIU);
        if(vivo) attron(COLOR_PAIR(RANA_SU_COCCODRILLO));
        else attron(COLOR_PAIR(ACQUA));
        if(vivo) beep();
        mvaddstr(Y, X, RANA);
        refresh();
        usleep(100000);
        }
        
        
        
        
        while(getch()=='\0');
        endwin();
        
	return 0;
}
