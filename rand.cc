#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

int main(void)
{
  char buf[256]; 
  char letter;
  int i;
  int my_move=0;
  scanf("%s", buf);
  letter=buf[0];
  scanf("%s", buf);
  timeval tv;
  gettimeofday(&tv, NULL); 
  srand(tv.tv_usec);
  if (!strcmp(buf, "start"))
  {
    my_move=1;
  }
  else letter=buf[0];
   
  for (i=0; i<48; i++)
  {
    if (my_move)
    {
      letter=(rand()%26) + 'A';
      printf("%c\n", letter);
      fflush(stdout);
      printf("%d\n", i);
      fflush(stdout);
      my_move=0;
    }
    else
    {
      if (i>0)
      {
        scanf("%s", buf);
        letter=buf[0];
      }
      printf("%d\n", i);
      fflush(stdout);
      my_move=1;
      fprintf(stderr, "HOI");
    }
  }
  return 0;
}
