#include <iostream>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <cassert>
#include <ctime>
#include <chrono>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;
using namespace std::chrono;

bool mostrar;
int umbral;

double uniforme()
{
 int t = rand();
 double f = ((double)RAND_MAX+1.0);
 return (double)t/f;
}

void rellena_random(int *T, int num){
  srand(time(0));
  for (int j=0; j<num; j++) {
    double u=uniforme();
    int k=(int)(num*u);
    T[j]=k;
  }
}

void rellena_n_rep(int *T, int num){
  for (int j=0; j<num; j++) {
    T[j]=j;
  }
}

void rellena_rep(int *T, int num){
  for (int j=0; j<num; j++) {
    T[j]=0;
  }
}

void modo_secuencial(int *T, int &n){
  int i, j, k;
  if(n>1){
  	for ( i=0 ; i<n ; i++) {
  		j=i+1;
  		do{

  			if ( T[j] == T[i] ) {
  				for ( k= j +1; k<n; k++)
  					T[k-1]= T[k];
  				n--;
  			} else
  				j++;
  		} while (j < n);
  	}
  }
}

void modo_secuencial(vector<pair<int, int> > T, int &n){
  int i, j, k;
  if(n > 1){
  	for ( i=0 ; i<n ; i++) {
  		j=i+1;
  		do{
  			if ( T[j].first == T[i].first ) {
  				for ( k= j+1; k<n; k++)
  					T[k-1]= T[k];
  				n--;
  			} else
  				j++;
  		} while (j < n);
  	}
  }
}

void modo_chapuza(vector<pair<int, int> >& T, int &n,int ini){
  if(n <= umbral){
    modo_secuencial(T,n);
  }
  else{
    int l, l2, lo;
    l=l2=lo=n/2;
    if(mostrar){
      cout << "Subvector 1:";
      for (int j=ini; j<ini+l; j++) {cout << T[j].first << " ";}
      cout << endl;
      cout << "Subvector 2:";
      for (int j=ini+l; j<ini+n; j++) {cout << T[j].first <<  " ";}
      cout << endl;
      cout << endl;
    }
    modo_chapuza(T,l,ini);
    if(n%2!=0) l2++;
    modo_chapuza(T,l2,ini+lo);

    n = l+l2;
    if ( T[ini+l-1].first == T[ini+lo].first ) {
        for(int i = ini+lo; i<=ini+lo+l2; i++){
          T[i-((ini+lo)-(ini+l-1))] = T[i];
        }
        n--;
		}else{
      for(int i = ini+lo; i<=ini+lo+l2; i++){
        T[i-((ini+lo)-(ini+l))] = T[i];
      }
    }
  }
}

void modo_hash(int *T, int &n, int ini){
  if(n <= umbral){
    modo_secuencial(T,n);
  }
  else{
    unordered_map<int, bool> tabla;
    int l=n/2;
    int l2=l;
    if(mostrar){
      cout << "Subvector 1:";
      for (int j=ini; j<ini+l; j++) {cout << T[j] << " ";}
      cout << endl;
      cout << "Subvector 2:";
      for (int j=ini+l; j<ini+n; j++) {cout << T[j] <<  " ";}
      cout << endl;
      cout << endl;
    }
    modo_hash(T,l,ini);
    if(n%2!=0) l2++;
    modo_hash(T,l2,ini+l);

    n=l+l2;
    for(int i=0;i<l;i++)
      tabla.insert({T[i],true});
    int lo=l;
    for(int i=l;i<n;i++){
      if(tabla[T[i]]!=true){
        T[lo]=T[i];
        lo++;
        tabla[T[i]]=true;
      }
    }
    n=lo;
  }
}

int main(int argc, char * argv[])
{
  if (argc != 6)
    {
      cerr << "Formato " << argv[0] << " <num_elem> <modo_ejec> <umbral> <modo_v> <solo_tiempos>" << endl;
      return -1;
    }

  int n = atoi(argv[1]);
  int modo_ejec = atoi(argv[2]);
  umbral = atoi(argv[3]);
  int modo_v = atoi(argv[4]);
  mostrar = atoi(argv[5]);
  unsigned long t_eje=0;

  int * T = new int[n];
  vector<pair<int, int> > Tc;
  assert(T);

  if(modo_v == 0) rellena_n_rep(T,n);
  else if(modo_v == 1) rellena_rep(T,n);
  else rellena_random(T,n);
  if(mostrar){
    cout << "Vector generado:" << endl;
    for (int j=0; j<n; j++) {cout << T[j] << " ";}
    cout << endl << endl;
  }

  for(int i=0; i<n; i++){
    Tc.push_back(pair<int, int>(T[i], i));
  }

  high_resolution_clock::time_point t_antes = high_resolution_clock::now();

  if(modo_ejec == 0) modo_secuencial(T,n);
  else if(modo_ejec == 1){
    sort(Tc.begin(), Tc.end(), [](const pair<int, int> &x, const pair<int, int> &y){
      if(x.first!=y.first)
        return x.first<y.first;
      else
        return x.second>y.second;});
    modo_chapuza(Tc,n,0);
    vector<pair<int, int> >::iterator it=Tc.begin();
    for(int i = 0; i<n; ++i){++it;}
    sort(Tc.begin(), it, [](const pair<int, int> &x, const pair<int, int> &y){return x.second<y.second;});
  }
  else modo_hash(T,n,0);

  high_resolution_clock::time_point t_despues = high_resolution_clock::now();
  t_eje += chrono::duration_cast<chrono::microseconds>(t_despues - t_antes).count();

  if(mostrar){
    cout << "Vector sin repeticiones:" << endl;
    if(modo_ejec==1) for (int j=0; j<n; j++) {cout << Tc[j].first << " ";}
    else for (int j=0; j<n; j++) {cout << T[j] << " ";}
    cout << endl;
  }

  if(mostrar) cout << endl << "Tiempo: ";
  cout << t_eje << endl;

  delete[] T;

  return 0;
}
