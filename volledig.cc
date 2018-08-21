#include <math.h>
#include <stdio.h>
#include <assert.h>
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
#include "extio.h"
#include "grafisch.h"
#include "volledig.h"
int scores[52][2];
int superscores[PRGMAX][PRGMAX][52][2];
int totaalth[PRGMAX];
int totaalui[PRGMAX];
int winstth[PRGMAX];
int winstui[PRGMAX];
int gelijk[PRGMAX];
int totaal;

int alllog(char prog[][256], int prgptrmax, bool log, char *logfile, bool shelllog, char *shelllogfile)
{
  int i, j;
  int nthwin1, nuiwin1, nthwin2, nuiwin2, ngel, thx1, uix1, thx2, uix2;
  if (!shelllog || !log)
  {
    printf("Forgotten to mention logfile or shelllogfile\n");
    return 1;
  }
  FILE *logstr=fopen(logfile, "a");
  FILE *shelllogstr=fopen(shelllogfile, "r");
  assert(logstr!=NULL);
  assert(shelllogstr!=NULL);
  
  for (i=0; i<prgptrmax; i++)
  {
    totaalui[i]=0;
    totaalth[i]=0;
    winstth[i]=0;
    winstui[i]=0;
    gelijk[i]=0;
  }
  totaal=52*(prgptrmax-1);
  for (i=0; i<prgptrmax; i++)
  {
    for (j=i+1; j<prgptrmax; j++)
    {
      fscanf(shelllogstr, "%d %d %d %d %d %d %d %d %d\n",
          &nthwin1, &nuiwin1, &nthwin2, &nuiwin2, &ngel, &thx1, &uix1, &thx2, &uix2);
      
      printf("%s vs %s: %d %d %d %d %d %d %d %d %d\n", prog[i], prog[j], nthwin1, nuiwin1, nthwin2, nuiwin2, ngel, thx1, uix1, thx2, uix2);
      totaalth[i]+=thx1;
      totaalui[i]+=uix1;
      totaalth[j]+=thx2;
      totaalui[j]+=uix2;
      winstth[i]+=nthwin1;
      winstui[i]+=nuiwin1;
      winstth[j]+=nthwin2;
      winstui[j]+=nuiwin2;
      gelijk[i]+=ngel;
      gelijk[j]+=ngel;
    }
  }
  printf("\nIeder programma speelt %d partijtjes\n\n", totaal);
  fprintf(logstr, "\nIeder programma speelt %d partijtjes\n\n", totaal);
  for (i=0; i<prgptrmax; i++)
  {
    printf("%s\nwinst thuis %3d, winst uit %3d, gelijk %3d, totaal thuis %5d, \
totaal uit %5d\n", 
        prog[i], winstth[i], winstui[i], gelijk[i], totaalth[i], totaalui[i]);
    fprintf(logstr, "%s\nwinst thuis %3d, winst uit %3d, gelijk %3d, \
totaal thuis %5d, totaal uit %5d\n", 
        prog[i], winstth[i], winstui[i], gelijk[i], totaalth[i], totaalui[i]);
  }
  
  FILE *asdf=fopen("comp/error.txt", "r");
  if (asdf!=NULL)
  {
    char c;
    fprintf(logstr, "\n\nEr is een error opgetreden!\n\n");
    while (!feof(asdf))
    {
      c=fgetc(asdf);
      fprintf(logstr, "%c", c);
      printf("%c", c);
    }
    fclose(asdf);
  }
  fclose(logstr);
  fclose(shelllogstr);
  return 0;
}

int alllog2(char prog[][256], int prgptrmax, bool log, char *logfile, bool shelllog, bool shelllog2, 
    char *shelllogfile, char *shelllogfile2)
{
  int i, j;
  int nthwin1, nuiwin1, nthwin2, nuiwin2, ngel, thx1, uix1, thx2, uix2;
  if (!shelllog || !log || !shelllog2)
  {
    printf("Forgotten to mention logfile, shelllogfile or shelllogfile2\n");
    return 1;
  }
  FILE *logstr=fopen(logfile, "a");
  FILE *shelllogstr=fopen(shelllogfile, "r");
  FILE *shelllogstr2=fopen(shelllogfile2, "r");
  assert(logstr!=NULL);
  assert(shelllogstr!=NULL);
  
  for (i=0; i<prgptrmax; i++)
  {
    totaalui[i]=0;
    totaalth[i]=0;
    winstth[i]=0;
    winstui[i]=0;
    gelijk[i]=0;
  }
  totaal=52*(prgptrmax-1);
  for (i=1; i<prgptrmax; i++)
  {
    for (j=i+1; j<prgptrmax; j++)
    {
      fscanf(shelllogstr, "%d %d %d %d %d %d %d %d %d\n",
          &nthwin1, &nuiwin1, &nthwin2, &nuiwin2, &ngel, &thx1, &uix1, &thx2, &uix2);
      
      printf("%s vs %s: %d %d %d %d %d %d %d %d %d\n", prog[i], prog[j], nthwin1, nuiwin1, nthwin2, nuiwin2, ngel, thx1, uix1, thx2, uix2);
      totaalth[i]+=thx1;
      totaalui[i]+=uix1;
      totaalth[j]+=thx2;
      totaalui[j]+=uix2;
      winstth[i]+=nthwin1;
      winstui[i]+=nuiwin1;
      winstth[j]+=nthwin2;
      winstui[j]+=nuiwin2;
      gelijk[i]+=ngel;
      gelijk[j]+=ngel;
    }
  }
  for (i=1; i<prgptrmax; i++)
  {
    fscanf(shelllogstr2, "%d %d %d %d %d %d %d %d %d\n", 
        &nthwin1, &nuiwin1, &nthwin2, &nuiwin2, &ngel, &thx1, &uix1, &thx2, &uix2);
    
    printf("%s vs %s: %d %d %d %d %d %d %d %d %d\n", 
        prog[0], prog[i], nthwin1, nuiwin1, nthwin2, nuiwin2, ngel, thx1, uix1, thx2, uix2);
    totaalth[0]+=thx1;
    totaalui[0]+=uix1;
    totaalth[i]+=thx2;
    totaalui[i]+=uix2;
    winstth[0]+=nthwin1;
    winstui[0]+=nuiwin1;
    winstth[i]+=nthwin2;
    winstui[i]+=nuiwin2;
    gelijk[0]+=ngel;
    gelijk[i]+=ngel;
  }
  printf("\nIeder programma speelt %d partijtjes\n\n", totaal);
  fprintf(logstr, "\nIeder programma speelt %d partijtjes\n\n", totaal);
  for (i=0; i<prgptrmax; i++)
  {
    printf("%s\nwinst thuis %3d, winst uit %3d, gelijk %3d, totaal thuis %5d, \
totaal uit %5d\n", 
        prog[i], winstth[i], winstui[i], gelijk[i], totaalth[i], totaalui[i]);
    fprintf(logstr, "%s\nwinst thuis %3d, winst uit %3d, gelijk %3d, \
totaal thuis %5d, totaal uit %5d\n", 
        prog[i], winstth[i], winstui[i], gelijk[i], totaalth[i], totaalui[i]);
  }
  
  FILE *asdf=fopen("comp/error.txt", "r");
  if (asdf!=NULL)
  {
    fprintf(logstr, "\n\nEr is een error opgetreden!\n\n");
    fclose(asdf);
  }
  fclose(logstr);
  fclose(shelllogstr);
  return 0;
}

int all2(char prog[][256], int prgptrmax, bool silent, bool verbose, 
    bool fulllog, bool log, char *logfile, bool shell, char * shellfile, bool shelllog, char *shelllogfile)
{
  int i, n, m;
//  FILE *output=fopen(logfile, "w");
  FILE *shoutput=fopen(shellfile, "w");
//  assert(output!=NULL);
  assert(shoutput!=NULL);
//  fclose(output);
  m=(prgptrmax-1);
  n=0;
  if (!shelllog || !shell || !log)
  {
    printf("Forgotten to mention logfile, shellfile or shelllogfile\n");
    return 1;
  }
  fprintf(shoutput, "#!/bin/sh\n");
  for (i=1; i<prgptrmax; i++)
  {
    fprintf(shoutput, "echo specialcomp: %d/%d: %s vs %s\n", ++n, m, prog[0], prog[i]);
    fprintf(shoutput, "./bubbles -Cs %s %s -l %s -a %s\n", prog[0], prog[i], logfile, shelllogfile);
  }
  return 0;
}

int all(char prog[][256], int prgptrmax, bool silent, bool verbose, 
    bool fulllog, bool log, char *logfile, bool shell, char * shellfile, bool shelllog, char *shelllogfile)
{
  int i, j, n, m;
  FILE *output=fopen(logfile, "w");
  FILE *shoutput=fopen(shellfile, "w");
  assert(output!=NULL);
  assert(shoutput!=NULL);
  fclose(output);
  m=(prgptrmax*(prgptrmax-1))/2;
  n=0;
  if (!shelllog || !shell || !log)
  {
    printf("Forgotten to mention logfile, shellfile or shelllogfile\n");
    return 1;
  }
  fprintf(shoutput, "#!/bin/sh\n");
  for (i=0; i<prgptrmax; i++)
  {
    for (j=i+1; j<prgptrmax; j++)
    {
      fprintf(shoutput, "echo %d/%d: %s vs %s\n", ++n, m, prog[i], prog[j]);
      fprintf(shoutput, "./bubbles -Cs %s %s -l %s -a %s\n", prog[i], prog[j], logfile, shelllogfile);
    }    
  }
  return 0;
}

int volledig(char *prog1cmd, char *prog2cmd, bool silent, bool verbose, bool log, 
    char *logfile, bool matrix, bool wait, bool shelllog, char *shelllogfile, bool fulllog)
{
  pid_t mypid;
  int i, l;
  char beginletter;
  char buf[256];
  char prog1[256], prog2[256];
  int error=0;
  int prog1_move;
  int sco1, sco2;
  int hsco1, lsco1;
  int hsco2, lsco2;
  double thxk1, thx1, uixk1, uix1, thvar1, uivar1;
  double thxk2, thx2, uixk2, uix2, thvar2, uivar2;
  double gem1, gem2;
  int nthwin1, nuiwin1;
  int nthwin2, nuiwin2;
  int ngel;
  double thgem1, thgem2;
  double uigem1, uigem2;
  char *endptr;
  
  WINDOW *bordscr1=NULL, *bordscr2=NULL;
  WINDOW *ptussenscr1=NULL, *ptussenscr2=NULL;
  WINDOW *tussenscr1=NULL, *tussenscr2=NULL;
  WINDOW *progscr1=NULL, *progscr2=NULL;
  WINDOW *sprogscr1=NULL, *sprogscr2=NULL;
  WINDOW *queryscr=NULL;

  mypid=getpid();
  init_woorden();
  
  if (!silent)
  {
    initscr();
    cbreak();
    start_color();
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_WHITE);
    attron(A_BOLD);
    noecho();
    getmaxyx(stdscr, maxy, maxx);
    wprintw(stdscr, "Competitie Software Codecup 2004\n");
    wprintw(stdscr, "  VOLLEDIG                      %s vs %s\n", prog1cmd, prog2cmd);
    wrefresh(stdscr);
    
    progscr1=subwin(stdscr, 1, maxx/2-20, 3, 0);
    progscr2=subwin(stdscr, 1, maxx/2-20, 3, maxx/2-18);
    bordscr1=subwin(stdscr, maxy-7, maxx/2-20, 4, 0);
    bordscr2=subwin(stdscr, maxy-7, maxx/2-20, 4, maxx/2-18);
    sprogscr1=subwin(stdscr, 1, 14, 3, maxx-35);
    sprogscr2=subwin(stdscr, 1, 14, 3, maxx-16);
    ptussenscr1=subwin(stdscr, maxy-7, 16, 4, maxx-37);
    tussenscr1=subwin(stdscr, maxy-9, 14, 5, maxx-36);
    ptussenscr2=subwin(stdscr, maxy-7, 16, 4, maxx-18);
    tussenscr2=subwin(stdscr, maxy-9, 14, 5, maxx-17);
    queryscr=subwin(stdscr, 2, maxx, maxy-2, 0);
    wprintw(sprogscr1, "%s", prog1cmd);
    wrefresh(sprogscr1);
    wprintw(sprogscr2, "%s", prog2cmd);
    wrefresh(sprogscr2);
    
    scrollok(bordscr1, true);
    scrollok(bordscr2, true);
    scrollok(tussenscr1, true);
    scrollok(tussenscr2, true);
    wborder(ptussenscr1, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(ptussenscr1);
    wborder(ptussenscr2, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(ptussenscr2);
  }
  
//  for (l=('V'-'A')*2; l<52; l++)
  for (l=0; l<52; l++)
  {
    error=0;
    read_write_reset();
    if (pipe(prog1error) || pipe(prog2error) || pipe(prog1input) || pipe(prog1output) || 
        pipe(prog2input) || pipe(prog2output))
    {
      fprintf (stderr, "Pijpen mislukt.\n");
      return 1;
    }
    if (l%2)
    {
      strcpy(prog1, prog2cmd);
      strcpy(prog2, prog1cmd);
    }
    else
    {
      strcpy(prog1, prog1cmd);
      strcpy(prog2, prog2cmd);
    }
    beginletter=(l/2)+'A';
    prog1pid=fork();
    if (prog1pid==0)
    {
      close(prog1error[0]);
      dup2(prog1error[1], fileno(stderr));
      close(prog1output[0]);
      dup2(prog1output[1], fileno(stdout));
      close(prog1input[1]);
      dup2(prog1input[0], fileno(stdin));
      execl(prog1, prog1, NULL);
      echo();
      endwin();
      kill(mypid, SIGKILL);
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
      execl(prog2, prog2, NULL);
      // komt als het goed is niet meer hier
      echo();
      endwin();
      kill(mypid, SIGKILL);
      kill(prog1pid, SIGKILL);
      return 1;
    }
    if (!silent)
    {
      wclear(queryscr);
      wprintw(queryscr, " VOLLEDIG: %2d/52: %s(%d) vs %s(%d)  <|>  `%c`\n", 
          l+1, prog1, 1+(l%2), prog2, 2-(l%2), beginletter);
      wrefresh(queryscr);
    }
    
    init_bord();
    bord_zet(beginletter, 24);
    bordtoprog1();
    bordtoprog2();
    
    sprintf(buf, "%c\n", beginletter);
    write_prog1(buf, &error);
    write_prog2(buf, &error);
    write_prog1("start\n", &error);
    prog1_move=1;

    if (!silent)
    {
      wclear(progscr1);
      wprintw(progscr1, "%s", prog1);
      wrefresh(progscr1);
      wclear(progscr2);
      wprintw(progscr2, "%s", prog2);
      wrefresh(progscr2);
      if (verbose) 
      {
        wclear(bordscr1);
        wrefresh(bordscr1);
        wclear(bordscr2);
        wrefresh(bordscr2);
      }
      else { wmove(progscr1, 0, 0); wrefresh(progscr1); }
    }
    for (i=0; i<48; i++)
    {
      if (prog1_move)
      {
        if (verbose)
        {
          wprintw(bordscr1, "%d: ", i+1);
          wrefresh(bordscr1);
        }
        getprinterror(i, prog1error, NULL, false, "");
        if (!silent) wread_prog1(progscr1, strlen(prog1)+2, buf, &error);
        else read_prog1(buf, &error);
        if (error)
        {
          printf("ladielada1\n");
          sprintf(errormsg, "Timeout door %s in: %c(%d): %s vs %s\n", prog1, beginletter, i, prog1, prog2);
          break; 
        }
        letters[i]=buf[0];
        if (letters[i]<'A' || letters[i]>'Z')
        {
          if (!silent) 
          { 
            wclear(bordscr1);
            wprintw(bordscr1, "Illegal char `%c`\n", letters[i]);
            wrefresh(bordscr1); 
            wgetch(bordscr1);
          }
          sprintf(errormsg, "Illegal char `%c` door %s in: %c(%d): %s vs %s\n", letters[i], prog1, beginletter, i, prog1, prog2);
          error=1;
          break;
        }
        if (!silent) wread_prog1(progscr1, strlen(prog1)+2, buf, &error);
        else read_prog1(buf, &error);
        getprinterror(i, prog1error, NULL, false, "");
        if (error)
        {
          printf("ladielada1\n");
          sprintf(errormsg, "Timeout door %s in: %c(%d): %s vs %s\n", prog1, beginletter, i, prog1, prog2);
          break; 
        }
        prog1_pos[i]=(int)strtol(buf, &endptr, 10);
        if (*endptr!='\0' || prog1_pos[i]<0 || prog1_pos[i]>48)
        {
          sprintf(errormsg, "Illegal number `%d` door %s in: %c(%d): %s vs %s\n", 
              prog1_pos[i], prog1, beginletter, i, prog1, prog2);
          error=1;
          break;
        }
        if (verbose)
        {
          wprintw(bordscr1, "(`%c`, %d)", letters[i], prog1_pos[i]);
          wrefresh(bordscr1);
        }
        prog1tobord();
        if (bord_bezet(prog1_pos[i]))
        {
          sprintf(errormsg, "Illegal number `%d` (position taken) door %s in: %c(%d): %s vs %s\n", 
              prog1_pos[i], prog1, beginletter, i, prog1, prog2);
          error=1;
          break;
        }
        bord_zet(letters[i], prog1_pos[i]);
        bordtoprog1();
        prog1tobord();
        sprintf(buf, "%c\n", letters[i]);
        write_prog2(buf, &error); 
        if (error) { printf("error met writen"); break;}
        if (verbose)
        {
          wprintw(bordscr2, "%d: ", i+1);
          wrefresh(bordscr2);
        }
        else if (!silent) { wmove(progscr2, 0, 0); wrefresh(progscr2); }
        getprinterror(i, prog2error, NULL, false, "");
        if (!silent) wread_prog2(progscr2, strlen(prog2)+2, buf, &error);
        else read_prog2(buf, &error);
        if (error)
        {
          printf("ladielada2\n");
          sprintf(errormsg, "Timeout door %s in: %c(%d): %s vs %s\n", prog2, beginletter, i, prog1, prog2);
          break; 
        }
        prog2_pos[i]=(int)strtol(buf, &endptr, 10);
        if (*endptr!='\0' || prog2_pos[i]<0 || prog2_pos[i]>48)
        {
          sprintf(errormsg, "Illegal number `%d` door %s in: %c(%d): %s vs %s\n", 
              prog2_pos[i], prog2, beginletter, i, prog1, prog2);
          error=2;
          break;
        }
        if (verbose)
        {
          wprintw(bordscr2, "(%d)\n", prog2_pos[i]);
          wrefresh(bordscr2);
          wprintw(bordscr1, "\n");
          wrefresh(bordscr1);
        }
        prog2tobord();
        if (bord_bezet(prog2_pos[i]))
        {
          sprintf(errormsg, "Illegal number `%d` (position taken) door %s in: %c(%d): %s vs %s\n", 
              prog2_pos[i], prog2, beginletter, i, prog1, prog2);
          error=2;
          break;
        }
        bord_zet(letters[i], prog2_pos[i]);
        bordtoprog2();
        prog1_move=0;
      }
      else
      {
        if (verbose)
        {
          wprintw(bordscr2, "%d: ", i+1);
          wrefresh(bordscr2);
        }
        getprinterror(i, prog2error, NULL, false, "");
        if (!silent) wread_prog2(progscr2, strlen(prog2)+2, buf, &error); 
        else read_prog2(buf, &error); 
        if (error)
        {
          printf("ladielada2\n");
          sprintf(errormsg, "Timeout door %s in: %c(%d): %s vs %s\n", prog2, beginletter, i, prog1, prog2);
          break; 
        }
        letters[i]=buf[0];
        if (letters[i]<'A' || letters[i]>'Z')
        {
          if (!silent) 
          {
            wclear(bordscr2);
            wprintw(bordscr2, "Illegal char `%c`\n", letters[i]);
            wrefresh(bordscr2); 
            wgetch(bordscr2);
          }
          sprintf(errormsg, "Illegal char `%c` door %s in: %c(%d): %s vs %s\n", letters[i], prog2, beginletter, i, prog1, prog2);
          error=2;
          break;
        }
        if (!silent) wread_prog2(progscr2, strlen(prog2)+2, buf, &error); 
        else read_prog2(buf, &error); 
        getprinterror(i, prog2error, NULL, false, "");
        if (error)
        {
          sprintf(errormsg, "Timeout door %s in: %c(%d): %s vs %s\n", prog2, beginletter, i, prog1, prog2);
          break; 
        }
        prog2_pos[i]=(int)strtol(buf, &endptr, 10);
        if (*endptr!='\0' || prog2_pos[i]<0 || prog2_pos[i]>48)
        {
          sprintf(errormsg, "Illegal number `%d` door %s in: %c(%d): %s vs %s\n", 
              prog2_pos[i], prog2, beginletter, i, prog1, prog2);
          error=2;
          break;
        }
        if (verbose)
        {
          wprintw(bordscr2, "(`%c`, %d)", letters[i], prog2_pos[i]);
          wrefresh(bordscr2);
        }
        prog2tobord();
        if (bord_bezet(prog2_pos[i]))
        {
          sprintf(errormsg, "Illegal number `%d` (position taken) door %s in: %c(%d): %s vs %s\n", 
              prog2_pos[i], prog2, beginletter, i, prog1, prog2);
          error=2;
          break;
        }
        bord_zet(letters[i], prog2_pos[i]);
        bordtoprog2();
        sprintf(buf, "%c\n", letters[i]);
        write_prog1(buf, &error);
        if (error) { printf("error met writen"); break;}
        if (verbose)
        {
          wprintw(bordscr1, "%d: ", i+1);
          wrefresh(bordscr1);
        }
        getprinterror(i, prog1error, NULL, false, "");
        if (!silent) wread_prog1(progscr1, strlen(prog1)+2, buf, &error); 
        else read_prog1(buf, &error); 
        if (error)
        {
          sprintf(errormsg, "Timeout door %s in: %c(%d): %s vs %s\n", prog1, beginletter, i, prog1, prog2);
          break;
        }
        prog1_pos[i]=(int)strtol(buf, &endptr, 10);
        if (*endptr!='\0' || prog1_pos[i]<0 || prog1_pos[i]>48)
        {
          sprintf(errormsg, "Illegal number `%d` door %s in: %c(%d): %s vs %s\n", 
              prog1_pos[i], prog1, beginletter, i, prog1, prog2);
          error=1;
          break;
        }
        if (verbose)
        {
          wprintw(bordscr1, "(%d)\n", prog1_pos[i]);
          wrefresh(bordscr1);
          wprintw(bordscr2, "\n");
          wrefresh(bordscr2);
        }
        else if (!silent) { wmove(progscr1, 0, 0); wrefresh(progscr1); }
        prog1tobord();
        if (bord_bezet(prog1_pos[i]))
        {
          sprintf(errormsg, "Illegal number `%d` (position taken) door %s in: %c(%d): %s vs %s\n", 
              prog1_pos[i], prog1, beginletter, i, prog1, prog2);
          error=1;
          break;
        }
        bord_zet(letters[i], prog1_pos[i]);
        bordtoprog1();
        prog1_move=1;
      }
    }
    if (error==1)
    {
      sco2=100;
      sco1=0;
    }
    else if (error==2)
    {
      sco2=0;
      sco1=100;
    }
    else
    {
      prog1tobord();
      sco1=bord_waarde();
      prog2tobord();
      sco2=bord_waarde();
    }
    if (l%2)
    {
      scores[l][0]=sco2;
      scores[l][1]=sco1;
      if (!silent)
      {
        if (sco1<sco2) wcolor_set(tussenscr2, 1, NULL);
        else if (sco1>sco2) wcolor_set(tussenscr2, 2, NULL);
        else wcolor_set(tussenscr2, 3, NULL);
        wprintw(tussenscr2, " %c: %3d - %3d\n", beginletter, sco2, sco1);
        wrefresh(tussenscr2);
      }
    }
    else
    {
      scores[l][0]=sco1;
      scores[l][1]=sco2;
      if (!silent)
      {
        if (sco1<sco2) wcolor_set(tussenscr1, 1, NULL);
        else if (sco1>sco2) wcolor_set(tussenscr1, 2, NULL);
        else wcolor_set(tussenscr1, 3, NULL);
        wprintw(tussenscr1, "%s%c: %3d - %3d\n", (error)?"ER":"", beginletter, sco1, sco2);
        wrefresh(tussenscr1);
      }
    }
    if (silent) { printf("%c", beginletter); fflush(stdout); }
      
    sleep(1);
    if (matrix)
    {
      wclear(bordscr1);
      wprintbordstep(bordscr1, 0, true, 48, beginletter, letters, prog1_pos, 1, 2, 3);
      wrefresh(bordscr1);
      wclear(bordscr2);
      wprintbordstep(bordscr2, 0, false, 48, beginletter, letters, prog2_pos, 1, 2, 3);
      wrefresh(bordscr1);
      if (wait)
      {
        mvwprintw(queryscr, 1, 0, "Druk op een toets...");
        wrefresh(queryscr);
        if (wgetch(queryscr)=='c') wait=false;
        wclear(queryscr);
        wrefresh(queryscr);
      }
    }
    if (error) break;
    close(prog1input[0]); close(prog1input[1]); close(prog1output[0]); close(prog1output[1]); 
    close(prog2input[0]); close(prog2input[1]); close(prog2output[0]); close(prog2output[1]); 
    close(prog1error[0]); close(prog1error[1]);
    close(prog2error[0]); close(prog2error[1]);

    kill(prog1pid, SIGKILL);
    kill(prog2pid, SIGKILL);
  }
  if (error)
  {
//    if (!silent) endwin();
    printf("\nERROR %s: %s\n", (error==1)?prog1:prog2, errormsg);
    if (shelllog)
    {
      FILE *asdf=fopen("comp/error.txt", "a");
      fprintf(asdf, "\nERROR %s: %s\n", (error==1)?prog1:prog2, errormsg);
      fclose(asdf);
/*      FILE *shelllogstr=fopen(shelllogfile, "a");
      assert(shelllogstr!=NULL);
      if (error==1)
      {
        fprintf(shelllogstr, "%s %s %d %d %d %d %d %d %d %d %d ERROR %s\n",  prog1cmd, prog2cmd,
            0, 0, 26, 26, 0, 0, 0, 2600, 2600, errormsg);
      }
      else
      {
        fprintf(shelllogstr, "%s %s %d %d %d %d %d %d %d %d %d ERROR %s\n",  prog1cmd, prog2cmd,
           26, 26, 0, 0, 0, 2600, 2600, 0, 0, errormsg);
      }
      fclose(shelllogstr);*/
    }
  }
//  else
  {
    if (!silent)
    {
      wclear(queryscr);
      wprintw(queryscr, "Einde\nDruk op een toets...");
      wrefresh(queryscr);
      wgetch(queryscr);
      endwin();
    }
    nthwin1=0; nuiwin1=0; gem1=0; 
    nthwin2=0; nuiwin2=0; gem2=0;
    ngel=0;
    thgem1=0; uigem1=0;
    thgem2=0; uigem2=0;
    thxk1=0; thx1=0; uixk1=0; uix1=0; 
    thxk2=0; thx2=0; uixk2=0; uix2=0;
    lsco1=hsco1=scores[0][0];
    lsco2=hsco2=scores[0][0];
    for (l=0; l<52; l++)
    {
      if (scores[l][0]>scores[l][1])
      {
        if (l%2) nuiwin1++;
        else nthwin1++;
      }
      else if (scores[l][1]>scores[l][0])
      {
        if (l%2) nthwin2++;
        else nuiwin2++;
      }
      else ngel++;
      if (l%2) 
      {
        uix1+=scores[l][0];
        uixk1+=scores[l][0]*scores[l][0];
        thx2+=scores[l][1];
        thxk2+=scores[l][1]*scores[l][1];
      }
      else
      {
        thx1+=scores[l][0];
        thxk1+=scores[l][0]*scores[l][0];
        uix2+=scores[l][1];
        uixk2+=scores[l][1]*scores[l][1];
      }
    } 
    gem1=(thx1+uix1)/52.; gem2=(thx2+uix2)/52.;
    thgem1=thx1/26.; uigem1=uix1/26.;
    thgem2=thx2/26.; uigem2=uix2/26.;
    thvar1=(thxk1-(thx1*thx1)/26.)/26.;
    uivar1=(uixk1-(uix1*uix1)/26.)/26.;
    thvar2=(thxk2-(thx2*thx2)/26.)/26.;
    uivar2=(uixk2-(uix2*uix2)/26.)/26.;
    
    printf("\n - %s vs %s - \ngemiddeld %#.4g - %#.4g\n\n", prog1cmd, prog2cmd, gem1, gem2);
    printf("%s: %dx winst thuis %dx winst uit\nscoort thuis %#.4g (%#.2g)\nscoort uit %#.4g (%#.2g)\n\n", 
        prog1cmd, nthwin1, nuiwin1, thgem1, sqrt(thvar1), uigem1, sqrt(uivar1));
    printf("%s: %dx winst thuis %dx winst uit\nscoort thuis %#.4g (%#.2g)\nscoort uit %#.4g (%#.2g)\n\n", 
        prog2cmd, nthwin2, nuiwin2, thgem2, sqrt(thvar2), uigem2, sqrt(uivar2));
    printf("%dx gelijk\n\n", ngel);
    if (log) 
    {
      FILE *logstr=fopen(logfile, "a");
      assert(logstr!=NULL);
      fprintf(logstr, " - %s vs %s - \ngemiddeld %#.4g - %#.4g\n\n", prog1cmd, prog2cmd, gem1, gem2);
      if (fulllog)
      {
        for (l=0; l<26; l++)
        {
          bool tie1, tie2, won1, won2;
          tie1=scores[l*2][0]==scores[l*2][1];
          tie2=scores[l*2+1][0]==scores[l*2+1][1];
          won1=scores[l*2][0]>scores[l*2][1];
          won2=scores[l*2+1][0]>scores[l*2+1][1];
          
          fprintf(logstr, "%c: %c%3d  %c%3d      %c%3d  %c%3d\n", 'A'+l, 
              (won1?'+':(tie1?'=':'-')), scores[l*2][0], (won1?'-':(tie1?'=':'+')), scores[l*2][1],
              (won2?'+':(tie2?'=':'-')), scores[l*2+1][0], (won2?'-':(tie2?'=':'+')), scores[l*2+1][1]);
        }
      }
      fprintf(logstr, "%s: %dx winst thuis %dx winst uit\nscoort thuis %#.4g (%#.2g)\nscoort uit %#.4g (%#.2g)\n\n", 
          prog1cmd, nthwin1, nuiwin1, thgem1, sqrt(thvar1), uigem1, sqrt(uivar1));
      fprintf(logstr, "%s: %dx winst thuis %dx winst uit\nscoort thuis %#.4g (%#.2g)\nscoort uit %#.4g (%#.2g)\n\n", 
          prog2cmd, nthwin2, nuiwin2, thgem2, sqrt(thvar2), uigem2, sqrt(uivar2));
      fprintf(logstr, "%dx gelijk\n\n", ngel);
      fclose(logstr);
    }
    if (shelllog)
    {
      FILE *shelllogstr=fopen(shelllogfile, "a");
      assert(shelllogstr!=NULL);
      fprintf(shelllogstr, "%d %d %d %d %d %d %d %d %d\n",
          nthwin1, nuiwin1, nthwin2, nuiwin2, ngel, (int)thx1, (int)uix1, (int)thx2, (int)uix2);
      fclose(shelllogstr);
    }
  }
  return 0;
}
