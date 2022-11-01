#ifndef _MINA
#define _MINA

#include <iostream>
#include <vector>
using namespace std;

typedef enum {TIERRA, PIEDRA, MURO, GEMA, SALIDA, LIBRE, MINERO} tElemento;
using tPlano = vector<vector<tElemento>>;

struct tCoords { int x = -1, y = -1; }; // Para detectar errores
struct tMina {
	tCoords minero;
	tPlano plano;
};

tElemento char2elem (char c);
ostream & operator<<(ostream & out, tElemento const& e);
void cargar_mina(istream & fichero, tMina & mina);
void dibujar1_1(tMina const& mina);
void dibujar3_1(tMina const& mina);
bool dentroPlano(tMina const& mina, int x, int y);
bool esElemQueCae(tElemento &elem);

#endif
