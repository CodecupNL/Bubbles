#include <stdio.h>
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
#include "volledig.h"
#include "enkel.h"
#include "debug.h"
#define NOBREAK

enum TMode { MODGEEN, MODGRF, MODENK, MODVOL, MODCOM, MODLOG, MODDB, MODX, MODQ };
enum TError { ERR_GEEN, ERR_MODESET, ERR_UNKNOWN, ERR_PRGCNT };

void printhelp(char *progname)
{
  printf("\
BUBBLES v1.0\nCodecup 2004 LuckyWords\n\n");
  printf("\
Usage: %s -[SGCALsvmwba] [prog1 prog2 ...] [-l logfile]\n", progname);
  printf("\n\
Options:\n\n");
  printf("\
  S    plays a single game, asks for starting letter (or random)\n\n");
  printf("\
  G    plays a single game in a graphical environment\n\
       asks for starting letter (or random)\n\n");
  printf("\
  C    plays 52 games in a graphical environment (competition)\n\
       outputs statistics afterwards\n\n");
  printf("\
  A    writes a shell file which will let all programs play 52 games\n\
       against every opponent (all)\n\
       outputs statistics afterwards\n\n");
  printf("\
  s    no graphical ouput (silent)\n\
       use with -C\n\n");
  printf("\
  v    extra output (verbose)\n\
       use with -G or -C\n\n");
  printf("\
  m    print matrix\n\
       with -C this option prints a matrix when a game is completed\n\
       with -G this option prints a matrix each time when a programm places a letter\n\n");
}

int main(int argc, char *argv[])
{
  int result=0;
  int i, j, n;
  TMode mode=MODGEEN;
  TError error=ERR_GEEN;
  bool reverse=false;
  bool matrix=false;
  bool wait=false;
  bool verbose=false;
  bool log=false;
  bool shell=false;
  bool shelllog=false;
  bool shelllog2=false;
  bool fulllog=false;
  bool silent=false;
  bool nextlog=false;
  bool nextshell=false;
  bool nextshelllog=false;
  bool nextshelllog2=false;
  char prog[PRGMAX][256];
  char logfile[256]="log";
  char shellfile[256]="competitie.sh";
  char shelllogfile[256]="shelllog";
  char shelllogfile2[256]="shelllog2";
  char errormsg[256];
  int prgptr=0;
  
  for (i=1; i<argc; i++)
  {
    if (argv[i][0]=='-')
    {
      n=strlen(argv[i]);
      for (j=1; j<n; j++)
      {
        switch (argv[i][j])
        {
          case 'D': 
            if (mode!=MODGEEN)
            {
              error=ERR_MODESET;
              sprintf(errormsg, "Too many Capitals (S)");
              break;
            }
            mode=MODDB;
            break;
          case 'S': 
            if (mode!=MODGEEN)
            {
              error=ERR_MODESET;
              sprintf(errormsg, "Too many Capitals (S)");
              break;
            }
            mode=MODENK;
            break;
          case 'G':
            if (mode!=MODGEEN)
            {
              error=ERR_MODESET;
              sprintf(errormsg, "Too many Capitals (G)");
              break;
            }
            mode=MODGRF;
            break;
          case 'C': 
            if (mode!=MODGEEN)
            {
              error=ERR_MODESET;
              sprintf(errormsg, "Too many Capitals (C)");
              break;
            }
            mode=MODVOL; 
            break;
          case 'X': 
            if (mode!=MODGEEN)
            {
              error=ERR_MODESET;
              sprintf(errormsg, "Too many Capitals (X)");
              break;
            }
            mode=MODX; 
            break;
          case 'Q': 
            if (mode!=MODGEEN)
            {
              error=ERR_MODESET;
              sprintf(errormsg, "Too many Capitals (Q)");
              break;
            }
            mode=MODQ; 
            break;
          case 'A': 
            if (mode!=MODGEEN)
            {
              error=ERR_MODESET;
              sprintf(errormsg, "Too many Capitals (A)");
              break;
            }
            mode=MODCOM; 
            break;
          case 'L': 
            if (mode!=MODGEEN)
            {
              error=ERR_MODESET;
              sprintf(errormsg, "Too many Capitals (L)");
              break;
            }
            mode=MODLOG; 
            break;

          case 'r':
            reverse=true;
            break;
          case 's':
            silent=true;
            break;
          case 'v':
            verbose=true;
            break;
          case 'w':
            wait=true;
            break;
          case 'm':
            matrix=true;
            break;
          case 'c':
            if (nextlog || nextshell || nextshelllog) break;
            nextshelllog2=true;
            break;
          case 'a':
            if (nextlog || nextshell || nextshelllog2) break;
            nextshelllog=true;
            break;
          case 'b':
            if (nextlog || nextshelllog || nextshelllog2) break;
            nextshell=true;
            break;
          case 'l':
            if (nextshell || nextshelllog || nextshelllog2) break;
            if (argv[i][j+1]=='+')
            {
              fulllog=true;
              j++;
            }
            nextlog=true;
            break;
          case '-':
            if (!strcmp(argv[i], "--help"))
            {
              printhelp(argv[0]);
              break;
            }
            NOBREAK;
          default:
            error=ERR_UNKNOWN;
            sprintf(errormsg, "Unkown option %c", argv[i][j]);
        }
        if (error) break;
      }
    }
    else
    {
      if (prgptr==PRGMAX) 
      {
        error=ERR_PRGCNT;
        break;
      }
      if (nextlog==true)
      {
        strcpy(logfile, argv[i]);
        log=true;
        nextlog=false;  
      } 
      else if (nextshelllog2==true)
      {
        strcpy(shelllogfile2, argv[i]);
        shelllog2=true;
        nextshelllog2=false;  
      }
      else if (nextshelllog==true)
      {
        strcpy(shelllogfile, argv[i]);
        shelllog=true;
        nextshelllog=false;  
      }
      else if (nextshell==true)
      {
        strcpy(shellfile, argv[i]);
        shell=true;
        nextshell=false;  
      }
      else
      {
        strcpy(prog[prgptr], argv[i]);
        prgptr++;
      }
    }
    if (error) break;
  }

  switch (error)
  {
    case ERR_GEEN:
      if (silent && verbose) 
      {
        printf("Silent option on: switching to non-verbose...\n");
        verbose=false;
      }
      if (silent && matrix) 
      {
        printf("Silent option on: switching to non-matrix...\n");
        matrix=false;
      }
      if (silent && wait) 
      {
        printf("Silent option on: switching to non-waiting...\n");
        wait=false;
      }
      switch (mode)
      {
        case MODX:
          if (prgptr<2)
          {
            printhelp(argv[0]);
            printf("\nERROR: Not enough programs were given...\n");
            result=1;
            break;
          }
          result=alllog2(prog, prgptr, log, logfile, shelllog, shelllog2, shelllogfile, shelllogfile2);
          break;
        case MODLOG:
          if (prgptr<2)
          {
            printhelp(argv[0]);
            printf("\nERROR: Not enough programs were given...\n");
            result=1;
            break;
          }
          result=alllog(prog, prgptr, log, logfile, shelllog, shelllogfile);
          break;
        case MODQ:
          if (prgptr<2)
          {
            printhelp(argv[0]);
            printf("\nERROR: Not enough programs were given...\n");
            result=1;
            break;
          }
          result=all2(prog, prgptr, silent, verbose, fulllog, log, logfile, shell, shellfile, shelllog2, shelllogfile2);
          break;
        case MODCOM:
          if (prgptr<2)
          {
            printhelp(argv[0]);
            printf("\nERROR: Not enough programs were given...\n");
            result=1;
            break;
          }
          result=all(prog, prgptr, silent, verbose, fulllog, log, logfile, shell, shellfile, shelllog, shelllogfile);
          break;
        case MODVOL:
          if (prgptr<2)
          {
            printhelp(argv[0]);
            printf("\nERROR: Not enough programs were given...\n");
            result=1;
            break;
          }
          if (!matrix && wait) wait=false;
          result=volledig(prog[0], prog[1], silent, verbose, log, logfile, matrix, wait, shelllog, shelllogfile, fulllog);
          break;
        case MODDB:
          if (prgptr<2)
          {
            printhelp(argv[0]);
            printf("\nERROR: Not enough programs were given...\n");
            result=1;
            break;
          }
          result=debug(prog[0], prog[1], matrix, log, fulllog, logfile, wait, reverse);
          break;
        case MODGRF:
          if (prgptr<2)
          {
            printhelp(argv[0]);
            printf("\nERROR: Not enough programs were given...\n");
            result=1;
            break;
          }
          result=grafisch(prog[0], prog[1], matrix, log, fulllog, logfile);
          break;
        case MODENK:
          break;
        case MODGEEN:
          printhelp(argv[0]);
          result=1;
          break;
      }
      break;
    case ERR_PRGCNT:
      printhelp(argv[0]);
      printf("\nERROR: Too many programs...\n");
      break;
    case ERR_MODESET:
      printhelp(argv[0]);
      printf("\nERROR: Too many Capitals given as option...\n");
      break;
    case ERR_UNKNOWN:
      printhelp(argv[0]);
      printf("\nERROR: %s...\n", errormsg);
      break;
  }
  if (error) return error;
  else if (result) return result+10;
  else return 0;
}

