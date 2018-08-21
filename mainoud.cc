#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
//#include <sys/wait.h>
#include <unistd.h>
#include <curses.h>
#include <stdlib.h>
#include "woorden.h"
#include "bord.h"


int prog1pid, prog2pid;
int prog1output[2], prog1input[2], prog2output[2], prog2input[2];

char prog1buf[256];
int prog1bufptr=-1;
char prog2buf[256];
int prog2bufptr;

char letters[48];
int prog1_pos[48];
int prog2_pos[48];
char prog1_bord[49];
char prog2_bord[49];

void bordtoprog1(void)
{
  int i=0;
  for (i=0; i<49; i++) prog1_bord[i]=bord[i];
}

void bordtoprog2(void)
{
  int i=0;
  for (i=0; i<49; i++) prog2_bord[i]=bord[i];
}

void prog1tobord(void)
{
  int i=0;
  for (i=0; i<49; i++) bord[i]=prog1_bord[i];
}

void prog2tobord(void)
{
  int i=0;
  for (i=0; i<49; i++) bord[i]=prog2_bord[i];
}

void doubLLebord()
{
  int i, j;
  for (i=0; i<7; i++)
  {
    for (j=0; j<7; j++)
    {
      if (prog1_bord[i*7+j]==0) printw(". ");
      printw("%c ", prog1_bord[i*7+j]);
    }
    prog1tobord();
    printw("%x\t\t", bord_rijwaarde(i));
    for (j=0; j<7; j++)
    {
      if (prog2_bord[i*7+j]==0) printw(". ");
      printw("%c ", prog2_bord[i*7+j]);
    }
    prog2tobord();
    printw("%d\n", bord_rijwaarde(i));
  }
  prog1tobord();
  for (j=0; j<7; j++)
  {
    printw("%x ", bord_kolomwaarde(j));
  }
  printw("%d\t\t", bord_waarde());
  prog2tobord();
  for (j=0; j<7; j++)
  {
    printw("%x ", bord_kolomwaarde(j));
  }
  printw("%d\n", bord_waarde());
}

int read_from_pipedes(int filedes[2], char *buf, int max)
{
  int n;
  n=read(filedes[0], buf, max-1);
  if (n<0) return n;
  buf[n]='\0';
  return n;
}


int read_from_pipedes_line(int *bufptr, char *buffer, int filedes[2], char *buf, int max)
{
  int i, n;
  if (*bufptr==-1) // firstuse
  {
    n=read_from_pipedes(filedes, buffer, max);
    if (n<=0) return n;
    *bufptr=0;
  }
  else if (buffer[*bufptr]=='\0')
  {
    n=read_from_pipedes(filedes, buffer, max);
    if (n<=0) return n;
    *bufptr=0;
  }  
  else (*bufptr)++;
  n=-1;
  if (buffer[*bufptr]=='\0' || buffer[*bufptr]=='\n')
  {
    return 0;
  }
  for (i=*bufptr; i<max; i++)
  {
    if (buffer[i]=='\n') { n=i; break; }
    else if (buffer[i]=='\0') { n=i; break; }
  }
  if (n==-1) n=max-1;
  for (i=*bufptr; i<n; i++)
  {
    buf[i-*bufptr]=buffer[i];
  }
  buf[i-*bufptr]='\0';
  n-=*bufptr;
  *bufptr+=n;
  return n;
}

int write_to_pipedes(int filedes[2], char *buf)
{
  int i=0, j, n;
  n=strlen(buf);
  while (i<n)
  {
    j=write(filedes[1], buf+i, n-i);
    if (j<0) return j;
    i+=j;
  }
  return n;
}

void write_prog1(char *buf, bool *error)
{
  if (write_to_pipedes(prog1input, buf)<0) *error=true;
}

void write_prog2(char *buf, bool *error)
{
  if (write_to_pipedes(prog2input, buf)<0) *error=true;
}

void read_prog2(char *buf, bool *error)
{
  int n;
  if (*error) return;
  while ((n=read_from_pipedes_line(&prog2bufptr, prog2buf, prog2output, buf, 256))==0);
  if (n<0)
  {
    printw("Error reading from pipe to program2...\nAborting...\n");
    refresh();
    *error=true;
  }
}

void read_prog1(char *buf, bool *error)
{
  int n;
  if (*error) return;
  while ((n=read_from_pipedes_line(&prog1bufptr, prog1buf, prog1output, buf, 256))==0);
  if (n<0)
  {
    printw("Error reading from pipe to program1...\nAborting...\n");
    refresh();
    *error=true;
  }
}

int main(int argc, char *argv[])
{
  int i, n;
  char letter;
  int pos;
  char buf[256];
  bool error=false;
  int prog1_move;

  init_woorden();
  init_bord();
  srand(time(NULL));
  initscr();
  noecho();
  scrollok(stdscr, true);
  
  if (pipe(prog1input) || pipe(prog1output) || pipe(prog2input) || pipe(prog2output))
  {
     fprintf (stderr, "Pijpen mislukt.\n");
     return 1;
  }
  
  prog1pid=fork();
  if (prog1pid==0)
  {
    close(prog1output[0]);
    dup2(prog1output[1], fileno(stdout));
    close(prog1input[1]);
    dup2(prog1input[0], fileno(stdin));
    execl(argv[1], NULL);
  }
  else
  {
    prog2pid=fork();
    if (prog2pid==0)
    {
      close(prog2output[0]);
      dup2(prog2output[1], fileno(stdout));
      close(prog1input[1]);
      dup2(prog2input[0], fileno(stdin));
      execl(argv[2], NULL);
    }
    else
    {
      printw("Competitie Software Codecup 2004\n\n");
      printw("Geef beginletter (`0` voor random): ");
      echo();
      scanw("%c", &letter);
      noecho();
      if (letter<'A' || letter>'Z') letter=(rand()%26)+'A';
      else letter=((letter-'A')&0x1F)+'A';
      sprintf(buf, "%c\n", letter);
      printw("Potje: ./prog tegen ./prog : met beginletter %c\n", letter);
      bord_zet(letter, 24);
      bordtoprog1();
      bordtoprog2();
      refresh();
      write_prog1(buf, &error);
      write_prog2(buf, &error);
      write_prog1("start\n", &error);
      prog1_move=1;
      for (i=0; i<48 && !error; i++)
      {
        if (prog1_move)
        {
          read_prog1(buf, &error);
          letters[i]=buf[0];
          printw("PROG1: LETTERS[%d]=%c\n", i, letters[i]);
          refresh();
          read_prog1(buf, &error);
          prog1_pos[i]=atoi(buf);
          prog1tobord();
          bord_zet(letters[i], prog1_pos[i]);
          bordtoprog1();
          printw("PROG1: POS[%d]=%d\n", i, prog1_pos[i]);
          prog1tobord();
          refresh();
          sprintf(buf, "%c\n", letters[i]);
          write_prog2(buf, &error);
          read_prog2(buf, &error);
          prog2_pos[i]=atoi(buf);
          prog2tobord();
          bord_zet(letters[i], prog2_pos[i]);
          bordtoprog2();
          printw("PROG2: POS[%d]=%d\n", i, prog2_pos[i]);
          refresh();
          prog1_move=0;
        }
        else
        {
          read_prog2(buf, &error);
          letters[i]=buf[0];
          printw("PROG2: LETTERS[%d]=%c\n", i, letters[i]);
          refresh();
          read_prog2(buf, &error);
          prog2_pos[i]=atoi(buf);
          prog2tobord();
          bord_zet(letters[i], prog2_pos[i]);
          bordtoprog2();
          printw("PROG2: POS[%d]=%d\n", i, prog2_pos[i]);
          refresh();
          sprintf(buf, "%c\n", letters[i]);
          write_prog1(buf, &error);
          read_prog1(buf, &error);
          prog1_pos[i]=atoi(buf);
          prog1tobord();
          bord_zet(letters[i], prog1_pos[i]);
          bordtoprog1();
          printw("PROG1: POS[%d]=%d\n", i, prog1_pos[i]);
          refresh();
          prog1_move=1;
        }
      }
      if (error)
      {
      }
    }
  }
  kill(prog1pid, SIGTERM);
  kill(prog2pid, SIGTERM);
  printw("Overzicht\n");
  printw("Programma 1\t\tProgramma 2\n");
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
  } 
  printw("Einde\n");
  refresh();
  getch();
  doubLLebord();
  refresh();
  getch();
  endwin();
}

