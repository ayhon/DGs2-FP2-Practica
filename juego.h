#ifndef _JUEGO_H
#define _JUEGO_H

#include <iostream>
#include <iostream>
#include "mina.h"
#include "sprites.h"
using namespace std;

#ifdef DOMJUDGE
const bool DEBUG = false;
#else
const bool DEBUG = false;
#endif

enum tTecla { ARRIBA, ABAJO, DCHA, IZDA, SALIR, FIN, TNT, NADA, ERROR_TECLA };
enum tEstado { EXPLORANDO, EXITO, ABANDONO, FRACASO, EXPLOTADO};
const tCoords tdirs4[4] = { {-1,0}, {1,0}, {0, 1}, {0, -1} };

struct tJuego {
	tMina mina;
	tEstado estado = EXPLORANDO;
	int gemasRecogidas = 0;
	int numMovimientosMinero = 0;
	int resolucion = 0;
	int dispositivoDeEntrada = 1;
	string archivoDeEntrada = "";
	int dinamitasUsadas = 0;
}; 
void leerMovimiento(tJuego & juego, tTecla & tecla, istream & movimientos);
void realizarMovimientos(tJuego& juego, tTecla mov);
void dibujar(tJuego const& juego);
void jugar(tJuego & juego, istream & entrada, istream & movimientos);

//Custom
void Log(string const& msg);
void queCaiga(tJuego & juego, int x, int y);
void avalancha(tJuego & juego, int x, int y);
void printStats(tJuego const& juego);
void moverMinero(tJuego& juego, int hasta_x, int hasta_y);
//PR04
void explosion(tJuego& juego, int x, int y);
void lanzarDinamita(tJuego& juego);

#endif
