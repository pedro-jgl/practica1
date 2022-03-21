#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;



Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;

	cout << "Posicion: fila " << sensores.posF << " columna " << sensores.posC << " ";
	switch(sensores.sentido){
		case 0: cout << "Norte" << endl; break;
		case 1: cout << "Este" << endl; break;
		case 2: cout << "Sur " << endl; break;
		case 3: cout << "Oeste" << endl; break;
	}
	cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];
	cout << endl;

	cout << "Superficie: ";
	for (int i=0; i<sensores.superficie.size(); i++)
		cout << sensores.superficie[i];
	cout << endl;

	cout << "Colisión: " << sensores.colision << endl;
	cout << "Reset: " << sensores.reset << endl;
	cout << "Vida: " << sensores.vida << endl;
	cout << endl;


	//Actualizar variables de estado
	if (sensores.nivel < 2)
		brujula = sensores.sentido;
	
	switch(ultimaAccion){
		case actFORWARD:
			switch (brujula){
				case 0:
					fil--;
				break;
				case 1:
					col++;
				break;
				case 2:
					fil++;
				break;
				case 3:
					col--;
				break;
			}
		break;
		case actTURN_L:
			if (sensores.nivel>=2)
				brujula = (brujula + 3) % 4;	//Sumar 3 es lo mismo que restar 1 en módulo 4
		break;
		case actTURN_R:
			if (sensores.nivel>=2)
				brujula = (brujula + 1) % 4;
		break;
	}

	if (sensores.terreno[0] == 'G' and !bien_situado){
		bien_situado = true;
		punteroMapa = &mapaResultado;
		desfase_x = sensores.posF - fil;
		desfase_y = sensores.posC - col;
		fil = sensores.posF;
		col = sensores.posC;

		for (int i = 0; i < tamMapa; i++)
			for (int j = 0; j < tamMapa; j++)
				mapaResultado[i][j] = mapaCiego[i-desfase_x][j-desfase_y];
	}
	
	
	switch(brujula){
		case 0:
			for (int i = 0; i < 4; i++)
				for (int j = -i; j <= i; j++)
					(*punteroMapa)[fil-i][col+j] = sensores.terreno[i*(i+1)+j];
		break;
		case 1:
			for (int j = 0; j < 4; j++)
				for (int i = -j; i <= j; i++)
					(*punteroMapa)[fil+i][col+j] = sensores.terreno[j*(j+1)+i];
		break;
		case 2:
			for (int i = 0; i < 4; i++)
				for (int j = i; j >= -i; j--)
					(*punteroMapa)[fil+i][col+j] = sensores.terreno[i*(i+1)-j];
		break;
		case 3:
			for (int j = 0; j < 4; j++)
				for (int i = j; i >= -j; i--)
					(*punteroMapa)[fil+i][col-j] = sensores.terreno[j*(j+1)-i];
		break;
	}
	


	//Decidir qué acción tomar de manera aleatoria
	girar_derecha = (rand()%2==0);
	girar = rand() % 5;
	entrar = rand() % 10;

	if (sensores.terreno[0] == 'K')
		bikini = true;
	if (sensores.terreno[0] == 'D')
		zapatillas = true;

	
	if ( sensores.terreno[2] != 'M' and sensores.terreno[2] != 'P' and sensores.superficie[2] == '_'){
		if (bikini and zapatillas){
			if (girar < 4)
				accion = actFORWARD;
			else{
				if (girar_derecha)
					accion = actTURN_R;
				else
					accion = actTURN_L;
			}

		}
		else if (zapatillas){
			if ( sensores.terreno[2] == 'A'){
				if (girar_derecha)
					accion = actTURN_R;
				else
					accion = actTURN_L;
			}
			else{
				if (girar < 4)
					accion = actFORWARD;
				else{
					if (girar_derecha)
						accion = actTURN_R;
					else
						accion = actTURN_L;
				}
			}
		}
		else if (bikini){
			if ( sensores.terreno[2] == 'B'){
				if (girar_derecha)
					accion = actTURN_R;
				else
					accion = actTURN_L;
			}
			else{
				if (girar < 4)
					accion = actFORWARD;
				else{
					if (girar_derecha)
						accion = actTURN_R;
					else
						accion = actTURN_L;
				}
			}
		}
		else{
			if ( sensores.terreno[2] == 'B' or sensores.terreno[2] == 'A'){
				if (girar_derecha){
					accion = actTURN_R;
				} else{
					accion = actTURN_L;
				}
			}
		}	
	} else{
		if (girar_derecha){
			accion = actTURN_R;
		} else{
			accion = actTURN_L;
		}
	}

	ultimaAccion = accion;

	// Determinar el efecto de la ultima accion enviada
	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
