#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <curses.h>
#include <stdlib.h>
#include "woorden.h"
#include "bord.h"
#include "extio.h"
#include "grafisch.h"

/*****/

int maxy, maxx;

int woordenver[7][7];
int woordenhor[7][7];
  WINDOW *bordscr1, *bordscr2;
  WINDOW *progscr1, *progscr2;
  WINDOW *queryscr;

int config_kolomwaarde(int kol)
{
  int hoogste=0, result;
  int bestever[7];
  int w1, w2, w3;
  int i;

  for (int i=0; i<7; i++)
  {
    woordenver[kol][i]=0;
  }

  if (bord_lbekend(VER, kol, 7))
  {
    woordenver[kol][0]=7;
    return 13;
  }

  if (bord_lbekend(VER, kol, 6))
  {
    woordenver[kol][0]=6;
    return 8;
  }
  
  if (bord_lbekend(VER, kol+VER*1, 6))
  {
    woordenver[kol][1]=6;
    return 8;
  }

  w1=bord_lbekend(VER, kol, 5); w2=bord_lbekend(VER, kol+VER*5, 2);
  result=w1+w2;
  if (result==6)
  {
    woordenver[kol][0]=5;
    woordenver[kol][5]=2;
    return 6;
  }

  bestever[0]=(w1)?5:0; bestever[1]=0; bestever[2]=0; bestever[3]=0; bestever[4]=0;
  bestever[5]=(w2)?2:0; bestever[6]=0;
  hoogste=result;
  
  w1=bord_lbekend(VER, kol, 2); w2=bord_lbekend(VER, kol+VER*2, 5);
  result=w1+w2;
  if (result==6)
  {
    woordenver[kol][0]=2;
    woordenver[kol][2]=5;
    return 6;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=(w1)?2:0; bestever[1]=0;
    bestever[2]=(w2)?5:0; bestever[3]=0; bestever[4]=0; bestever[5]=0; bestever[6]=0;
  }
  if (hoogste==5)
  {
    for (i=0; i<7; i++) woordenver[kol][i]=bestever[i];
    return 5;
  }

  if (bord_lbekend(VER, kol+VER*1, 5))
  {
    woordenver[kol][1]=5;
    return 5;
  }

/* 4/3 */
  w1=bord_lbekend(VER, kol, 4); w2=bord_lbekend(VER, kol+VER*4, 3);
  result=w1+w2;
  if (result==5)
  {
    woordenver[kol][0]=4;
    woordenver[kol][4]=3;
    return 5;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=(w1)?4:0; bestever[1]=0; bestever[2]=0; bestever[3]=0;
    bestever[4]=(w2)?3:0; bestever[5]=0; bestever[6]=0;
  }
  
  w1=bord_lbekend(VER, kol, 3); w2=bord_lbekend(VER, kol+VER*3, 4);
  result=w1+w2;
  if (result==5)
  {
    woordenver[kol][0]=3;
    woordenver[kol][3]=4;
    return 5;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=(w1)?3:0; bestever[1]=0; bestever[2]=0;
    bestever[3]=(w2)?4:0; bestever[4]=0; bestever[5]=0; bestever[6]=0;
  }
  

/* 4/2 */
  w1=bord_lbekend(VER, kol, 4); w2=bord_lbekend(VER, kol+VER*4, 2);
  result=w1+w2;
  if (result==4)
  {
    woordenver[kol][0]=4;
    woordenver[kol][4]=2;
    return 4;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=(w1)?4:0; bestever[1]=0; bestever[2]=0; bestever[3]=0;
    bestever[4]=(w2)?2:0; bestever[5]=0;
    bestever[6]=0;
  }

  w1=bord_lbekend(VER, kol, 4); w2=bord_lbekend(VER, kol+VER*5, 2);
  result=w1+w2;
  if (result==4)
  {
    woordenver[kol][0]=4;
    woordenver[kol][5]=2;
    return 4;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=(w1)?4:0; bestever[1]=0; bestever[2]=0; bestever[3]=0;
    bestever[4]=0;
    bestever[5]=(w2)?2:0; bestever[6]=0;
  }
  
  w1=bord_lbekend(VER, kol+VER*1, 4); w2=bord_lbekend(VER, kol+VER*5, 2);
  result=w1+w2;
  if (result==4)
  {
    woordenver[kol][1]=4;
    woordenver[kol][5]=2;
    return 4;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=0;
    bestever[1]=(w1)?4:0; bestever[2]=0; bestever[3]=0; bestever[4]=0;
    bestever[5]=(w2)?2:0; bestever[6]=0;
  }

/* 2/4 */
  w1=bord_lbekend(VER, kol, 2); w2=bord_lbekend(VER, kol+VER*2, 4);
  result=w1+w2;
  if (result==4)
  {
    woordenver[kol][0]=2;
    woordenver[kol][2]=4;
    return 4;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=(w1)?2:0; bestever[1]=0;
    bestever[2]=(w2)?4:0; bestever[3]=0; bestever[4]=0; bestever[5]=0;
    bestever[6]=0;
  }

  w1=bord_lbekend(VER, kol, 2); w2=bord_lbekend(VER, kol+VER*3, 4);
  result=w1+w2;
  if (result==4)
  {
    woordenver[kol][0]=2;
    woordenver[kol][3]=4;
    return 4;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=(w1)?2:0; bestever[1]=0;
    bestever[2]=0;
    bestever[3]=w2?4:0; bestever[4]=0; bestever[5]=0; bestever[6]=0;
  }

  w1=bord_lbekend(VER, kol+VER*1, 2); w2=bord_lbekend(VER, kol+VER*3, 4);
  result=w1+w2;
  if (result==4)
  {
    woordenver[kol][1]=2;
    woordenver[kol][3]=4;
    return 4;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=0;
    bestever[1]=w1?2:0; bestever[2]=0;
    bestever[3]=w2?4:0; bestever[4]=0; bestever[5]=0; bestever[6]=0;
  }

/* 3/3 */
  w1=bord_lbekend(VER, kol, 3); w2=bord_lbekend(VER, kol+VER*3, 3);
  result=w1+w2;
  if (result==4)
  {
    woordenver[kol][0]=3;
    woordenver[kol][3]=3;
    return 4;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=w1?3:0; bestever[1]=0; bestever[2]=0;
    bestever[3]=w2?3:0; bestever[4]=0; bestever[5]=0;
    bestever[6]=0;
  }

  w1=bord_lbekend(VER, kol, 3); w2=bord_lbekend(VER, kol+VER*4, 3);
  result=w1+w2;
  if (result==4)
  {
    woordenver[kol][0]=3;
    woordenver[kol][4]=3;
    return 4;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=w1?3:0; bestever[1]=0; bestever[2]=0;
    bestever[3]=0;
    bestever[4]=w2?3:0; bestever[5]=0; bestever[6]=0;
  }

  w1=bord_lbekend(VER, kol+VER*1, 3); w2=bord_lbekend(VER, kol+VER*4, 3);
  result=w1+w2;
  if (result==4)
  {
    woordenver[kol][1]=3;
    woordenver[kol][4]=3;
    return 4;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=0;
    bestever[1]=w1?3:0; bestever[2]=0; bestever[3]=0;
    bestever[4]=w2?3:0; bestever[5]=0; bestever[6]=0;
  }

/* 3/2/2 */
  w1=bord_lbekend(VER, kol, 3); w2=bord_lbekend(VER, kol+VER*3, 2); w3=bord_lbekend(VER, kol+VER*5, 2);
  result=w1+w2+w3;
  if (result==4)
  {
    woordenver[kol][0]=3;
    woordenver[kol][3]=2;
    woordenver[kol][5]=2;
    return 4;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=w1?3:0; bestever[1]=0; bestever[2]=0;
    bestever[3]=w2?2:0; bestever[4]=0;
    bestever[5]=w3?2:0; bestever[6]=0;
  }

  w1=bord_lbekend(VER, kol, 2); w2=bord_lbekend(VER, kol+VER*2, 3); w3=bord_lbekend(VER, kol+VER*5, 2);
  result=w1+w2+w3;
  if (result==4)
  {
    woordenver[kol][0]=2;
    woordenver[kol][2]=3;
    woordenver[kol][5]=2;
    return 4;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=w1?2:0; bestever[1]=0;
    bestever[2]=w2?3:0; bestever[3]=0; bestever[4]=0;
    bestever[5]=w3?2:0; bestever[6]=0;
  }

  w1=bord_lbekend(VER, kol, 2); w2=bord_lbekend(VER, kol+VER*2, 2); w3=bord_lbekend(VER, kol+VER*4, 3);
  result=w1+w2+w3;
  if (result==4)
  {
    woordenver[kol][0]=2;
    woordenver[kol][2]=2;
    woordenver[kol][4]=3;
    return 4;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=w1?2:0; bestever[1]=0;
    bestever[2]=w2?2:0; bestever[3]=0;
    bestever[4]=w3?3:0; bestever[5]=0; bestever[6]=0;
  }

/*       */
  if (hoogste==3)
  {
    for (i=0; i<7; i++) woordenver[kol][i]=bestever[i];
    return 3;
  }

/* 3/2 */
  w1=bord_lbekend(VER, kol, 3); w2=bord_lbekend(VER, kol+VER*4, 2);
  result=w1+w2;
  if (result==3)
  {
    woordenver[kol][0]=3;
    woordenver[kol][4]=2;
    return 3;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=w1?3:0; bestever[1]=0; bestever[2]=0;
    bestever[3]=0;
    bestever[4]=w2?2:0; bestever[5]=0;
    bestever[6]=0;
  }

  w1=bord_lbekend(VER, kol+VER*1, 3); w2=bord_lbekend(VER, kol+VER*4, 2);
  result=w1+w2;
  if (result==3)
  {
    woordenver[kol][1]=3;
    woordenver[kol][4]=2;
    return 3;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=0;
    bestever[1]=w1?3:0; bestever[2]=0; bestever[3]=0;
    bestever[4]=w2?2:0; bestever[5]=0;
    bestever[6]=0;
  }

  w1=bord_lbekend(VER, kol+VER*1, 3); w2=bord_lbekend(VER, kol+VER*5, 2);
  result=w1+w2;
  if (result==3)
  {
    woordenver[kol][1]=3;
    woordenver[kol][5]=2;
    return 3;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=0;
    bestever[1]=w1?3:0; bestever[2]=0; bestever[3]=0;
    bestever[4]=0;
    bestever[5]=w2?2:0; bestever[6]=0;
  }

/* 2/3 */
  w1=bord_lbekend(VER, kol, 2); w2=bord_lbekend(VER, kol+VER*3, 3);
  result=w1+w2;
  if (result==3)
  {
    woordenver[kol][0]=2;
    woordenver[kol][3]=3;
    return 3;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=w1?2:0; bestever[1]=0;
    bestever[2]=0;
    bestever[3]=w2?3:0; bestever[4]=0; bestever[5]=0;
    bestever[6]=0;
  }

  w1=bord_lbekend(VER, kol+VER*1, 2); w2=bord_lbekend(VER, kol+VER*3, 3);
  result=w1+w2;
  if (result==3)
  {
    woordenver[kol][1]=2;
    woordenver[kol][3]=3;
    return 3;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=0;
    bestever[1]=w1?2:0; bestever[2]=0;
    bestever[3]=w2?3:0; bestever[4]=0; bestever[5]=0;
    bestever[6]=0;
  }

  w1=bord_lbekend(VER, kol+VER*1, 2); w2=bord_lbekend(VER, kol+VER*4, 3);
  result=w1+w2;
  if (result==3)
  {
    woordenver[kol][1]=2;
    woordenver[kol][4]=3;
    return 3;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=0;
    bestever[1]=w1?2:0; bestever[2]=0;
    bestever[3]=0;
    bestever[4]=w2?3:0; bestever[5]=0; bestever[6]=0;
  }

/* 2/2/2 */
  w1=bord_lbekend(VER, kol, 2); w2=bord_lbekend(VER, kol+VER*2, 2); w3=bord_lbekend(VER, kol+VER*4, 2);
  result=w1+w2+w3;
  if (result==3)
  {
    woordenver[kol][0]=2;
    woordenver[kol][2]=2;
    woordenver[kol][4]=2;
    return 3;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=w1?2:0; bestever[1]=0;
    bestever[2]=w2?2:0; bestever[3]=0;
    bestever[4]=w3?2:0; bestever[5]=0;
    bestever[6]=0;
  }

  w1=bord_lbekend(VER, kol, 2); w2=bord_lbekend(VER, kol+VER*2, 2); w3=bord_lbekend(VER, kol+VER*5, 2);
  result=w1+w2+w3;
  if (result==3)
  {
    woordenver[kol][0]=2;
    woordenver[kol][2]=2;
    woordenver[kol][5]=2;
    return 3;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=w1?2:0; bestever[1]=0;
    bestever[2]=w2?2:0; bestever[3]=0;
    bestever[4]=0;
    bestever[5]=w3?2:0; bestever[6]=0;
  }

  w1=bord_lbekend(VER, kol, 2); w2=bord_lbekend(VER, kol+VER*3, 2); w3=bord_lbekend(VER, kol+VER*5, 2);
  result=w1+w2+w3;
  if (result==3)
  {
    woordenver[kol][0]=2;
    woordenver[kol][3]=2;
    woordenver[kol][5]=2;
    return 3;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=w1?2:0; bestever[1]=0;
    bestever[2]=0;
    bestever[3]=w2?2:0; bestever[4]=0;
    bestever[5]=w3?2:0; bestever[6]=0;
  }

  w1=bord_lbekend(VER, kol+VER*1, 2); w2=bord_lbekend(VER, kol+VER*3, 2); w3=bord_lbekend(VER, kol+VER*5, 2);
  result=w1+w2+w3;
  if (result==3)
  {
    woordenver[kol][1]=2;
    woordenver[kol][3]=2;
    woordenver[kol][5]=2;
    return 3;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=0;
    bestever[1]=w1?2:0; bestever[2]=0;
    bestever[3]=w2?2:0; bestever[4]=0;
    bestever[5]=w3?2:0; bestever[6]=0;
  }

  if (hoogste==2)
  {
    for (i=0; i<7; i++) woordenver[kol][i]=bestever[i];
    return 2;
  }

/* 2/2 */
  w1=bord_lbekend(VER, kol+VER*1, 2); w2=bord_lbekend(VER, kol+VER*4, 2);
  result=w1+w2;
  if (result==2)
  {
    woordenver[kol][1]=2;
    woordenver[kol][4]=2;
    return 2;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=0;
    bestever[1]=w1?2:0; bestever[2]=0;
    bestever[3]=0;
    bestever[4]=w2?2:0;
    bestever[5]=0; bestever[6]=0;
  }

  for (i=0; i<7; i++) woordenver[kol][i]=bestever[i];
  return hoogste;
}

int config_rijwaarde(int rij)
{
  int hoogste=0, result, rijxver=rij*VER;
  int bestever[7];
  int w1, w2, w3;
  int i;

  for (int i=0; i<7; i++)
  {
    woordenhor[rij][i]=0;
    bestever[i]=0;
  }

  if (bord_lbekend(HOR, rijxver, 7))
  {
    woordenhor[rij][0]=7;
    return 13;
  }

  if (bord_lbekend(HOR, rijxver, 6))
  {
    woordenhor[rij][0]=6;
    return 8;
  }
  
  if (bord_lbekend(HOR, rijxver+1, 6))
  {
    woordenhor[rij][1]=6;
    return 8;
  }

  w1=bord_lbekend(HOR, rijxver, 5); w2=bord_lbekend(HOR, rijxver+5, 2);
  result=w1+w2;
  if (result==6)
  {
    woordenhor[rij][0]=5;
    woordenhor[rij][5]=2;
    return 6;
  }

  bestever[0]=(w1)?5:0; bestever[1]=0; bestever[2]=0; bestever[3]=0; bestever[4]=0;
  bestever[5]=(w2)?2:0; bestever[6]=0;
  hoogste=result;
  
  w1=bord_lbekend(HOR, rijxver, 2); w2=bord_lbekend(HOR, rijxver+2, 5);
  result=w1+w2;
  if (result==6)
  {
    woordenhor[rij][0]=2;
    woordenhor[rij][2]=5;
    return 6;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=(w1)?2:0; bestever[1]=0;
    bestever[2]=(w2)?5:0; bestever[3]=0; bestever[4]=0; bestever[5]=0; bestever[6]=0;
  }
  if (hoogste==5)
  {
    for (i=0; i<7; i++) woordenhor[rij][i]=bestever[i];
    return 5;
  }

  if (bord_lbekend(HOR, rijxver+1, 5))
  {
    woordenhor[rij][1]=5;
    return 5;
  }

/* 4/3 */
  w1=bord_lbekend(HOR, rijxver, 4); w2=bord_lbekend(HOR, rijxver+4, 3);
  result=w1+w2;
  if (result==5)
  {
    woordenhor[rij][0]=4;
    woordenhor[rij][4]=3;
    return 5;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=(w1)?4:0; bestever[1]=0; bestever[2]=0; bestever[3]=0;
    bestever[4]=(w2)?3:0; bestever[5]=0; bestever[6]=0;
  }
  
  w1=bord_lbekend(HOR, rijxver, 3); w2=bord_lbekend(HOR, rijxver+3, 4);
  result=w1+w2;
  if (result==5)
  {
    woordenhor[rij][0]=3;
    woordenhor[rij][3]=4;
    return 5;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=(w1)?3:0; bestever[1]=0; bestever[2]=0;
    bestever[3]=(w2)?4:0; bestever[4]=0; bestever[5]=0; bestever[6]=0;
  }
  

/* 4/2 */
  w1=bord_lbekend(HOR, rijxver, 4); w2=bord_lbekend(HOR, rijxver+4, 2);
  result=w1+w2;
  if (result==4)
  {
    woordenhor[rij][0]=4;
    woordenhor[rij][4]=2;
    return 4;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=(w1)?4:0; bestever[1]=0; bestever[2]=0; bestever[3]=0;
    bestever[4]=(w2)?2:0; bestever[5]=0;
    bestever[6]=0;
  }

  w1=bord_lbekend(HOR, rijxver, 4); w2=bord_lbekend(HOR, rijxver+5, 2);
  result=w1+w2;
  if (result==4)
  {
    woordenhor[rij][0]=4;
    woordenhor[rij][5]=2;
    return 4;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=(w1)?4:0; bestever[1]=0; bestever[2]=0; bestever[3]=0;
    bestever[4]=0;
    bestever[5]=(w2)?2:0; bestever[6]=0;
  }
  
  w1=bord_lbekend(HOR, rijxver+1, 4); w2=bord_lbekend(HOR, rijxver+5, 2);
  result=w1+w2;
  if (result==4)
  {
    woordenhor[rij][1]=4;
    woordenhor[rij][5]=2;
    return 4;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=0;
    bestever[1]=(w1)?4:0; bestever[2]=0; bestever[3]=0; bestever[4]=0;
    bestever[5]=(w2)?2:0; bestever[6]=0;
  }

/* 2/4 */
  w1=bord_lbekend(HOR, rijxver, 2); w2=bord_lbekend(HOR, rijxver+2, 4);
  result=w1+w2;
  if (result==4)
  {
    woordenhor[rij][0]=2;
    woordenhor[rij][2]=4;
    return 4;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=(w1)?2:0; bestever[1]=0;
    bestever[2]=(w2)?4:0; bestever[3]=0; bestever[4]=0; bestever[5]=0;
    bestever[6]=0;
  }

  w1=bord_lbekend(HOR, rijxver, 2); w2=bord_lbekend(HOR, rijxver+3, 4);
  result=w1+w2;
  if (result==4)
  {
    woordenhor[rij][0]=2;
    woordenhor[rij][3]=4;
    return 4;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=(w1)?2:0; bestever[1]=0;
    bestever[2]=0;
    bestever[3]=w2?4:0; bestever[4]=0; bestever[5]=0; bestever[6]=0;
  }

  w1=bord_lbekend(HOR, rijxver+1, 2); w2=bord_lbekend(HOR, rijxver+3, 4);
  result=w1+w2;
  if (result==4)
  {
    woordenhor[rij][1]=2;
    woordenhor[rij][3]=4;
    return 4;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=0;
    bestever[1]=w1?2:0; bestever[2]=0;
    bestever[3]=w2?4:0; bestever[4]=0; bestever[5]=0; bestever[6]=0;
  }

/* 3/3 */
  w1=bord_lbekend(HOR, rijxver, 3); w2=bord_lbekend(HOR, rijxver+3, 3);
  result=w1+w2;
  if (result==4)
  {
    woordenhor[rij][0]=3;
    woordenhor[rij][3]=3;
    return 4;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=w1?3:0; bestever[1]=0; bestever[2]=0;
    bestever[3]=w2?3:0; bestever[4]=0; bestever[5]=0;
    bestever[6]=0;
  }

  w1=bord_lbekend(HOR, rijxver, 3); w2=bord_lbekend(HOR, rijxver+4, 3);
  result=w1+w2;
  if (result==4)
  {
    woordenhor[rij][0]=3;
    woordenhor[rij][4]=3;
    return 4;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=w1?3:0; bestever[1]=0; bestever[2]=0;
    bestever[3]=0;
    bestever[4]=w2?3:0; bestever[5]=0; bestever[6]=0;
  }

  w1=bord_lbekend(HOR, rijxver+1, 3); w2=bord_lbekend(HOR, rijxver+4, 3);
  result=w1+w2;
  if (result==4)
  {
    woordenhor[rij][1]=3;
    woordenhor[rij][4]=3;
    return 4;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=0;
    bestever[1]=w1?3:0; bestever[2]=0; bestever[3]=0;
    bestever[4]=w2?3:0; bestever[5]=0; bestever[6]=0;
  }

/* 3/2/2 */
  w1=bord_lbekend(HOR, rijxver, 3); w2=bord_lbekend(HOR, rijxver+3, 2); w3=bord_lbekend(HOR, rijxver+5, 2);
  result=w1+w2+w3;
  if (result==4)
  {
    woordenhor[rij][0]=3;
    woordenhor[rij][3]=2;
    woordenhor[rij][5]=2;
    return 4;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=w1?3:0; bestever[1]=0; bestever[2]=0;
    bestever[3]=w2?2:0; bestever[4]=0;
    bestever[5]=w3?2:0; bestever[6]=0;
  }

  w1=bord_lbekend(HOR, rijxver, 2); w2=bord_lbekend(HOR, rijxver+2, 3); w3=bord_lbekend(HOR, rijxver+5, 2);
  result=w1+w2+w3;
  if (result==4)
  {
    woordenhor[rij][0]=2;
    woordenhor[rij][2]=3;
    woordenhor[rij][5]=2;
    return 4;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=w1?2:0; bestever[1]=0;
    bestever[2]=w2?3:0; bestever[3]=0; bestever[4]=0;
    bestever[5]=w3?2:0; bestever[6]=0;
  }

  w1=bord_lbekend(HOR, rijxver, 2); w2=bord_lbekend(HOR, rijxver+2, 2); w3=bord_lbekend(HOR, rijxver+4, 3);
  result=w1+w2+w3;
  if (result==4)
  {
    woordenhor[rij][0]=2;
    woordenhor[rij][2]=2;
    woordenhor[rij][4]=3;
    return 4;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=w1?2:0; bestever[1]=0;
    bestever[2]=w2?2:0; bestever[3]=0;
    bestever[4]=w3?3:0; bestever[5]=0; bestever[6]=0;
  }

/*       */
  if (hoogste==3)
  {
    for (i=0; i<7; i++) woordenhor[rij][i]=bestever[i];
    return 3;
  }

/* 3/2 */
  w1=bord_lbekend(HOR, rijxver, 3); w2=bord_lbekend(HOR, rijxver+4, 2);
  result=w1+w2;
  if (result==3)
  {
    woordenhor[rij][0]=3;
    woordenhor[rij][4]=2;
    return 3;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=w1?3:0; bestever[1]=0; bestever[2]=0;
    bestever[3]=0;
    bestever[4]=w2?2:0; bestever[5]=0;
    bestever[6]=0;
  }

  w1=bord_lbekend(HOR, rijxver+1, 3); w2=bord_lbekend(HOR, rijxver+4, 2);
  result=w1+w2;
  if (result==3)
  {
    woordenhor[rij][1]=3;
    woordenhor[rij][4]=2;
    return 3;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=0;
    bestever[1]=w1?3:0; bestever[2]=0; bestever[3]=0;
    bestever[4]=w2?2:0; bestever[5]=0;
    bestever[6]=0;
  }

  w1=bord_lbekend(HOR, rijxver+1, 3); w2=bord_lbekend(HOR, rijxver+5, 2);
  result=w1+w2;
  if (result==3)
  {
    woordenhor[rij][1]=3;
    woordenhor[rij][5]=2;
    return 3;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=0;
    bestever[1]=w1?3:0; bestever[2]=0; bestever[3]=0;
    bestever[4]=0;
    bestever[5]=w2?2:0; bestever[6]=0;
  }

/* 2/3 */
  w1=bord_lbekend(HOR, rijxver, 2); w2=bord_lbekend(HOR, rijxver+3, 3);
  result=w1+w2;
  if (result==3)
  {
    woordenhor[rij][0]=2;
    woordenhor[rij][3]=3;
    return 3;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=w1?2:0; bestever[1]=0;
    bestever[2]=0;
    bestever[3]=w2?3:0; bestever[4]=0; bestever[5]=0;
    bestever[6]=0;
  }

  w1=bord_lbekend(HOR, rijxver+1, 2); w2=bord_lbekend(HOR, rijxver+3, 3);
  result=w1+w2;
  if (result==3)
  {
    woordenhor[rij][1]=2;
    woordenhor[rij][3]=3;
    return 3;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=0;
    bestever[1]=w1?2:0; bestever[2]=0;
    bestever[3]=w2?3:0; bestever[4]=0; bestever[5]=0;
    bestever[6]=0;
  }

  w1=bord_lbekend(HOR, rijxver+1, 2); w2=bord_lbekend(HOR, rijxver+4, 3);
  result=w1+w2;
  if (result==3)
  {
    woordenhor[rij][1]=2;
    woordenhor[rij][4]=3;
    return 3;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=0;
    bestever[1]=w1?2:0; bestever[2]=0;
    bestever[3]=0;
    bestever[4]=w2?3:0; bestever[5]=0; bestever[6]=0;
  }

/* 2/2/2 */
  w1=bord_lbekend(HOR, rijxver, 2); w2=bord_lbekend(HOR, rijxver+2, 2); w3=bord_lbekend(HOR, rijxver+4, 2);
  result=w1+w2+w3;
  if (result==3)
  {
    woordenhor[rij][0]=2;
    woordenhor[rij][2]=2;
    woordenhor[rij][4]=2;
    return 3;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=w1?2:0; bestever[1]=0;
    bestever[2]=w2?2:0; bestever[3]=0;
    bestever[4]=w3?2:0; bestever[5]=0;
    bestever[6]=0;
  }

  w1=bord_lbekend(HOR, rijxver, 2); w2=bord_lbekend(HOR, rijxver+2, 2); w3=bord_lbekend(HOR, rijxver+5, 2);
  result=w1+w2+w3;
  if (result==3)
  {
    woordenhor[rij][0]=2;
    woordenhor[rij][2]=2;
    woordenhor[rij][5]=2;
    return 3;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=w1?2:0; bestever[1]=0;
    bestever[2]=w2?2:0; bestever[3]=0;
    bestever[4]=0;
    bestever[5]=w3?2:0; bestever[6]=0;
  }

  w1=bord_lbekend(HOR, rijxver, 2); w2=bord_lbekend(HOR, rijxver+3, 2); w3=bord_lbekend(HOR, rijxver+5, 2);
  result=w1+w2+w3;
  if (result==3)
  {
    woordenhor[rij][0]=2;
    woordenhor[rij][3]=2;
    woordenhor[rij][5]=2;
    return 3;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=w1?2:0; bestever[1]=0;
    bestever[2]=0;
    bestever[3]=w2?2:0; bestever[4]=0;
    bestever[5]=w3?2:0; bestever[6]=0;
  }

  w1=bord_lbekend(HOR, rijxver+1, 2); w2=bord_lbekend(HOR, rijxver+3, 2); w3=bord_lbekend(HOR, rijxver+5, 2);
  result=w1+w2+w3;
  if (result==3)
  {
    woordenhor[rij][1]=2;
    woordenhor[rij][3]=2;
    woordenhor[rij][5]=2;
    return 3;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=0;
    bestever[1]=w1?2:0; bestever[2]=0;
    bestever[3]=w2?2:0; bestever[4]=0;
    bestever[5]=w3?2:0; bestever[6]=0;
  }

  if (hoogste==2)
  {
    for (i=0; i<7; i++) woordenhor[rij][i]=bestever[i];
    return 2;
  }

/* 2/2 */
  w1=bord_lbekend(HOR, rijxver+1, 2); w2=bord_lbekend(HOR, rijxver+4, 2);
  result=w1+w2;
  if (result==2)
  {
    woordenhor[rij][1]=2;
    woordenhor[rij][4]=2;
    return 2;
  }
  if (result>hoogste)
  {
    hoogste=result;
    bestever[0]=0;
    bestever[1]=w1?2:0; bestever[2]=0;
    bestever[3]=0;
    bestever[4]=w2?2:0;
    bestever[5]=0; bestever[6]=0;
  }

  for (i=0; i<7; i++) woordenhor[rij][i]=bestever[i];
  return hoogste;
}

/**/
int kleur[49];

void wprintwborder(WINDOW *w, int i)
{
  switch (i)
  {
    case  0: waddch(w, ACS_DEGREE); break;
    case  1: waddch(w, ' '); break;
    case  2: waddch(w, ' '); break;
    case  3: waddch(w, ACS_ULCORNER); break;
    case  4: waddch(w, ' '); break;
    case  5: waddch(w, ACS_HLINE); break;
    case  6: waddch(w, ACS_URCORNER); break;
    case  7: waddch(w, ACS_TTEE); break;
    case  8: waddch(w, ' '); break;
    case  9: waddch(w, ACS_LLCORNER); break;
    case 10: waddch(w, ACS_VLINE); break;
    case 11: waddch(w, ACS_LTEE); break;
    case 12: waddch(w, ACS_LRCORNER); break;
    case 13: waddch(w, ACS_BTEE); break;
    case 14: waddch(w, ACS_RTEE); break;
    case 15: waddch(w, ACS_PLUS); break;
  }
}
 
void wprintbordstep(WINDOW *desscr, int yoff, bool begonnen, int step, char beginletter, 
    char letters[48], int pos[48], int colorpair1, int colorpair2, int colorpair3)
{
  int i, j;
  int mh=0, mv=0;
  int r[7], k[7];
  int grid[8][8]=
  {
    { 3, 7, 7, 7, 7, 7, 7, 6},
    {11,15,15,15,15,15,15,14},
    {11,15,15,15,15,15,15,14},
    {11,15,15,15,15,15,15,14},
    {11,15,15,15,15,15,15,14},
    {11,15,15,15,15,15,15,14},
    {11,15,15,15,15,15,15,14},
    { 9,13,13,13,13,13,13,12}
  };

  for (i=0; i<49; i++) bord[i]=0;
  for (i=0; i<step; i++)
  {
    bord[pos[i]]=letters[i];
    
    if (begonnen) kleur[pos[i]]=(i%2)?colorpair1:colorpair2;
    else kleur[pos[i]]=(i%2)?colorpair2:colorpair1;
  }
  bord[24]=beginletter;
  kleur[24]=colorpair3;
  for (i=0; i<7; i++)
  {
    r[i]=config_rijwaarde(i);
    k[i]=config_kolomwaarde(i);
  }
  for (i=0; i<7; i++)
  {
    for (j=0; j<7; j++)
    {
      if (j==0)
      {
        mh=woordenhor[i][0];
        mv=woordenver[i][0];
      }
      if (mh>0)
      {
        if (j!=0) grid[i][j]&=0xD;
        if (j!=0) grid[i+1][j]&=0x7;
        mh--;
      }
      else
      {
        mh=woordenhor[i][j]-1;
      }
      if (mv>0)
      {
        if (j!=0) grid[j][i]&=0xE;
        if (j!=0) grid[j][i+1]&=0xB;
        mv--;
      }
      else
      {
        mv=woordenver[i][j]-1;
      }
    }
  }
  for (i=0; i<15; i++)
  {
    wmove(desscr, yoff+i, 0);
    if (i==0)
    {
      for (j=0; j<15; j++)
      {
        if (j%2) waddch(desscr, ACS_HLINE | A_BOLD);
        else 
        {
          wprintwborder(desscr, grid[i/2][j/2]);
        }
      }
    }
    else if (i==14)
    {
      for (j=0; j<15; j++)
      {
        if (j%2) waddch(desscr, ACS_HLINE | A_BOLD);
        else 
        {
          wprintwborder(desscr, grid[i/2][j/2]);
        }
      }
    }
    else if (i%2)
    {
      for (j=0; j<15; j++)
      {
        if ((j%2))
        {
          if (bord[(i/2)*7+j/2]==0) waddch(desscr, '.');
          else waddch(desscr, bord[(i/2)*7+j/2] | COLOR_PAIR(kleur[(i/2)*7+j/2]));
        }
        else 
        {
          if ((grid[i/2][(j/2)]&0x02)) waddch(desscr, ACS_VLINE);
          else waddch(desscr, ' ');
        }
      }
      wprintw(desscr, " %x", r[i/2]);
    }
    else 
    {
      for (j=0; j<15; j++)
      {
        if (j%2) 
        {
          if ((grid[i/2][(j/2)]&0x01)) waddch(desscr, ACS_HLINE);
          else waddch(desscr, ' ');
        }
        else 
        {
          wprintwborder(desscr, grid[i/2][j/2]);
        }
      }
    }
  }
  wmove(desscr, yoff+i, 0);
  wprintw(desscr, " %x %x %x %x %x %x %x  %d",
      k[0], k[1], k[2], k[3], k[4], k[5], k[6],
      k[0]+k[1]+k[2]+k[3]+k[4]+k[5]+k[6]+
      r[0]+r[1]+r[2]+r[3]+r[4]+r[5]+r[6]);
  wrefresh(desscr);
}

void writelog(bool fulllog, char *logfile, char *prog1, char *prog2, 
    char beginletter, char letters[48], int prog1_pos[48], int prog2_pos[48], int sco1, int sco2)
{
  int i, j, n1, n2;
  FILE *output=fopen(logfile, "a");
  char bord1[49];
  char bord2[49];
  bool mine1[49];
  bool mine2[49];
  assert(output!=NULL);
  char buf1[17], buf2[17];
  n1=strlen(prog1);
  n2=strlen(prog2);
  
  for (i=0; i<17; i++)
  {
    if (i>=16-n1) buf1[i]=prog1[i-(16-n1)];
    else buf1[i]=' ';
    if (i>=16-n2) buf2[i]=prog2[i-(16-n2)];
    else buf2[i]=' ';
  }
  fprintf(output, "%s vs %s <|> `%c`: %3d-%3d", buf1, buf2, beginletter, sco1, sco2);
  if (fulllog)
  {
    fprintf(output, ":\n");
    bord1[24]=beginletter;
    bord2[24]=beginletter;
    for (i=0; i<48; i++)
    {
      bord1[prog1_pos[i]]=letters[i];
      if (i%2) mine1[prog1_pos[i]]=true;
      else mine1[prog1_pos[i]]=false;
      bord2[prog2_pos[i]]=letters[i];
      if (i%2) mine2[prog2_pos[i]]=true;
      else mine2[prog2_pos[i]]=false;
    }
    mine1[24]=mine2[24]=true;
    for (i=0; i<7; i++)
    {
      fprintf(output, "   ");
      for (j=0; j<7; j++)
      {
        if (mine1[i*7+j]) fprintf(output, "%c ", ((bord1[i*7+j]-'A') | 0x20)+'A');
        else fprintf(output, "%c ", bord1[i*7+j]);
      }
      fprintf(output, "      ");
      for (j=0; j<7; j++)
      {
        if (mine2[i*7+j]) fprintf(output, "%c ", bord2[i*7+j]);
        else fprintf(output, "%c ", ((bord2[i*7+j]-'A') | 0x20)+'A');
      }
      fprintf(output, "\n");
    }
  }
  fprintf(output, "\n");
  fclose(output);
}

int grafisch(char *prog1cmd, char *prog2cmd, bool bp, bool log, bool fulllog, 
    char *logfile)
{
  pid_t mypid;
  int i;
  int sco1, sco2;
  char beginletter;
  char buf[256];
  char prog1[256], prog2[256];
  int error=0;
  int prog1_move;

  mypid=getpid();
  init_woorden();
  init_bord();
  
  initscr();
  start_color();
  init_pair(0, COLOR_RED, COLOR_WHITE);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_BLUE, COLOR_BLACK);
  init_pair(4, COLOR_RED, COLOR_WHITE);
  attron(COLOR_PAIR(0) | A_BOLD);
  noecho();
  getmaxyx(stdscr, maxy, maxx);
  wprintw(stdscr, "Competitie Software Codecup 2004\n");
  wprintw(stdscr, "  GRAFISCH                      %s vs %s\n", 
      prog1cmd, prog2cmd);
  wrefresh(stdscr);
  
  progscr1=subwin(stdscr, 1, maxx/2-15, 2, 15);
  progscr2=subwin(stdscr, 1, maxx/2-15, 2, maxx/2+15);
  bordscr1=subwin(stdscr, 19, 19, 3, 15);
  bordscr2=subwin(stdscr, 19, 19, 3, maxx/2+15);
  queryscr=subwin(stdscr, 2, maxx, maxy-2, 0);
  
  scrollok(bordscr1, true);
  scrollok(bordscr2, true);
  
  if (pipe(prog1error) || pipe(prog2error) || pipe(prog1input) || pipe(prog1output) || 
      pipe(prog2input) || pipe(prog2output))
  {
     fprintf (stderr, "Pijpen mislukt.\n");
     return 1;
  }
  
  strcpy(prog1, prog1cmd);
  strcpy(prog2, prog2cmd);
  prog1pid=fork();
  if (prog1pid==0)
  {
    close(prog1error[0]);
    dup2(prog1error[1], fileno(stderr));
    close(prog1output[0]);
    dup2(prog1output[1], fileno(stdout));
    close(prog1input[1]);
    dup2(prog1input[0], fileno(stdin));
    execl(prog1, NULL);
    kill(mypid, SIGTERM);
    return 1;
  }
  prog2pid=fork();
  if (prog2pid==0)
  {
    close(prog2error[0]);
    dup2(prog2error[1], fileno(stderr));
    close(prog2output[0]);
    dup2(prog2output[1], fileno(stdout));
    close(prog1input[1]);
    dup2(prog2input[0], fileno(stdin));
    execl(prog2, NULL);
    // komt als het goed is niet meer hier
    kill(mypid, SIGTERM);
    kill(prog1pid, SIGTERM);
    return 1;
  }
  wprintw(queryscr, "Geef beginletter (<RET> voor random): ");
  echo();
  wscanw(queryscr, "%c", &beginletter);
  noecho();
  wclear(queryscr);
  wrefresh(queryscr);
  beginletter-='A';
  if ((beginletter<0 || beginletter>26) && (beginletter<32 || beginletter>58)) 
  {
    srand(time(NULL));
    beginletter=(rand()%26)+'A';
  }
  else beginletter=(beginletter&0x1F)+'A';
  bord_zet(beginletter, 24);
  bordtoprog1();
  bordtoprog2();
  wrefresh(stdscr);
  
  sprintf(buf, "%c\n", beginletter);
  write_prog1(buf, &error);
  write_prog2(buf, &error);
  write_prog1("start\n", &error);
  prog1_move=1;
  wprintw(progscr1, "%s", prog1);
  wrefresh(progscr1);
  wprintw(progscr2, "%s", prog2);
  wrefresh(progscr2);
  read_write_reset();
  for (i=0; i<48 && !error; i++)
  {
    if (prog1_move)
    {
      mvwprintw(bordscr1, 0, 0, "Reading...");
      wrefresh(bordscr1);
      read_prog1(buf, &error); if (error) break;
      letters[i]=buf[0];
      read_prog1(buf, &error); if (error) break;
      wclear(bordscr1);
      wrefresh(bordscr1);
      prog1_pos[i]=atoi(buf);
      prog1tobord();
      bord_zet(letters[i], prog1_pos[i]);
      bordtoprog1();
      if (bp)
      { 
        wprintbordstep(bordscr1, 1, true, i+1, beginletter, letters, 
            prog1_pos, 1, 2, 3);
        wrefresh(bordscr1);
      }
      sprintf(buf, "%c\n", letters[i]);
      write_prog2(buf, &error); if (error) break;
      mvwprintw(bordscr2, 0, 0, "Reading...");
      wrefresh(bordscr2);
      read_prog2(buf, &error); if (error) break;
      wclear(bordscr2);
      wrefresh(bordscr2);
      prog2_pos[i]=atoi(buf);
      prog2tobord();
      bord_zet(letters[i], prog2_pos[i]);
      bordtoprog2();
      if (bp)
      { 
        wprintbordstep(bordscr2, 1, false, i+1, beginletter, letters, 
            prog2_pos, 1, 2, 3);
        wrefresh(bordscr2);
      }
      prog1_move=0;
    }
    else
    {
      mvwprintw(bordscr2, 0, 0, "Reading...");
      wrefresh(bordscr2);
      read_prog2(buf, &error); if (error) break;
      letters[i]=buf[0];
      read_prog2(buf, &error); if (error) break;
      wclear(bordscr2);
      wrefresh(bordscr2);
      prog2_pos[i]=atoi(buf);
      prog2tobord();
      bord_zet(letters[i], prog2_pos[i]);
      bordtoprog2();
      if (bp)
      { 
        wprintbordstep(bordscr2, 1, false, i+1, beginletter, letters, 
            prog2_pos, 1, 2, 3);
        wrefresh(bordscr2);
      }
      sprintf(buf, "%c\n", letters[i]);
      write_prog1(buf, &error); if (error) break;
      mvwprintw(bordscr1, 0, 0, "Reading...");
      wrefresh(bordscr1);
      read_prog1(buf, &error); if (error) break;
      wclear(bordscr1);
      wrefresh(bordscr1);
      prog1_pos[i]=atoi(buf);
      prog1tobord();
      bord_zet(letters[i], prog1_pos[i]);
      if (bp)
      { 
        wprintbordstep(bordscr1, 1, true, i+1, beginletter, letters, 
            prog1_pos, 1, 2, 3);
        wrefresh(bordscr1);
      }
      bordtoprog1();
      prog1_move=1;
    }
  }
  if (error)
  {
  }
  kill(prog1pid, SIGTERM);
  kill(prog2pid, SIGTERM);
/*  printw("Overzicht\n");
  printw("%s\t\t\t%s\n", prog1, prog2);
  for (i=0; i<49; i++)
  {
    if ((i%2)==0)
    {
      if (i==0) printw("   %c %2d\n", letters[i], prog1_pos[i]);
      else if (i==48) printw("%2d\n", prog1_pos[i-1]);
      else printw("%2d %c %2d\n", prog1_pos[i-1], letters[i], prog1_pos[i]);
    }
    else
    {
      printw("\t\t\t%2d %c %2d\n", prog2_pos[i-1], letters[i], prog2_pos[i]);
    }
  }*/
  wprintbordstep(bordscr1, 1, true, 48, beginletter, letters, prog1_pos, 1, 2, 3);
  wprintbordstep(bordscr2, 1, false, 48, beginletter, letters, prog2_pos, 1, 2, 3);
  wprintw(queryscr, "Einde\nDruk op een toets...");
  wrefresh(queryscr);
  wgetch(queryscr);
  endwin();
  prog1tobord();
  sco1=bord_waarde();
  prog2tobord();
  sco2=bord_waarde();
  if (log) writelog(fulllog, logfile, prog1, prog2, beginletter, letters, prog1_pos, prog2_pos, sco1, sco2);
  return 0;
}

