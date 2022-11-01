#ifndef _SPRITES_H
#define _SPRITES_H

// Diferencias entre windows y linux. (MacOS como que no, sorry)
#ifdef _WIN32
#include <conio.h>
#include <Windows.h>
#elif __linux__
#include <curses.h>
#elif __APPLE_
#include <curses.h>
#endif

#include <iostream>
#include <vector>
#include "mina.h"
enum tColor { NEGRO, AZUL, VERDE, CYAN, ROJO, MORADO, AMARILLO, BLANCO, GRIS};
enum tTheme { THEME_DEFAULT, THEME_BnW};


const vector<int> ctr = { 30,34,32,36,31,35,33,37,90 };
//^Sirve para traducir los colores de Windows a Linux (Color TRanslate -> ctr)
const tTheme ThemeName = THEME_DEFAULT;

tColor theme(char opt, tElemento elem);
void sprite(tElemento elem, int section, tColor colorFondo = NEGRO, tColor colorTexto = BLANCO);
void printTitleScreen(tColor colorFondo = NEGRO, tColor colorTexto = AMARILLO);
void printGameOver(tColor colorFondo = NEGRO, tColor colorTexto = ROJO);
void printVictory(tColor colorFondo = NEGRO, tColor colorTexto = VERDE);
void colorear(tColor colorFondo, string msg, tColor colorTexto = tColor(15));
void systemClear(bool partial = false); // Por defecto, borra toda la pantalla 
void systemPause();

#endif
