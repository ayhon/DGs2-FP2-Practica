//Author: Ayhon
#include <iostream>
#include <climits>
#include <string>
#include <vector>
#include <algorithm>
#include "juego.h"
using namespace std;
#ifdef DOMJUDGE
const bool DEBUG = false;
#else
const bool DEBUG = false;
#endif

void Log(string const& msg){ if (DEBUG) cout << msg << '\n';}

void moverMinero(tJuego& juego, int hasta_x, int hasta_y) {
	int& desde_x = juego.mina.minero.x,
		 desde_y = juego.mina.minero.y;
	tPlano& plano = juego.mina.plano;
	plano[hasta_x][hasta_y] = plano[desde_x][desde_y];
	plano[desde_x][desde_y] = LIBRE;
	juego.mina.minero.x = hasta_x;
	juego.mina.minero.y = hasta_y;
}

void queCaiga(tJuego & juego, int x, int y) {
	// !! No se encarga de que (x,y) deba caer
	tPlano& plano = juego.mina.plano;
	if(dentroPlano(juego.mina, x+1, y) && plano[x+1][y] == LIBRE) {
			int nx;
			for(nx = x+1; nx < plano.size() && plano[nx][y] == LIBRE; ++nx);
			swap(plano[x][y], plano[nx-1][y]);
	}
}

void avalancha(tJuego & juego, int x, int y) {
	int tx;
	for(tx = x; tx >= 0 && esElemQueCae(juego.mina.plano[tx][y]); --tx) {
		queCaiga(juego, tx, y);
	}
}

void printStats(tJuego const& juego) {
	cout << "Movimientos: " << juego.numMovimientosMinero << '\n';
	cout << "Gemas: " << juego.gemasRecogidas << '\n';
}

istream& operator>>(istream& movimientos, tTecla& tecla) {
	char input;
	movimientos.get(input);
	switch(input){
		case 'A':
			tecla = ARRIBA;
			break;
		case 'Z':
			tecla = ABAJO;
			break;
		case 'M':
			tecla = DCHA;
			break;
		case 'N':
			tecla = IZDA;
			break;
		case 'S':
			tecla = SALIR;
			break;
		case '\n':
			tecla = FIN;
			break;
		default:
			cout << "Error al leer {" << input << "}\n";
	}
	return movimientos;
}

void leerMovimiento(tJuego & juego, tTecla & tecla, istream & movimientos) {
	movimientos >> tecla;
	if (tecla == SALIR) juego.estado = ABANDONO;
}

void dibujar(tJuego const& juego) {
	printStats(juego);
	switch(juego.resolucion) {
		case 1:
			dibujar1_1(juego.mina);
			break;
		case 2:
			dibujar3_1(juego.mina);
			break;
		default:
			Log("No se acepta la resolución " + to_string(juego.resolucion) + "\n");
	}
	if(juego.estado == EXITO) cout << "Mina recorrida\n";
	else cout << "Pide rescate\n";
}

void realizarMovimiento(tJuego& juego, tTecla& tecla) {
	juego.numMovimientosMinero++;
	tPlano& plano = juego.mina.plano;
	int x = juego.mina.minero.x,
		y = juego.mina.minero.y;
	int nx = x, ny = y;

	nx += tdirs4[int(tecla)].x;
	ny += tdirs4[int(tecla)].y;
	bool hazMovim = false;
	if(dentroPlano(juego.mina, nx, ny)) {
		switch(plano[nx][ny]) {
			case MURO:
				Log("Te chocaste contra un muro");
				break;
			case PIEDRA:
				if(tecla == DCHA || tecla == IZDA) {
					int x_detras = nx + tdirs4[int(tecla)].x,
						y_detras = ny + tdirs4[int(tecla)].y;
					// Mover la piedra si hay espacio
					if(dentroPlano(juego.mina, x_detras, y_detras) && plano[x_detras][y_detras] == LIBRE) {
						swap(plano[x_detras][y_detras], plano[nx][ny]);
						hazMovim = true;
						// Que caiga la piedra si tiene que caer
						avalancha(juego, x_detras, y_detras);
					}
					else Log("No hay un espacio libre detras de esa piedra");
				}
				break;
			case TIERRA:
			case LIBRE:
				hazMovim = true;
				break;
			case GEMA:
				hazMovim = true;
				++juego.gemasRecogidas;
				break;
			case SALIDA:
				hazMovim = true;
				juego.estado = EXITO;
				break;
			default:
				Log("No se pudo detectar el elemento");
		}
		if (hazMovim) {
			moverMinero(juego, nx, ny); //Actualiza juego.mina.minero pero no x o y
			if (x != 0 && esElemQueCae(plano[x-1][y]));
				avalancha(juego, x-1, y);
		}
		else Log("No te mueves este turno");
	}
	else Log("Has realizado un movimiento indebido, hacia ("+to_string(nx)+"-"+to_string(ny)+")\n");
	if (DEBUG) { dibujar(juego); cout << '\n'; }
}


void jugar(tJuego & juego, istream & entrada, istream & movimientos) {
	cargar_mina(cin, juego.mina);
	tTecla input;
	leerMovimiento(juego, input, movimientos);
	realizarMovimiento(juego, input);
	while(!(input == FIN || input == SALIR || juego.estado == EXITO)) {
		leerMovimiento(juego, input, movimientos);
		if(input != SALIR) realizarMovimiento(juego, input);
	}
	dibujar(juego);
	while(input != FIN) leerMovimiento(juego, input, movimientos); // Agotar movimientos no usados
}
