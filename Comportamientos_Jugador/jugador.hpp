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
      iniciado = false;
      ultimaAccion = actIDLE;

       // Posición inicial en el mapa ciego, la mitad del doble del máximo (200) menos uno
      fil = col = fil_pasos = col_pasos = TAM_MAX-1;
      tamMapa = size;

      desfase_x = desfase_y = -1;

      vector<unsigned char> aux(2*TAM_MAX, '?');
      vector<float> aux2(2*TAM_MAX, 0);

      for (unsigned int i = 0; i < 2*TAM_MAX; i++){
        mapaCiego.push_back(aux);
        mapaPisadas.push_back(aux2);
        mapaPisadas_aux.push_back(aux2);
      }

      punteroMapa = &mapaCiego;
      en_camino = false;
      bateria_llena = true;
      recuperarPisadas = false;
      fil_aux = col_aux = 0;
      algunaVezSituado = false;
      primera_iter = true;
      nuevoDesfase_x = nuevoDesfase_y = desfase_antiguox = desfase_antiguoy = -1;
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    void calculaMovimientos(int i);
    int interact(Action accion, int valor);
    int lugarMenosVisitado(int brujula);
    bool giraDerecha(int brujula);
    bool salirAguaBosque(Sensores sensores);

  private:
  
  // Declarar aquí las variables de estado

  //g_x, g_y guardan donde está la casilla 'G' de posicionamiento en nuestro mapaCiego para
  //poder trasladarlo al mapaResultado una vez posicionados
  int fil, col, fil_pasos, col_pasos, brujula, tamMapa, desfase_x, desfase_y, fil_aux, col_aux, nuevoDesfase_x, nuevoDesfase_y, desfase_antiguox, desfase_antiguoy;
  bool girar_derecha, bien_situado, bikini, zapatillas, iniciado, en_camino, bateria_llena, recuperarPisadas, algunaVezSituado, primera_iter;
  Action ultimaAccion;
  vector< vector< unsigned char> > mapaCiego;
  vector< vector< unsigned char> > * punteroMapa;
  vector< vector< float > > mapaPisadas, mapaPisadas_aux;
  vector<Action> vectorAcciones;
};

#endif
