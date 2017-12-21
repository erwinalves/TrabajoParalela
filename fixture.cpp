#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <fstream>
#include <cstring>
#include <vector>
#include <xlsxwriter.h>



#define MAX 99999

using namespace std;

typedef char cadena[80];

char *cad;


int ubic[16];
double bloq_dist[16][16];
double copy_dist[16][16];


//Estructuras
struct Point
{
  float lat;
  float lon;
};

struct Equipo
{
  string nom;
  string est;
  Point coor;
};

struct Partido
{
  string l;
  string v;
  int fech;
};

struct indices{
  int i,j;
};

//definicion variables
int num_est;
vector<Equipo> equipos;
vector<Partido> partidos;




bool habi[16];//vector que indica si los equipos estan habilitados

double **dist;

void parti_nuevo(string equi_l, string equi_v, int fech_Parti) //funcion crea partidos nuevos
{
  Partido p;

  p.l = equi_l;
  p.v = equi_v;
  p.fech = fech_Parti;

  partidos.push_back(p);
}

double torad(double dato) // convierte a radianes
{
  float const pi = 3.14159265359;
  return dato = dato*(pi/180);
}



double dist_total(float lat1, float lon1, float lat2, float lon2)//en km
{
  float earthRadius = 6378.7;


  //conversion a radianes
  lat1 = torad(lat1);
  lat2 = torad(lat2);
  lon1 = torad(lon1);
  lon2 = torad(lon2);

  return earthRadius*acos(((sin(lat1))*(sin(lat2)))+(((cos(lat1))*(cos(lat2))*(cos(lon2 - lon1)))));
}

void matriz_dist() //crea dos matrices identicas llenadas con las distancias
{
  for(int i=0; i<num_est; i++)
  {
    for(int j=0; j<num_est; j++)
    {
      if(i==j){
        dist[i][j] = MAX;
        copy_dist[i][j] = MAX;

      }

      else{
        dist[i][j] = dist_total(equipos[i].coor.lat, equipos[i].coor.lon, equipos[j].coor.lat, equipos[j].coor.lon);
        copy_dist[i][j] = dist_total(equipos[i].coor.lat, equipos[i].coor.lon, equipos[j].coor.lat, equipos[j].coor.lon);
      }
    }
  }
}


void row_changed() // FUNCION QUE COPIA FILAS DESDE MATRIZ ORGINAL SIN CAMBIAR LA DIAGONAL NI LOS -1
{
  int i=0;

  for(int i=0; i<num_est; i++)
  {
    for(int j=0; j<num_est; j++)
    {
      if(i!=ubic[i])// SI la POSICION EL NUMERO(J) contadorENIDO EN EL VECTOR ES IMPAR(VISITA) ENTRA
      {
        if(i==j){
          copy_dist[i][j] = MAX;
        }
        if(copy_dist[i][j]!=MAX)
        {
          copy_dist[i][j] = dist[ubic[i]][j];
        }
      }
    }
  }
}

  int leer_archivo(cadena archivo){
    ifstream fs;
    fs.open(archivo);
    if (!fs) cout<<"El fichero no se puede leer o no existe.\n";
    else {
    int contador=0;
    cadena word;
    cout<<"Leyendo el fichero para guardar los equipos..."<<endl;
    while(!fs.eof())
    {
      int contadoradorr=0;
      fs.getline(word,80,'\n');
      Equipo equip_aux;

      cad = strtok(word,";");
      while(cad != NULL)
      {
        if (contadoradorr==0){
        //nombre del equipo
        equip_aux.nom = cad;

        }
        if (contadoradorr==1){
        //nombre del estadio
        equip_aux.est = cad;


        }
        if (contadoradorr==2){
        //coordenada x
        equip_aux.coor.lat = atof(cad);


        }
        if (contadoradorr==3){
        //coordenada y
        equip_aux.coor.lon = atof(cad);

        equipos.insert(equipos.begin()+contador,equip_aux);
        contador++;
        }
        cad = strtok(NULL, ";");
        contadoradorr++;
      }
    }
    return contador;
  }
}

bool crearExcel(cadena argumento){
  cout<<"Se creara el Excel"<<endl;
  string ruta;
  //crea ruta para guardar el archivo
  if (argumento!=NULL)
    ruta = strcat(argumento,"/fixture.xlsx");
  else
    ruta = "fixture.xlsx";

  lxw_workbook  *workbook  = workbook_new(ruta.c_str()); //Se crea el fichero excel
  lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL); //Se agrega una hoja al fichero
  //formatos que se aplicaran en la hoja
  lxw_format *bold = workbook_add_format(workbook);
  lxw_format *bold_local = workbook_add_format(workbook);
  lxw_format *bold_fecha = workbook_add_format(workbook);
  lxw_format *local = workbook_add_format(workbook);
  lxw_format *fecha = workbook_add_format(workbook);
  format_set_bold(bold);
  format_set_bold(bold_local);
  format_set_bold(bold_fecha);
  format_set_align(bold,LXW_ALIGN_LEFT);
  format_set_align(local,LXW_ALIGN_RIGHT);
  format_set_align(fecha,LXW_ALIGN_LEFT);
  format_set_align(bold_local,LXW_ALIGN_RIGHT);
  format_set_align(bold_fecha,LXW_ALIGN_LEFT);
  int cont=1;
  //formato de columnas
  worksheet_set_column(worksheet, 0, 0, 30, local);
  worksheet_set_column(worksheet, 2, 2, 30, NULL);
  worksheet_set_column(worksheet, 1, 1, 20, fecha);
  //nombres de columnas
  worksheet_write_string(worksheet, 0, 0, "Euipo Local", bold_local);
  worksheet_write_string(worksheet, 0, 2, "Ronda", bold_fecha);
  worksheet_write_string(worksheet, 0, 1, "Equipo Visita", bold);

  for(int i=0; i<partidos.size(); i++)
  {
      char cadena[4];

      worksheet_write_string(worksheet, cont, 0, partidos[i].l.c_str(), NULL);

      sprintf(cadena,"%d",partidos[i].fech);//entero a string
      worksheet_write_string(worksheet, cont, 2, cadena, NULL);

      worksheet_write_string(worksheet, cont, 1, partidos[i].v.c_str(), NULL);
      cont++;
  }
  cout<<"Excel creado."<<endl;
  return workbook_close(workbook);
}

indices estadoPartido(){


  indices indice;

  for(int i=0;i<num_est;i++)
  {
    for(int j=0;j<num_est;j++)
    {
      if(habi[i] && habi[j])
      {
        if(copy_dist[i][j]!=MAX)
        {
          indice.i = i;
          indice.j = j;

          copy_dist[indice.i][indice.j]=MAX;
          habi[indice.i]=false;
          habi[indice.j]=false;
  
          return indice;
        }
      }
    }
  }
  
  copy_dist[indice.i][indice.j]=MAX;
  habi[indice.i]=false;
  habi[indice.j]=false;
  
  return indice;
}

void Equipo_listo(){
  for(int i=0;i<num_est;i++) habi[i]=true;
}

indices caminoMinimo(){
  int menor=MAX;
  indices indice;
  indice.i=-1;
  indice.j=-1;
  for(int i=0;i<num_est;i++){
    for(int j=0;j<num_est;j++){
      if(habi[i] && habi[j]){
        if(copy_dist[i][j]<=menor){
          menor=copy_dist[i][j];
          indice.i=i;
          indice.j=j;
        }
      }
    }
  }
  if(indice.i != -1 && indice.j != -1)
  {
    habi[indice.i]=false;
    habi[indice.j]=false;
    copy_dist[indice.i][indice.j]=MAX;
  }
  return indice;
}

indices random_indices(){
  indices indice;
  srand(time(NULL));
  indice.i=rand()%num_est;
  indice.j=rand()%num_est;
  while(indice.i==indice.j)
  {
    indice.j=rand()%num_est;
  }
  cout<<indice.i<<" "<<indice.j<<endl;
  return indice;
}

void copiar_matriz(double real[][16], double copia[][16]){
  for(int i=0;i<num_est;i++){
    for(int j=0;j<num_est;j++){
      copia[i][j]=real[i][j];
    }
  }
}

void fixture()
{
  indices indice;
  for(int k=0;k<(2*(num_est-1));k++){ // por cada fecha se inicia un recorrido 30
    int block = 0, deshabi = 0;
    Equipo_listo();
    for(int i=0; i<num_est;i++)
    {
      ubic[i]=i;
    }
    copiar_matriz(copy_dist,bloq_dist);
    
    INICIO:
    for(int y=0;y<(num_est/2);y++) // se inician los partidos por ronda o fecha 8
    {
      deshabi++;
      if(block==1)
      {
        Equipo_listo();
      }
        if(block == 0)
        { 
          indice=caminoMinimo();
        }

        else 
        {
          indice=estadoPartido();
        }

      if(dist[indice.i][indice.j]==MAX)
      {
        
        copiar_matriz(bloq_dist,copy_dist);

        block++;
        
        if(deshabi == 1)
        {
          goto INICIO;
        }
      }

      ubic[indice.i]=indice.j;
      copy_dist[indice.i][indice.j]=MAX;
      
      parti_nuevo(equipos[indice.i].nom, equipos[indice.j].nom, k+1);
    }
    
    row_changed();
    
  }
}

int main(int argc, char *argv[])
{


  switch(argc){ //chequea cuantos argumentos se han ingresado cuando se ejecuta el programa
    case 1:
      cout<<"No se encuentra el fichero de equipos."<<endl;
      break;
    case 2:
      cout<<"La ruta de salida para el fichero de Excel no ha sido indicada."<<endl;
      cout<<"El archivo quedará guardado en la misma ruta del programa."<<endl;
    case 3:
      {
        Equipo_listo();
        num_est = leer_archivo(argv[1]);


        //aca a la matriz de distancias se le asigna memoria

        dist = new double*[num_est];
        for(int i=0; i<num_est; i++)
        {
          dist[i] = new double[num_est];
        }
            //fin de asignacion de memoria

               
        //calcula y guarda las distancias en la matriz de distancia

        matriz_dist();

        //realiza el fixture del torneo nacional

        fixture();


        crearExcel(argv[2]);

        //con esto se libera memoria para usarla despues
        for(int i = 0; i < num_est; i++)
        {
          delete[] dist[i];
        }
        delete[] dist;
      }
      break;
      default:
      cout<<"Exceso argumentos."<<endl;
      break;
  }
  return 0;
}
