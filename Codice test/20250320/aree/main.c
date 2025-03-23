#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

typedef enum{
	BASE,
	MARCIAPIEDE,
	ACQUA,
	SPONDA,
	COCCODRILLO,
	RANA_SU_COCCODRILLO,
	NERO
}colori;

#define RANA "<00>"

#define COCCODRILLO_SU "______^^"
#define COCCODRILLO_GIU "uu__uu_<"
#define COCCODRILLO_NO "        "

#define W_RANA 4
#define W_COCCODRILLO 8

#define DIM_LINES 24
#define DIM_COLS 80

#define DURATA_MANCHE_S 12

int altezzaSponda();
int altezzaMarciapiede();

void adattaFinestra();

void inizializzaColori();

void coloraAmbienteGioco();

int main(){
	initscr();
        noecho();
        curs_set(0);
        cbreak();
        keypad(stdscr, 1);
        
        int dim_msg_benvenuto = strlen("BENVENUTO!")/2;
        mvprintw(LINES/2, COLS/2 - dim_msg_benvenuto, "BENVENUTO!");
        refresh();
        sleep(2);
        clear();

        if (LINES != DIM_LINES || COLS != DIM_COLS) {
                adattaFinestra();
        }       

        inizializzaColori();

        coloraAmbienteGioco();
        
        int x=0, y=10;
        int X=30, Y=10;
        
        attron(COLOR_PAIR(COCCODRILLO));
        
        int vivo;
        time_t start, ora = 0;
        time(&start);
        do {  
                attron(COLOR_PAIR(NERO));
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

                attron(COLOR_PAIR(NERO));
                
                time(&ora);
                mvprintw(0, 0, "Tempo: %d ", DURATA_MANCHE_S - (int)(ora - start));
                refresh();
                
        } while (ora - start < DURATA_MANCHE_S); 

        endwin();
        
	return 0;
}

int altezzaSponda(){
	return 6;
}

int altezzaMarciapiede(){
	return 6;
}

void adattaFinestra() {
        int dim_msg_dim = strlen("Ridimensiona lo schermo per un'esperienza ottimale")/2;
        int dim1 = strlen("TU: LINES = xx COLS = yy")/2;  
        int dim2 = strlen("DIM: LINES = xx COLS = yy")/2;

        mvprintw(LINES/2, COLS/2 - dim_msg_dim, "Ridimensiona lo schermo per un'esperienza ottimale");
        refresh();
        sleep(2);

        clear();
        refresh();
        
        while (LINES != DIM_LINES || COLS != DIM_COLS) {
                WINDOW* indicaz_win = newwin(DIM_LINES, DIM_COLS, 0, 0);
                box(indicaz_win, ACS_VLINE, ACS_HLINE);
                mvwprintw(indicaz_win, DIM_LINES/2, DIM_COLS/2 - dim1, "TU: LINES = %d COLS = %d", LINES, COLS);
                mvwprintw(indicaz_win, DIM_LINES/2 + 1, DIM_COLS/2 - dim2, "DIM: LINES = %d COLS = %d", DIM_LINES, DIM_COLS);
                wrefresh(indicaz_win);
                
                delwin(indicaz_win);
        }
        usleep(10000);
}

void inizializzaColori() {
        start_color();
        init_pair(MARCIAPIEDE, COLOR_BLACK, COLOR_WHITE);
        init_pair(ACQUA, COLOR_WHITE, COLOR_BLUE);
        init_pair(SPONDA, COLOR_BLACK, COLOR_GREEN);
        init_pair(COCCODRILLO, COLOR_GREEN, COLOR_BLUE);
        init_pair(RANA_SU_COCCODRILLO, COLOR_WHITE, COLOR_GREEN);
        init_pair(NERO, COLOR_WHITE, COLOR_BLACK);
}

void coloraAmbienteGioco() {
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
}