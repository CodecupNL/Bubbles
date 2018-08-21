extern int maxy, maxx;

extern int woordenver[7][7];
extern int woordenhor[7][7];

void wprintbordstep(WINDOW *desscr, int yoff, bool begonnen, int step, char beginletter, 
    char letters[48], int pos[48], int colorpair1, int colorpair2, int colorpair3);
void wprintwborder(WINDOW *w, int i);
int config_rijwaarde(int rij);
int config_kolomwaarde(int kol);
int grafisch(char *prog1cmd, char *prog2cmd, bool bp, bool log, bool fulllog, char *logfile);
void writelog(char *logfile, char *prog1, char *prog2, char beginletter, int sco1, int sco2);
