
class trie
{
  public:
    trie *first_child;
    trie *next;
     char c;
//    trie *childs[26];
    bool end;

    trie();
    ~trie();
    trie *isinnext(char x);
};

void init_woorden(void);
void show_woorden(void);
void deinit_woorden(void);

extern trie *wb;
extern int nnodes;
