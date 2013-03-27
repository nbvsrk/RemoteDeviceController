/* 
 * File:   mystdlib.h
 * Author: newmek7
 *
 * Created on 26 March, 2013, 10:25 AM
 */

#ifndef MYSTDLIB_H
#define	MYSTDLIB_H
#include <sys/types.h>
void initTermios(int echo);
void resetTermios(void);
char getch_(int echo);
char getch(void);
char getche(void);
pid_t spawn(char* program, char** args);

#endif	/* MYSTDLIB_H */

