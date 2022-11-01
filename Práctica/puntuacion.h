#ifndef _PUNTUACION_H
#define _PUNTUACION_H

#include <iostream>
#include <string>
#include <vector>
#include "sprites.h"
#include "juego.h"
using namespace std;

const unsigned int NUM_TOTAL_MINAS = 5;
const int DEFAULT_TAB_SIZE = 9;
const int PTOS_GEMAS = 10;  // A
const int PTOS_TNT = 2; 	// B
const int PTOS_MOVS = 1;


struct tDatosMina {
	int IdMina = -1;
	int numMovimientos = 0;
	int numGemas = 0;
	int numDinamitas = 0;
	int puntosMina = 0;
};
struct tPuntuacionJugador {
	string nombre = "";
	int puntTotal = 0;
	int minasRecorridas = 0;
	tDatosMina vMinasRecorridas[NUM_TOTAL_MINAS];
};
struct tPuntuaciones {
	int capacidad = 2, numJugadores = 0;
	tPuntuacionJugador* arrayClasificacion;
};

int calcPuntos(tJuego & juego, tDatosMina & mina);
// ^ Calcula los puntos totales de un jugador
int calcPuntos(tJuego & juego, tPuntuacionJugador & jugador);
// ^ Calcula los puntos de una mina

void inicializar_pj(tPuntuacionJugador& pj);
// ^Inicializa `pj`
void cargar_marcador(istream & entrada, tPuntuaciones & marcador);
// ^Carga los datos de `entrada` en `marcador`
void guardar_marcador(ostream & salida, tPuntuaciones & marcador);
// ^Guarda los datos de `marcador` en el fichero `salida`
void mostrar_minas_usuario(const tPuntuaciones & marcador, int pos, int tabSize = 0);
// ^Muestra los datos de `marcador.arrayClasificacion[pos]`
void mostrar_puntuaciones_alfabetico(const tPuntuaciones & marcador);
// ^Muestra las puntuaciones de todos los usuarios (orden αβ)
void mostrar_datos_usuario(const tPuntuaciones & marcador);
// ^Muestra todos los datos de todos los usuarios (orden αβ)

void inicializar_marcador(tPuntuaciones & marcador);
// ^Inicializa el array dinámico
void aumentar_capacidad(tPuntuaciones & marcador);
// ^Duplica el tamaño del array
void destruir(tPuntuaciones & marcador);
// ^Libera la memoria dinámica de marcador
bool buscar(const string& nombre, const tPuntuaciones& marcador, int& pos);
// ^Busqueda binaria de`nombre` en `marcador`. `pos` es la posición donde iría
void insertar(tPuntuaciones& marcador, string const& nombre, int pos);
// ^Inserta ordenadamente en la posición pos. Si no hay espacio, se amplía

#endif
