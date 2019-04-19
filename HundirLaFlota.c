#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NUMBARCOS1 4
#define NUMBARCOS2 2
#define NUMBARCOS3 1

void cabecera();
int menu();
void hundirLaFlota(int opcion, int f,  int c);
void colocarBarcosManualmente(char *t, int f, int c);
void colocarBarcosAutomaticamente(char *t, int f, int c);
void inicializarTablero(char *t, int f, int c);
void imprimirTablero(char *t, int f, int c);
void imprimirTableroArchivo(char *t, int f, int c, FILE *pa);
int comprobacionEspacioParaBarco(char *t, int f, int c, int iniFila, int iniCol, int tamBarco, int orientacion);
int compruebaGanador (int *b, int f, int c);
int compruebaDisparo(char *t, int f, int c, int posFila, int posCol);
void juegoManual(int f, int c);
void juegoAutomatico(int f, int c);

int main(int argc, char *argv[]){
//Función principal. 
//	- comprueba que el número de argumentos del main es correcto. 
//		--Si no son correctos, mensaje de error y fin del programa
//		--Si son correctos:
//				--- lee las filas y las columnas del tablero
//				--- mientras la opción leída no sea 3, juega al hundir la flota 
	
		if (argc != 3){
			printf("Los argumentos dados son insuficientes\n");
			exit(-1);
		}
		int opcion;	
		int filas = atoi(argv[1]);
		int columnas = atoi(argv[2]);

		if (filas < 5 || columnas < 5){
			printf("El tablero necesita ser de 5x5 minimo\n");
			exit(-1);
		}



		cabecera();

	do{
		opcion = menu();
		hundirLaFlota(opcion, filas, columnas);
	} while (opcion!=3);

	return 0;
}

void cabecera(){

		printf("\t\t\t      ╔╦═╦╦╦═════════╦╦╦╦══════╗\n");
		printf("\t\t\t      ║║ ╩╬╬═╦═╦╦╦═╦═╬╬╝╠═╗╔═╦╗║\n");
		printf("\t\t\t      ║║  ║║╩╣║║║║╩╣║║║║║║║╠╝╠╣║\n");
		printf("\t\t\t      ║╚══╩╩═╩╩╩═╩═╩╩╩╩═╩═╝╚═╩╝║\n");
		printf("\t\t\t      ╚════════════════════════╝\n");
		printf("\t\t\t   ╔╦╗╔╦═════╦╦╦══╦╦══╦═╦╦══╦╦═══╗\n");
		printf("\t\t\t   ║║╚╝╠╗╔╦═╦╝╠╬═╗║╠═╗║═╣╠═╦╣╠╦═╗║\n");
		printf("\t\t\t   ║║╔╗║╚╝║║║║║║╠╝║╠╝║║╔╣║║╠╗╔╬╝║║\n");
		printf("\t\t\t   ║╚╝╚╩══╩╩╩═╩╩╝.╚╩═╝╚╝╚╩═╝╚═╩═╝║\n");
		printf("\t\t\t   ╚═════════════════════════════╝\n");
		for (int i = 0; i < 4; ++i){
			printf("\n");
		}
}

int menu(){
//Función menu.
//	- INPUTS: nada
//	- OUTPUTS: la opción leída (1, 2 ó 3)
//  - Presenta el menú por pantalla y lee una opción. Si no es 1, 2 ó 3 da un mensaje de error y vuelve a leerla hasta que sea correcta.
	int opcion;

	do
	{
		printf("\t\t\t\t¿Que quieres hacer?\n\n");
		printf("\t//////////////////////////////////////////////////////////////////\n");
		printf("\t//\t\t\t[1] Juego manual\t\t\t//\n");
		printf("\t//\t\t\t[2] Juego automatico\t\t\t//\n");
		printf("\t//\t\t\t[3] Salir\t\t\t\t//\n");
		printf("\t//////////////////////////////////////////////////////////////////\n\n");
		scanf("%d", &opcion);

		if (opcion < 1 || opcion > 3)
		{
			printf("\n\nPorfavor, introduzca una de las opciones dadas\n\n");
		}

	} while (opcion < 1 && opcion > 3);


	return opcion;

}

void hundirLaFlota(int opcion, int f,  int c){
//Función hundirLaFlota
//	- INPUTS: opción (1, 2 ó 3), filas y columnas del tablero
//  - OUTPUTS: nada
//  - Según la opción leída llama a jugar manual (opción 1), jugar automático (opción 2) o muestra un mensaje de fin (opción 3)

	switch (opcion){

		case 1:
			juegoManual(f,c);
			break;

		case 2:
			juegoAutomatico(f,c);
			break;

		case 3:
			printf("Gracias por jugar!\n");
			break;
	}


}	
void juegoManual(int f, int c){
//Función juegoManual
// 	- INPUTS: número de filas y número de columnas del tablerro
//	- OUTPUTS: nada
//	- Juega de forma manual:
//		-- Crea los 4 tableros, los inicializa (llamando a la función inicializar tantas veces como tableros haya) y en 2 pone los barcos de forma manual o automática, según indique el usuario
//		-- Escribe por pantalla los tableros y las tiradas al inicio del juego
//		-- Por turnos, cada jugador elige una fila y columna de 1 al número de filas y de 1 al número de columnas. Se comprueba si hay barco indicando agua o tocado (se indica por pantalla).
//		-- Se comprueba si hay ganador. Si lo hay acaba la partida indicando quíén ha ganado por pantalla
//		-- EXTRA: comprobar que no se ha repetido ese disparo
	int opcion;

	char* tableroOcultoA;
	tableroOcultoA = (char*)malloc(sizeof(char)*f*c);

	char* tableroMostradoA;
	tableroMostradoA = (char*)malloc(sizeof(char)*f*c);

	char* tableroOcultoB;
	tableroOcultoB = (char*)malloc(sizeof(char)*f*c);

	char* tableroMostradoB;
	tableroMostradoB = (char*)malloc(sizeof(char)*f*c);

	do{
		printf("\t\t\t   ¿Como quieres colocar los barcos?\n\n");
		printf("\t//////////////////////////////////////////////////////////////////\n");
		printf("\t//\t\t\t[1] De forma manual\t\t\t//\n");
		printf("\t//\t\t\t[2] De forma automatica\t\t\t//\n");
		printf("\t//////////////////////////////////////////////////////////////////\n\n");
	
		scanf("%d", &opcion);

		if (opcion < 1 || opcion > 2){
			printf("\n\nPorfavor, introduzca una de las opciones dadas\n\n");
		}

	} while (opcion < 1 && opcion > 2);

	inicializarTablero(tableroOcultoA, f, c);
	inicializarTablero(tableroOcultoB, f, c);
	inicializarTablero(tableroMostradoA, f, c);
	inicializarTablero(tableroMostradoB, f, c);

	imprimirTablero(tableroOcultoA, f, c);

	if (opcion == 1){
		colocarBarcosManualmente(tableroOcultoA, f, c);
		colocarBarcosAutomaticamente(tableroOcultoB, f, c);
	}else if (opcion == 2){
		colocarBarcosAutomaticamente(tableroOcultoA, f, c);
		colocarBarcosAutomaticamente(tableroOcultoB, f, c);
	}

	free(tableroMostradoA);
	free(tableroMostradoB);
	free(tableroOcultoA);
	free(tableroOcultoB);

}
void juegoAutomatico(int f, int c){
//Función juegoAutomatico
// 	- INPUTS: número de filas y número de columnas del tablerro
//	- OUTPUTS: nada
//	- Juega de forma automática:
//		-- Abre el archivo para escribir en él todas las salidas
//		-- Crea los 4 tableros, los inicializa (llamando a la función inicializar tantas veces como tableros haya) y en 2 pone los barcos automáticamente
//		-- Escribe en el archivo los tableros y las tiradas al inicio del juego
//		-- Por turnos, cada jugador genera una fila y columna automáticamente (se indica en el archivo). Se comprueba si hay barco indicando agua o tocado (se indica en el archivo).
//		-- Se comprueba si hay ganador. Si lo hay acaba la partida indicando quíén ha ganado en el archivo
//		-- EXTRA: comprobar que no se ha repetido ese disparo


	return;
}


void inicializarTablero(char *t, int f, int c){
//Función inicializarTablero
//INPUTS:
//	- puntero a un tablero (barcos, tiradas o lo que sea)
//	- filas
//	- columnas
//OUTPUS: nada
//Inicializa a 0 la tabla

	for (int i = 0; i < f; ++i)
		{
			for (int j = 0; j < c; ++j)
			{
				*(t+i*c+j) = '*';
			}
		}	

}

void imprimirTablero(char *t, int f, int c){
//Funcion imprimirTablero
//INPUTS:
//	- puntero a un tablero (barcos, tiradas o lo que sea)
//	- filas
//	- columnas
//OUTPUTS: nada
//Muestra por pantalla el tabler dado

	//Imprime la cabezera de las columnas
	printf("\n\n");
	for (int i = 0; i < 8; ++i){
		printf(" ");
	}
	for (int i = 0; i < (c*8)-6; ++i){
		printf("=");
	}
	printf("\n");

	for (int i = 0; i < 8; ++i){
		printf(" ");
	}

	printf("|1\t");

	for (int i = 1; i < c-1; ++i){
		printf("%d\t", i+1);
	}

	printf("%d|\n", c);

	for (int i = 0; i < 8; ++i){
		printf(" ");
	}
	for (int i = 0; i < (c*8)-6; ++i){
		printf("=");
	}
	printf("\n");

	//Imprime el tablero
	for (int i = 0; i < f; ++i){

		printf("\t|%c", *(t+i*c));
		for (int j = 1; j < c; ++j){

			printf("\t%c", *(t+i*c+j));
		}
		if (f < 10){
		printf(" |%d|\n", i+1);
		}else if(f > 10){
		
			if (i+1 < 10){
				printf(" | %d|\n", i+1);
			}else{
				printf(" |%d|\n", i+1);
			}
		}

		if (i < f-1 && f <10){
			printf("\t|");
			for (int k = 0; k < c-1; ++k){
				printf("\t");
			}
			printf("  | |\n");
		}else if (i < f-1 && f > 10){
			printf("\t|");
			for (int k = 0; k < c-1; ++k){
				printf("\t");
			}
			printf("  |  |\n");
		}		
	}

	//Imprime el cierre del marco
	for (int i = 0; i < 8; ++i){
		printf(" ");
	}
	for (int i = 0; i < (c*8)-6; ++i){
		printf("=");
	}
	printf("\n\n");

}

void imprimirTableroArchivo(char *t, int f, int c, FILE *pa){
//Función imprimirTableroArchivo
//INPUTS:
//	- puntero a un tablero (barcos, tiradas o lo que sea)
//	- filas
//	- columnas
// 	- puntero a archivo
//OUTPUTs: nada
//Imprime en el archivo la matriz pasada	
	
	return;
}

int compruebaGanador (int *b, int f, int c){
//Función compruebaGanador
//INPUTs:
//	- puntero a un tablero de barcos
//	- filas
//	- columnas
//OUTPUTS: nada
//Busca si quedan barcos por hundir. Si no, devuelve 0. 
	
}

int compruebaDisparo(char *t, int f, int c, int posFila, int posCol){
//Función compruebaDisparo
//INPUTS:
//	- tablero de barcos
//	- filas
//	- columnas
//	- tiro (fila y columna)
//OUTPUTS: 0 si agua, 1 si hay un barco de 1, 2 si hay un barco de 2, 3 si hay un barco de 3
	
}

int comprobacionEspacioParaBarco(char *t, int f, int c, int iniFila, int iniCol, int tamBarco, int orientacion){
//Función comprobacionEspacioParaBarco
//INPUTS:
//	- tablero de barcos
//	- filas
//	- columnas
//	- posición (fila)
//	- posición (columna)
//	- tamaño del barco (1, 2 ó 3)
//	- orientación (0 horizontal, 1 vertical)
//OUTPUTS: 1 si cabe, 0 si no cabe
//Comprueba si cabe un barco a partir de la posición indicada en la orientación indicada. Devuelve 0 si no cabe, 1 si sí.
	
	int salida = 0;

	switch(tamBarco){

		case 1:
			if ((iniFila <= f && iniFila >= 0 && iniCol <= c && iniCol >= 0) && *(t+c*iniFila+iniCol) == '*'){
				salida = 1;
			}
			break;
		case 2:
			if ((iniFila < f && iniFila >= 0 && iniCol <= c && iniCol > 0) && *(t+c*iniFila+iniCol) == '*' && *(t+c*iniFila+iniCol+1) == '*' && orientacion == 0){
				salida = 1;
			}else if((iniFila < f && iniFila >= 0 && iniCol <= c && iniCol > 0) && *(t+c*iniFila+iniCol) == '*' && *(t+c*(iniFila-1)+iniCol) == '*' && orientacion == 1){
				salida = 1;
			}
			break;
		case 3:
			if ((iniFila < f && iniFila >= 0 && iniCol <= c && iniCol > 0) && *(t+c*iniFila+iniCol) == '*' && *(t+c*iniFila+iniCol+1) == '*' && *(t+c*iniFila+iniCol-1) == '*' && orientacion == 0){
				salida = 1;
			}else if((iniFila < f && iniFila >= 0 && iniCol <= c && iniCol > 0) && *(t+c*iniFila+iniCol) == '*' && *(t+c*(iniFila-1)+iniCol) == '*' && *(t+c*(iniFila+1)+iniCol) == '*' && orientacion == 1){
				salida = 1;
			}
			break;
	}

	return salida;
}

void colocarBarcosAutomaticamente(char *t, int f, int c){
//Función colocarBarcosAutomaticamente
//INPUTS:
//	- Tablero de barcos
//	- filas
//	- columnas	
//OUTPUTS: nada
//Coloca de forma manual 4 barcos de 1 posición, 2 de 2 posiciones y 1 de 3 posiciones en el tablero
	
}


void colocarBarcosManualmente(char *t, int f, int c){
//Función colocarBarcosManualmente
//INPUTS:
//	- Tablero de barcos
//	- filas
//	- columnas	
//OUTPUTS: nada
//Coloca de forma manual 4 barcos de 1 posición, 2 de 2 posiciones y 1 de 3 posiciones en el tablero

	int barcos1 = NUMBARCOS1;
	int barcos2 = NUMBARCOS2;
	int barcos3 = NUMBARCOS3;
	int filaColocar;
	int columnaColocar;
	int barcoColocar;
	int orientacion;

	do{
		printf("\t\t\t   ¿Que tipo de barco quieres colocar?\n\n");
		printf("\t//////////////////////////////////////////////////////////////////\n");
		printf("\t//\t\t[1] Barco de 1 posiciones. Restantes:[%d]\t//\n", barcos1);
		printf("\t//\t\t[2] Barco de 2 posiciones. Restantes:[%d]\t//\n", barcos2);
		printf("\t//\t\t[3] Barco de 3 posiciones. Restantes:[%d]\t//\n", barcos3);
		printf("\t//////////////////////////////////////////////////////////////////\n\n");
		scanf("%d", &barcoColocar);

		switch (barcoColocar){

			case 1:
				if (barcos1 > 0){
					printf("¿En que fila quieres colocar e barco?\n");
					scanf("%d", &filaColocar);
					filaColocar--;
					printf("¿En que columna quieres colocar el barco?\n");
					scanf("%d", &columnaColocar);
					columnaColocar--;
					if (comprobacionEspacioParaBarco(t, f,c ,filaColocar, columnaColocar, 1, orientacion) == 1){
						*(t+c*filaColocar+columnaColocar) = '1';
						imprimirTablero(t, f, c);
						barcos1--;
					}else{
						printf("El barco no cabe en esa posicion\n");
					}
				}else{
					printf("No hay mas barcos de ese ese tipo disponibles\n");
				}
				break;
			case 2:
				if (barcos2 > 0){
					printf("¿En que fila quieres colocar e barco?\n");
					scanf("%d", &filaColocar);
					filaColocar--;
					printf("¿En que columna quieres colocar el barco?\n");
					scanf("%d", &columnaColocar);
					columnaColocar--; 
					printf("¿Como quieres orientarlos?\n[0] Horizontal\n[1] Vertical");
					scanf("%d", &orientacion);
					if (comprobacionEspacioParaBarco(t, f,c ,filaColocar, columnaColocar, 2, orientacion) == 1){
						if (orientacion == 0){
							*(t+c*filaColocar+columnaColocar) = '2';
							*(t+c*filaColocar+columnaColocar+1) = '2';
						}else if (orientacion == 1){
							*(t+c*(filaColocar-1)+columnaColocar) = '2';
							*(t+c*(filaColocar)+columnaColocar) = '2';	
						}
						imprimirTablero(t, f, c);
						barcos2--;
					}else{
						printf("El barco no cabe en esa posicion\n");
					}
				}else{
					printf("No hay mas barcos de ese ese tipo disponibles\n");
				}	
					break;
			case 3:
				if (barcos3 > 0){
					printf("¿En que fila quieres colocar e barco?\n");
					scanf("%d", &filaColocar);
					filaColocar--;
					printf("¿En que columna quieres colocar el barco?\n");
					scanf("%d", &columnaColocar);
					columnaColocar-- ;
					printf("¿Como quieres orientarlos?\n");
					scanf("%d", &orientacion);
					if (comprobacionEspacioParaBarco(t, f,c ,filaColocar, columnaColocar, 3, orientacion) == 1){
						if (orientacion == 0){
							*(t+c*filaColocar+columnaColocar+1) = '3';
							*(t+c*filaColocar+columnaColocar-1) = '3';
							*(t+c*filaColocar+columnaColocar) = '3';
						}else if (orientacion == 1){
							*(t+c*(filaColocar+1)+columnaColocar) = '3';
							*(t+c*(filaColocar+1)+columnaColocar) = '3';
							*(t+c*(filaColocar)+columnaColocar) = '3';
						}
						imprimirTablero(t, f, c);
						barcos3--;
					}else{
						printf("El barco no cabe en esa posicion\n");
					}
				}else{
					printf("No hay mas barcos de ese ese tipo disponibles\n");
				}
				break;

			default:
				printf("Porfavor, introduzca una de las opciones dadas\n");
		}

	} while (barcos1 != 0 || barcos2 != 0 || barcos3 != 0);

 }