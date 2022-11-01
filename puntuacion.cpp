// TODO: Arreglar busquedaBinaria()
#include <iostream>
#include <algorithm>
#include <iomanip> 
#include "puntuacion.h"
using namespace std;

void inicializar_pj(tPuntuacionJugador& pj) {
	// ^Inicializa `pj`
	pj.nombre = "";
	pj.puntTotal = 0;
	pj.minasRecorridas = 0;
	for(int i = 0; i < NUM_TOTAL_MINAS; i++) {
		tDatosMina & datos = pj.vMinasRecorridas[i];
		datos.IdMina = -1;
		datos.numMovimientos = 0;
		datos.numGemas = 0;
		datos.numDinamitas =0;
		datos.puntosMina = 0;
	}
}

void cargar_marcador(istream & entrada, tPuntuaciones & marcador) {
	// ^Carga los datos de `entrada` en `marcador`
	string nombreAux; entrada >> nombreAux;
	/// int i = 0;
	while(nombreAux != "000") {
		int pos; /// pos = i;
		if(!buscar(nombreAux, marcador, pos)){
			// Si no está nombreAux en marcador, inserta
			insertar(marcador, nombreAux, pos);
		}

		tPuntuacionJugador & jugador = marcador.arrayClasificacion[pos];
		entrada >> jugador.puntTotal;
		entrada >> jugador.minasRecorridas;
		for(int i = 0; i < jugador.minasRecorridas; i++) {
			tDatosMina & dato = jugador.vMinasRecorridas[i];
			entrada >> dato.IdMina >> dato.numMovimientos >> dato.numGemas;
			entrada >> dato.numDinamitas >> dato.puntosMina;
		}

		/// i++;
		entrada >> nombreAux; 
	}
}

void guardar_marcador(ostream & salida, tPuntuaciones & marcador) {
	// ^Guarda los datos de `marcador` en el fichero `salida`
	for(int i = 0; i < marcador.numJugadores; i++) {
		tPuntuacionJugador & jugador = marcador.arrayClasificacion[i];
		salida << jugador.nombre << '\n' << jugador.puntTotal << '\n' << jugador.minasRecorridas << '\n';
		for(int j = 0; j < NUM_TOTAL_MINAS; j++) {
			tDatosMina & dato = jugador.vMinasRecorridas[j];
			if(dato.IdMina != -1) {
				salida << dato.IdMina << " " << dato.numMovimientos << " ";
				salida << dato.numGemas << " " << dato.numDinamitas << " ";
				salida << dato.puntosMina << "\n";
			} 
		}
	}
	salida << "000\n";
}

void mostrar_minas_usuario(const tPuntuaciones & marcador, int pos, int tabSize) {
	// ^Muestra los datos de `marcador.arrayClasificacion[pos]`
	int tab = (tabSize? tabSize : 9);
	tPuntuacionJugador & jugador = marcador.arrayClasificacion[pos];
	
	// Cabecera inicial
	// TODO: Arreglar problema de optimiación
	cout << left << '|' << setw(tab) << jugador.nombre 
		<< left << '|' << setw(2*tab) << "Movimientos "
		<< left << '|' << setw(tab) << "Gemas "
		<< left << '|' << setw(2*tab) << "Dinamitas "
		<< left << '|' << setw(tab) << "Puntos "
		<< left << '|' << setw(2*tab) << "Puntos en total" << "\n";

	for(int i = 0; i < NUM_TOTAL_MINAS;i++) {
		tDatosMina & dato = jugador.vMinasRecorridas[i];
		if(dato.IdMina != -1) {
			cout << left << '|' << setw(tab) << "Mina"+to_string(dato.IdMina)
				<< left << '|' << setw(2*tab) << dato.numMovimientos 
				<< left << '|' << setw(tab) << dato.numGemas 
				<< left << '|' << setw(2*tab) << dato.numDinamitas 
				<< left << '|' << setw(tab) << dato.puntosMina;
			if(i == 0) cout << left << '|' << setw(2*tab) << jugador.puntTotal << '\n';
			else cout << '\n';
		}
	}
}

void mostrar_puntuaciones_alfabetico(const tPuntuaciones & marcador) {
	// ^Muestra las puntuaciones de todos los usuarios (orden αβ)
	string msg = "----------- LISTA DE JUGADORES -----------\n";
	colorear(NEGRO, msg, AZUL);
	int tab = msg.size() * 2 / 5;
	for(int i = 0 ; i < marcador.numJugadores; i++) {
		tPuntuacionJugador & jugador = marcador.arrayClasificacion[i];
		cout <<  setw(tab) << " ";
		colorear(NEGRO, jugador.nombre, ROJO); cout << " " << jugador.puntTotal << '\n';
	}
}

void mostrar_datos_usuario(const tPuntuaciones & marcador) {
	// ^Muestra todos los datos de todos los usuarios (orden αβ)
	string msg = "----- JUGADORES ORDENADOS POR NOMBRE -----\n";
	colorear(NEGRO, msg, AZUL);
	for(int i = 0; i < marcador.numJugadores; i++) {
		mostrar_minas_usuario(marcador, i);
		cout << '\n';
	}
}

void inicializar_marcador(tPuntuaciones & marcador) {
	// ^Inicializa el array dinámico
	marcador.numJugadores = 0; 
	marcador.capacidad = 2; 
	marcador.arrayClasificacion = new tPuntuacionJugador[marcador.capacidad];
}

void aumentar_capacidad(tPuntuaciones & marcador) {
	// ^Duplica el tamaño del array
	marcador.capacidad *= 2;
	tPuntuacionJugador * nuevoMarcador = new tPuntuacionJugador[marcador.capacidad];

	for(int i = 0; i < marcador.numJugadores; i++)
		nuevoMarcador[i] = marcador.arrayClasificacion[i];

	marcador.arrayClasificacion = nuevoMarcador;
}
void destruir(tPuntuaciones & marcador) {
	// ^Libera la memoria dinámica de marcador
	int capacidad = 0, numJugadores = 0;
	delete[] marcador.arrayClasificacion;
}

bool binarySearch(const string& nombre, const tPuntuaciones & marcador, int & pos, int ini, int fin) {
	// Devuelve la posición donde estaría `nombre` en el array
	bool res = false;
	if(ini <= fin) {
		int mid = ini + (fin-ini)/2;
		string & nombreMid = marcador.arrayClasificacion[mid].nombre;
		if(nombreMid == nombre){
			pos = mid;
			res = true;
		} 
		else if(nombreMid < nombre)
			res = binarySearch(nombre, marcador, pos, mid+1, fin);
		else if(nombreMid > nombre) {
			pos = mid; // Si debe estar a la izquierda, podría estar en mid (Mejor que lo anterior)
			res = binarySearch(nombre, marcador, pos, ini, mid-1);
		}
	}
	return res;
}

bool buscar(const string& nombre, const tPuntuaciones& marcador, int& pos) {
	// ^Busqueda binaria de`nombre` en `marcador`. `pos` es la posición donde iría
	pos = marcador.numJugadores;
	/* Suponemos inicialmente que está al final, pues si no está y nunca es menor que un elemento
	 * del marcador, allí es donde deberá acabar
	 */
	return binarySearch(nombre, marcador, pos, 0, marcador.numJugadores-1);
}

void insertar(tPuntuaciones& marcador, string const& nombre, int pos) {
	// ^Inserta ordenadamente en la posición pos. Si no hay espacio, se amplía
	marcador.numJugadores++;
	if(marcador.numJugadores >= marcador.capacidad) {
		aumentar_capacidad(marcador);
	}

	// -2 pues empezamos por el ahora penúltimo
	for(int i = marcador.numJugadores-2; i >= pos; i--) 
		marcador.arrayClasificacion[i+1] = marcador.arrayClasificacion[i];

	inicializar_pj(marcador.arrayClasificacion[pos]);
	marcador.arrayClasificacion[pos].nombre = nombre;
}

int calcPuntos(tJuego & juego, tDatosMina & mina) {
	// ^ Calcula los puntos de una mina
	return juego.mina.plano.size() + juego.mina.plano[0].size() + mina.numGemas * PTOS_GEMAS  - mina.numDinamitas * PTOS_TNT - mina.numMovimientos * PTOS_MOVS;
}

int calcPuntos(tJuego & juego, tPuntuacionJugador & jugador) {
	// ^ Calcula los puntos totales de un jugador
	int res = 0;
	for(int i = 0; i < NUM_TOTAL_MINAS; i++) {
		if(jugador.vMinasRecorridas[i].IdMina != -1) 
			res += jugador.vMinasRecorridas[i].puntosMina;
	}
	return res;
}
