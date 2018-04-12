#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <cstdlib>
#include <iostream>

using namespace std;

typedef struct {
        short int CodM;
        char Nombre[31];
        int Aula;
        int Capacidad;
} tvMaterias;

typedef struct {
        int Legajo;
        short int CodM;
} tInscripciones;

typedef struct {
    int Legajo;
    char NomAp[31];
    int Antiguedad;
} tvAlu;


typedef struct nodo{
    tvAlu datos;
    struct nodo *sgte;
};

 nodo *inicio;
 nodo *fin;

 tvMaterias rMaterias;
 tvMaterias vMaterias[40];
 tvAlu rAlu;
 tvAlu vAlu[1000]; //Maximo 1000 alumnos
 int cantidadDeAlumnosQueHay=0; //Lo maximo es 1000 alumnos, pero no sabemos cuantos hay exactamente.
 int cantidadDeMateriasQueHay=0; //Lo maximo son 40 materias, pero no sabemos cuantas hay exactamente.


 nodo *nuevonodo(tvAlu);
 void insertar(nodo **, tvAlu);
 void insertarFinal(nodo **, tvAlu);

//-----------------------------------------------------------------------------
 void cargaVAlu(int &cantidadDeAlumnosQueHay, tvAlu vAlu[], tvAlu rAlu)
//-----------------------------------------------------------------------------
 {
    FILE *Alumnos;
	Alumnos = fopen ("Alumnos","rb");
	int l=0;
	fread (&rAlu, sizeof(rAlu), 1, Alumnos);
	while (!feof(Alumnos)){
        vAlu[l].Legajo = rAlu.Legajo;
        strcpy(vAlu[l].NomAp,rAlu.NomAp);
        vAlu[l].Antiguedad = rAlu.Antiguedad;
        l++;
        cantidadDeAlumnosQueHay++;
        fread (&rAlu, sizeof(rAlu), 1, Alumnos);
	}
	fclose(Alumnos);
 }

//-----------------------------------------------------------------------------
void CargaVMaterias (int &cantidadDeMateriasQueHay, tvMaterias rMaterias,tvMaterias vMaterias[])
//-----------------------------------------------------------------------------
{
    FILE *materias;
    materias = fopen("Materias", "rb");
    int i = 0;
    fread (&rMaterias, sizeof(rMaterias) ,1 ,materias );
     while ((!feof(materias))){
            //AHORA AL VECTOR LE METO LOS RESPECTIVOS CAMPOS QUE LEO DEL ARCHIVO.
            vMaterias[i].CodM = rMaterias.CodM;
            strcpy(vMaterias[i].Nombre, rMaterias.Nombre);//SIRVE PARA COPIAR CARACTERES.
            vMaterias[i].Aula = rMaterias.Aula;
            vMaterias[i].Capacidad = rMaterias.Capacidad;
            cantidadDeMateriasQueHay++;
            i++;
            fread (&rMaterias,sizeof(rMaterias),1,materias);
        }


    fclose(materias);
}


//-----------------------------------------------------------------------------
int BuscaPOSCodM(tInscripciones rInscripciones, tvMaterias vMaterias[])
//-----------------------------------------------------------------------------
{
int A = -1;

do {
   A++;
   if (rInscripciones.CodM == vMaterias[A].CodM){return A;}
} while ((rInscripciones.CodM != vMaterias[A].CodM) && (A < cantidadDeMateriasQueHay));

}


//-----------------------------------------------------------------------------
tvAlu buscaAlumnoRechazado (tvAlu vAlu[], tInscripciones rInscripciones, int cantidadDeAlumnosQueHay)
//-----------------------------------------------------------------------------
{
    tvAlu rAluAux;
    int A = -1;
                do {
                   A++;
                   if (rInscripciones.Legajo == vAlu[A].Legajo)
                    {

                         rAluAux.Legajo = vAlu[A].Legajo;
                         rAluAux.Antiguedad = vAlu[A].Antiguedad;
                         strcpy(rAluAux.NomAp,vAlu[A].NomAp);
                         return rAluAux;
                    }
                } while ((rInscripciones.Legajo != vAlu[A].Legajo) && (A < cantidadDeAlumnosQueHay));
}


//-----------------------------------------------------------------------------
nodo *nuevonodo(tvAlu rAlumnoRechazado)
//-----------------------------------------------------------------------------

{
    nodo *a;
    a=(nodo*)malloc(sizeof(nodo));

    a->datos.Antiguedad = rAlumnoRechazado.Antiguedad;
    a->datos.Legajo = rAlumnoRechazado.Legajo;
    strcpy(a->datos.NomAp,rAlumnoRechazado.NomAp);
    a->sgte = NULL;

    return (a); /* Retorna la direccion donde esta el nodo ese */
}


//-----------------------------------------------------------------------------
void cargar_Lista(nodo **l, FILE *x)
//-----------------------------------------------------------------------------
{
    int POS;
    tInscripciones rInscripciones;
    tvAlu rAlumnoRechazado;
    fread(&rInscripciones,sizeof(rInscripciones),1,x);
    while(!feof(x))
    {
        POS = BuscaPOSCodM(rInscripciones, vMaterias);
        if (vMaterias[POS].Capacidad > 0){
            vMaterias[POS].Capacidad -=1;
        }
        if (vMaterias[POS].Capacidad == 0){ //SI LA CAPACIDAD ES 0, HABRA QUE RECHAZAR AL ALUMNO.
            rAlumnoRechazado = buscaAlumnoRechazado(vAlu, rInscripciones, cantidadDeAlumnosQueHay);

            insertar(l,rAlumnoRechazado);
        }

        fread(&rInscripciones,sizeof(rInscripciones),1,x);
    }

}


//-----------------------------------------------------------------------------
void insertar(nodo **l, tvAlu x)
//-----------------------------------------------------------------------------
{
    nodo *nuevo;
    nuevo= nuevonodo(x);
    if(*l==NULL) *l=nuevo;
       else
            if((*l)->datos.Antiguedad>x.Antiguedad)
                {
                    nuevo->sgte=*l;
                    *l=nuevo;
                }
                else insertar(&(*l)->sgte,x);

}

//-----------------------------------------------------------------------------
void listar (nodo *l)
//-----------------------------------------------------------------------------
{
    if(l!=NULL)
    {
            printf("%5d \t%-15s %5d\n",l->datos.Legajo,l->datos.NomAp,l->datos.Antiguedad);
            listar(l->sgte);
    }
}

//*****************************************************************************
int main()
//*****************************************************************************
{
    system("color E");
    inicio = NULL;
    fin    = NULL;
    nodo *lista = NULL; //CAPAZ QUE FALTA PASARLO A BuscaYPoneInscripciones

    cargaVAlu(cantidadDeAlumnosQueHay, vAlu, rAlu);
    CargaVMaterias (cantidadDeMateriasQueHay,rMaterias,vMaterias);  //Vuelca en un vector llamado vInscripcion, todos sus datos

 //--------------

    FILE *inscripciones;
    inscripciones = fopen("Inscripciones", "rb");
    cargar_Lista(&lista, inscripciones);

    int op;
    do {
        system("cls");
        cout << "1. Rechazados ordenados por antiguedad" << endl;
        cout << "0. Salir" << endl << endl;
        cout << "Ingrese una opcion: ";
        cin >> op;
         switch (op) {
            case 0:
                    cout << "\n";
                    cout << "Fin de programa." << endl;
                    cout << "\n";
                break;
            case 1:
                    printf("Legajo \t NomApe \t Antiguedad\n");
                    listar(lista);
                    system("pause");
                break;

            default:
                    cout << "Numero de Opcion erronea" << endl;
                    system("pause");
                break;
        }
    }while (op != 0);
    return 0;
}
