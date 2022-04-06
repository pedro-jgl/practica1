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
	if (sensores.reset){
		primera_iter = true;

		mapaCiego.clear();
		vector<unsigned char> aux(2*TAM_MAX, '?');

      	for (unsigned int i = 0; i < 2*TAM_MAX; i++)
        	mapaCiego.push_back(aux);

		fil = col = fil_pasos = col_pasos = TAM_MAX-1;
		brujula = 0;
		bien_situado = false;
		punteroMapa = &mapaCiego;
		
		vectorAcciones.clear();

		ultimaAccion = actIDLE;

		if (algunaVezSituado){
			for (int i = 0; i < mapaPisadas.size(); i++)
				for (int j = 0; j < mapaPisadas.size(); j++)
					mapaPisadas_aux[i][j] = mapaPisadas[i][j];

			recuperarPisadas = true;
		}

		mapaPisadas.clear();
		vector<float> aux2(2*TAM_MAX, 0);

      	for (unsigned int i = 0; i < 2*TAM_MAX; i++)
        	mapaPisadas.push_back(aux2);
	}

	if (sensores.nivel < 2)
		brujula = sensores.sentido;

	if (sensores.nivel == 0 and !iniciado){
		bien_situado = true;
		punteroMapa = &mapaResultado;
		fil = sensores.posF;
		col = sensores.posC;
	}
	iniciado = true;
	
	switch(ultimaAccion){
		case actFORWARD:
			switch (brujula){
				case 0:
					fil--;
					fil_pasos--;
				break;
				case 1:
					col++;
					col_pasos++;
				break;
				case 2:
					fil++;
					fil_pasos++;
				break;
				case 3:
					col--;
					col_pasos--;
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
		if (recuperarPisadas){
			fil_aux = sensores.posF - desfase_x;
			col_aux = sensores.posC - desfase_y;
			//********************************************* ACABAR ESTE PASO ********************************************************************//
			for (int i = 0; i < mapaPisadas_aux.size(); i++)
				for (int j = 0; j < mapaPisadas_aux.size(); j++)
					mapaPisadas[i][j] += mapaPisadas_aux[i][j];
			
			mapaPisadas_aux.clear();
			recuperarPisadas = false;
		}
		bien_situado = true;
		punteroMapa = &mapaResultado;
		desfase_x = sensores.posF - fil;
		desfase_y = sensores.posC - col;
		fil = sensores.posF;
		col = sensores.posC;
		for (int i = 0; i < tamMapa; i++)
			for (int j = 0; j < tamMapa; j++)
				if (mapaResultado[i][j] == '?'){
					mapaResultado[i][j] = mapaCiego[i-desfase_x][j-desfase_y];
				}
		
		algunaVezSituado = true;
	}
	

	switch(brujula){
		case 0:
			for (int i = 0; i < 4; i++)
				for (int j = -i; j <= i; j++){
					(*punteroMapa)[fil-i][col+j] = sensores.terreno[i*(i+1)+j];
					if (i*(i+1)+j == 0)
						mapaPisadas[fil_pasos-i][col_pasos+j] += 1;
					else if (i*(i+1)+j < 4)
						mapaPisadas[fil_pasos-i][col_pasos+j] += 0.5;
					else
						mapaPisadas[fil_pasos-i][col_pasos+j] += 0.25;
				}
		break;
		case 1:
			for (int j = 0; j < 4; j++)
				for (int i = -j; i <= j; i++){
					(*punteroMapa)[fil+i][col+j] = sensores.terreno[j*(j+1)+i];

					if (j*(j+1)+i == 0)
						mapaPisadas[fil_pasos+i][col_pasos+j] += 1;
					else if (j*(j+1)+i < 4)
						mapaPisadas[fil_pasos+i][col_pasos+j] += 0.5;
					else
						mapaPisadas[fil_pasos+i][col_pasos+j] += 0.25;
				}
		break;
		case 2:
			for (int i = 0; i < 4; i++)
				for (int j = i; j >= -i; j--){
					(*punteroMapa)[fil+i][col+j] = sensores.terreno[i*(i+1)-j];

					if (i*(i+1)-j == 0)
						mapaPisadas[fil_pasos+i][col_pasos+j] += 1;
					else if (i*(i+1)-j < 4)
						mapaPisadas[fil_pasos+i][col_pasos+j] += 0.5;
					else
						mapaPisadas[fil_pasos+i][col_pasos+j] += 0.25;
				}
		break;
		case 3:
			for (int j = 0; j < 4; j++)
				for (int i = j; i >= -j; i--){
					(*punteroMapa)[fil+i][col-j] = sensores.terreno[j*(j+1)-i];

					if (j*(j+1)-i == 0)
						mapaPisadas[fil_pasos+i][col_pasos-j] += 1;
					else if (j*(j+1)-i < 4)
						mapaPisadas[fil_pasos+i][col_pasos-j] += 0.5;
					else
						mapaPisadas[fil_pasos+i][col_pasos-j] += 0.25;
				}
		break;
	}



	//Decidir qué acción tomar
	if (sensores.bateria < 1500 and !en_camino)
		bateria_llena = false;
	
	if (sensores.terreno[0] == 'X' and !bateria_llena){
		for (int i = 0; i < 70; i++)
			vectorAcciones.push_back(actIDLE);

		en_camino = true;
		bateria_llena = true;
	}

	if ( !en_camino ){
		//Aquí vemos si en nuestro campo de visión hay casillas de posicionamiento, bikini y zapatillas
		bool encontrada = false;
		int pos_mejora = -1;
		vector<unsigned char> mejoras;
		mejoras.push_back('G');
		mejoras.push_back('K');
		mejoras.push_back('D');
		mejoras.push_back('X');
		vector<bool *> tengo_mejoras;
		tengo_mejoras.push_back(&bien_situado);
		tengo_mejoras.push_back(&bikini);
		tengo_mejoras.push_back(&zapatillas);
		tengo_mejoras.push_back(&bateria_llena);

		for (int i = 0; i < 3 and !encontrada; i++){
			for (int j = 1; j < sensores.terreno.size() and !(*tengo_mejoras[i]); j++){
				if (sensores.terreno[j] == mejoras[i]){
					encontrada = true;
					pos_mejora = j;
				}
			}
		}

		if (encontrada){
			vectorAcciones.clear();
			calculaMovimientos(pos_mejora);
			encontrada = false;
			pos_mejora = -1;
			en_camino = true;
		}

		
	}


	if (sensores.terreno[0] == 'K')
		bikini = true;
	if (sensores.terreno[0] == 'D')
		zapatillas = true;


	if (vectorAcciones.empty()){
		int destino = lugarMenosVisitado(brujula);

		calculaMovimientos(destino);
		accion = vectorAcciones.back();
		vectorAcciones.pop_back();

		if (accion == actFORWARD and ( sensores.terreno[2] == 'M' or sensores.terreno[2] == 'P' or sensores.superficie[2] != '_' or (sensores.terreno[2] == 'A' and !bikini) or (sensores.terreno[2] == 'B' and !zapatillas) ) ){
			vectorAcciones.clear();
			girar_derecha = (rand()%5 < 4)? giraDerecha(brujula) : !giraDerecha(brujula);	//Una de cada 5 veces gira al contrario, para romper bucles.
			accion = actTURN_L;
			if (girar_derecha)
				accion = actTURN_R;
		}
	}
	else{
		accion = vectorAcciones.back();
		vectorAcciones.pop_back();

		if (accion == actFORWARD and ( sensores.terreno[2] == 'M' or sensores.terreno[2] == 'P' or sensores.superficie[2] != '_'  or (sensores.terreno[2] == 'A' and !bikini) or (sensores.terreno[2] == 'B' and !zapatillas) ) ){
			vectorAcciones.clear();
			//girar_derecha = (rand()%2==0);
			girar_derecha = (rand()%5 < 4)? giraDerecha(brujula) : !giraDerecha(brujula);
			accion = actTURN_L;
			if (girar_derecha)
				accion = actTURN_R;
		}

		if (vectorAcciones.empty())
			en_camino = false;
	}

	if (primera_iter)
		primera_iter = false;


	ultimaAccion = accion;
	// Determinar el efecto de la ultima accion enviada
	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}

void ComportamientoJugador::calculaMovimientos(int i){
	int tam;
	vector<Action> aux;

	if (i > 1){
		if (i <= 3){
			aux.push_back(actFORWARD);
			if (i != 2){
				if (i ==1)
					aux.push_back(actTURN_L);
				else
					aux.push_back(actTURN_R);
					
				aux.push_back(actFORWARD);
			}

		}
		else if (i <= 8){
			aux.push_back(actFORWARD);
			aux.push_back(actFORWARD);

			if (i != 6){
				if (i < 6)
					aux.push_back(actTURN_L);
				else
					aux.push_back(actTURN_R);
				
				for (int j = 2 - i%2; j > 0; j--)
					aux.push_back(actFORWARD);

			}

		}
		else if (i <= 15){
			aux.push_back(actFORWARD);
			aux.push_back(actFORWARD);
			aux.push_back(actFORWARD);

			if (i != 12){
				if (i < 12)
					aux.push_back(actTURN_L);
				else
					aux.push_back(actTURN_R);
				
				for (int j = 3 - i%3; j > 0; j--)
					aux.push_back(actFORWARD);
			}

		}
	}

	tam = aux.size();

	for (int j = 0; j < tam; j++){
		vectorAcciones.push_back(aux.back());
		aux.pop_back();
	}
}

int ComportamientoJugador:: lugarMenosVisitado(int brujula){
	vector <double> v;
	double  min = -1;
	int pos_min;

	for (int i = 0; i < 16; i++)
		v.push_back(0);

	switch(brujula){
		case 0:
			for (int i = 0; i < 4; i++)
				for (int j = -i; j <= i; j++){
					v[i*(i+1)+j] = mapaPisadas[fil_pasos-i][col_pasos+j];
				}
		break;
		case 1:
			for (int j = 0; j < 4; j++)
				for (int i = -j; i <= j; i++){
					v[j*(j+1)+i] = mapaPisadas[fil_pasos+i][col_pasos+j];
				}
		break;
		case 2:
			for (int i = 0; i < 4; i++)
				for (int j = i; j >= -i; j--){
					v[i*(i+1)-j] = mapaPisadas[fil_pasos+i][col_pasos+j];
				}
		break;
		case 3:
			for (int j = 0; j < 4; j++)
				for (int i = j; i >= -j; i--){
					v[j*(j+1)-i] = mapaPisadas[fil_pasos+i][col_pasos-j];
				}
		break;
	}

	min = v[0];
	pos_min = 0;
	for (int i = 1; i < 16; i++){
		if (v[i] < min){
			pos_min = i;
			min = v[i];
		}
	}

	if (min == v[12])
		pos_min = 12;

	return pos_min;
}

bool ComportamientoJugador:: giraDerecha(int brujula){
	int brujD, brujI, tam = mapaPisadas.size(), tope_i, inicio_i, tope_j, inicio_j;
	double sum = 0;
	bool girar_derecha = false;
	vector<int> vbruj;
	vector<double> valor;

	brujD = (brujula + 1) % 4;
	brujI = (brujula + 3) % 4;
	vbruj.push_back(brujD);
	vbruj.push_back(brujI);
	valor.push_back(0);
	valor.push_back(0);

	for (int k = 0; k < 2; k++){
		sum = 0;
		switch(vbruj[k]){
			case 0:
				inicio_i = 1;
				tope_i =  (fil_pasos < tamMapa)? fil_pasos : tamMapa;				//min(fil_pasos,tamMapa)
				inicio_j = (col_pasos < tamMapa)? -col_pasos : -tamMapa;			//-min(col_pasos,tamMapa)
				tope_j = (tam-col_pasos < tamMapa)? tam-col_pasos : tamMapa;		//min(tam-col_pasos,tamMapa)

				for (int i = inicio_i; i < tope_i; i++)
					for (int j = inicio_j; j < tope_j; j++)
						sum += mapaPisadas[fil_pasos-i][col_pasos+j];

				valor[k] = sum / (tope_i-inicio_i)*(tope_j-inicio_j);
			break;
			case 1:
				inicio_i = (fil_pasos < tamMapa)? -fil_pasos : -tamMapa;			//-min(fil_pasos,tamMapa)
				tope_i =  (tam-fil_pasos < tamMapa)? tam-fil_pasos : tamMapa;		//min(tam-fil_pasos,tamMapa)
				inicio_j = 1;
				tope_j = (tam-col_pasos < tamMapa)? tam-col_pasos : tamMapa;		//min(tam-col_pasos,tamMapa)
				
				for (int i = inicio_i; i < tope_i; i++)
					for (int j = inicio_j; j < tope_j; j++)
						sum += mapaPisadas[fil_pasos+i][col_pasos+j];

				valor[k] = sum / (tope_i-inicio_i)*(tope_j-inicio_j);
			break;
			case 2:
				inicio_i = 1;
				tope_i =  (tam-fil_pasos < tamMapa)? tam-fil_pasos : tamMapa;		//min(tam-fil_pasos,tamMapa)
				inicio_j = (col_pasos < tamMapa)? -col_pasos : -tamMapa;			//-min(col_pasos,tamMapa)
				tope_j = (tam-col_pasos < tamMapa)? tam-col_pasos : tamMapa;		//min(tam-col_pasos,tamMapa)
				
				for (int i = inicio_i; i < tope_i; i++)
					for (int j = inicio_j; j < tope_j; j++)
						sum += mapaPisadas[fil_pasos+i][col_pasos+j];

				valor[k] = sum / (tope_i-inicio_i)*(tope_j-inicio_j);
			break;
			case 3:
				inicio_i = (fil_pasos < tamMapa)? -fil_pasos : -tamMapa;			//-min(fil_pasos,tamMapa)
				tope_i =  (tam-fil_pasos < tamMapa)? tam-fil_pasos : tamMapa;		//min(tam-fil_pasos,tamMapa)
				inicio_j = 1;
				tope_j = (col_pasos < tamMapa)? col_pasos : tamMapa;				//min(col_pasos,tamMapa)
				
				for (int i = inicio_i; i < tope_i; i++)
					for (int j = inicio_j; j < tope_j; j++)
						sum += mapaPisadas[fil_pasos+i][col_pasos-j];

				valor[k] = sum / (tope_i-inicio_i)*(tope_j-inicio_j);
			break;
		}
	}

	if (valor[1] < valor[2])
		girar_derecha = true;

	return girar_derecha;
}

bool ComportamientoJugador:: salirAguaBosque(Sensores sensores){
	int pos = 0;
	bool salida = false;
	
	for (int i = 1; i < 16 and !salida; i++){
		if ( sensores.terreno[i] != 'M' and sensores.terreno[i] != 'P' and sensores.superficie[i] == '_'  and (sensores.terreno[i] != 'A' or bikini) and (sensores.terreno[i] != 'B' or zapatillas) ){
			salida = true;
			pos = i;
		}
	}

	if (salida)
		calculaMovimientos(pos);
	
	return salida;
}



