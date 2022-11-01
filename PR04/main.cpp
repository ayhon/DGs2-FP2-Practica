#include <iostream>
#include "juego.h"
using namespace std;

void resuelveCaso() {   
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
            cout << "Error al sacar el dispositivo de entrada\n";
    }
}

int main() {   
    int numMinas; cin >> numMinas;
    for (int i = 0; i < numMinas; i++)
    {
        resuelveCaso();
    }
    
}