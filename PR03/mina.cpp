//Author: Ayhon
#include <iostream>
#include <climits>
#include <string>
#include <vector>
#include "mina.h"
using namespace std;

#ifdef DOMJUDGE
const bool PRETTY_PRINT = false;
#else
const bool PRETTY_PRINT = false;
#endif

string to_string(tElemento const& elem) {
	string res = "";
    switch(elem) {
		case MURO:
			res = "MURO";
			break;
		case MINERO:
			res = "MINERO";
			break;
		case TIERRA:
			res = "TIERRA";
			break;
		case PIEDRA:
			res = "PIEDRA";
			break;
		case GEMA:
			res = "GEMA";
			break;
		case SALIDA:
			res = "SALIDA";
			break;
		case LIBRE:
			res = "LIBRE";
	}
	return res;
}

tElemento char2elem (char c) {
	tElemento elem;
	switch(c) {
		case 'M':
			elem = MURO;
			break;
		case 'J':
			elem = MINERO;
			break;
		case 'T':
			elem = TIERRA;
			break;
		case 'P':
			elem = PIEDRA;
			break;
		case 'G':
			elem = GEMA;
			break;
		case 'S':
			elem = SALIDA;
			break;
		case '-':
			elem = LIBRE;
			break;
		default:
			cout << "Caracter {" << c << "} no reconocido\n";
	}
	return elem;
}

ostream & operator<< (ostream & out, tElemento const& e) {
	string c;
	switch(e) {
		case PIEDRA:
			if(!PRETTY_PRINT) c = "@";
			else c = "▓";
			break;
		case MURO:
			if(!PRETTY_PRINT) c = "X";
			else c = "▉";
			break;
		case TIERRA:
			if(!PRETTY_PRINT) c = ".";
			else c = "░";
			break;
		case GEMA:
			if(!PRETTY_PRINT) c = "G";
			else c = "€";
			break;
		case SALIDA:
			if(!PRETTY_PRINT) c = "S";
			else c = "§";
			break;
		case MINERO:
			if(!PRETTY_PRINT) c = "M";
			else c = "¶";
			break;
		case LIBRE:
			if(!PRETTY_PRINT) c = "-";
			else c = " ";
			break;
		default:
			cout << "Elemento {" << e << "} no reconocido\n";
	}
	out << c;
	return out;
}

void cargar_mina(istream & fichero, tMina & mina) {
	// Requiere que mina venga con su propio tamaño.
	int numFilas, numColumnas;
	fichero >> numFilas >> numColumnas;
	fichero.ignore(INT_MAX, '\n');
	mina.plano = tPlano(numFilas, vector<tElemento> (numColumnas));
	string input;
	for (size_t i = 0; i < mina.plano.size(); ++i){
		getline(fichero, input);
		for (size_t j = 0; j < mina.plano[i].size(); ++j){
			mina.plano[i][j] = char2elem(input[j]);
			if(input[j] == 'J') {
				mina.minero.x = i;
				mina.minero.y = j;
			}
		}
	}
}

void dibujar1_1(tMina const& mina) {
	for (size_t i = 0; i < mina.plano.size(); ++i){
		for (size_t j = 0; j < mina.plano[i].size(); ++j){
			cout << mina.plano[i][j];
		}
		cout << '\n';
	}
}

void dibujar3_1(tMina const& mina) {
	size_t ratio = 3;
	for (size_t i = 0; i < mina.plano.size(); ++i){
		for (size_t r1 = 0; r1 < ratio; ++r1){
			for (size_t j = 0; j < mina.plano[i].size(); ++j){
				for (size_t r2 = 0; r2 < ratio; ++r2){
					cout << mina.plano[i][j];
				}
			}
			cout << '\n';
		}
	}
}

bool dentroPlano(tMina const& mina, int x, int y) {
	return x < mina.plano.size() &&
		y < mina.plano[0].size() &&
		x >= 0 &&
		y >= 0;
}

bool esElemQueCae(tElemento &elem) {
	return (elem == PIEDRA || elem == GEMA);
}
