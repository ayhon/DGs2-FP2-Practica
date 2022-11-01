#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <climits>
#include "juego.h"
#include "puntuacion.h"
using namespace std;

const int ULTIMO_NIVEL = 5;
const string ARCHIVO_MARCADOR = "puntuaciones.txt";

void escogerResolucion(tJuego & juego, ostream& flujoSalida = cout, istream& flujoEntrada = cin) {
	//^Menú para escoger la resolución
    flujoSalida << "Escoge la resolución del juego\n";
    flujoSalida << "1. Jugar partida a escala 1:1\n";
    flujoSalida << "2. Jugar partida a escala 3:1\n";
    flujoSalida << "0. Salir\n";
    flujoSalida << "> "; flujoEntrada >> juego.resolucion;
    while(juego.resolucion > 2 || juego.resolucion < 0)  {
        flujoSalida << "Entrada no valida. Escoge de nuevo\n"; 
        flujoSalida << "> "; flujoEntrada >> juego.resolucion;
    }
}

void escogerDispEntrada(tJuego & juego, ostream& flujoSalida = cout, istream& flujoEntrada = cin) {
	//^Menú para escoger el dispositivo de entrada
    flujoSalida << "Escoge el dispositivo de entrada\n";
    flujoSalida << "1. Usar teclado\n";
    flujoSalida << "2. Usar un fichero\n";
    flujoSalida << "0. Salir\n";
    flujoSalida << "> "; flujoEntrada >> juego.dispositivoDeEntrada;
    while(juego.dispositivoDeEntrada > 2 || juego.dispositivoDeEntrada < 0)  {
        flujoSalida << "Entrada no valida. Escoge de nuevo\n"; 
        flujoSalida << "> "; flujoEntrada >> juego.dispositivoDeEntrada;
    }
    if (juego.dispositivoDeEntrada == 2) {
        string nombreArchivo;
        flujoSalida << "Elige un archivo del que leer los movimientos\n";
        flujoSalida << "> "; flujoEntrada >> nombreArchivo;
        if (nombreArchivo.substr(nombreArchivo.size() - 4) != ".txt")
            nombreArchivo += ".txt";
        juego.archivoDeEntrada = nombreArchivo;
    }
}
string get_name_dialog(tPuntuaciones & marcador, int & pos, int tabSize = 0) {
	//^Menú para coger el nombre del jugador. Lo coloca en `pos` en el marcador
	int tab = (tabSize? tabSize : DEFAULT_TAB_SIZE);
	string nombre;
	cout << "Introduce tu nombre de jugador/a: ";
	cin >> nombre;

	if(buscar(nombre, marcador, pos)) {
		cout << setw(tab) << " " << "Ya estás registrado/a.\n\n"
			<< setw(tab) << " " << "Mira las minas que has recorrido ordenadas por nivel\n\n";
		mostrar_minas_usuario(marcador, pos);
	}
	else {
		cout << setw(tab) << " " << "Eres nuevo/a: " << nombre << "\n\n"
			<< setw(tab) << " " << "Mira las puntuaciones de otros jugadores\n\n";
		mostrar_puntuaciones_alfabetico(marcador);
		insertar(marcador, nombre, pos);
	}
	cout << '\n';
	cin.ignore(INT_MAX, '\n');
	return nombre;
}

int get_level(string nombre, tColor colorTexto = AMARILLO) {
	//^Menú para pedirle el nivel al jugador
	int res;
	colorear(NEGRO, nombre, colorTexto);
	cout << ", ¿Qué mina quieres explorar?.\n"
		<< "Introduce un número entre 1 y " << NUM_TOTAL_MINAS << " para explorar una mina, y 0 para salir\n> ";
	cin >> res;
	while(res < 0 && res > NUM_TOTAL_MINAS) {
		cout << "> "; cin >> res;
	}
	cin.ignore(INT_MAX, '\n');
	return res;
}

/*void resuelveCaso() {   
    tJuego juego;
    cin >> juego.resolucion >> juego.dispositivoDeEntrada;
    switch(juego.dispositivoDeEntrada) {   
        case 1:
            jugar(juego, cin, cin);
            break;
        case 2:
            jugar(juego, cin, cin);
            break;
        default:
            jugar(juego, cin, cin);
            Log("Error al sacar el dispositivo de entrada\n");
    }
}*/

int main() {   
#ifdef _WIN32
    system("chcp 65001");
	/*
//Intenté que no hubieran demasiados #ifdef _WIN32 en la práctica, pero falló
#elif __linux__
    system("pause() { echo \"Presiona Enter para continuar\" ; read; }");
    system("alias cls=clear");
	*/
#endif
    tJuego juego;
    printTitleScreen();
    escogerResolucion(juego);
    if(juego.resolucion != 0) {
        escogerDispEntrada(juego);
		cin.ignore(INT_MAX, '\n');
        if (juego.dispositivoDeEntrada != 0) {
			// Se ha elegido la resolución del juego, y el dispositivo de entrada.
            int nivel = 0;
			tPuntuaciones marcador; inicializar_marcador(marcador);

			ifstream archivoMarcador; archivoMarcador.open(ARCHIVO_MARCADOR);
			if(archivoMarcador.is_open()) {
				cargar_marcador(archivoMarcador, marcador);
				archivoMarcador.close();
			}
			else {
				colorear(AMARILLO, "[WARN]:");
				Log(" No se encontró el archivo "+ARCHIVO_MARCADOR+" para cargar el marcador");
			}

			int pos = 0; // Posición donde se colocará el jugador
			string username = get_name_dialog(marcador, pos);
			nivel = get_level(username);

			tPuntuacionJugador & jugador = marcador.arrayClasificacion[pos];

            while (nivel != 0) {
				// Reseteamos los valores del juego
				juego.numMovimientosMinero = 0;
				juego.gemasRecogidas = 0;
				juego.dinamitasUsadas = 0;
                juego.estado = EXPLORANDO; // Si no hemos salido, es que estamos explorando aún

                ifstream archivoNivel, archivoEntrada;
				archivoNivel.open(to_string(nivel) + ".txt");

				if (archivoNivel.is_open()) {
					systemClear();
					if (juego.dispositivoDeEntrada == 1) {
						jugar(juego, archivoNivel, cin);
					}
					else {
						archivoEntrada.open(juego.archivoDeEntrada);
						jugar(juego, archivoNivel, archivoEntrada);
						archivoEntrada.close();
					}
					archivoNivel.close();

					// TODO: Mover esto a su propia función (juego, jugador, nivel)
					if(jugador.vMinasRecorridas[nivel-1].IdMina == -1){
						jugador.minasRecorridas++;
						jugador.vMinasRecorridas[nivel-1].IdMina = nivel;
					} 
					jugador.vMinasRecorridas[nivel-1].numMovimientos = juego.numMovimientosMinero;
					jugador.vMinasRecorridas[nivel-1].numDinamitas = juego.dinamitasUsadas;
					jugador.vMinasRecorridas[nivel-1].numGemas = juego.gemasRecogidas;
					// Esto se puede optmizar un poco
					jugador.vMinasRecorridas[nivel-1].puntosMina = max(calcPuntos(juego, jugador.vMinasRecorridas[nivel-1]), jugador.vMinasRecorridas[nivel-1].puntosMina);
					jugador.puntTotal = calcPuntos(juego, jugador);
				}
				else {
					Log("No se encontró el archivo del nivel " + to_string(nivel) + "\n");
				}

				if (juego.estado != EXPLORANDO) { 
					// Se imprime "Game Over" si se falla en una mina
					if (juego.estado == EXITO)
						printVictory();
					else
						printGameOver();
				}
				systemClear();
				mostrar_datos_usuario(marcador);
				nivel = get_level(username);
            }
			
			// Guardamos los datos del marcador a la salida
			ofstream archivoMarcadorGuardado; archivoMarcadorGuardado.open(ARCHIVO_MARCADOR);
			if(archivoMarcadorGuardado.is_open()) {
				guardar_marcador(archivoMarcadorGuardado, marcador);
				archivoMarcadorGuardado.close();
			}
			else {
				colorear(ROJO, "[ERROR]:");
				Log(" No se ha podido guardar el marcador\n");
				systemPause();
			}
			destruir(marcador);
			cout << "Gracias por jugar\n"; // Porque hay que ser educado
        }
	}

   return 0;
}
