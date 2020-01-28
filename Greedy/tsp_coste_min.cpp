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
    return aux;
}

int main(int argc , char * argv[]){
    if (argc!=3){
        cerr<<"Uso: ./tsp_coste_min archivo_ciudades solo_texto{0:activado 1:desactivado}"<<endl;
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
    bool abierto[MAX];
    int distancias[MAX][MAX];


    for(int i=0;i<num_ciudades;i++){
        f>>nada;
        f>>coordenadas[i].x;
        f>>coordenadas[i].y;
    }

    f.close();

    for(int i=0;i<num_ciudades;i++) abierto[i]=true;
    for(int i=0;i<num_ciudades;i++){ 
        for(int j=0;j<num_ciudades;j++){
         distancias[i][j]=calcularDistancia(coordenadas[i], coordenadas[j]);
         //cout << "Distancia desde " << i+1 << " hasta " << j+1 << "= " << distancias[i][j] << endl;
        }
        //cout << endl;
    }
    if(atoi(argv[2])) cout << "Calculando circuito minimo..." << endl;
    vector<int> solucion;
    int actual, min, ciudad_min=0, distancia=0;
    unsigned long t_eje=0;

    high_resolution_clock::time_point t_antes = high_resolution_clock::now();

    solucion.push_back(0);
    actual=0;
    abierto[0]=false;
    while(ciudad_min!=-1){
        min=-1;
        ciudad_min=-1;
        for(int i=0;i<num_ciudades;i++){
            if(min==-1){
                if(abierto[i]){
                    min=distancias[actual][i];
                    ciudad_min=i;
                }
            }
            else{
                if(abierto[i] && distancias[actual][i]<min){
                    min=distancias[actual][i];
                    ciudad_min=i;
                }
            }
        }
        if(ciudad_min!=-1){
            distancia+=min;
            actual=ciudad_min;
            abierto[actual]=false;
            solucion.push_back(actual);
        }
    }
    solucion.push_back(0);
    distancia+=distancias[actual][0];

    high_resolution_clock::time_point t_despues = high_resolution_clock::now();
    t_eje += chrono::duration_cast<chrono::microseconds>(t_despues - t_antes).count();

    if(atoi(argv[2])){
        cout << "\nDISTANCIA TOTAL: " << distancia << endl;
        cout << "CIRCUITO MINIMO: ";
        for(int i=0;i<solucion.size();i++) cout << solucion[i]+1 << " ";
        cout << "\nTIEMPO DE CALCULO: ";
    }

    cout << t_eje << endl;

    return 0;
}