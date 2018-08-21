int alllog(char prog[][256], int prgptrmax, bool log, char *logfile, bool shelllog, char *shelllogfile);
int alllog2(char prog[][256], int prgptrmax, bool log, char *logfile, bool shelllog, bool shelllog2, 
    char *shelllogfile, char *shelllogfile2);
int all2(char prog[][256], int prgptrmax, bool silent, bool verbose, 
    bool fulllog, bool log, char *logfile, bool shell, char * shellfile, bool shelllog, char *shelllogfile);
int all(char prog[][256], int prgptrmax, bool silent, bool verbose, 
    bool fulllog, bool log, char *logfile, bool shell, char * shellfile, bool shelllog, char *shelllogfile);
int volledig(char *prog1cmd, char *prog2cmd, bool silent, bool verbose, bool log, 
    char *logfile, bool matrix, bool wait, bool shelllog, char *shelllogfile, bool fulllog);

#define PRGMAX 32
