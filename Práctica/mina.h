#ifndef _MINA_H
#define _MINA_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

enum tElemento {TIERRA, PIEDRA, MURO, GEMA, SALIDA, LIBRE, MINERO, DINAMITA, ERROR_ELEM};
using tPlano = vector<vector<tElemento>>;
using tPlanoVisitados = vector<vector<bool>>; // tPlanoVisitados recoge aquellas casillas por las que ya ha habido movimientos (Jugador o gema)

struct tCoords { int x = -1, y = -1; }; // Para detectar errores, inicializamos a -1
struct tMina {
	tCoords minero;
	tPlano plano;
	tPlanoVisitados visitados;
};

tElemento char2elem (char c);
ostream & operator<<(ostream & out, tElemento const& e);
void cargar_mina(istream & fichero, tMina & mina);
void dibujar1_1(tMina const& mina);
void dibujar3_1(tMina const& mina);
bool dentroPlano(tMina const& mina, int x, int y);
bool esElemQueCae(tElemento &elem);
string to_string(tElemento const& elem);

#endif
