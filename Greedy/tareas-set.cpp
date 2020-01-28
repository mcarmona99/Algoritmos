#include <iostream>
#include <random>
#include <time.h>
#include <chrono>
#include <set>

using namespace std;
using namespace std::chrono;

struct tarea{
  int nTarea;
  double beneficio;
  double deadline;
};

struct ComparaTarea{
	bool operator()(const tarea &a, const tarea &n) const{
		if ((a.beneficio > n.beneficio) or (a.beneficio == n.beneficio and a.deadline < n.deadline) or (a.beneficio == n.beneficio and a.deadline == n.deadline and a.nTarea > n.nTarea))
			return true;
		else
			return false;
	}
};

int main(int narg, char ** arg){
  if(narg < 5){
    cout<<"Faltan Argumentos"<<endl;
    return -1;
  }

  int n_tareas = atoi(arg[1]);
  tarea * tareas = new tarea[n_tareas];
  tarea * solucion = new tarea[n_tareas];
  bool * posiciones = new bool[n_tareas];
  int j;
  unsigned long t_eje=0;
  const unsigned long n_iteraciones = atoi(arg[4]);
  set<tarea,ComparaTarea> tareasOrdenadas;

  srand (time(NULL));

  for(int k = 0; k<n_iteraciones; ++k){
    for(int i = 0; i<n_tareas; ++i)
      posiciones[i] = false;
    for(int i = 0; i<n_tareas; ++i){
      tareas[i].nTarea = i;
      tareas[i].beneficio = 1 + rand() % (n_tareas*2);
      tareas[i].deadline = rand() % (n_tareas/2);
    }

    if(arg[2][0] == 's'){
      for(int i = 0; i<n_tareas; ++i){
        cout<<"Tarea "<<i<<"\nBeneficio: "<<tareas[i].beneficio<<"\nLimite: "<<tareas[i].deadline<<endl<<endl;
      }
    }


    high_resolution_clock::time_point t_antes = high_resolution_clock::now();
    for(int i = 0; i<n_tareas; ++i){
      tareasOrdenadas.insert(tareas[i]);
    }
    set<tarea,ComparaTarea>::iterator it = tareasOrdenadas.begin();
    while(it != tareasOrdenadas.end()){
      j = (*it).deadline;
      while(j >= 0 && posiciones[j]){
        --j;
      }
      if(j >= 0){
        solucion[j] = (*it);
        posiciones[j] = true;
      }
      ++it;
    }
    high_resolution_clock::time_point t_despues = high_resolution_clock::now();
    t_eje += chrono::duration_cast<chrono::microseconds>(t_despues - t_antes).count();

    if(arg[2][0] == 's'){
      cout<<"Solucion---------------------------------------------------------"<<endl;
      for(int i = 0; i<n_tareas; ++i){
        if(posiciones[i])
          cout<<"Tarea "<<solucion[i].nTarea<<"\nBeneficio: "<<solucion[i].beneficio<<"\nLimite: "<<solucion[i].deadline<<endl<<endl;
      }
    }
  }
  cout<<"Tiempo: "<<t_eje/n_iteraciones<<endl;

  delete[] solucion;
  delete[] tareas;
  delete[] posiciones;
  return 0;
}
