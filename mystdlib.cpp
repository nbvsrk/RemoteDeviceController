#include "mystdlib.h"
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static struct termios old, mnew;

/* Initialize new terminal i/o settings */
void initTermios(int echo) {
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    mnew = old; /* make new settings same as old settings */
    mnew.c_lflag &= ~ICANON; /* disable buffered i/o */
    mnew.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
    tcsetattr(0, TCSANOW, &mnew); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) {
    tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) {
    char ch;
    initTermios(echo);
    ch = getchar();
    resetTermios();
    return ch;
}

/* Read 1 character without echo */
char getch(void) {
    return getch_(0);
}

/* Read 1 character with echo */
char getche(void) {
    return getch_(1);
}

pid_t spawn(char* program, char** args) {
    pid_t cpid = fork();
    if (cpid == 0) {
	execvp(program, args);
	fprintf(stderr, "an error occured in execvp\n");
    } else {
	return cpid;
    }
    return 0;
}
