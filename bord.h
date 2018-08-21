
/* bord[49] 'A'=0, 'B'=1,....
   (leeg)=-1
   letters in int, positie in int
   letter voor positie...

*/

extern int score[8];
extern char bord[49];

#define HOR 1
#define VER 7

void init_bord(void);
void show_bord(void);
void bord_zet(int l, int p);
bool bord_bezet(int p);
int bord_kolomwaarde(int kol);
int bord_rijwaarde(int rij);
int bord_waarde();
int bord_winst(int let, int pos);
int bord_lbekend(int dx, int p, int l);

