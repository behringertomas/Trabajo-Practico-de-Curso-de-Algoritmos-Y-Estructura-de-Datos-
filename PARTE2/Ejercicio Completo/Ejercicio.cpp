#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <cstdlib>
#include <iostream>

using namespace std;
typedef char str30[30+1];

typedef struct {
        short int CodM;
        str30 Nombre;
        int Aula;
        int Capacidad;
        int Rechazados; //CAMPO AGREGADO POR NOSOTROS
        int CapacidadOriginal; //CAMPO AGREGADO POR NOSOTROS
        int Antiguedad3; //CAMPO AGREGADO POR NOSOTROS (EJERCICIO 3)
} tvMaterias;

typedef struct {
        short int CodM;
        str30 Nombre;
        int Aula;
        int Capacidad;
} tMaterias;

typedef struct {
        int Legajo;
        short int CodM;
} tInscripciones;

typedef struct {
       int Legajo;
       str30 NomAp;
       int Antiguedad;
       int cantInscripciones; //CAMPO AGREGADO POR NOSOTROS (EJERCICIO 4)
} tvAlu;

typedef struct {
    int Legajo;
    str30 NomAp;
    int Antiguedad;
} tAlu;

 tMaterias rMaterias;
 tvMaterias vMaterias[40];
 tAlu rAlu;
 tvAlu vAlu[1000]; //Maximo 1000 alumnos
 tvAlu rAluAUX; //SOLO LO USAMOS PARA EL ORDENAMIENTO
 int cantidadDeAlumnosQueHay=0; //Lo maximo es 1000 alumnos, pero no sabemos cuantos hay exactamente.
 int cantidadDeMateriasQueHay=0; //Lo maximo son 40 materias, pero no sabemos cuantas hay exactamente.
 int cantMateriasSinInscripciones=0;
 int cantMateriasCompletas=0;


//-----------------------------------------------------------------------------
 void cargaVAlu(int &cantidadDeAlumnosQueHay, tvAlu vAlu[], tAlu rAlu)
//-----------------------------------------------------------------------------
 {
    FILE *Alumnos;
	Alumnos = fopen ("Alumnos","rb");
	int l=0;
	fread (&rAlu, sizeof(rAlu), 1, Alumnos);
	while (!feof(Alumnos)){
        vAlu[l].Legajo = rAlu.Legajo;
        copy(rAlu.NomAp, rAlu.NomAp+31, vAlu[l].NomAp);
        vAlu[l].Antiguedad = rAlu.Antiguedad;
        vAlu[l].cantInscripciones = 0;
        l++;
        cantidadDeAlumnosQueHay++;
        fread (&rAlu, sizeof(rAlu), 1, Alumnos);
	}
	fclose(Alumnos);
 }


 void mostrarArchivoAlumnos () {
     FILE *f;
     tAlu Ralu;

     f = fopen ("Alumnos","rb");
     if (f != NULL) {
           cout << "Archivo de Alumnos:" << endl;
           cout << "Legajo   NomYAp    Antiguedad" << endl;
           fread (&Ralu, sizeof(Ralu), 1, f);
           while (!feof(f)){
                 cout << Ralu.Legajo << "         ";
                 cout << Ralu.NomAp << "            ";
                 cout << Ralu.Antiguedad  << "            ";
                 cout << endl;
                 fread (&Ralu, sizeof(Ralu), 1, f);
           }
     }
     fclose (f);
     system("PAUSE");
}

//-----------------------------------------------------------------------------
void CargaVMaterias (int &cantidadDeMateriasQueHay, tMaterias rMaterias,tvMaterias vMaterias[])
//-----------------------------------------------------------------------------
{
    FILE *materias;
    materias = fopen("Materias", "rb");
    int i = 0;
    fread (&rMaterias, sizeof(rMaterias) ,1 ,materias );
     while ((!feof(materias))){
            //AHORA AL VECTOR LE METO LOS RESPECTIVOS CAMPOS QUE LEO DEL ARCHIVO.
            vMaterias[i].CodM = rMaterias.CodM;
            copy(rMaterias.Nombre, rMaterias.Nombre+31,vMaterias[i].Nombre);//SIRVE PARA COPIAR CARACTERES.
            vMaterias[i].Aula = rMaterias.Aula;
            vMaterias[i].Capacidad = rMaterias.Capacidad;
            vMaterias[i].Rechazados = 0;
            vMaterias[i].CapacidadOriginal = vMaterias[i].Capacidad;
            vMaterias[i].Antiguedad3 = 0; //CAMPO PARA EJERCICIO 3
            cantidadDeMateriasQueHay++;
            i++;
            fread (&rMaterias,sizeof(rMaterias),1,materias);
        }


    fclose(materias);
}


//-----------------------------------------------------------------------------
void Ordena(tvAlu vAlu[], int cantidadDeAlumnosQueHay, tvAlu rAluAUX)
//-----------------------------------------------------------------------------
 {
    for (int p=1; p<=cantidadDeAlumnosQueHay; p++){
        for (int i =0; i<(cantidadDeAlumnosQueHay-p); i++){
            if (vAlu[i+1].cantInscripciones > vAlu[i].cantInscripciones)
            {
            rAluAUX= vAlu[i];
            vAlu[i] = vAlu[i+1];
            vAlu[i+1] = rAluAUX;
            }
        }
    }
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

//----------------------------------------------------------------------------------------------
void AntiguedadMayor3yCantInscripciones(tInscripciones rInscripciones, tvMaterias vMaterias[], int POS, tvAlu vAlu[], int cantidadDeAlumnosQueHay)
//----------------------------------------------------------------------------------------------
{
 int i=-1;

 do{
    i++;
    if (rInscripciones.Legajo == vAlu[i].Legajo){ //Si encontro el legajo que buscaba, pasa a fijarse la antiguedad
       vAlu[i].cantInscripciones ++;
       if (vAlu[i].Antiguedad>3){vMaterias[POS].Antiguedad3++;}
    }
 }while ((rInscripciones.Legajo != vAlu[i].Legajo) && (i < cantidadDeAlumnosQueHay));

}

//-----------------------------------------------------------------------------
void BuscaYPoneInscripciones(tvMaterias vMaterias[], int &cantMateriasSinInscripciones, int &cantMateriasCompletas)
//-----------------------------------------------------------------------------
{

    tInscripciones rInscripciones;
    FILE *inscripciones;
    inscripciones = fopen("Inscripciones", "rb");
    int POS;

    fread (&rInscripciones, sizeof(rInscripciones) ,1 ,inscripciones); //Lee un legajo y un codigo de materia
        while ( (!feof(inscripciones)) ){
            POS = BuscaPOSCodM(rInscripciones, vMaterias);//EN POS DEVOLVIO LA POSICION EN DONDE SE ENCUENTRA EL CODIGO DE MATERIA QUE SE ESTA TRABAJANDO DENTRO DEL vMaterias
                    if (vMaterias[POS].Capacidad > 0){
                    vMaterias[POS].Capacidad -=1;
                    AntiguedadMayor3yCantInscripciones(rInscripciones, vMaterias, POS, vAlu, cantidadDeAlumnosQueHay); //VERIFICA SI EL INSCRIPTO TIENE ANTIGÜEDAD MAYOR A 3 AÑOS (EJERCICIO3)
                    } else {
                    vMaterias[POS].Rechazados +=1; //PARA EL EJERCICIO 1, CUENTA LOS RECHAZADOS
                    }
        fread (&rInscripciones, sizeof(rInscripciones) ,1 ,inscripciones);
        }


    Ordena(vAlu, cantidadDeAlumnosQueHay, rAluAUX); //Ordena los alumnos por cantidad de Inscripciones para luego mostrar los 10 con mas inscripciones.

        for (int k=0; k<cantidadDeMateriasQueHay; k++){
            if (vMaterias[k].CapacidadOriginal == vMaterias[k].Capacidad) //SI LA CANTIDAD ORIGINAL, ES IGUAL A LA CANTIDAD DISPONIBLE... NADIE SE INSCRIBIO
            cantMateriasSinInscripciones++; //EJERCICIO 2-A
            if (vMaterias[k].Capacidad == 0)
            cantMateriasCompletas++; //EJERCICIO 2-B
        }

}


//*****************************************************************************
int main()
//*****************************************************************************
{
    system("color 9f");
    cargaVAlu(cantidadDeAlumnosQueHay, vAlu, rAlu);
    CargaVMaterias (cantidadDeMateriasQueHay,rMaterias,vMaterias); //Vuelca en un vector llamado vInscripcion, todos sus datos
    BuscaYPoneInscripciones(vMaterias,cantMateriasSinInscripciones, cantMateriasCompletas);

int op;
    do {

        system("cls");
        //mostrarArchivoAlumnos();
        cout << "1. Cantidad de alumnos que no pueden cursar una materia porque la capacidad de esa materia ya fue ocupada por otros alumnos" << endl;
        cout << "2. Cantidad de materias que no tengan ningún alumno inscripto y que tengan la capacidad completa" << endl;
        cout << "3. Cantidad de alumnos por materia con mas de 3 años de antigüedad en la facultad. " << endl;
        cout << "4. Los 10 alumnos con mayor cantidad de materias inscriptas" << endl;
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
                    system("cls");
                    cout << "\n";
                    cout << "CodigoMateria:\tRechazados:"<<endl;
                    for (int j=0; j<cantidadDeMateriasQueHay; j++){
                    cout << vMaterias[j].CodM << "\t\t";
                    cout << vMaterias[j].Rechazados<<endl;}
                    system("pause");
                break;

            case 2:
                    system("cls");
                    cout << "\n";
                    cout << "La cantidad de materias que no recibieron inscripciones son: " << cantMateriasSinInscripciones << endl;
                    cout << "La cantidad de materias que agotaron capacidad son: " << cantMateriasCompletas << endl;
                    cout << "\n";
                    system("pause");
                break;

            case 3:
                    system("cls");
                    cout << "\n";
                    cout << "CodMateria \tCantidad de Alumnos con mas de 3 años de antigüedad en la facultad"<< endl;
                    for (int i=0; i<cantidadDeMateriasQueHay; i++){
                    cout << vMaterias[i].CodM << "\t\t";
                    cout << vMaterias[i].Antiguedad3 << endl; }
                    system("pause");
                break;

            case 4:
                    system("cls");
                    cout << "\n";
                    cout << "Legajo: " << "\t" << "CantidadInscripciones: " << endl;
                    for (int i=0; i < 10; i++){ //IMPRIME LOS 10 ALUMNOS CON MAS INSCRIPCIONES ORDENADO
                    cout << vAlu[i].Legajo << "\t\t";
                    cout << vAlu[i].cantInscripciones << endl;}
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
