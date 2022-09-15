#define _XOPEN_SOURCE 500
#include <signal.h>
typedef void (*sighandler_t)(int);

sighandler_t my_sigset(int,sighandler_t);
int my_sighold(int);
int my_sigrelse(int);
int my_sigignore(int);
int my_sigpause(int);