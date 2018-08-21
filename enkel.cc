#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
//#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include "woorden.h"
#include "bord.h"
#include "extio.h"

/*****/

int enkel(char *prog1cmd, char *prog2cmd, int *sco1, int *sco2)
{
  pid_t mypid;
  int i;
  char beginletter;
  char buf[256];
  char prog1[256], prog2[256];
  int error=0;
  int prog1_move;

  mypid=getpid();
  init_woorden();
  init_bord();
  
  if (pipe(prog1input) || pipe(prog1output) || pipe(prog2input) || pipe(prog2output))
  {
     fprintf (stderr, "Pijpen mislukt.\n");
     return 1;
  }
  
  strcpy(prog1, prog1cmd);
  strcpy(prog2, prog2cmd);
  prog1pid=fork();
  if (prog1pid==0)
  {
    close(prog1output[0]);
    dup2(prog1output[1], fileno(stdout));
    close(prog1input[1]);
    dup2(prog1input[0], fileno(stdin));
    execl(prog1, NULL);
//    kill(mypid, SIGTERM);
    exit(1);
  }
  prog2pid=fork();
  if (prog2pid==0)
  {
    close(prog2output[0]);
    dup2(prog2output[1], fileno(stdout));
    close(prog1input[1]);
    dup2(prog2input[0], fileno(stdin));
    execl(prog2, NULL);
    // komt als het goed is niet meer hier
    kill(mypid, SIGTERM);
//    kill(prog1pid, SIGTERM);
    exit(1);
  }
  printf("Geef beginletter (<RET> voor random): ");
  scanf("%c", &beginletter);
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
  
  sprintf(buf, "%c\n", beginletter);
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
      read_prog1(buf, &error);
      prog1_pos[i]=atoi(buf);
      prog1tobord();
      bord_zet(letters[i], prog1_pos[i]);
      bordtoprog1();
      prog1tobord();
      sprintf(buf, "%c\n", letters[i]);
      write_prog2(buf, &error);
      read_prog2(buf, &error);
      prog2_pos[i]=atoi(buf);
      prog2tobord();
      bord_zet(letters[i], prog2_pos[i]);
      bordtoprog2();
      prog1_move=0;
    }
    else
    {
      read_prog2(buf, &error);
      letters[i]=buf[0];
      read_prog2(buf, &error);
      prog2_pos[i]=atoi(buf);
      prog2tobord();
      bord_zet(letters[i], prog2_pos[i]);
      bordtoprog2();
      sprintf(buf, "%c\n", letters[i]);
      write_prog1(buf, &error);
      read_prog1(buf, &error);
      prog1_pos[i]=atoi(buf);
      prog1tobord();
      bord_zet(letters[i], prog1_pos[i]);
      bordtoprog1();
      prog1_move=1;
    }
  }
  if (error)
  {
    return -1;
  }
  kill(prog1pid, SIGTERM);
  kill(prog2pid, SIGTERM);
  prog1tobord();
  *sco1=bord_waarde();
  prog2tobord();
  *sco2=bord_waarde();
  return beginletter;
}
