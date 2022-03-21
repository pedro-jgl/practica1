#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
#include <vector>
using namespace std;

// Tamaño máximo del mapa
const int TAM_MAX = 100;


class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado
      brujula = 0;
      girar_derecha = false;
      bien_situado = false;
      bikini = zapatillas = false;
      entrar = 0;
      girar = 0;
      ultimaAccion = actIDLE;

       // Posición inicial en el mapa ciego, la mitad del doble del máximo (200) menos uno
      fil = col = TAM_MAX-1;
      tamMapa = size;

      desfase_x = desfase_y = -1;

      vector<unsigned char> aux(2*TAM_MAX, '?');

      for (unsigned int i = 0; i < 2*TAM_MAX; i++)
        mapaCiego.push_back(aux);

      punteroMapa = &mapaCiego;
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);

  private:
  
  // Declarar aquí las variables de estado

  //g_x, g_y guardan donde está la casilla 'G' de posicionamiento en nuestro mapaCiego para
  //poder trasladarlo al mapaResultado una vez posicionados
  int fil, col, brujula, tamMapa, desfase_x, desfase_y, girar, entrar;
  bool girar_derecha, bien_situado, bikini, zapatillas;
  Action ultimaAccion;
  vector< vector< unsigned char> > mapaCiego;
  vector< vector< unsigned char> > * punteroMapa;
};

#endif
