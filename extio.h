#include <curses.h>

extern int prog1pid, prog2pid;
extern int prog1output[2], prog1input[2], prog2output[2], prog2input[2];
extern int prog1error[2], prog2error[2];

extern char prog1buf[256];
extern int prog1bufptr;
extern char prog2buf[256];
extern int prog2bufptr;

extern char letters[48];
extern int prog1_pos[48];
extern int prog2_pos[48];
extern char prog1_bord[49];
extern char prog2_bord[49];
extern char errormsg[256];

void bordtoprog1(void);
void bordtoprog2(void);
void prog1tobord(void);
void prog2tobord(void);
int read_from_pipedes(int filedes[2], char *buf, int max);
int read_from_pipedes_line(int *bufptr, char *buffer, int filedes[2], char *buf, int max);
int write_to_pipedes(int filedes[2], char *buf);
void write_prog1(char *buf, int *error);
void write_prog2(char *buf, int *error);
void read_prog2(char *buf, int *error);
void read_prog1(char *buf, int *error);
void read_write_reset(void);
void wread_prog1(WINDOW *scr, int x, char *buf, int *error);
void wread_prog2(WINDOW *scr, int x, char *buf, int *error);
void getprinterror(int i, int filedes[2], WINDOW *scr, bool log, char *logfile);
