#include <iostream>
#include <ctime>
#include <chrono>
#include <cmath>

using namespace std;
using namespace std::chrono;

const bool traza = false;
const bool mensajes = false;
int UMBRAL;

void algoritmoSecuencial(int ** m, int f, int ini, bool mostrarMensaje){
  int aux;

  if(f > 1){
    for(int fila = 0; fila < f; ++fila){
      for(int col = fila; col < f; ++col){
        aux = m[fila][col+ini];
        m[fila][col+ini] = m[col][fila+ini];
        m[col][fila+ini] = aux;
      }
      if(mostrarMensaje){
        cout<<endl<<"Fila nº:"<<fila<<" transpuesta"<<endl;
        for(int i = 0; i<f; ++i){
          for(int j = 0; j<f; ++j){
            cout<<m[i][j]<<" ";
          }
          cout<<endl;
        }
      }
    }
  }
}

void DivideYVenceras(int ** m, int longi, int ini, bool traza){
  if(longi > UMBRAL){
    //Inicializacion
     int aux; int l = longi/2;

        DivideYVenceras(m,l, ini, traza);
        if(traza){
          cout<<"Sub-Matriz nº"<<0<<":"<<endl;
          for(int j = 0; j<l; ++j){
            for(int k = 0; k<l; ++k){
              cout<<m[j][k+ini]<<" ";
            }
            cout<<endl;
          }
        }

        DivideYVenceras(m,l, ini+l, traza);
        if(traza){
          cout<<"Sub-Matriz nº"<<1<<":"<<endl;
          for(int j = 0; j<l; ++j){
            for(int k = 0; k<l; ++k){
              cout<<m[j][k+ini+l]<<" ";
            }
            cout<<endl;
          }
        }

        DivideYVenceras(m+l,l, ini, traza);
        if(traza){
          cout<<"Sub-Matriz nº"<<2<<":"<<endl;
          for(int j = 0; j<l; ++j){
            for(int k = 0; k<l; ++k){
              cout<<m[j+l][k+ini]<<" ";
            }
            cout<<endl;
          }
        }

        DivideYVenceras(m+l,l, ini+l, traza);
        if(traza){
          cout<<"Sub-Matriz nº"<<3<<":"<<endl;
          for(int j = 0; j<l; ++j){
            for(int k = 0; k<l; ++k){
              cout<<m[j+l][k+ini+l]<<" ";
            }
            cout<<endl;
          }
        }


    for(int j = 0; j<l; ++j){
      for(int k = 0; k<l; ++k){

        aux = m[j][k+ini+l];
        m[j][k+ini+l] = m[j+l][k+ini];
        m[j+l][k+ini] = aux;
      }
    }

  }else{
    algoritmoSecuencial(m, longi, ini, false);
  }


}

int main(int narg, char ** arg){
  const int n_filas = atoi(arg[1]), n_col = atoi(arg[2]); UMBRAL = atoi(arg[4]);
  int ** m; unsigned long t_eje=0;
  int tamano;
  if(n_filas > n_col){
    for(int i = 20; i>0; --i){
      if(pow(2,i) >= n_filas){
        tamano = pow(2,i);
      }
    }
  }else{
    for(int i = 20; i>0; --i){
      if(pow(2,i) >= n_col){
        tamano = pow(2,i);
      }
    }
  }

  m = new int *[tamano];
  for(int i = 0; i<tamano; ++i){
    m[i] = new int[tamano];
  }
  for(int i = 0; i<tamano; ++i){
    for(int j = 0; j<tamano; ++j){
      if(j < n_col && i< n_filas){
        m[i][j] = i*n_col+j;
        if(mensajes)
          cout<<m[i][j]<<" ";
      }else{
        m[i][j] = 0;
      }
    }
    if(i< n_filas)
      if(mensajes)
        cout<<endl;
  }

  high_resolution_clock::time_point t_antes = high_resolution_clock::now();
  if(arg[3][0] == 's')
    algoritmoSecuencial(m, tamano, 0, traza);
  else
    DivideYVenceras(m, tamano, 0, traza);
  high_resolution_clock::time_point t_despues = high_resolution_clock::now();
  t_eje += chrono::duration_cast<chrono::microseconds>(t_despues - t_antes).count();

  if(mensajes){
    cout<<"\nResultado final:"<<endl;
    for(int i = 0; i<tamano; ++i){
      for(int j = 0; j<tamano; ++j){
        if(j < n_filas && i< n_col)
          cout<<m[i][j]<<" ";
      }
      if( i< n_col)
        cout<<endl;
    }
  }

  cout<<"Tiempo: "<<t_eje<<endl;

  for(int i = 0; i<tamano; ++i)
    delete[] m[i];
  delete[] m;

  return 0;
}
