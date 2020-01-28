/* Implementación del TSP usando programción dinámica y una tabla hash para
 almacenar los resultados de los subproblemas. La generación de claves para la
 búsqueda en la tabla hash es muy lenta, por lo que esta implementación sólo
 es válida para unas 10 ciudades, quizás 12. En teoría esta implementación
 funciona con cualquier número de ciudades, a pesar del tiempo que tarde.
*/

/* Esta implementación está basada en el algoritmo visto en clases de teoría
en el que se define g(i,S) como el camino mínimo desde i hasta 1 pasando por
todos los vértices de S una y sólo una vez. El caso base es g(i,{}), que es el
camino mínimo desde i hasta el vértice 1 sin pasar por ningún nodo intermedio,
es decir, el camino directo cuya longitud está reflejada en la matriz de
adyacencia. La solución al TSP según este planteamiento del problema es
g(1,{ciudades - {1}})
*/

/*
COMPILACIÓN: g++ travelsmp_hash.cpp -o tsphash
PARA EJECUTAR USA: ./tsphash archivoCiudades 0
*/

/*
PRECAUCIÓN: NO USAR CON MÁS DE 10 CIUDADES
*/

#include <cassert>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
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

int calcularLongitud(vector<int> recorrido, int distan[MAX][MAX]) {
  int longitud = 0;
  for (int i = 0; i < recorrido.size(); ++i) {
    if (i == recorrido.size() - 1)
      longitud += distan[recorrido[i]][recorrido[0]];
    else
      longitud += distan[recorrido[i]][recorrido[i + 1]];
  }
  return longitud;
}
// Memoria para almacenar resultados de subproblemas
unordered_map<string, pair<int, int>> memoria;

// Implementación de la función g(i,S)
pair<int, int> funcionG(int i, list<int> ciudades, const int C[MAX][MAX]) {
  list<int>::iterator it, it2;
  string claveActual = to_string(i) + ',';
  string clave;
  int minimo = 999999999;
  int valor;

  for (it = ciudades.begin(); it != ciudades.end(); it++)
    claveActual += to_string(*it) + ',';

  pair<int, int> resultado;

  if (ciudades.empty())  // caso base
  {
    memoria[claveActual].first = C[i][0];
    memoria[claveActual].second = 0;
    resultado.first = C[i][0];
    resultado.second = 0;
  } else {  // Aplicamos recursividad/búsqueda de subproblemas ya resueltos en
            // memoria
    for (it = ciudades.begin(); it != ciudades.end(); it++) {
      clave.clear();
      clave = to_string(*it) + ',';
      list<int> ciudadesRestantes = ciudades;
      ciudadesRestantes.remove(*it);

      for (it2 = ciudadesRestantes.begin(); it2 != ciudadesRestantes.end();
           it2++)
        clave += to_string(*it2) + ',';

      if (memoria.find(clave) != memoria.end()) {
        valor = C[i][*it] + memoria[clave].first;
      } else {
        valor = C[i][*it] + funcionG(*it, ciudadesRestantes, C).first;
      }
      if (valor < minimo) {
        minimo = valor;
        resultado.first = valor;
        resultado.second = *it;
        memoria[claveActual].first = valor;
        memoria[claveActual].second = *it;
      }
    }
  }
  return resultado;
}

int main(int argc, char *argv[]) {
  // Comprobacion nº arg
  if (argc != 3) {
    cerr << "Uso: ./tsp_coste_min archivo_ciudades solo_texto{0:activado "
            "1:desactivado}"
         << endl;
    return -1;
  }
  // Abrir archivo
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
  bool abierto[MAX];
  int distancias[MAX][MAX];

  // Leyendo fichero
  for (int i = 0; i < num_ciudades; i++) {
    f >> nada;
    f >> coordenadas[i].x;
    f >> coordenadas[i].y;
  }

  f.close();

  // Inicializacion
  unsigned long t_eje = 0;
  list<int> ciudades;

  // Calculo de las distancias
  for (int i = 0; i < num_ciudades; i++) {
    ciudades.push_back(i);
    for (int j = 0; j < num_ciudades; j++) {
      distancias[i][j] = calcularDistancia(coordenadas[i], coordenadas[j]);
    }
  }

  if (atoi(argv[2])) cout << "Calculando circuito minimo..." << endl;

  ciudades.remove(0);

  high_resolution_clock::time_point t_antes = high_resolution_clock::now();

  // Algoritmo con programación dinámica
  pair<int, int> resultado = funcionG(0, ciudades, distancias);

  high_resolution_clock::time_point t_despues = high_resolution_clock::now();

  t_eje +=
      chrono::duration_cast<chrono::microseconds>(t_despues - t_antes).count();

  cout << "Tiempo de ejecución: " << t_eje << endl;
  cout << "Distancia minima " << resultado.first << endl;

  cout << "Ciclo hamiltoniano mínimo: ";
  string claves;
  claves[0] = '0';
  list<int>::iterator it;
  int siguienteCiudad;
  // Imprimir circuito con origen en la ciudad 0
  cout << "0-->";
  for (int i = 0; i < num_ciudades; i++) {
    for (it = ciudades.begin(); it != ciudades.end(); it++)
      claves += to_string(*it) + ',';
    siguienteCiudad = memoria[claves].second;
    if (siguienteCiudad)
      cout << siguienteCiudad << "-->";
    else
      cout << siguienteCiudad << endl;
    claves.clear();
    claves += to_string(siguienteCiudad) + ',';
    ciudades.remove(siguienteCiudad);
  }

  return 0;
}
