/*
 * IMPLEMENTACIÓN DE CONIO.H PARA LINUX
 * Nada que entienda en profundidad, esto está aquí solo para que la práctica
 * funcione en Windows. No lo he escrito yo, lo he sacado de StackOverflow, link
 * más abajo. 
 */

#ifndef _CONIO_LINUX
#define _CONIO_LINUX
// found on http://stackoverflow.com/a/7469410/1579481
#include <termios.h>
#include <stdio.h>

static struct termios old, newp;

/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  newp = old; /* make new settings same as old settings */
  newp.c_lflag &= ~ICANON; /* disable buffered i/o */
  newp.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &newp); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char _getch()
{
  return getch_(0);
}

/* Read 1 character with echo */
char _getche(void)
{
  return getch_(1);
}
#endif
