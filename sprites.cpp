#include <iostream>
#include <string>
#include "sprites.h"
using namespace std;

#ifdef __WIN32
// Abstracción del método extraño de windows para cambiar la posción del cursor
// Proporcionado por Cai (Uso Linux de todas formas...)
void setCursorPosition(int x, int y) {
	/* Configurar la posición del cursor en la consola (sirve para evitar parpadeo de pantalla por cls)
	Source: https://stackoverflow.com/questions/34842526/update-console-without-flickering-c */

	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}
#endif

void systemClear(bool partial) {
	//^Clear para Windows, Linux y MacOS. Acepta limpieza parcial o total
#ifdef _WIN32
	if(partial) {
		setCursorPosition(0, 0);
		cout.flush();
	}
	else system("cls");
#else
	if(partial) {
		// Ver `man console_codes`
		cout << "\e[0;0H" << endl;
	}
	else system("clear");
#endif
}

void systemPause() {
	//^Pausa dependiendo del OS
#ifdef _WIN32
	system("pause");
#elif __linux__
	cout << "Presione una tecla para continuar . . .\n";
	system("read");
#elif __APPLE__
	cout << "Presione una tecla para continuar . . .\n";
	system("read");
#endif
}

tColor theme(char opt, tElemento elem) {
	//^Devuelve el color de texto o fondo (según opt), segun el tema elegido
	tColor res;

	switch(ThemeName) {
		case THEME_DEFAULT:
			switch(elem) {
				case TIERRA:
					res = tColor(opt=='t'? AMARILLO : GRIS); 
					break;
				case PIEDRA:
					res = tColor(opt=='t'? GRIS : NEGRO);
					break;
				case MURO:
					res = tColor(opt=='t'? GRIS : GRIS);
					break;
				case GEMA:
					res = tColor(opt=='t'? VERDE : GRIS);
					break;
				case SALIDA:
					res = tColor(opt=='t'? MORADO : NEGRO);
					break;
				case MINERO:
					res = tColor(opt=='t'? AZUL : NEGRO);
					break;
				case LIBRE:
					res = tColor(opt=='t'? NEGRO : NEGRO);
					break;
				case DINAMITA:
					res = tColor(opt=='t'? ROJO : NEGRO); 
					break;
				default:
					res = tColor(opt=='t'? MORADO : AMARILLO);
			}
			break;
		case THEME_BnW:
		default:
			res = tColor(opt=='t'? BLANCO : NEGRO);
	}

	return res;
}

void printTitleScreen(tColor colorFondo, tColor colorTexto) {
	//^Pues eso
	systemClear();
    colorear(colorFondo, "                                                                                                 \n", colorTexto);
    colorear(colorFondo, " ███╗   ███╗ █████╗ ███████╗████████╗███████╗██████╗     ███╗   ███╗██╗███╗   ██╗███████╗██████╗ \n", colorTexto);
	colorear(colorFondo, " ████╗ ████║██╔══██╗██╔════╝╚══██╔══╝██╔════╝██╔══██╗    ████╗ ████║██║████╗  ██║██╔════╝██╔══██╗\n", colorTexto);
	colorear(colorFondo, " ██╔████╔██║███████║███████╗   ██║   █████╗  ██████╔╝    ██╔████╔██║██║██╔██╗ ██║█████╗  ██████╔╝\n", colorTexto);
	colorear(colorFondo, " ██║╚██╔╝██║██╔══██║╚════██║   ██║   ██╔══╝  ██╔══██╗    ██║╚██╔╝██║██║██║╚██╗██║██╔══╝  ██╔══██╗\n", colorTexto);
	colorear(colorFondo, " ██║ ╚═╝ ██║██║  ██║███████║   ██║   ███████╗██║  ██║    ██║ ╚═╝ ██║██║██║ ╚████║███████╗██║  ██║\n", colorTexto);
	colorear(colorFondo, " ╚═╝     ╚═╝╚═╝  ╚═╝╚══════╝   ╚═╝   ╚══════╝╚═╝  ╚═╝    ╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚══════╝╚═╝  ╚═╝\n", colorTexto);
}

void printGameOver(tColor colorFondo, tColor colorTexto) {
	//^Pues eso
	systemClear();
    colorear(colorFondo, "                                     \n", colorTexto);
	colorear(colorFondo, "   ▄████  ▄▄▄       ███▄ ▄███▓▓█████ \n", colorTexto);
	colorear(colorFondo, "  ██▒ ▀█▒▒████▄    ▓██▒▀█▀ ██▒▓█   ▀ \n", colorTexto);
	colorear(colorFondo, " ▒██░▄▄▄░▒██  ▀█▄  ▓██    ▓██░▒███   \n", colorTexto);
	colorear(colorFondo, " ░▓█  ██▓░██▄▄▄▄██ ▒██    ▒██ ▒▓█  ▄ \n", colorTexto);
	colorear(colorFondo, " ░▒▓███▀▒ ▓█   ▓██▒▒██▒   ░██▒░▒████▒\n", colorTexto);
	colorear(colorFondo, "  ░▒   ▒  ▒▒   ▓▒█░░ ▒░   ░  ░░░ ▒░ ░\n", colorTexto);
	colorear(colorFondo, "   ░   ░   ▒   ▒▒ ░░  ░      ░ ░ ░  ░\n", colorTexto);
	colorear(colorFondo, " ░ ░   ░   ░   ▒   ░      ░      ░   \n", colorTexto);
	colorear(colorFondo, "       ░       ░  ░       ░      ░  ░\n", colorTexto);
	colorear(colorFondo, "                                     \n", colorTexto);
	colorear(colorFondo, "  ▒█████   ██▒   █▓▓█████  ██▀███    \n", colorTexto);
	colorear(colorFondo, " ▒██▒  ██▒▓██░   █▒▓█   ▀ ▓██ ▒ ██▒  \n", colorTexto);
	colorear(colorFondo, " ▒██░  ██▒ ▓██  █▒░▒███   ▓██ ░▄█ ▒  \n", colorTexto);
	colorear(colorFondo, " ▒██   ██░  ▒██ █░░▒▓█  ▄ ▒██▀▀█▄    \n", colorTexto);
	colorear(colorFondo, " ░ ████▓▒░   ▒▀█░  ░▒████▒░██▓ ▒██▒  \n", colorTexto);
	colorear(colorFondo, " ░ ▒░▒░▒░    ░ ▐░  ░░ ▒░ ░░ ▒▓ ░▒▓░  \n", colorTexto);
	colorear(colorFondo, "   ░ ▒ ▒░    ░ ░░   ░ ░  ░  ░▒ ░ ▒░  \n", colorTexto);
	colorear(colorFondo, " ░ ░ ░ ▒       ░░     ░     ░░   ░   \n", colorTexto);
	colorear(colorFondo, "     ░ ░        ░     ░  ░   ░       \n", colorTexto);
	colorear(colorFondo, "               ░                     \n", colorTexto);
	systemPause();
}

void printVictory(tColor colorFondo, tColor colorTexto) {
	//^Pues eso
	systemClear();
	colorear(NEGRO, "                                                                                 \n", colorTexto);
	colorear(NEGRO, "  ▄█    █▄   ▄█   ▄████████     ███      ▄██████▄     ▄████████  ▄█     ▄████████\n", colorTexto);
	colorear(NEGRO, " ███    ███ ███  ███    ███ ▀█████████▄ ███    ███   ███    ███ ███    ███    ███\n", colorTexto);
	colorear(NEGRO, " ███    ███ ███▌ ███    █▀     ▀███▀▀██ ███    ███   ███    ███ ███▌   ███    ███\n", colorTexto);
	colorear(NEGRO, " ███    ███ ███▌ ███            ███   ▀ ███    ███  ▄███▄▄▄▄██▀ ███▌   ███    ███\n", colorTexto);
	colorear(NEGRO, " ███    ███ ███▌ ███            ███     ███    ███ ▀▀███▀▀▀▀▀   ███▌ ▀███████████\n", colorTexto);
	colorear(NEGRO, " ███    ███ ███  ███    █▄      ███     ███    ███ ▀███████████ ███    ███    ███\n", colorTexto);
	colorear(NEGRO, " ███    ███ ███  ███    ███     ███     ███    ███   ███    ███ ███    ███    ███\n", colorTexto);
	colorear(NEGRO, "  ▀██████▀  █▀   ████████▀     ▄████▀    ▀██████▀    ███    ███ █▀     ███    █▀ \n", colorTexto);
	colorear(NEGRO, "                                                     ███    ███                  \n", colorTexto);
	systemPause();
}

void sprite(tElemento elem, int section, tColor colorFondo, tColor colorTexto) {
	//^Imprime el sprite de 3x3 del elemento dado, aunque solo el 1x3 especificado por section
    switch(elem) {
        case MINERO:
            if(section == 0) {
				colorear(colorFondo, "┏━┓", colorTexto);
            }
            else if (section == 1) {
				colorear(colorFondo, "┃w┃", colorTexto);
            }
            else if (section == 2) {
				colorear(colorFondo, "┣━┫", colorTexto);
            }
			break;
		case DINAMITA:
            if(section == 0) {
				colorear(colorFondo, "╔═╗", colorTexto);
            }
            else if (section == 1) {
				colorear(colorFondo, "TNT", colorTexto);
			}
            else if (section == 2) {
				colorear(colorFondo, "╚═╝", colorTexto);
            }
			break;
		case GEMA:
            if(section == 0) {
				colorear(colorFondo, "╭─╮", colorTexto);
            }
            else if (section == 1) {
				colorear(colorFondo, "│€│", colorTexto);
			}
            else if (section == 2) {
				colorear(colorFondo, "╰─╯", colorTexto);
            }
			break;
		case SALIDA:
            if(section == 0) {
				colorear(colorFondo, "┎─┐", colorTexto);
            }
            else if (section == 1) {
				colorear(colorFondo, "┠─┘", colorTexto);
			}
            else if (section == 2) {
				colorear(colorFondo, "┃  ", colorTexto);
            }
			break;
		default:
			string msg = "";
			for (size_t i = 0; i < 3; i++) {
				msg += to_string(elem);
			}
			colorear(colorFondo, msg, colorTexto);
    }
}

void colorear(tColor colorFondo, string msg, tColor colorTexto) {
	//^Imprime msg por consola con los colores de fondo y texto especificados
    #ifdef _WIN32
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handle, int(colorTexto) | (int(colorFondo) << 4));
        cout << msg;
        SetConsoleTextAttribute(handle, 15 | (0 << 4));
    #elif __linux__
        cout << "\e[1;" << ctr[int(colorTexto)] << ";" << ctr[int(colorFondo)] + 10 << "m" + msg + "\e[m";
	#elif __APPLE__
        cout << "\e[1;" << ctr[int(colorTexto)] << ";" << ctr[int(colorFondo)] + 10 << "m" + msg + "\e[m";
    #endif
}
