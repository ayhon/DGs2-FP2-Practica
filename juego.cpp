//Author: Ayhon
#include <iostream>
#include <climits>
#include <string>
#include <vector>
#include <algorithm>
#include "juego.h"

#ifdef _WIN32
#include <conio.h>
#elif __linux__
#include "conio.h"
#elif __APPLE__
#include "conio.h"
#endif

using namespace std;

void Log(string const& msg){ 
	//^Imrpime msg si la constante DEBUG está activada
	if (DEBUG) cout << msg << '\n';
}

void explosion(tJuego& juego, int x, int y) { 
	//^Genera el efecto de la explosion de una dinamita en (x,y)
	tPlano& plano = juego.mina.plano; // Para no escribir demasiado

	// Recorre las 8 casilla al rededor de (x,y), más la propia (x,y)
	for (int i = x-1; i <= x+1; i++) {
		for (int j = y-1; j <= y+1; j++) {
			if(dentroPlano(juego.mina, i, j)){
				if(plano[i][j] == MINERO) juego.estado = EXPLOTADO; // Desafortunada situación :(
				plano[i][j] = LIBRE;
			} 
		}
	}
	// Si hay elementos que pueden caer, hacer que caigan
	for (size_t j = y-1; j <= y+1; j++) {
		if(dentroPlano(juego.mina, x-2, j)) avalancha(juego, x-2, j);
	}
	
}

void lanzarDinamita(tJuego& juego) {
	//^Accion del minero de lanzar una dinamita
	juego.dinamitasUsadas++;
	int& x = juego.mina.minero.x,
		y = juego.mina.minero.y;
	tPlano& plano = juego.mina.plano;
	int tx; // target x
	for (tx = x + 1; tx < plano.size() && plano[tx][y] == LIBRE; tx++) {
		// Pequeña animación
		tElemento tmp = plano[tx][y];
		plano[tx][y] = DINAMITA;
		dibujar(juego);
		system("sleep .1");
		plano[tx][y] = tmp;
	}
	tx--; // Volvamos a la última posición válida
	explosion(juego, tx, y);
}

void moverMinero(tJuego& juego, int hasta_x, int hasta_y) {
	//^Mueve al minero a (hasta_x, hasta_y). 
	// No se encarga de saber si esto es posible
	int& desde_x = juego.mina.minero.x,
		 desde_y = juego.mina.minero.y;
	tPlano& plano = juego.mina.plano;

	// Actualizar posiciones en el plano
	plano[hasta_x][hasta_y] = plano[desde_x][desde_y];
	plano[desde_x][desde_y] = LIBRE;

	// Actualizar la posición del minero
	juego.mina.minero.x = hasta_x;
	juego.mina.minero.y = hasta_y;
}

void queCaiga(tJuego & juego, int x, int y) {
	//^Permite al elemento en (x,y) caer hasta donde pueda
	// No se encarga de que el elemento (x,y) deba caer
	tPlano& plano = juego.mina.plano;
	if(dentroPlano(juego.mina, x+1, y) && plano[x+1][y] == LIBRE) {
			int nx; // nx es la nueva altura donde ira el elemento
			// Hallamos el final del hoyo
			for(nx = x+1; nx < plano.size() && plano[nx][y] == LIBRE; ++nx) juego.mina.visitados[nx][y] = true; // visitados nos dice si ha habido movimiento en esa casilla o no
			swap(plano[x][y], plano[nx-1][y]); // nx-1 pues queremos justo la posición anterior, la que sigue en el hoyo
	}
}

void avalancha(tJuego & juego, int x, int y) {
	//^Permite que caigan varios elementos seguidos, si están en una pila
	int tx;
	for(tx = x; tx >= 0 && esElemQueCae(juego.mina.plano[tx][y]); --tx) {
		queCaiga(juego, tx, y);
	}
}

void printStats(tJuego const& juego) {
	//^Imprime las estadísticas del minero actuales
	cout << "Movimientos: " << juego.numMovimientosMinero << '\n';
	cout << "Gemas: " << juego.gemasRecogidas << '\n';
	cout << "Dinamitas: " << juego.dinamitasUsadas << '\n';
	if(DEBUG) {
		cout << "Estado: " << (int) juego.estado << '\n';
		cout << "Pos minero: " << juego.mina.minero.x << "-" << juego.mina.minero.y << '\n';
	}
}

istream& operator>>(istream& movimientos, tTecla& tecla) {
	//^Sobrecarga de >> para teclas
	// Usado para leer de fichero
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
		case 'D':
			tecla = TNT;
			break;
		case '\n':
			tecla = FIN;
			break;
		default:
			tecla = ERROR_TECLA;
	}
	return movimientos;
}

tTecla leerTeclado() {
	//^Lee las teclas pulsadas y las traduce a tTecla
	// Funciona en Windows y Linux (MacOS pendiente de confirmación)
	tTecla tecla = ERROR_TECLA;
#ifdef _WIN32
	int dir = _getch();
	if (dir == 0xe0)
		dir = _getch();
	switch (dir) {
	case 27:
		tecla = SALIR;
		break;
	case 107: // Vim keybindings
	case 72:
		tecla = ARRIBA;
		break;
	case 106: // Vim keybindings
	case 80:
		tecla = ABAJO;
		break;
	case 108: // Vim keybindings
	case 77:
		tecla = DCHA;
		break;
	case 104: // Vim keybindings
	case 75:
		tecla = IZDA;
		break;
	case 105: // Vim keybindings
	case 32:
	case 64:
	case 44:
		tecla = TNT;
		break;
	}
	return tecla;
#else
	bool specialKey = false;
	int dir = _getch();
	Log(to_string(dir) + " ");
	if (dir == 27) {
		dir = _getch();
		Log(to_string(dir) + " ");
		if (dir == 91) {
			specialKey = true;
			dir = _getch();
			Log(to_string(dir) + " ");
		}
		else
			dir = 7;
	}
	Log(to_string(dir) + " ");
	switch (dir) {
	// Vim keybindings
	// Se ponen separados para evitar tocar `specialKey`
	case 104: 
		tecla = IZDA;
		break;
	case 105: 
		tecla = TNT;
		break;
	case 106: 
		tecla = ABAJO;
		break;
	case 107: 
		tecla = ARRIBA;
		break;
	case 108: 
		tecla = DCHA;
		break;

	case 65:
		if (specialKey)
			tecla = ARRIBA;
		break;
	case 66:
		if (specialKey)
			tecla = ABAJO;
		break;
	case 67:
		if (specialKey)
			tecla = DCHA;
		break;
	case 68:
		if (specialKey)
			tecla = IZDA;
		else
			tecla = TNT;
		break;
	case 83:
	case 115:
	case 27:
		tecla = SALIR;
		break;
	case 32:
	case 64:
	case 112:
	case 44:
		tecla = TNT;
		break;
	}

	return tecla;
#endif
}

void leerMovimiento(tJuego & juego, tTecla & tecla, istream & movimientos) {
	//^Lee el movimiento según el dispositivo de entrada, y trata la tecla de salida
	if(juego.dispositivoDeEntrada == 1)
		tecla = leerTeclado();
	else if(juego.dispositivoDeEntrada == 2)
		movimientos >> tecla;

	if (tecla == SALIR)
		juego.estado = ABANDONO;
}

void dibujar(tJuego const& juego) {
	//^Dibuja el tablero en pantalla
	systemClear(true); // Con true como parámetro, la limpieza de la consola es parcial (Solo mueve el cursor, sobreescribe), lo que evita parpadeos
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
	if(juego.estado == EXITO) cout << "Mina recorrida";
	else if(juego.estado == EXPLOTADO) cout << "Le alcanzo la dinamita";
	else if (juego.estado == ABANDONO) cout << "Pide rescate";
	cout << '\n';
}

void realizarMovimiento(tJuego& juego, tTecla& tecla) {
	//^Realiza el movimiento en tTecla en el juego especificado y dibuja el plano resultante

	juego.numMovimientosMinero++;
	tPlano& plano = juego.mina.plano;
	int x = juego.mina.minero.x,
		y = juego.mina.minero.y;
	int nx = x, ny = y;

	nx += tdirs4[int(tecla)].x;
	ny += tdirs4[int(tecla)].y;
	bool hazMovim = false;
	if(int(tecla) < 4) {
		if(dentroPlano(juego.mina, nx, ny)) {
			// El switch comprueba si se puede hacer un movimiento, y lo prepara para ello
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
				juego.mina.visitados[nx][ny] = true;
				if (x != 0 && esElemQueCae(plano[x-1][y]))
					avalancha(juego, x-1, y);
			}
			else Log("No te mueves este turno");
		}
	}
	else if (tecla == TNT) {
		lanzarDinamita(juego);
	}
	else Log("Has realizado un movimiento indebido, hacia ("+to_string(nx)+"-"+to_string(ny)+")\n");
	dibujar(juego);
}


void jugar(tJuego & juego, istream & entrada, istream & movimientos) {
	cargar_mina(entrada, juego.mina);
	dibujar(juego); // Imprime el mapa inicial
	tTecla input;
	do{
		leerMovimiento(juego, input, movimientos);
		if(juego.estado != ABANDONO && input != FIN)  {
			realizarMovimiento(juego, input);
		} 
	} while(input != FIN && juego.estado == EXPLORANDO);
	dibujar(juego); // Esto debería poder quitarse
	/*
	if(juego.dispositivoDeEntrada = 2) 
		while(input != FIN) leerMovimiento(juego, input, movimientos); // Agotar movimientos no usados
	*/
}
