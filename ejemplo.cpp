#include "mpi.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <fstream>
using std::ifstream;
#include <string>
using std::string;
using std::getline;
#include <sstream>
using std::stringstream;
#include <math.h>
#include <ctime>
#include <math.h>
#include <cmath> 
#define earthRadiusKm 6371.0
#include <time.h>

using namespace std;

struct Equipo{
  string nombre;
  string nombre_est;
  float latitud;
  float longitud; 
};

struct Partido{
  int visita;
  int local;
  int fecha;
};

void leer_archivo(Equipo V[])// agregar lo que recibirá y donde se guardará
{
   ifstream is("equiposcampeonato.csv");
   
   if (is){
      string linea;
      int i=0;
      // Mientras se pueda leer una linea del archivo ...
      while (getline(is, linea) && i<16){
         stringstream ss(linea);
         
          // Obtenemos el nombre y descartamos el ';'
         
         getline(ss, V[i].nombre, ';');
         
         // Obtenemos el nombre del estadio, este es el resto de la linea
         
         getline(ss, V[i].nombre_est,';');

         // Obtenemos la coordenada 1
         
         ss >> V[i].latitud;
         
         // Descartamos el caracter ';' a continuacion del numero
         char ch;
         ss >> ch;
         
         ss >> V[i].longitud;
         i++;
      }
      is.close();
   } 
}

// Esta funcion convierte grados decimales a radianes
double deg2rad(float deg) {
  return (deg * M_PI / 180)
  ;
}

// esta funcion convierte radianes a grados decimales
//
double rad2deg(double rad) {
  return (rad * 180 / M_PI);
}

/**
 * @param lat1d Latitud del primer punto en grados
 * @param lon1d Longitud del primer punto en grados
 * @param lat2d Latitud del segundo punto en grados
 * @param lon2d Longitud del segundo punto en grados
 * @return la distancia entre 2 puntos en kilometros
 */
float distanceEarth(float lat1d, float lon1d, float lat2d, float lon2d) {
  float lat1r, lon1r, lat2r, lon2r, u, v;
  lat1r = deg2rad(lat1d);
  lon1r = deg2rad(lon1d);
  lat2r = deg2rad(lat2d);
  lon2r = deg2rad(lon2d);
  u = sin((lat2r - lat1r)/2);
  v = sin((lon2r - lon1r)/2);
  return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}

void llenarDistancias(){
    
}

void inicializarMatriz(int matriz[][16]){
  for(int i=0;i<16;i++)
    for(int j=0;j<16;j++)
      matriz[i][j]=0;
}

float menorValor(float vector[],int posicion){
  float aux=50000;
    for(int i=0;i<16;i++){
      if(aux>vector[i] && i!=posicion){
          aux = vector[i];
        }
    }
    return aux;
}

int menorPosicion(float vector[],int posicion){
  float aux=50000;
  int respuesta=0;
    for(int i=0;i<16;i++){
      if(aux>vector[i] && i!=posicion){
          aux = vector[i];
          respuesta = i;
        }
    }
    return respuesta;
}

bool verificarNumeroVector(int vector[], int posicion){
  bool respuesta = true;
  int i=0;
  while(i<16){
    if(vector[i]==posicion){
      return true;
    }
  }
  return false;
}

void crearFechas(Equipo e[],int matriz[][16], Partido p[], float MD[][16], float distancia[]){
  float auxvector[16],aux=0;
  int i=0;
  for(int i=0;i<16;i++){
    for(int j=0;j<16;j++){
        auxvector[j]=MD[i][j];
      }
    distancia[i]=menorValor(auxvector,i);
  }
  aux=menorValor(distancia,17);
}



int main(int argc, char *argv[]){

Equipo Equipos[16]; // Vector del tipo Equipo
Partido partidos[240];
float pruebasDistancia[8];
leer_archivo( Equipos); // funcion que lee el archivo y lo separa
float MD[16][16]; //matriz de distancias
int cruces[16][16];
inicializarMatriz(cruces);
for (int i=0; i<16;i++){
  for (int j=i; j<16 ; j++){
    if ( i==j){
      MD[i][j]=0;
      cruces[i][j]=1;
    }
    else{
      MD[i][j]= distanceEarth (Equipos[i].latitud, Equipos[i].longitud, Equipos[j].latitud, Equipos[j].longitud);
      MD[j][i]= MD[i][j];
    }
  }
}

for (int i=0; i<16;i++){
  for (int j=0; j<16 ; j++){
    cout<<"Distancia entre el estadio "<<Equipos[i].nombre_est<<" y el estadio "<<Equipos[j].nombre_est<<" es = "<< MD[i][j]<< endl;
  }
}

crearFechas(Equipos,cruces,partidos,MD,pruebasDistancia);
for (int i=0; i<16;i++){
    for (int j=0; j<16 ; j++){
        cout<<" "<<cruces[i][j];
    }
    cout<<endl;
}
}

