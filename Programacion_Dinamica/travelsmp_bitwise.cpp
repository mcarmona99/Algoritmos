/*Esta implementación usa OPERACIONES A NIVEL DE BITS para evitar la costosa
generación de claves de la versión anterior. Se usa un entero de 32 bits
almacenar la información sobre ciudades visitadas, en el que cada bit representa
si una ciudad ha sido o no visitada, por ejemplo, 0001 significaría que la
ciudad 1 ha sido visitada y el resto no. Para almacenar los subproblemas se
utiliza una matriz de tamaño [2^N]*[N] siendo N el número de ciudades 2^N porque
es el número de subconjuntos en el conjunto de todas las ciudades. N acota
superiormente el número de posibles ciudades a tomar como siguiente ciudad al
camino*/

/*El fundamento teórico de esta solución es exactamente el mismo (conclusión
practicamente directa del Principio de obtimalidad de Bellman), pero está
implementado desde un punto de vista algo diferente al algoritmo con la tabla
hash. Aquí la funcion tsp(v,c) se define como el camino más corto desde la
ciudad c hasta el origen habiendo ya visitado las ciudades v. El caso base es
tsp(todas_las_ciudades, x), es decir, el camino más corto desde la ciudad x
hasta el origen habiendo visitado ya todas las ciudades, caso en el que sólo
queda una posibilidad, volver a la ciudad origen (la distancia de ese paso de
vuelta al origen es la que se encuentra en la matriz de adyacencia*/

/*
COMPILACIÓN: g++ travelsmp_bitwise.cpp -o tspbitwise
PARA EJECUTAR USA: ./tspbitwise archivoCiudades 0
*/

/*
PRECAUCIÓN: NO USAR CON MÁS DE 22 CIUDADES
*/

#include <cassert>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;
using namespace std::chrono;

#define MAX 22

struct coord {
  float x;
  float y;
};

int calcularDistancia(coord ciudad1, coord ciudad2) {
  float dx, dy;
  dx = ciudad2.x - ciudad1.x;
  dy = ciudad2.y - ciudad1.y;
  dx = dx * dx;
  dy = dy * dy;
  float d = sqrt(dx + dy);
  int aux = d;
  d = d - aux;
  if (d >= 0.5) aux++;
  return aux;
}

int tsp(int visitadas, int ciudad, const int &n, const int &todas_visitadas,
        pair<int, int> *memoria[22], int distancias[22][22]) {
  // Si ya hemos visitado todas, lo unico que nos queda
  // es devolver el coste para volver al origen
  if (visitadas == todas_visitadas) {
    memoria[visitadas][ciudad].second = 0;
    memoria[visitadas][ciudad].first = distancias[ciudad][0];
    return distancias[ciudad][0];
  }
  // Buscar en memoria

  if (memoria[visitadas][ciudad].first != -1) {
    return memoria[visitadas][ciudad].first;
  }

  // Ir a aquellas ciudades no visitadas
  int resultado = 999999;
  int mejorProximaCiudad = -1;
  for (int ciud = 0; ciud < n; ciud++) {
    if ((visitadas & (1 << ciud)) == 0)  // ciudad no visitada
    {
      // la distancia es el valor de ir de la ciudad actual
      // a la no visitada
      int valor =
          distancias[ciudad][ciud] + tsp(visitadas | (1 << ciud), ciud, n,
                                         todas_visitadas, memoria, distancias);
      if (valor < resultado) {
        resultado = valor;
        mejorProximaCiudad = ciud;
      }
    }
  }
  memoria[visitadas][ciudad].second = mejorProximaCiudad;
  return memoria[visitadas][ciudad].first = resultado;
}
int main(int argc, char *argv[]) {
  if (argc != 3) {
    cerr << "Uso: ./tsp_coste_min archivo_ciudades solo_texto{0:activado "
            "1:desactivado}"
         << endl;
    return -1;
  }
  ifstream f;
  f.open(argv[1]);

  if (!f) {
    cerr << "No puedo abrir el fichero " << argv[1] << endl;
    return -1;
  }

  if (atoi(argv[2])) cout << "Cargando ciudades..." << endl;

  int num_ciudades, nada;
  f >> num_ciudades;

  coord coordenadas[MAX];
  int distancias[MAX][MAX];

  for (int i = 0; i < num_ciudades; i++) {
    f >> nada;
    f >> coordenadas[i].x;
    f >> coordenadas[i].y;
  }

  f.close();

  for (int i = 0; i < num_ciudades; i++) {
    for (int j = 0; j < num_ciudades; j++) {
      distancias[i][j] = calcularDistancia(coordenadas[i], coordenadas[j]);
    }
  }

  // AQUÍ EMPIEZA TSP CON PROGRAMACIÓN DINÁMICA

  int n = num_ciudades;
  int todas_visitadas = (1 << n) - 1;
  pair<int, int> **memoria;
  memoria = new pair<int, int> *[4194304];
  for (int i = 0; i < 4194304; i++) {
    memoria[i] = new pair<int, int>[22];
  }

  for (int i = 0; i < (1 << n); i++) {
    for (int j = 0; j < n; j++) memoria[i][j].first = -1;
  }

  // Se empieza en la ciudad 0, solo esa ha sido visitada (00...01)
  unsigned long t_eje;
  high_resolution_clock::time_point t_antes = high_resolution_clock::now();
  cout << "Longitud circuito minimo "
       << tsp(1, 0, n, todas_visitadas, memoria, distancias) << endl;
  high_resolution_clock::time_point t_despues = high_resolution_clock::now();
  t_eje =
      chrono::duration_cast<chrono::microseconds>(t_despues - t_antes).count();
  // devolver el camino
  cout << "Tiempo de ejecución: " << t_eje << endl;

  int estado = 1;
  int pos = 0;
  int estadoSiguiente;
  int posSiguiente;
  cout << 0 << "-->";
  for (int i = 0; i < n; i++) {
    if (memoria[estado][pos].second) {
      cout << memoria[estado][pos].second << "-->";
    } else {
      cout << memoria[estado][pos].second << endl;
    }
    estadoSiguiente = estado | (1 << memoria[estado][pos].second);
    posSiguiente = memoria[estado][pos].second;
    pos = posSiguiente;
    estado = estadoSiguiente;
  }
}
