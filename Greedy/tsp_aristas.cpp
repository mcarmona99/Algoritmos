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
    if(aux==0) aux=1;
    return aux;
}

int main(int argc , char * argv[]){
  //Comprobacion nº arg
    if (argc!=3){
        cerr<<"Uso: ./tsp_coste_min archivo_ciudades solo_texto{0:activado 1:desactivado}"<<endl;
        return -1;
    }
    //Abrir archivo
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

    //Leyendo fichero
    for(int i=0;i<num_ciudades;i++){
        f>>nada;
        f>>coordenadas[i].x;
        f>>coordenadas[i].y;
    }

    f.close();

    //Inicializacion
    vector<int> salida;
    int distancia=0;
    unsigned long t_eje=0;
    int contador[MAX];
    contador[0]=1;
    for(int i=1;i<num_ciudades;i++) contador[i]=0;
    int aristas=num_ciudades-1;
    int min=0, ciudad1, ciudad2;
    coord ciudad_min;
    int solucion[MAX][MAX];
    int arbol[MAX];

    //Calculo de las distancias
    for(int i=0;i<num_ciudades;i++){
        arbol[i]=i;
        for(int j=0;j<num_ciudades;j++){
            distancias[i][j]=calcularDistancia(coordenadas[i], coordenadas[j]);
            solucion[i][j]=0;
        }
    }
    if(atoi(argv[2])) cout << "Calculando circuito minimo..." << endl;

    high_resolution_clock::time_point t_antes = high_resolution_clock::now();
    //Algoritmo
    while(aristas>0){
      min=0;
        for(int i=0;i<num_ciudades;i++){
            for(int j=0;j<i;j++){
                if(min==0){
                    if(arbol[i]!=arbol[j] && contador[i]<2 && contador[j]<2){
                        min=distancias[i][j];
                        ciudad1=i;
                        ciudad2=j;
                    }
                }
                else{
                    if(arbol[i]!=arbol[j] && contador[i]<2 && contador[j]<2 && distancias[i][j]<min){
                        min=distancias[i][j];
                        ciudad1=i;
                        ciudad2=j;
                    }
                }
            }
        }

        //cout << ciudad1 << "," << ciudad2 << endl;

        contador[ciudad1]++;
        contador[ciudad2]++;
        distancia+=min;

        solucion[ciudad1][ciudad2]=1;
        solucion[ciudad2][ciudad1]=1;

        //Actualizar los grupos de ciudades
        if(arbol[ciudad1] == ciudad1)
          if(arbol[ciudad2] == ciudad2)
            arbol[ciudad1] = arbol[ciudad2] = -ciudad1;
          else
            arbol[ciudad1] = arbol[ciudad2];
        else
          if(arbol[ciudad2] == ciudad2)
            arbol[ciudad2] = arbol[ciudad1];
          else{
            int a = arbol[ciudad2];
            for(int k = 0; k<num_ciudades; ++k)
              if(arbol[k] == a)
                arbol[k] = arbol[ciudad1];
          }



        aristas--;
    }
    //Conectar la última ciudad
    for(int i=0;i<num_ciudades;i++) if(contador[i]<2){
        solucion[0][i]=1;
        solucion[i][0]=1;
    }

    high_resolution_clock::time_point t_despues = high_resolution_clock::now();
    t_eje += chrono::duration_cast<chrono::microseconds>(t_despues - t_antes).count();

    int actual=0;
    int anterior=0;
    salida.push_back(actual);
    for(int i=0;i<num_ciudades;i++) if(solucion[actual][i]==1){
        actual=i;
        salida.push_back(actual);
        i=num_ciudades;
    }
    while(actual!=0){
        for(int i=0;i<num_ciudades;i++) if(solucion[actual][i]==1 && i!=anterior){
            anterior=actual;
            actual=i;
            salida.push_back(actual);
            i=num_ciudades;
        }
    }

    //Imprimir resultado
    if(atoi(argv[2])){
        cout << "\nDISTANCIA TOTAL: " << distancia << endl;
        cout << "CIRCUITO MINIMO: ";
        for(int i=0;i<salida.size();i++) cout << salida[i]+1 << " ";
        cout << "\nTIEMPO DE CALCULO: ";
    }

    cout << t_eje << endl;

    return 0;
}
