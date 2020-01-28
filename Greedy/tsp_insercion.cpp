#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <cassert>
#include <chrono>
using namespace std;
using namespace std::chrono;

#define MAX 280

struct coord{
    float x;
    float y;

};

//funciones que devuelven indice de ciudad mas al norte, este, oeste

int ciudadMasAlNorte(coord puntos[], int n_ciu){
  float ymayor=puntos[0].y;
  int inorte=0;
  for(int i=1;i<n_ciu;i++){
    if (ymayor<puntos[i].y){
      ymayor=puntos[i].y;
      inorte=i;
    }
  }
  return inorte;
}

int ciudadMasAlEste(coord puntos[], int n_ciu){
  float xmayor=puntos[0].x;
  int ieste=0;
  for(int i=1;i<n_ciu;i++){
    if (xmayor<puntos[i].x){
      xmayor=puntos[i].x;
      ieste=i;
    }
  }
  return ieste;
}

int ciudadMasAlOeste(coord puntos[], int n_ciu){
  float xmenor=puntos[0].x;
  int ioeste=0;
  for(int i=1;i<n_ciu;i++){
    if (xmenor>puntos[i].x){
      xmenor=puntos[i].x;
      ioeste=i;
    }
  }
  return ioeste;
}


int calcularDistancia(coord ciudad1, coord ciudad2){
    float dx, dy;
    dx=ciudad2.x-ciudad1.x;
    dy=ciudad2.y-ciudad1.y;
    dx=dx*dx;
    dy=dy*dy;
    float d=sqrt(dx+dy);
    int aux=d;
    d=d-aux;
    if(d>=0.5) aux++;
    return aux;
}

int calcularLongitud(vector<int> recorrido, int distan[MAX][MAX]){
  int longitud=0;
  for(int i=0;i<recorrido.size();++i){
    if (i==recorrido.size()-1)
      longitud+=distan[recorrido[i]][recorrido[0]];
    else
      longitud+=distan[recorrido[i]][recorrido[i+1]];
  }
  return longitud;
}


int main(int argc , char * argv[]){
    if (argc!=3){
        cerr<<"Uso: ./tsp_insercion archivo_ciudades solo_texto{0:activado 1:desactivado}"<<endl;
        return -1;
    }
    ifstream f;
    f.open(argv[1]);

    if (!f){
        cerr<<"No puedo abrir el fichero "<<argv[1]<<endl;
        return -1;
    }

    if(atoi(argv[2])) cout << "Cargando ciudades..." << endl;

    int num_ciudades, nada;
    f>>num_ciudades;

    coord coordenadas[MAX];
    int distancias[MAX][MAX];
    bool visitadas[MAX];


    for(int i=0;i<num_ciudades;i++){
        visitadas[i]=0;
        f>>nada;
        f>>coordenadas[i].x;
        f>>coordenadas[i].y;
    }

    f.close();

    for(int i=0;i<num_ciudades;i++){
        for(int j=0;j<num_ciudades;j++){
         distancias[i][j]=calcularDistancia(coordenadas[i], coordenadas[j]);
        }
        //cout << endl;
    }

    //--------------------------------------------
    //busqueda del triangulo más grande:
    unsigned long t_eje=0;
    vector<int> solucion;

    int norte=ciudadMasAlNorte(coordenadas,num_ciudades);
    int este=ciudadMasAlEste(coordenadas,num_ciudades);
    int oeste=ciudadMasAlOeste(coordenadas,num_ciudades);

    solucion.push_back(norte);
    solucion.push_back(este);
    solucion.push_back(oeste);
    visitadas[norte]=1;
    visitadas[este]=1;
    visitadas[oeste]=1;

    //ver matriz----------------------
    if(atoi(argv[2])){
    cout << "   ";
    for(int i=0;i<num_ciudades;i++){
      cout << i << " " ;
    }
    cout<< endl;

    for(int i=0;i<num_ciudades;i++){
        cout << i << "  ";
        for(int j=0;j<num_ciudades;j++){
          cout<< distancias[i][j] << " ";
        }
        cout << endl;
    }
  }
    //---------------------------------------------------
    if(atoi(argv[2])){
    cout << "RECORRIDO INICIAL. ";
    cout << "Nodo inicial: " << solucion[0] << endl;
    for(int i=0;i<solucion.size();i++){
      cout << solucion[i] << "   ";
    }
    cout << " -> "<<solucion[0] << endl;
    cout << "Longitud del recorrido: " << calcularLongitud(solucion,distancias)<<endl;
  }

    //ELECCION DE NUEVA ciudad
    int nuevaciudad;
    int ciudadMasEconomica;
    int menorLong;
    //variables
    vector<int> aux;
    vector<int> aux2;
    vector<int> recorridosolucion;
    vector<int> posiblesolucion;
    int pos;
    int minlong;

    int flag;

    if(atoi(argv[2])) cout << "Calculando circuito minimo..." << endl;
    high_resolution_clock::time_point t_antes = high_resolution_clock::now();

    while(solucion.size()<num_ciudades){

    for (int i=0;i<num_ciudades;i++){
      pos=1;
      if(!visitadas[i]){
        flag=i;
        //visitadas[i]=true;
        nuevaciudad=i;
        ciudadMasEconomica=i;
        aux=solucion;
        aux2=solucion;
        aux.resize(solucion.size()+1);
        aux[0]=nuevaciudad;
        for (int j=0;j<solucion.size();j++){
          aux[j+1]=aux2[j];
        }
        minlong=calcularLongitud(aux,distancias);
        recorridosolucion=aux;

        while(pos<solucion.size()+1){
          aux=solucion;
          aux2=solucion;
          aux.resize(solucion.size()+1);
          aux[pos]=nuevaciudad;
          for (int j=0;j<pos;j++){
            aux[j]=aux2[j];
          }
          for (int j=pos;j<solucion.size();j++){
            aux[j+1]=aux2[j];
          }

          if(calcularLongitud(aux,distancias)<minlong){
            minlong=calcularLongitud(aux,distancias);
            recorridosolucion=aux;
          }

          pos++;
        }

        menorLong=minlong;
        ciudadMasEconomica=i;
        posiblesolucion=recorridosolucion;
        if(atoi(argv[2])) cout<< "ciudad mas economica (a la primera): " << i << endl;
        break;

        //------------------------------------------------------
      }
    }

    for (int i=flag+1;i<num_ciudades;i++){
      pos=1;
      if(!visitadas[i]){
        nuevaciudad=i;
        //visitadas[i]=true;
        //METER NUEVACIUDAD EN VECTOR SOLUCION:------------------------
        //CASO 0
        aux=solucion;
        aux2=solucion;
        aux.resize(solucion.size()+1);
        aux[0]=nuevaciudad;
        for (int j=0;j<solucion.size();j++){
          aux[j+1]=aux2[j];
        }
        minlong=calcularLongitud(aux,distancias);
        recorridosolucion=aux;

        //DEMAS CASOS
        while(pos<solucion.size()+1){
          aux=solucion;
          aux2=solucion;
          aux.resize(solucion.size()+1);
          aux[pos]=nuevaciudad;
          for (int j=0;j<pos;j++){
            aux[j]=aux2[j];
          }
          for (int j=pos;j<solucion.size();j++){
            aux[j+1]=aux2[j];
          }

          if(calcularLongitud(aux,distancias)<minlong){
            minlong=calcularLongitud(aux,distancias);
            recorridosolucion=aux;
          }

          pos++;
        }
        if(minlong<menorLong){
          menorLong=minlong;
          ciudadMasEconomica=i;
          if(atoi(argv[2])) cout<< "ciudad mas economica a meter: " << i << endl;
          posiblesolucion=recorridosolucion;
        }

        //------------------------------------------------------
      }
    }
    solucion=posiblesolucion;
    visitadas[ciudadMasEconomica]=true;
    if(atoi(argv[2])){
    for(int i=0;i<solucion.size();i++){
      cout << solucion[i] << "   ";
    }
    cout << "->  "<<solucion[0] << endl;
    cout << "Longitud del recorrido: " << calcularLongitud(solucion,distancias)<<endl;
  }
  }

  high_resolution_clock::time_point t_despues = high_resolution_clock::now();
  t_eje += chrono::duration_cast<chrono::microseconds>(t_despues - t_antes).count();
  cout << t_eje << endl;
    return 0;
}
