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

  
int debug(char *prog1cmd, char *prog2cmd, bool bp, bool log, bool fulllog, 
    char *logfile, bool wait, bool reverse)
{
  pid_t mypid;
  int i;
  int sco1, sco2;
  char beginletter;
  char buf[256];
  char prog1[256], prog2[256];
  int error=0;
  int prog1_move;
  char *endptr;
  strcpy(errormsg, "");
  
  WINDOW *progscr=NULL;
  WINDOW *bordscr=NULL;
  WINDOW *titlescr=NULL;
  WINDOW *queryscr=NULL;
  WINDOW *pdebugscr=NULL, *debugscr=NULL;
  WINDOW *pstderrscr=NULL, *stderrscr=NULL;
  mypid=getpid();
  init_woorden();
  init_bord();
  if (log)
  {
    FILE *logstr=fopen(logfile, "w");
    fclose(logstr);
  }
  
  initscr();
  start_color();
  init_pair(0, COLOR_RED, COLOR_WHITE);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_BLUE, COLOR_BLACK);
  init_pair(4, COLOR_RED, COLOR_WHITE);
  wattron(stdscr, A_BOLD);
  noecho();
  getmaxyx(stdscr, maxy, maxx);
  titlescr=newwin(2, maxx, 0, 0);
  wattron(titlescr, A_BOLD);
  wprintw(titlescr, "Competitie Software Codecup 2004\n");
  wprintw(titlescr, "  GRAFISCH                      %s vs %s\n", 
      prog1cmd, prog2cmd);
  printw("%d, %d", maxy, maxx);
  wrefresh(titlescr);
  progscr=newwin(1, maxx, 2, 0);
  wattron(progscr, A_BOLD);
  bordscr=newwin(19, 19, 3, 0);
  wattron(bordscr, A_BOLD);
  pdebugscr=newwin(maxy-5, int((double(maxx-21)/5.)*2.), 3, 20);
  wattron(pdebugscr, A_BOLD);
  wborder(pdebugscr, 0, 0, 0, 0, 0, 0, 0, 0);
  wrefresh(pdebugscr);
  pstderrscr=newwin(maxy-5, int((double(maxx-21)/5.)*3.), 3, 20+int(double(maxx-21)/5.*2.));
  wattron(pdebugscr, A_BOLD);
  wborder(pstderrscr, 0, 0, 0, 0, 0, 0, 0, 0);
  wrefresh(pstderrscr);
  debugscr=newwin(maxy-7, int((double(maxx-21)/5.)*2.)-4, 4, 22);
  wattron(debugscr, A_BOLD);
  stderrscr=newwin(maxy-7, int((double(maxx-21)/5.)*3.)-4, 4, 22+int(double(maxx-21)/5.*2.));
  wattron(stderrscr, A_BOLD);
  queryscr=newwin(2, maxx, maxy-2, 0);
  wattron(queryscr, A_BOLD);
  
  scrollok(bordscr, true);
  scrollok(debugscr, true);
  scrollok(stderrscr, true);
  
  if (pipe(prog1error) || pipe(prog2error) || pipe(prog1input) || pipe(prog1output) || 
      pipe(prog2input) || pipe(prog2output))
  {
     fprintf (stderr, "Pijpen mislukt.\n");
     return 1;
  }
  
  if (reverse)
  {
    strcpy(prog2, prog1cmd);
    strcpy(prog1, prog2cmd);
  }
  else
  {
    strcpy(prog1, prog1cmd);
    strcpy(prog2, prog2cmd);
  }
  wprintw(debugscr, "starting program %s...\n", prog1); 
  wrefresh(debugscr);
  prog1pid=fork(); if (prog1pid==0)
  {
    close(prog1error[0]);
    dup2(prog1error[1], fileno(stderr));
    close(prog1output[0]);
    dup2(prog1output[1], fileno(stdout));
    close(prog1input[1]);
    dup2(prog1input[0], fileno(stdin));
    execl(prog1, prog1, NULL);
    kill(mypid, SIGTERM);
    return 1;
  }
  wprintw(debugscr, "starting program %s...\n", prog2); 
  wrefresh(debugscr);
  prog2pid=fork(); if (prog2pid==0)
  {
    close(prog2error[0]);
    dup2(prog2error[1], fileno(stderr));
    close(prog2output[0]);
    dup2(prog2output[1], fileno(stdout));
    close(prog1input[1]);
    dup2(prog2input[0], fileno(stdin));
    execl(prog2, prog2, NULL);
    // komt als het goed is niet meer hier
    kill(mypid, SIGTERM);
    kill(prog1pid, SIGTERM);
    return 1;
  }
  wprintw(debugscr, "started programs %s & %s succesfully\n", prog1, prog2); 
  wrefresh(debugscr);
  wprintw(debugscr, "\nrequesting beginletter...\n", prog1); 
  wrefresh(debugscr);
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
  if (fulllog)
  {
    FILE *logstr=fopen(logfile, "a");
    fprintf(logstr, "Beginletter: %c\n", beginletter);
    fclose(logstr);
  }
  sprintf(buf, "%c\n", beginletter);
  wprintw(debugscr, "telling prog1 '%c'...\n", beginletter); 
  wrefresh(debugscr);
  write_prog1(buf, &error);
  wprintw(debugscr, "telling prog2 '%c'...\n", beginletter); 
  wrefresh(debugscr);
  write_prog2(buf, &error);
  wprintw(debugscr, "telling prog1 'start'...\n", buf); 
  wrefresh(debugscr);
  write_prog1("start\n", &error);
  prog1_move=1;
  wprintw(progscr, "%s vs %s", prog1, prog2);
  wrefresh(progscr);
  read_write_reset();
  for (i=0; i<48 && !error; i++)
  {
    if (prog1_move)
    {
      wprintw(debugscr, "%d: reading prog1 letter...\n", i); 
      wrefresh(debugscr);
      if (!reverse) getprinterror(i, prog1error, stderrscr, log, logfile);
      else getprinterror(i, prog1error, NULL, false, logfile);
      read_prog1(buf, &error); if (error) break;
      if (!reverse) getprinterror(i, prog1error, stderrscr, log, logfile);
      else getprinterror(i, prog1error, NULL, false, logfile);
      letters[i]=buf[0];
      if (letters[i]<'A' || letters[i]>'Z')
      {
        sprintf(errormsg, "Illegal char `%c`\n", letters[i]);
        error=1;
        break;
      }
      wprintw(debugscr, "%d: reading prog1 letter->number...\n", i); 
      wrefresh(debugscr);
      if (!reverse) getprinterror(i, prog1error, stderrscr, log, logfile);
      else getprinterror(i, prog1error, NULL, false, logfile);
      read_prog1(buf, &error); if (error) break;
      if (!reverse) getprinterror(i, prog1error, stderrscr, log, logfile);
      else getprinterror(i, prog1error, NULL, false, logfile);
      prog1_pos[i]=(int)strtol(buf, &endptr, 10);
      if (*endptr!='\0' || prog1_pos[i]<0 || prog1_pos[i]>48)
      {
        sprintf(errormsg, "Illegal number `%d`\n", prog1_pos[i]);
        error=1;
        break;
      }  
      if (!reverse) wprintw(debugscr, "%d:  %c , %d\n", i, letters[i], prog1_pos[i]);
      if (!reverse) wrefresh(debugscr);
      if (fulllog && !reverse) // prog1 prog1
      {
        FILE *logstr=fopen(logfile, "a");
        fprintf(logstr, "%d:  %c , %d\n", i, letters[i], prog1_pos[i]);
        fclose(logstr);
      }
      prog1tobord();
      if (bord_bezet(prog1_pos[i]))
      {
        sprintf(errormsg, "Illegal number `%d` (position taken)\n", prog1_pos[i]);
        error=1;
        break;
      }
      bord_zet(letters[i], prog1_pos[i]);
      bordtoprog1();
      if (bp && !reverse)
      { 
        wprintbordstep(bordscr, 1, true, i+1, beginletter, letters, 
            prog1_pos, 1, 2, 3);
        wrefresh(bordscr);
      }
      if (wait && !reverse)
      {
        wmove(queryscr, 0, 0);
        wrefresh(queryscr);
        wgetch(queryscr);
      }
      sprintf(buf, "%c\n", letters[i]);
      wprintw(debugscr, "%d: writing prog2 letter...\n", i); 
      wrefresh(debugscr);
      if (reverse) getprinterror(i, prog2error, stderrscr, log, logfile);
      else getprinterror(i, prog2error, NULL, false, logfile);
      write_prog2(buf, &error); if (error) break;
      if (reverse) getprinterror(i, prog2error, stderrscr, log, logfile);
      else getprinterror(i, prog2error, NULL, false, logfile);
      wprintw(debugscr, "%d: reading prog2 letter->number...\n", i); 
      wrefresh(debugscr);
      if (fulllog && reverse) // prog1 prog2
      {
        FILE *logstr=fopen(logfile, "a");
        fprintf(logstr, "%d: (%c)", i, letters[i]);
        fclose(logstr);
      }
      read_prog2(buf, &error); if (error) break;
      if (reverse) getprinterror(i, prog2error, stderrscr, log, logfile);
      else getprinterror(i, prog2error, NULL, false, logfile);
      prog2_pos[i]=(int)strtol(buf, &endptr, 10);
      if (*endptr!='\0' || prog2_pos[i]<0 || prog2_pos[i]>48)
      {
        sprintf(errormsg, "Illegal number `%d`\n", prog2_pos[i]);
        error=2;
        break;
      }  
      if (reverse) wprintw(debugscr, "%d: (%c), %d\n", i, letters[i], prog2_pos[i]);
      if (reverse) wrefresh(debugscr);
      if (fulllog && reverse)
      {
        FILE *logstr=fopen(logfile, "a");
        fprintf(logstr, " %d\n", prog1_pos[i]);
        fclose(logstr);
      }
      prog2tobord();
      if (bord_bezet(prog2_pos[i]))
      {
        sprintf(errormsg, "Illegal number `%d` (position taken)\n", prog2_pos[i]);
        error=2;
        break;
      }
      bord_zet(letters[i], prog2_pos[i]);
      bordtoprog2();
      if (bp && reverse)
      { 
        wprintbordstep(bordscr, 1, false, i+1, beginletter, letters, 
            prog2_pos, 1, 2, 3);
        wrefresh(bordscr);
      }
      if (wait && reverse)
      {
        wmove(queryscr, 0, 0);
        wrefresh(queryscr);
        wgetch(queryscr);
      }
      prog1_move=0;
    }
    else
    {
      wprintw(debugscr, "%d: reading prog2 letter...\n", i); 
      wrefresh(debugscr);
      if (reverse) getprinterror(i, prog2error, stderrscr, log, logfile);
      else getprinterror(i, prog2error, NULL, false, logfile);
      read_prog2(buf, &error); if (error) break;
      if (reverse) getprinterror(i, prog2error, stderrscr, log, logfile);
      else getprinterror(i, prog2error, NULL, false, logfile);
      letters[i]=buf[0];
      if (letters[i]<'A' || letters[i]>'Z')
      {
        sprintf(errormsg, "Illegal char `%c`\n", letters[i]);
        error=2;
        break;
      }
      wprintw(debugscr, "%d: reading prog2 letter->number...\n", i); 
      wrefresh(debugscr);
      if (reverse) getprinterror(i, prog2error, stderrscr, log, logfile);
      else getprinterror(i, prog2error, NULL, false, logfile);
      read_prog2(buf, &error); if (error) break;
      if (reverse) getprinterror(i, prog2error, stderrscr, log, logfile);
      else getprinterror(i, prog2error, NULL, false, logfile);
      prog2_pos[i]=(int)strtol(buf, &endptr, 10);
      if (*endptr!='\0' || prog2_pos[i]<0 || prog2_pos[i]>48)
      {
        sprintf(errormsg, "Illegal number `%d`\n", prog2_pos[i]);
        error=2;
        break;
      }  
      if (fulllog && reverse) //prog2 prog2
      {
        FILE *logstr=fopen(logfile, "a");
        fprintf(logstr, "%d:  %c  %d\n", i, letters[i], prog2_pos[i]);
        fclose(logstr);
      }
      if (reverse) wprintw(debugscr, "%d:  %c , %d\n", i, letters[i], prog2_pos[i]);
      if (reverse) wrefresh(debugscr);
      prog2tobord();
      if (bord_bezet(prog2_pos[i]))
      {
        sprintf(errormsg, "Illegal number `%d` (position taken)\n", prog2_pos[i]);
        error=2;
        break;
      }
      bord_zet(letters[i], prog2_pos[i]);
      bordtoprog2();
      if (bp && reverse)
      { 
        wprintbordstep(bordscr, 1, false, i+1, beginletter, letters, 
            prog2_pos, 1, 2, 3);
        wrefresh(bordscr);
      }
      if (wait && reverse)
      {
        wmove(queryscr, 0, 0);
        wrefresh(queryscr);
        wgetch(queryscr);
      }
      sprintf(buf, "%c\n", letters[i]);
      wprintw(debugscr, "%d: writing prog1 letter...\n", i); 
      wrefresh(debugscr);
      if (!reverse) getprinterror(i, prog1error, stderrscr, log, logfile);
      else getprinterror(i, prog1error, NULL, false, logfile);
      write_prog1(buf, &error); if (error) break;
      wprintw(debugscr, "%d: reading prog1 letter->number...\n", i); 
      wrefresh(debugscr);
      if (!reverse) getprinterror(i, prog1error, stderrscr, log, logfile);
      else getprinterror(i, prog1error, NULL, false, logfile);
      if (fulllog && !reverse) //prog2 prog1
      {
        FILE *logstr=fopen(logfile, "a");
        fprintf(logstr, "%d:  (%c)", i, letters[i]);
        fclose(logstr);
      }
      read_prog1(buf, &error); if (error) break;
      if (!reverse) getprinterror(i, prog1error, stderrscr, log, logfile);
      else getprinterror(i, prog1error, NULL, false, logfile);
      prog1_pos[i]=(int)strtol(buf, &endptr, 10);
      if (*endptr!='\0' || prog1_pos[i]<0 || prog1_pos[i]>48)
      {
        sprintf(errormsg, "Illegal number `%d`\n", prog1_pos[i]);
        error=1;
        break;
      }  
      if (!reverse) wprintw(debugscr, "%d: (%c), %d\n", i, letters[i], prog1_pos[i]);
      if (!reverse) wrefresh(debugscr);
      if (fulllog && !reverse)
      {
        FILE *logstr=fopen(logfile, "a");
        fprintf(logstr, " %d\n", prog1_pos[i]);
        fclose(logstr);
      }
      prog1tobord();
      if (bord_bezet(prog1_pos[i]))
      {
        sprintf(errormsg, "Illegal number `%d` (position taken)\n", prog1_pos[i]);
        error=1;
        break;
      }
      bord_zet(letters[i], prog1_pos[i]);
      bordtoprog1();
      if (bp && !reverse)
      { 
        wprintbordstep(bordscr, 1, true, i+1, beginletter, letters, 
            prog1_pos, 1, 2, 3);
        wrefresh(bordscr);
      }
      if (wait && !reverse)
      {
        wmove(queryscr, 0, 0);
        wrefresh(queryscr);
        wgetch(queryscr);
      }
      prog1_move=1;
    }
  }
  if (error)
  {
    sleep(2);
    if (!reverse) getprinterror(i, prog1error, stderrscr, log, logfile);
    else getprinterror(i, prog2error, debugscr, log, logfile);
    char errorprog[256];
    if (error==1)
    {
      sco1=0;
      sco2=100;
      strcpy(errorprog, prog1);
    }
    else 
    {
      sco1=100;
      sco2=0;
      strcpy(errorprog, prog2);
    }
    wprintw(progscr, "  |  %d - %d", sco1, sco2);
    wrefresh(progscr);
    if (reverse) wprintbordstep(bordscr, 1, false, i, beginletter, letters, prog2_pos, 1, 2, 3);
    else wprintbordstep(bordscr, 1, true, i, beginletter, letters, prog1_pos, 1, 2, 3);
    wprintw(queryscr, "Error(%s): %s\nDruk op een toets...", errorprog, errormsg);
    wrefresh(queryscr);
    wgetch(queryscr);
    endwin();
    if (log)
    {
      FILE *logstr=fopen(logfile, "a");
      fprintf(logstr, "Error(%s): %s\n", errorprog, errormsg);
      fclose(logstr);
    }
  }
  else
  {
    prog1tobord();
    sco1=bord_waarde();
    prog2tobord();
    sco2=bord_waarde();
    wprintw(progscr, "  |  %d - %d", sco1, sco2);
    wrefresh(progscr);
    if (reverse) wprintbordstep(bordscr, 1, false, 48, beginletter, letters, prog2_pos, 1, 2, 3);
    else wprintbordstep(bordscr, 1, true, 48, beginletter, letters, prog1_pos, 1, 2, 3);
    wprintw(queryscr, "Einde\nDruk op een toets...");
    wrefresh(queryscr);
    wgetch(queryscr);
    endwin();
  }
  kill(prog1pid, SIGTERM);
  kill(prog2pid, SIGTERM);
  return 0;
}

