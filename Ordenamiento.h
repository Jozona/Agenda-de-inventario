#ifndef _ORDENAMIENTO_
#define _ORDENAMIENTO_
#include <iostream>
#include <string>
#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include <sstream>
#include <ctype.h>
#include <CommCtrl.h>
#include <iomanip>
#include <stdlib.h>
#include <cstring>
#include <cctype> 
#include <regex>
#include <Windows.h>

using namespace std;

class Carnet {
public:
	//Identificador del carnet
	int ID;

	//Datos de la persona
	string nombre;
	string apellidoPaterno;
	string apellidoMaterno;
	SYSTEMTIME stmNacimiento;
	string nacimiento;
	string direccion;
	string estado;
	string ciudad;
	string sexo;
	string claveUnica;
	int telefono;

	//Datos de vacunacion
	string vacuna;
	int dosis;
	SYSTEMTIME stmDosis;
	SYSTEMTIME stdDosisDos;
	SYSTEMTIME stdDosisTres;
	SYSTEMTIME stdDosisCuatro;
	string primeraDosis;


	//Imagenes
	string fotoPerfil;
	string fotoDoc;

	//Datos del nodo
	Carnet* next;
	Carnet* prev;
};

class Ordenar {
public:
	void heapSort(Carnet*, int);
	void heapify(Carnet*, int);
	void agregarNodo(Carnet*&, Carnet*&, int);
	Carnet* encuentraNodo(Carnet*, int);
	void node_swap(struct Carnet* left, struct Carnet* right);
	void estanJuntos(struct Carnet* left, struct Carnet* right);
	void ejecutar();
	/*void connect(folio_v first, folio_v second);*/
};

#endif // !_ORDENAMIENTO_
