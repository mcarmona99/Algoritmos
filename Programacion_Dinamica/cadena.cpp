#include <iostream>
#include <random>
#include <time.h>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

string CalculoDinamico(char *p, char*s, int m, int n, vector<vector<string>> & solucion){
  string resultado = "", tmp1, tmp2;
  if(solucion[m][n] != "-1") return solucion[m][n];
  if(m > 0 && n > 0){
    if(p[m-1] == s[n-1]){
      resultado = CalculoDinamico(p,s,m-1,n-1,solucion);
      resultado.push_back(p[m-1]);
    }else{
      tmp1 = CalculoDinamico(p,s,m-1,n,solucion);
      tmp2 = CalculoDinamico(p,s,m,n-1,solucion);
      if(tmp1.size() > tmp2.size())
        resultado = tmp1;
      else
        resultado = tmp2;
    }
  }
  solucion[m][n] = resultado;
  return resultado;
}

int main(int narg, char ** arg){
  if(narg < 5){
    cout<<"Faltan Argumentos"<<endl;
    return -1;
  }
  //Declaración de variables
  bool mostrarCadenas = arg[3][0] == 's';
  int m = atoi(arg[1]), n = atoi(arg[2]), n_iter = atoi(arg[4]);
  char *p = new char[m], *s = new char[n];
  vector<vector<string>> solucion;
  unsigned long tiempo=0;

srand (time(NULL));
for(int k = 0; k<n_iter; ++k){
  //Inicializacion de valores
  solucion.resize(m+1);
  for(int i = 0; i<m+1; ++i){
    solucion[i].resize(n+1);
  }
  for(int i = 0; i<m; ++i){
    p[i] = 'a' + rand() % ('z'-'a');
    if(mostrarCadenas)
      cout<<p[i];
  }
  if(mostrarCadenas)
    cout<<endl;
  for(int i = 0; i<n; ++i){
    s[i] = 'a' + rand() % ('z'-'a');
    if(mostrarCadenas)
      cout<<s[i];
  }
  if(mostrarCadenas)
    cout<<endl;
  for(int i = 0; i<m+1; ++i){
    for(int j = 0; j<n+1; ++j){
      solucion[i][j] = "-1";
    }
  }

  //Calculos
  high_resolution_clock::time_point t_antes = high_resolution_clock::now();
  string sol = CalculoDinamico(p,s,m,n,solucion);
  high_resolution_clock::time_point t_despues = high_resolution_clock::now();
  tiempo += chrono::duration_cast<chrono::microseconds>(t_despues - t_antes).count();

  cout<<"Solucion: Tamanio = "<<sol.size()<<"\tCadena: "<<sol<<endl;
}
  cout<<"Tiempo: "<<tiempo/n_iter<<endl;

  delete[] p; delete[] s;
  return 0;
}
