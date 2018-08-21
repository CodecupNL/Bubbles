#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>
//#include <sys/wait.h>
#include <unistd.h>
#include <curses.h>
#include <stdlib.h>
#include "woorden.h"
#include "bord.h"
#include "extio.h"

int prog1pid, prog2pid;
int prog1output[2], prog1input[2], prog2output[2], prog2input[2];
int prog1error[2], prog2error[2];

char prog1buf[256];
int prog1bufptr;
char prog2buf[256];
int prog2bufptr;

char letters[48];
int prog1_pos[48];
int prog2_pos[48];
char prog1_bord[49];
char prog2_bord[49];
char errormsg[256];
struct timeval left1, left2;

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


int read_from_pipedes(int filedes[2], char *buf, int max, timeval *left)
{
  int n;
  fd_set rfds;
  struct timeval tv, start, stop;
  int retval;

  FD_ZERO(&rfds);
  FD_SET(filedes[0], &rfds);

  tv=*left;
//  tv.tv_sec = 10;
//  tv.tv_usec = 0;
  gettimeofday(&start, NULL);
  retval=select(filedes[0]+1, &rfds, NULL, NULL, &tv);
  gettimeofday(&stop, NULL);
  left->tv_usec -= (stop.tv_usec-start.tv_usec);
  if (left->tv_usec<0) 
  {
    left->tv_sec--;
    left->tv_usec+=1000000;
  }
  left->tv_sec -= (stop.tv_sec-start.tv_sec);
  if (left->tv_sec<0) return -2;
  if (retval)
  {
    n=read(filedes[0], buf, max-1);
    if (n<0) return -1;
    buf[n]='\0';
  }
  else return -2;
  return n;
}


int read_from_pipedes_line(int *bufptr, char *buffer, int filedes[2], char *buf, int max, timeval *left)
{
  int i, n;
  if (*bufptr==-1) // firstuse
  {
    n=read_from_pipedes(filedes, buffer, max, left);
    if (n<0) return n;
    *bufptr=0;
  }
  else if (buffer[*bufptr]=='\0')
  {
    n=read_from_pipedes(filedes, buffer, max, left);
    if (n<0) return n;
    *bufptr=0;
  }  
  else (*bufptr)++;

  n=-1;
  if (buffer[*bufptr]=='\0' || buffer[*bufptr]=='\n')
  {
    n=read_from_pipedes(filedes, buffer, max, left);
    if (n<0) return n;
    *bufptr=0;
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

void write_prog1(char *buf, int *error)
{
  if (write_to_pipedes(prog1input, buf)<0) 
  {
    strcpy(errormsg, "Error writing to pipe program 1...\n");
    *error=1;
  }
}

void write_prog2(char *buf, int *error)
{
  if (write_to_pipedes(prog2input, buf)<0) 
  {
    strcpy(errormsg, "Error writing to pipe program 2...\n");
    *error=2;
  }
}

void read_prog2(char *buf, int *error)
{
  int n;
  if (*error) return;
  
  n=read_from_pipedes_line(&prog2bufptr, prog2buf, prog2output, buf, 256, &left2);
  if (n==-1)
  {
    strcpy(errormsg, "Error reading from pipe to program 2...\n");
    *error=2;
  }
  else if (n==-2)
  {
    strcpy(errormsg, "Timeout program 2...\n");
    *error=2;
  }
}

void read_prog1(char *buf, int *error)
{
  int n;
  if (*error) return;
  n=read_from_pipedes_line(&prog1bufptr, prog1buf, prog1output, buf, 256, &left1);
  if (n==-1)
  {
    strcpy(errormsg, "Error reading from pipe to program 1...\n");
    *error=1;
  }
  else if (n==-2)
  {
    strcpy(errormsg, "Timeout program 1...\n");
    *error=1;
  }
}

void wread_prog2(WINDOW *scr, int x, char *buf, int *error)
{
  int n;
  if (*error) return;
  n=read_from_pipedes_line(&prog2bufptr, prog2buf, prog2output, buf, 256, &left2);
  if (n==-1)
  {
    strcpy(errormsg, "Error reading from pipe to program 2...\n");
    *error=2;
  }
  else if (n==-2)
  {
    strcpy(errormsg, "Timeout program 2...\n");
    *error=2;
  }
  mvwprintw(scr, 0, x, "%d.%d\n", left2.tv_sec, left2.tv_usec);
  wrefresh(scr);
}

void wread_prog1(WINDOW *scr, int x, char *buf, int *error)
{
  int n;
  if (*error) return;
  n=read_from_pipedes_line(&prog1bufptr, prog1buf, prog1output, buf, 256, &left1);
  if (n==-1)
  {
    strcpy(errormsg, "Error reading from pipe to program 1...\n");
    *error=1;
  }
  else if (n==-2)
  {
    strcpy(errormsg, "Timeout program 1...\n");
    *error=1;
  }
  mvwprintw(scr, 0, x, "%d.%d\n", left1.tv_sec, left1.tv_usec);
  wrefresh(scr);
}

void read_write_reset(void)
{
  prog1bufptr=-1;
  prog2bufptr=-1;
  left1.tv_sec=30;
  left1.tv_usec=0;
  left2.tv_sec=30;
  left2.tv_usec=0;
}

void getprinterror(int i, int filedes[2], WINDOW *scr, bool log, char *logfile)
{
  int n;
  int retval;
  fd_set rfds;
  struct timeval tv;
  char errorbuf[32768];

  FD_ZERO(&rfds);
  FD_SET(filedes[0], &rfds);

  tv.tv_sec = 0;
  tv.tv_usec = 0;
  retval=select(filedes[0]+1, &rfds, NULL, NULL, &tv);
  if (retval)
  {
    n=read(filedes[0], errorbuf, 32767);
    if (n<0) return;
    errorbuf[n]='\0';
  }
  else return;
  if (scr!=NULL) {
    wprintw(scr, "%s", errorbuf);
    wrefresh(scr);
  }
  if (log)
  {
    FILE *logstr=fopen(logfile, "a");
    fprintf(logstr, "%s", errorbuf);
    fclose(logstr);
  }
}

