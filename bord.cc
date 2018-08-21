#include <stdio.h>
#include <assert.h>
#include "woorden.h"
#include "bord.h"

char bord[49];

int score[8]={0, 0, 1, 2, 3, 5, 8, 13 };

void init_bord(void)
{
  int i;
  for (i=0; i<49; i++)
  {
    bord[i]=0;
  }
}

void show_bord()
{
  int i; 
  for (i=0; i<49; i++)
  {
    if (bord[i]) printf("%c ", bord[i]);
    else printf(". ");
    if ((i%7)==6) printf("%x\n", bord_rijwaarde(i/7));
  }
  for (i=0; i<7; i++)
  {
    printf("%x ", bord_kolomwaarde(i));
  }
  printf("%d\n", bord_waarde());
}

void bord_zet(int l, int p)
{
  bord[p]=l;
}

bool bord_bezet(int p)
{
  return bord[p]!=0;
}

int bord_lbekend(int dx, int p, int l)
{
  int i;
  fflush(stdout);
  trie *q=wb->first_child, *r=NULL;
  assert(q!=NULL);
  for (i=0; i<l; i++, p+=dx)
  {
    if (q==NULL) return 0;
    assert(0<=p && p<=48);
//    printf("%c", bord[p]);
    if (bord[p]<'A' || bord[p]>'Z') return 0;
    r=q->isinnext(bord[p]);
    if (r==NULL) return 0;
    q=r->first_child;
  }
//  printf("\n");
  assert(r!=NULL);
  if (r->end) return score[l];
  return 0;
}

int bord_waarde(void)
{
  return bord_kolomwaarde(0)+ bord_kolomwaarde(1)+ bord_kolomwaarde(2)+
         bord_kolomwaarde(3)+ bord_kolomwaarde(4)+ bord_kolomwaarde(5)+
         bord_kolomwaarde(6)+
         bord_rijwaarde(0)+ bord_rijwaarde(1)+ bord_rijwaarde(2)+
         bord_rijwaarde(3)+ bord_rijwaarde(4)+ bord_rijwaarde(5)+
         bord_rijwaarde(6);
}

int bord_winst(int let, int pos)
{
  int kol=pos%7, rij=pos/7;
  int w;
  w=-bord_kolomwaarde(kol)-bord_rijwaarde(rij);
  bord[pos]=let;
  w+=bord_kolomwaarde(kol)+bord_rijwaarde(rij);
  bord[pos]=0;
  return w;
}

int bord_kolomwaarde(int kol)
{
  int hoogste=0, result;

  if (bord_lbekend(VER, kol, 7)) return 13;

  if (bord_lbekend(VER, kol, 6)) return 8;
  
  if (bord_lbekend(VER, kol+VER*1, 6)) return 8;

  result=bord_lbekend(VER, kol, 5)+bord_lbekend(VER, kol+VER*5, 2);
  if (result==6) return result;
  hoogste=result;

  result=bord_lbekend(VER, kol, 2)+bord_lbekend(VER, kol+VER*2, 5);
  if (result==6) return result;
  if (hoogste==5 || result==5) return 5;

  if (bord_lbekend(VER, kol+VER*1, 5)) return 5;

/* 4/3 */
  result=bord_lbekend(VER, kol, 4)+bord_lbekend(VER, kol+VER*4, 3);
  if (result==5) return result;
  if (result>hoogste) hoogste=result;
  
  result=bord_lbekend(VER, kol, 3)+bord_lbekend(VER, kol+VER*3, 4);
  if (result==5) return result;
  if (result>hoogste) hoogste=result;
  

/* 4/2 */
  result=bord_lbekend(VER, kol, 4)+bord_lbekend(VER, kol+VER*4, 2);
  if (result==4) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(VER, kol, 4)+bord_lbekend(VER, kol+VER*5, 2);
  if (result==4) return result;
  if (result>hoogste) hoogste=result;
  
  result=bord_lbekend(VER, kol+VER*1, 4)+bord_lbekend(VER, kol+VER*5, 2);
  if (result==4) return result;
  if (result>hoogste) hoogste=result;

/* 2/4 */
  result=bord_lbekend(VER, kol, 2)+bord_lbekend(VER, kol+VER*2, 4);
  if (result==4) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(VER, kol, 2)+bord_lbekend(VER, kol+VER*3, 4);
  if (result==4) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(VER, kol+VER*1, 2)+bord_lbekend(VER, kol+VER*3, 4);
  if (result==4) return result;
  if (result>hoogste) hoogste=result;

/* 3/3 */
  result=bord_lbekend(VER, kol, 3)+bord_lbekend(VER, kol+VER*3, 3);
  if (result==4) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(VER, kol, 3)+bord_lbekend(VER, kol+VER*4, 3);
  if (result==4) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(VER, kol+VER*1, 3)+bord_lbekend(VER, kol+VER*4, 3);
  if (result==4) return result;
  if (result>hoogste) hoogste=result;

/* 3/2/2 */
  result=bord_lbekend(VER, kol, 3)+bord_lbekend(VER, kol+VER*3, 2)+bord_lbekend(VER, kol+VER*5, 2);
  if (result==4) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(VER, kol, 2)+bord_lbekend(VER, kol+VER*2, 3)+bord_lbekend(VER, kol+VER*5, 2);
  if (result==4) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(VER, kol, 2)+bord_lbekend(VER, kol+VER*2, 2)+bord_lbekend(VER, kol+VER*4, 3);
  if (result==4) return result;
  if (result>hoogste) hoogste=result;

/*       */
  if (hoogste==3) return 3;

/* 3/2 */
  result=bord_lbekend(VER, kol, 3)+bord_lbekend(VER, kol+VER*4, 2);
  if (result==3) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(VER, kol+VER*1, 3)+bord_lbekend(VER, kol+VER*4, 2);
  if (result==3) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(VER, kol+VER*1, 3)+bord_lbekend(VER, kol+VER*5, 2);
  if (result==3) return result;
  if (result>hoogste) hoogste=result;

/* 2/3 */
  result=bord_lbekend(VER, kol, 2)+bord_lbekend(VER, kol+VER*3, 3);
  if (result==3) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(VER, kol+VER*1, 2)+bord_lbekend(VER, kol+VER*3, 3);
  if (result==3) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(VER, kol+VER*1, 2)+bord_lbekend(VER, kol+VER*4, 3);
  if (result==3) return result;
  if (result>hoogste) hoogste=result;

/* 2/2/2 */
  result=bord_lbekend(VER, kol, 2)+bord_lbekend(VER, kol+VER*2, 2)+bord_lbekend(VER, kol+VER*4, 2);
  if (result==3) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(VER, kol, 2)+bord_lbekend(VER, kol+VER*2, 2)+bord_lbekend(VER, kol+VER*5, 2);
  if (result==3) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(VER, kol, 2)+bord_lbekend(VER, kol+VER*3, 2)+bord_lbekend(VER, kol+VER*5, 2);
  if (result==3) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(VER, kol+VER*1, 2)+bord_lbekend(VER, kol+VER*3, 2)+bord_lbekend(VER, kol+VER*5, 2);
  if (result==3) return result;
  if (result>hoogste) hoogste=result;

  if (hoogste==2) return 2;

/* 2/2 */
  result=bord_lbekend(VER, kol+VER*1, 2)+bord_lbekend(VER, kol+VER*4, 2);
  if (result==2) return result;
  if (result>hoogste) hoogste=result;

  return hoogste;
}

int bord_rijwaarde(int rij)
{
  int hoogste=0, result, rijxver=rij*VER;

  if (bord_lbekend(HOR, rijxver, 7)) return 13;

  if (bord_lbekend(HOR, rijxver, 6)) return 8;
  
  if (bord_lbekend(HOR, rijxver+1, 6)) return 8;

  result=bord_lbekend(HOR, rijxver, 5)+bord_lbekend(HOR, rijxver+5, 2);
  if (result==6) return result;
  hoogste=result;

  result=bord_lbekend(HOR, rijxver, 2)+bord_lbekend(HOR, rijxver+2, 5);
  if (result==6) return result;
  if (hoogste==5 || result==5) return 5;

  if (bord_lbekend(HOR, rijxver+1, 5)) return 5;

/* 4/3 */
  result=bord_lbekend(HOR, rijxver, 4)+bord_lbekend(HOR, rijxver+4, 3);
  if (result==5) return result;
  if (result>hoogste) hoogste=result;
  
  result=bord_lbekend(HOR, rijxver, 3)+bord_lbekend(HOR, rijxver+3, 4);
  if (result==5) return result;
  if (result>hoogste) hoogste=result;
  

/* 4/2 */
  result=bord_lbekend(HOR, rijxver, 4)+bord_lbekend(HOR, rijxver+4, 2);
  if (result==4) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(HOR, rijxver, 4)+bord_lbekend(HOR, rijxver+5, 2);
  if (result==4) return result;
  if (result>hoogste) hoogste=result;
  
  result=bord_lbekend(HOR, rijxver+1, 4)+bord_lbekend(HOR, rijxver+5, 2);
  if (result==4) return result;
  if (result>hoogste) hoogste=result;

/* 2/4 */
  result=bord_lbekend(HOR, rijxver, 2)+bord_lbekend(HOR, rijxver+2, 4);
  if (result==4) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(HOR, rijxver, 2)+bord_lbekend(HOR, rijxver+3, 4);
  if (result==4) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(HOR, rijxver+1, 2)+bord_lbekend(HOR, rijxver+3, 4);
  if (result==4) return result;
  if (result>hoogste) hoogste=result;

/* 3/3 */
  result=bord_lbekend(HOR, rijxver, 3)+bord_lbekend(HOR, rijxver+3, 3);
  if (result==4) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(HOR, rijxver, 3)+bord_lbekend(HOR, rijxver+4, 3);
  if (result==4) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(HOR, rijxver+1, 3)+bord_lbekend(HOR, rijxver+4, 3);
  if (result==4) return result;
  if (result>hoogste) hoogste=result;

/* 3/2/2 */
  result=bord_lbekend(HOR, rijxver, 3)+bord_lbekend(HOR, rijxver+3, 2)+bord_lbekend(HOR, rijxver+5, 2);
  if (result==4) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(HOR, rijxver, 2)+bord_lbekend(HOR, rijxver+2, 3)+bord_lbekend(HOR, rijxver+5, 2);
  if (result==4) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(HOR, rijxver, 2)+bord_lbekend(HOR, rijxver+2, 2)+bord_lbekend(HOR, rijxver+4, 3);
  if (result==4) return result;
  if (result>hoogste) hoogste=result;

/*       */
  if (hoogste==3) return 3;

/* 3/2 */
  result=bord_lbekend(HOR, rijxver, 3)+bord_lbekend(HOR, rijxver+4, 2);
  if (result==3) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(HOR, rijxver+1, 3)+bord_lbekend(HOR, rijxver+4, 2);
  if (result==3) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(HOR, rijxver+1, 3)+bord_lbekend(HOR, rijxver+5, 2);
  if (result==3) return result;
  if (result>hoogste) hoogste=result;

/* 2/3 */
  result=bord_lbekend(HOR, rijxver, 2)+bord_lbekend(HOR, rijxver+3, 3);
  if (result==3) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(HOR, rijxver+1, 2)+bord_lbekend(HOR, rijxver+3, 3);
  if (result==3) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(HOR, rijxver+1, 2)+bord_lbekend(HOR, rijxver+4, 3);
  if (result==3) return result;
  if (result>hoogste) hoogste=result;

/* 2/2/2 */
  result=bord_lbekend(HOR, rijxver, 2)+bord_lbekend(HOR, rijxver+2, 2)+bord_lbekend(HOR, rijxver+4, 2);
  if (result==3) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(HOR, rijxver, 2)+bord_lbekend(HOR, rijxver+2, 2)+bord_lbekend(HOR, rijxver+5, 2);
  if (result==3) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(HOR, rijxver, 2)+bord_lbekend(HOR, rijxver+3, 2)+bord_lbekend(HOR, rijxver+5, 2);
  if (result==3) return result;
  if (result>hoogste) hoogste=result;

  result=bord_lbekend(HOR, rijxver+1, 2)+bord_lbekend(HOR, rijxver+3, 2)+bord_lbekend(HOR, rijxver+5, 2);
  if (result==3) return result;
  if (result>hoogste) hoogste=result;

  if (hoogste==2) return 2;

/* 2/2 */
  result=bord_lbekend(HOR, rijxver+1, 2)+bord_lbekend(HOR, rijxver+4, 2);
  if (result==2) return result;
  if (result>hoogste) hoogste=result;

  return hoogste;
}

