#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "woorden.h"
int nnodes;
trie *wb=NULL;

trie::trie()
{
//  int i;
  nnodes++;
  first_child=NULL;
  next=NULL;
  end=false;
}

trie::~trie()
{
  nnodes--;
  if (next!=NULL) delete next;
  if (first_child!=NULL) delete first_child;
}

trie* trie::isinnext(char x)
{
  if (c==x) return this;
  if (next==NULL) return NULL;
  return next->isinnext(x);
}

void init_woorden(void)
{
  int i, n;
//  int letter;
  char buf[256]; 
  FILE *instr;
  trie *p, *q;

  nnodes=0;
  instr=fopen("words.dat", "r");
  assert(instr!=NULL);
  wb=new trie;
  assert(wb!=NULL);
  while (!feof(instr))
  {
    fscanf(instr, "%s\n", buf);
    n=strlen(buf);
    q=wb;
    for (i=0; i<n; i++)
    {
      p=q;
      if (p->first_child==NULL)
      {
        p->first_child=new trie;
        p->first_child->c=buf[i];
        q=p->first_child;
      }
      else
      {
        q=p->first_child;
        while (q->next!=NULL)
        {
          if (q->c==buf[i]) break;
          q=q->next;
        }
        if (q->c!=buf[i]) 
        {
          q->next=new trie;
          q=q->next;
          q->c=buf[i];
        }
      }
    }
    assert(q!=NULL);
    q->end=true;
  }
//  printf("%d*%d=%d", nnodes, sizeof(*wb), nnodes*sizeof(*wb));
  fclose(instr);
}

void show(trie *t, char *buf1, char buf2[])
{
//  int i;

  assert(t!=NULL);
  *buf1=t->c;
  if (t->end)
  {
    buf1[1]='\0';
    printf("%s\n", buf2);
  }
  if (t->first_child!=NULL)
  {
    show(t->first_child, buf1+1, buf2);
  }
  if (t->next!=NULL)
  {
    show(t->next, buf1, buf2);
  }
}
    
void show_woorden(void)
{
  char buf[256];
  show(wb->first_child, buf, buf);
  printf("aantal nodes=%d (*%d=%dkB)\n", nnodes, sizeof(*wb), 
      (sizeof(*wb)*nnodes)/1024);
}

void deinit_woorden(void)
{
  delete wb;
}

