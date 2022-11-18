/*
	TODO:
	-Limites al poner fecha de vacunacion
	-Borrar la memoria de todas las listas
	-Que no se puedan ingresar espacios en blanco al buscar carnets/editar
	-Mostrar CU en carnet
	-Si se cambia la curp, eliminar carnets asociados
	-Si se edita el nombre de la persona, que no se pueda crear otro carnet con el
	-Validacion de si se borra una vacuna en uso
	-menu


	NEW TODO:
	-Verificar que las validaciones estan en el orden correcto. Esto implica:
		-Test con pura letra
		-Test con puro numero
		-Test cuando falta algo
		-Test con algo en vacio
		-Y seguir buscando
	-Bloquear el ingreso de numeros en lugares donde no debe haber:
		-Nombres
		-Pues etc xd
	-Hacer las ventanas mas grandes!!
	-Que el programa se vea mejor!!!
	-Creo que aun falta que funcione correctamente el pedo de las fechas en las ventanas!

*/



#include <Windows.h>
#include "resource.h"
#include "Validaciones.h"
//#include "Ordenamiento.h"
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

using namespace std;

struct Usuario {
	string usuario;
	string contrasena;
	string nombre;
	string apellido;
	Usuario* next;
	Usuario* prev;
}*oUsuario, * aUsuario;

struct Persona {
	string nombre;
	string apellidoPaterno;
	string apellidoMaterno;
	string estado;
	string municipio;
	int postal;
	string direccion;
	string claveUnica;
	bool postracion;

	string docId;
	string fotoPath;
	int telefono;
	string nacimiento;
	SYSTEMTIME dtmNacimiento;
	string sexo;
	string gpoOcpal;
	string gpoRiesgo;
	string vacuna;
	
	Persona* next;
	Persona* prev;
}*oPersona, *aPersona;

struct Vacuna {
	string nombre;
	int numeroDeDosis;
	string clave;
	string descripcion;
	float precio;
	Vacuna* next;
	Vacuna* prev;
}*aVacuna, * oVacuna;

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
}*oCarnet, *aCarnet, *showCarnet;


#pragma region Funciones de ventanas
BOOL CALLBACK login(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK registro(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK home(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK personas(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK registrarPersonas(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK borrarPersonas(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK editarPersonas(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK vacunasHome(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK altaVacunas(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK bajaVacunas(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK editarVacunas(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK carnetsHome(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK altaCarnet(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK mostrarCarnet(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK editarCarnet(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK buscarCarnetEditar(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK bajaCarnet(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK buscarCarnet(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK funcionReportes(HWND, UINT, WPARAM, LPARAM);
#pragma endregion 
#pragma region Funciones Utilidad

string getText(HWND);
void guardarUsuarios(Usuario* ptrOrg);
void cargarUsuarios();
void liberarMemoriaUsuarios();
void liberarMemoriaPersonas();
void liberarMemoriaCarnets();
string cargarImagen(HWND, int);
void crearCarnet(HWND);
void enviarDatosCarnet(HWND);
void eliminarCarnet(HWND, int);
void opcionesDeMenu(HWND hwnd, WPARAM wParam, LPARAM lParam);

#pragma endregion
#pragma region Variables
HINSTANCE hgInst;
vld validaciones;
bool cerrarHome = false, ingresado = false, acceder = false;
bool imgPsn = false, imgDDI = false;
string claveTemp, claveTemporal;
HMENU hMenu;
string rutaDeImg = "";
string rutaDeImg2 = "";
string nombreUserActual;
#pragma endregion

//void heapSort(Carnet*, int);
//void heapify(Carnet*, int);
//void agregarNodo(Carnet*&, Carnet*&, int);
//Carnet* encuentraNodo(Carnet*, int);
//void node_swap(struct Carnet* left, struct Carnet* right);
//void estanJuntos(struct Carnet* left, struct Carnet* right);
//void ejecutar();

//Funcion principal
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR cmdLine, int cShow) {
	/*ordenamiento.ejecutar();*/
	oUsuario = aUsuario = NULL;
	oPersona = aPersona = NULL;
	oVacuna = aVacuna = NULL;
	oCarnet = aCarnet = showCarnet = NULL;
	hgInst = hInst;
	HWND hwnd = CreateDialog(hInst, MAKEINTRESOURCE(WND_LOGIN), NULL, login);
	ShowWindow(hwnd, cShow);
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	cargarUsuarios();
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	guardarUsuarios(aUsuario);
	liberarMemoriaUsuarios();
	liberarMemoriaPersonas();
	liberarMemoriaCarnets();
	return 0;
}
//Funcion para el inicio de sesion
BOOL CALLBACK login(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_INITDIALOG: {
		
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case BTN_LOGIN: {
			HWND hCdTUsuarioLog = GetDlgItem(hwnd, TXT_USERNAME);
			HWND hCdTPassLog = GetDlgItem(hwnd, TXT_PASSWORD);
			string sUsuario = getText(hCdTUsuarioLog);
			string sConstraseña = getText(hCdTPassLog);
			if (sUsuario.compare("") == 0 || sConstraseña.compare("") == 0) {
				MessageBox(NULL, "Datos incompletos/ Datos incorrectos", "Error", MB_OK);
				break;
			}
			if (oUsuario != NULL) {
				bool encontrado = true;
				while (aUsuario != NULL) {
					if (aUsuario->usuario.compare(sUsuario) == 0 && aUsuario->contrasena.compare(sConstraseña) == 0) {

						break;
					}
					if (aUsuario->next == NULL) {
						encontrado = false;

					}
					aUsuario = aUsuario->next;
				}
				if (encontrado == true) {
					nombreUserActual.append(aUsuario->nombre);
					aUsuario = oUsuario;
					HWND vntnHome = CreateDialog(hgInst, MAKEINTRESOURCE(DLG_INICIO), NULL, home);
					hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
					SetMenu(vntnHome, hMenu);
					ShowWindow(vntnHome, SW_SHOW);
					DestroyWindow(hwnd);
					break;

				}
				else {
					aUsuario = oUsuario;
					MessageBox(NULL, "El usuario no existe", "Error", MB_OK);
				}

			}
			else {
				MessageBox(NULL, "No hay usuarios registrados", "Error", MB_OK);
				break;
			}


		}break;
		case BTN_REGISTRAR_USER: {
			HWND vntnHome = CreateDialog(hgInst, MAKEINTRESOURCE(DLG_REGISTRO_USER), NULL, registro);
			ShowWindow(vntnHome, SW_SHOW);
			DestroyWindow(hwnd);
		
		}break;
		case ID_CANCEL: {
			if (HIWORD(wParam) == BN_CLICKED) {
					PostQuitMessage(0);
			}

		}break;
		}break;	
	}break;
	case WM_CLOSE: {

		break;
	}
	case WM_DESTROY: {
	}break;
	default: {
		DefWindowProc(hwnd, msg, wParam, lParam);
		break;
	}
	}
	return FALSE;
}
//Funcion para registrar usuarios
BOOL CALLBACK registro(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_INITDIALOG: {


	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case BTN_LISTO_RE: {
			string nombre = getText(GetDlgItem(hwnd, EB_NS_USUARIO));
			string apellido = getText(GetDlgItem(hwnd, EB_AS_USUARIO));
			string usuario = getText(GetDlgItem(hwnd, EB_USER_RE));
			string contrasena = getText(GetDlgItem(hwnd, EB_PASS_USUARIO));

			if (nombre.compare("") == 0 || apellido.compare("") == 0 || usuario.compare("") == 0 || contrasena.compare("") == 0) {
				MessageBox(NULL, "Datos incompletos.", "Error", MB_OK);
				break;
			}

			if (validaciones.hayEspacios(contrasena) || validaciones.hayEspacios(usuario)) {
				MessageBox(NULL, "Solo nos nombres pueden tener espacios.", "Error" ,MB_OK);
				break;
			}

			if (validaciones.soloLetras(apellido) || validaciones.soloLetras(nombre)) {
				MessageBox(NULL, "Tu nombre/apellido contiene caracteres invalidos.", "Error", MB_OK);
				break;
			}

			if (oUsuario != NULL) {
				bool encontrado = true;
				while (aUsuario->usuario.compare(usuario) != 0) {
					if (aUsuario->next == NULL) {
						encontrado = false;
						break;

					}
					aUsuario = aUsuario->next;
				}
				aUsuario = oUsuario;
				if (encontrado == true) {
					MessageBox(NULL, "Un usuario ya tiene ese nombre", "Error", MB_OK);
					aUsuario = oUsuario;
					break;

				}

			}


			if (oUsuario == NULL) {
				oUsuario = new Usuario;
				oUsuario->nombre.append(nombre);
				oUsuario->apellido.append(apellido);
				oUsuario->usuario.append(usuario);
				oUsuario->contrasena.append(contrasena);
				oUsuario->next = NULL;
				oUsuario->prev = NULL;
				aUsuario = oUsuario;
				MessageBox(NULL, "Usuario registrado con exito.", "Listo", MB_OK);
			}
			else {
				while (aUsuario->next != NULL) {
					aUsuario = aUsuario->next;
				}

				aUsuario->next = new Usuario;
				aUsuario->next->prev = aUsuario;
				aUsuario = aUsuario->next;
				aUsuario->nombre.append(nombre);
				aUsuario->apellido.append(nombre); 
				aUsuario->usuario.append(usuario);
				aUsuario->contrasena.append(contrasena);
				aUsuario->next = NULL;
				aUsuario = oUsuario;
				MessageBox(NULL, "Usuario registrado con exito.", "Listo", MB_OK);
			
			}
			
			
		}break;

		case BTN_REGRESAR_REGS: {
			HWND vntnHome = CreateDialog(hgInst, MAKEINTRESOURCE(WND_LOGIN), NULL, login);
			ShowWindow(vntnHome, SW_SHOW);
			DestroyWindow(hwnd);
		
		}break;
		}
	}break;
	case WM_CLOSE: {

	}break;
	case WM_DESTROY: {

	}break;
	default: {
		DefWindowProc(hwnd, msg, wParam, lParam);
	}break;
	}
	return FALSE;
}
//Funcion para el inicio
BOOL CALLBACK home(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_INITDIALOG: {
		SetWindowText(GetDlgItem(hwnd, IDC_NOMBRE_HOME), nombreUserActual.c_str());

	}break;
	case WM_COMMAND: {
		opcionesDeMenu(hwnd, wParam, lParam);
	}break;


	case WM_CLOSE: {
		cerrarHome = true;
		DestroyWindow(hwnd);
	}break;
	case WM_DESTROY: {
		if (cerrarHome)
			PostQuitMessage(0);
	}break;
	default: {
		DefWindowProc(hwnd, msg, wParam, lParam);
	}break;
	}
	return FALSE;
}
//Funcion para el inicio de las personas
BOOL CALLBACK personas(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_INITDIALOG: {
		SetWindowText(GetDlgItem(hwnd, NAME_PRNS), nombreUserActual.c_str());
		aPersona = oPersona;
		if (oPersona != NULL) {
			while (aPersona != NULL) {
				string persona = aPersona->nombre + " | " + aPersona->apellidoPaterno + " | " + aPersona->apellidoPaterno + " | " + aPersona->claveUnica;
				SendMessageA(GetDlgItem(hwnd, LISTA_PERSONAS), LB_ADDSTRING, NULL, (LPARAM)persona.c_str());
				aPersona = aPersona->next;
			}
			aPersona = oPersona;
		}
		
	}break;
	case WM_COMMAND: {
		opcionesDeMenu(hwnd, wParam, lParam);
		switch (LOWORD(wParam)) {
		case ID_BTNRP: {
			HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_RPERSONAS), NULL, registrarPersonas);
			ShowWindow(vntnDatos, SW_SHOW);
			hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(vntnDatos, hMenu);
			DestroyWindow(hwnd);
		}break;
		case BTN_BORRAR_PERSONA: {
			HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_BORRAR_PERSONA), NULL, borrarPersonas);
			ShowWindow(vntnDatos, SW_SHOW);
			hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(vntnDatos, hMenu);
			DestroyWindow(hwnd);
		
		}break;
		case BTN_EDITAR: {
			HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_EPERSONAS), NULL, editarPersonas);
			ShowWindow(vntnDatos, SW_SHOW);
			hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(vntnDatos, hMenu);
			DestroyWindow(hwnd);
		
		}break;
		}
	}break;
	case WM_CLOSE: {
		
	}break;
	case WM_DESTROY: {
		
	}break;
	default: {
		DefWindowProc(hwnd, msg, wParam, lParam);
	}break;
	}
	return FALSE;
}
//Funcion para registrar a las personas
BOOL CALLBACK registrarPersonas(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_INITDIALOG: {
		SetWindowText(GetDlgItem(hwnd, NAME_RP), nombreUserActual.c_str());
		//Validacion para no tomar en cuenta las fotos anteriores
		rutaDeImg = rutaDeImg2 = "";
		imgPsn = false;
		imgDDI = false;

		string ocupaciones[12] = { "Gerente", "Profesional", "Técnico y profesionale asociado", 
									"Trabajador de apoyo administrativo",
									"Trabajador de servicios y ventas", 
									"Trabajador de la agricultura, la silvicultura o la pesca",
									"Trabajador de artesanú} u oficios conexos", 
									"Operadores de plantas o máquinas y ensambladores",
									"Ocupaciones elementales", "Ocupaciones de las Fuerzas Armadas", 
									"Estudiante", "Otro" };
		string perfiles[8] = {		"Personal sanitario", "Persona de 80 años o mas", 
									"Personas de 65 a 79 años",
									"Personas con comorbilidades", 
									"Personas institucionalizadas",
									"Persona escencial", "Mayores a 55 años", 
									"Publico general" };
		int indice = 0;

		SendMessage(GetDlgItem(hwnd, CMB_SEXO), CB_ADDSTRING, 0, (LPARAM)"Femenino");
		SendMessage(GetDlgItem(hwnd, CMB_SEXO), CB_ADDSTRING, 1, (LPARAM)"Masculino");

		while (aVacuna != NULL) {
			SendMessage(GetDlgItem(hwnd, CMB_VCC), CB_ADDSTRING, indice, (LPARAM)aVacuna->nombre.c_str());
			aVacuna = aVacuna->next;
			indice++;
		}
		aVacuna = oVacuna;

		for (int i = 0; i < 12; i++) {
			SendMessage(GetDlgItem(hwnd, CMB_GOCP), CB_ADDSTRING, i, (LPARAM)ocupaciones[i].c_str());
		}

		for (int i = 0; i < 8; i++) {
			SendMessage(GetDlgItem(hwnd, CMB_PRSGO), CB_ADDSTRING, i, (LPARAM)perfiles[i].c_str());
		}

	}break;
	case WM_COMMAND: {
		opcionesDeMenu(hwnd, wParam, lParam);
		switch (LOWORD(wParam)) {
		case BTN_DDI: {
			rutaDeImg2 = cargarImagen(hwnd, BTM_DDI);
			if (rutaDeImg2.compare("") != 0) {
				imgDDI = true;
			}
		
		}break;

		case BTN_FOTOP: {
			rutaDeImg = cargarImagen(hwnd, BTM_PERSONA);
			if (rutaDeImg.compare("") != 0) {
				imgPsn = true;
			}

		}break;
		case ID_REGISTRAR_OK: {
			if (imgDDI == false) {
				MessageBox(NULL, "Falta imagen del documento.", "Error", MB_OK);
				break;
			}

			if (imgPsn == false) {
				MessageBox(NULL, "Falta foto de la persona.", "Error", MB_OK);
				break;
			}

			if (oVacuna == NULL) {
				MessageBox(NULL, "No hay ninguna vacuna registrada.", "Error", MB_OK);
				break;
			}

			HWND hDtpTiempo = GetDlgItem(hwnd, DTP_NACIMIENTO);
			int dtpSize = GetWindowTextLength(hDtpTiempo);
			char cadenaDelDTP[80];
			GetWindowText(hDtpTiempo, cadenaDelDTP, ++dtpSize);
			string fechaDTP(cadenaDelDTP);
			
			//Combo box
			string ocupaciones[12] = { "Gerente", "Profesional", "Técnico y profesionale asociado",
									"Trabajador de apoyo administrativo",
									"Trabajador de servicios y ventas",
									"Trabajador de la agricultura, la silvicultura o la pesca",
									"Trabajador de artesanú} u oficios conexos",
									"Operadores de plantas o máquinas y ensambladores",
									"Ocupaciones elementales", "Ocupaciones de las Fuerzas Armadas",
									"Estudiante", "Otro" };
			string perfiles[8] = {  "Personal sanitario", "Persona de 80 años o mas",
								    "Personas de 65 a 79 años",
								    "Personas con comorbilidades",
								    "Personas institucionalizadas",
								    "Persona escencial", "Mayores a 55 años",
								    "Publico general" };

			HWND txtNombre = GetDlgItem(hwnd, IDC_NOMBRE_PERSONA);
			HWND txtApellidoPaterno = GetDlgItem(hwnd, IDC_DIRECCION_PERSONA);
			HWND txtApellidoMaterno = GetDlgItem(hwnd, IDC_AM_PERSONA);
			HWND txtEstado = GetDlgItem(hwnd, EB_ESTADO);
			HWND txtMunicipio = GetDlgItem(hwnd, EB_CIUDAD);
			HWND intPostal = GetDlgItem(hwnd, EB_POSTAL);
			HWND txtDireccion = GetDlgItem(hwnd, EB_DIRECCION);
			HWND txtClaveUnica = GetDlgItem(hwnd, IDC_CLAVE_UNICA);

			string nombre = getText(txtNombre);
			string apellidoP = getText(txtApellidoPaterno);
			string apellidoM = getText(txtApellidoMaterno);
			string estado = getText(txtEstado);
			string municipio = getText(txtMunicipio);
			string postal = getText(intPostal);
			string direccion = getText(txtDireccion);
			string clave = getText(txtClaveUnica);
			string numero = getText(GetDlgItem(hwnd, EB_TELEFONO));
			

			string nomVcc = getText(GetDlgItem(hwnd, CMB_VCC));
			bool hayVcc = false;
			while (aVacuna != NULL) {
				if (nomVcc.compare(aVacuna->nombre) == 0) {
					hayVcc = true;
					break;
				}
				aVacuna = aVacuna->next;
			}
			aVacuna = oVacuna;

			if (!hayVcc) {
				MessageBox(NULL, "Nombre de vacuna incorrecto", "Error", MB_OK);
				break;
			}

			string sexo = getText(GetDlgItem(hwnd, CMB_SEXO));
			if (!(sexo.compare("Femenino") == 0 || sexo.compare("Masculino") == 0)) {
				MessageBox(NULL, "Datos incompletos/ Datos incorrectos", "Error", MB_OK);
				break;
			}

			string gpoRiesgo = getText(GetDlgItem(hwnd, CMB_PRSGO));
			bool bRiesgo = false;
			for (int i = 0; i < 8; i++) {
				if (perfiles[i].compare(gpoRiesgo) == 0) {
					bRiesgo = true;
					break;
				}
			}

			string oficio = getText(GetDlgItem(hwnd, CMB_GOCP));
			bool bOficio = false;
			for (int i = 0; i < 12; i++) {
				if (ocupaciones[i].compare(oficio) == 0) {
					bOficio = true;
					break;
				}
			}

			if (bRiesgo == false || bOficio == false) {
				MessageBox(NULL, "Grupo o perfil incorrecto.", "Error", MB_OK);
				break;
			}

			if (nombre.compare("") == 0 || apellidoP.compare("") == 0 || apellidoM.compare("") == 0 || estado.compare("") == 0 || municipio.compare("") == 0
				|| postal.compare("") == 0 || direccion.compare("") == 0 || clave.compare("") == 0 || numero.compare("") == 0) {
				MessageBox(NULL, "Datos incompletos/ Datos incorrectos", "Error", MB_OK);
				break;
			}

			if (!(validaciones.curpValida(clave))) {
				MessageBox(NULL, "Esa curp no es valida.", "Error", MB_OK);
				break;

			}

			if (IsDlgButtonChecked(hwnd, BR_SI) && IsDlgButtonChecked(hwnd, RB_NO)) {
				MessageBox(NULL, "Solo selecciona una opcion para la postracio.", "Error", MB_OK);
				HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_RPERSONAS), NULL, registrarPersonas);
				ShowWindow(vntnDatos, SW_SHOW);
				hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
				SetMenu(vntnDatos, hMenu);
				DestroyWindow(hwnd);
			}

			bool yaexiste = false;
			aPersona = oPersona;
			while (aPersona != NULL) {
				if (aPersona->claveUnica.compare(clave) == 0) {
					MessageBox(NULL, "No se puede repetir la persona.", "Error", MB_OK);
					yaexiste = true;
					break;

				}
				aPersona = aPersona->next;
			}
			aPersona = oPersona;

			if (yaexiste == true) {
				break;
			}

			if (oPersona == NULL) {
				//Datos generales
				oPersona = new Persona;
				oPersona->nombre.append(nombre);
				oPersona->apellidoPaterno.append(apellidoP);
				oPersona->apellidoMaterno.append(apellidoM);
				oPersona->estado.append(estado);
				oPersona->municipio.append(municipio);
				oPersona->postal = stoi(postal);
				oPersona->direccion.append(direccion);
				oPersona->claveUnica.append(clave);
				oPersona->sexo.append(sexo);
				oPersona->vacuna.append(nomVcc);
				oPersona->gpoOcpal.append(oficio);
				oPersona->gpoRiesgo.append(gpoRiesgo);
				int numcel = stoi(numero);
				oPersona->telefono = numcel;

				//Fecha 
				oPersona->nacimiento.append(fechaDTP);
				SendDlgItemMessage(hwnd, DTP_NACIMIENTO, DTM_GETSYSTEMTIME, 
				NULL, (LPARAM)& (oPersona->dtmNacimiento));

				//Checar si se intruceron fotos
				if (imgPsn == true) {
					oPersona->fotoPath.append(rutaDeImg);
					rutaDeImg = "";
					imgPsn = false;
				}
				if (imgDDI == true) {
					oPersona->docId.append(rutaDeImg2);
					rutaDeImg2 = "";
					imgDDI = false;
				}
				if (IsDlgButtonChecked(hwnd, BR_SI))
					oPersona->postracion = true;
				else
					oPersona->postracion = false;


				//Lista ligada
				oPersona->next = NULL;
				oPersona->prev = NULL;
				aPersona = oPersona;
				MessageBox(NULL, "Persona registrada exitosamente.", "Listo", MB_OK);
			
			}
			else {
				while (aPersona->next != NULL) {
					aPersona = aPersona->next;
				}
				aPersona->next = new Persona;
				aPersona->next->prev = aPersona;
				aPersona = aPersona->next;

				//Datos de la persona
				aPersona->nombre.append(nombre);
				aPersona->apellidoPaterno.append(apellidoP);
				aPersona->apellidoMaterno.append(apellidoM);
				aPersona->estado.append(estado);
				aPersona->municipio.append(municipio);
				aPersona->postal = stoi(postal);
				aPersona->direccion.append(direccion);
				aPersona->claveUnica.append(clave);
				aPersona->sexo.append(sexo);
				aPersona->vacuna.append(nomVcc);
				aPersona->gpoOcpal.append(oficio);
				aPersona->gpoRiesgo.append(gpoRiesgo);

				//Telefono
				int numcel = stoi(numero);
				aPersona->telefono = numcel;

				//Fecha 
				aPersona->nacimiento.append(fechaDTP);
				SendDlgItemMessage(hwnd, DTP_NACIMIENTO, DTM_GETSYSTEMTIME,
				NULL, (LPARAM)& (aPersona->dtmNacimiento));

				//Checar si se intruceron fotos
				if (imgPsn == true) {
					aPersona->fotoPath.append(rutaDeImg);
					rutaDeImg = "";
					imgPsn = false;
				}
				if (imgDDI == true) {
					aPersona->docId.append(rutaDeImg2);
					rutaDeImg2 = "";
					imgDDI = false;
				}

				//Botones radio
				if (IsDlgButtonChecked(hwnd, BR_SI))
					aPersona->postracion = true;
				else
					aPersona->postracion = false;

				aPersona->next = NULL;
				aPersona = oPersona;
				MessageBox(NULL, "Persona registrada exitosamente.", "Listo", MB_OK);
			}


		}break;

		case BTN_CANCELAR_R: {
			HWND vntnHome = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_PERSONAS), NULL, personas);
			hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(vntnHome, hMenu);
			ShowWindow(vntnHome, SW_SHOW);
			DestroyWindow(hwnd);
		
		}break;
		}
	}break;
	case WM_CLOSE: {

	}break;
	case WM_DESTROY: {

	}break;
	default: {
		DefWindowProc(hwnd, msg, wParam, lParam);
	}break;
	}
	return FALSE;
}
//Funcion para borrar a las personas
BOOL CALLBACK borrarPersonas(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_INITDIALOG: {
		SetWindowText(GetDlgItem(hwnd, NAME_BRRAR), nombreUserActual.c_str());

	}break;
	case WM_COMMAND: {
		opcionesDeMenu(hwnd, wParam, lParam);
		switch (LOWORD(wParam)) {
		case ID_BORRAR_PERSONA: {
			string clave = getText(GetDlgItem(hwnd, IDC_CLAVE_BORRAR));

			if (clave.compare("") == 0) {
				MessageBox(hwnd, "No has ingresado una clave.", "Listo", MB_OK);
				break;
			}

			bool encontrado = false;
			if (oPersona != NULL) {
				while (aPersona != NULL) {
					if (aPersona->claveUnica.compare(clave) == 0) {
						encontrado = true;
						break;
					}
					aPersona = aPersona->next;
				}

				if (encontrado) {
					if (aPersona->next == NULL && aPersona->prev == NULL) {
						Persona* temp = new Persona;
						temp = aPersona;
						delete temp;
						MessageBox(hwnd, "La cita de la persona fue eliminada.", "Listo", MB_OK);
						oPersona = NULL;
						aPersona = oPersona;
						break;
					}
					else if (aPersona->next == NULL && aPersona->prev != NULL) {
						Persona* temp = new Persona;
						temp = aPersona;
						temp->prev->next = NULL;
						delete temp;
						aPersona = NULL;
						aPersona = oPersona;
						MessageBox(hwnd, "La cita de la persona fue eliminada.", "Listo", MB_OK);
						break;
					
					}
					else if (aPersona->next != NULL && aPersona->prev == NULL) {
						Persona* temp = new Persona;
						temp = aPersona;
						aPersona->next->prev = NULL;
						oPersona = oPersona->next;
						delete temp;
						aPersona = NULL;
						aPersona = oPersona;
						MessageBox(hwnd, "La cita de la persona fue eliminada.", "Listo", MB_OK);
						break;
					}
					else if (aPersona->next != NULL && aPersona->prev != NULL) {
						Persona* temp = new Persona;
						temp = aPersona;
						aPersona->prev->next = aPersona->next;
						aPersona->next->prev = aPersona->prev;
						delete temp;
						aPersona = NULL;
						aPersona = oPersona;
						MessageBox(hwnd, "La cita de la persona fue eliminada.", "Listo", MB_OK);
						break;
					
					}
				
				}
				else {
					MessageBox(NULL, "La clave ingresada es incorrecta.", "Error", MB_OK);
					aPersona = oPersona;
					break;
				
				}
			
			
			}
			else {
				MessageBox(NULL, "No existe ninguna persona registrada.", "Error", MB_OK);
			
			}
		
		}break;
		case BTN_CANCELAR_BR: {
			HWND vntnHome = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_PERSONAS), NULL, personas);
			hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(vntnHome, hMenu);
			ShowWindow(vntnHome, SW_SHOW);
			DestroyWindow(hwnd);

		}break;
		}
	}break;
	
	case MN_SALIR: {
		if (HIWORD(wParam) == BN_CLICKED) {
			int i = MessageBox(hwnd, "¿Estas seguro de que quieres salir?", "Salir", MB_YESNO);
			if (i == IDYES) {
				PostQuitMessage(0);
			}
			else {
				break;
			}
		}

	}break;
	case WM_CLOSE: {

	}break;
	case WM_DESTROY: {

	}break;
	default: {
		DefWindowProc(hwnd, msg, wParam, lParam);
	}break;
	}
	return FALSE;
}
//Funcion para editar los datos de las personas
BOOL CALLBACK editarPersonas(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_INITDIALOG: {
		rutaDeImg = "";
		rutaDeImg2 = "";
		SetWindowText(GetDlgItem(hwnd, NAME_EPS), nombreUserActual.c_str());

		//Enviar los datos a la ventana
		string ocupaciones[12] = { "Gerente", "Profesional", "Técnico y profesionale asociado",
									"Trabajador de apoyo administrativo",
									"Trabajador de servicios y ventas",
									"Trabajador de la agricultura, la silvicultura o la pesca",
									"Trabajador de artesanú} u oficios conexos",
									"Operadores de plantas o máquinas y ensambladores",
									"Ocupaciones elementales", "Ocupaciones de las Fuerzas Armadas",
									"Estudiante", "Otro" };
		string perfiles[8] = { "Personal sanitario", "Persona de 80 años o mas",
									"Personas de 65 a 79 años",
									"Personas con comorbilidades",
									"Personas institucionalizadas",
									"Persona escencial", "Mayores a 55 años",
									"Publico general" };

		SendMessage(GetDlgItem(hwnd, CMB_SEXO_E), CB_ADDSTRING, 0, (LPARAM)"Femenino");
		SendMessage(GetDlgItem(hwnd, CMB_SEXO_E), CB_ADDSTRING, 1, (LPARAM)"Masculino");

		int indice = 0;

		while (aVacuna != NULL) {
			SendMessage(GetDlgItem(hwnd, CMB_VCC_E), CB_ADDSTRING, indice, (LPARAM)aVacuna->nombre.c_str());
			aVacuna = aVacuna->next;
			indice++;
		}
		aVacuna = oVacuna;

		for (int i = 0; i < 12; i++) {
			SendMessage(GetDlgItem(hwnd, CMB_GOCP_E), CB_ADDSTRING, i, (LPARAM)ocupaciones[i].c_str());
		}

		for (int i = 0; i < 8; i++) {
			SendMessage(GetDlgItem(hwnd, CMB_PRSGO_E), CB_ADDSTRING, i, (LPARAM)perfiles[i].c_str());
		}

	}break;
	case WM_COMMAND: {
		opcionesDeMenu(hwnd, wParam, lParam);
		switch (LOWORD(wParam)) {
		case BTN_FOTOP_E: {
			
			rutaDeImg = cargarImagen(hwnd, BTM_EPSNA);
		}break;
		case BTN_DDI_E: {
			rutaDeImg2 = cargarImagen(hwnd, BTM_DDI_E);
		}break;
		case IDC_BUSCAR: {
			claveTemp = "";
			claveTemp = getText(GetDlgItem(hwnd, IDC_BUCAR_CLAVE));
			if (claveTemp.compare("") == 0) {
				MessageBox(NULL, "Campo vacio.", "Error", MB_OK);
				break;
			}
			bool encontrar = false;

			bool enUso = false;
			aPersona = oPersona;
			while (aPersona != NULL) {
				if (aPersona->claveUnica.compare(claveTemp) == 0) {
					aCarnet = oCarnet;
					while (aCarnet != NULL) {
						if (aCarnet->claveUnica.compare(aPersona->claveUnica)){
							enUso = true;
						}
						aCarnet = aCarnet->next;
					}
					aCarnet = oCarnet;
					if (enUso) {
						break;
					}
				}
				aPersona = aPersona->next;
			}
			aPersona = oPersona;

			if (enUso) {
				MessageBox(NULL, "Al hacer cambios toma en cuenta que existen carnets asociados a esta persona.", "Error", MB_OK);
			}

			if (oPersona != NULL) {
				while (aPersona != NULL) {
					if (aPersona->claveUnica.compare(claveTemp) == 0) {
						encontrar = true;
						break;
					}
					aPersona = aPersona->next;
				}
				if (encontrar) {
					//Datos generales
					SendMessage(GetDlgItem(hwnd, EB_E_NOMBRE), WM_SETTEXT, NULL, (LPARAM)aPersona->nombre.c_str());
					SendMessage(GetDlgItem(hwnd, EB_E_AP), WM_SETTEXT, NULL, (LPARAM)aPersona->apellidoPaterno.c_str());
					SendMessage(GetDlgItem(hwnd, EB_E_AM), WM_SETTEXT, NULL, (LPARAM)aPersona->apellidoMaterno.c_str());
					SendMessage(GetDlgItem(hwnd, EB_CU_E), WM_SETTEXT, NULL, (LPARAM)aPersona->claveUnica.c_str());
					SendMessage(GetDlgItem(hwnd, EB_E_ESDO), WM_SETTEXT, NULL, (LPARAM)aPersona->estado.c_str());
					SendMessage(GetDlgItem(hwnd, EB_E_CIUDAD), WM_SETTEXT, NULL, (LPARAM)aPersona->municipio.c_str());

					//Codigo postal
					string postal = to_string(aPersona->postal);
					SendMessage(GetDlgItem(hwnd, EB_E_POSTAL), WM_SETTEXT, NULL, (LPARAM)postal.c_str());

					//Datos generales
					SendMessage(GetDlgItem(hwnd, EB_E_DIRECCION), WM_SETTEXT, NULL, (LPARAM)aPersona->direccion.c_str());
					SendMessage(GetDlgItem(hwnd, CMB_SEXO_E), WM_SETTEXT, NULL, (LPARAM)aPersona->sexo.c_str());
					SendMessage(GetDlgItem(hwnd, CMB_VCC_E), WM_SETTEXT, NULL, (LPARAM)aPersona->vacuna.c_str());
					SendMessage(GetDlgItem(hwnd, CMB_GOCP_E), WM_SETTEXT, NULL, (LPARAM)aPersona->gpoOcpal.c_str());
					SendMessage(GetDlgItem(hwnd, CMB_PRSGO_E), WM_SETTEXT, NULL, (LPARAM)aPersona->gpoRiesgo.c_str());

					//Telefono
					string telefono = to_string(aPersona->telefono);
					SendMessage(GetDlgItem(hwnd, EB_TELEFONO_E), WM_SETTEXT, NULL, (LPARAM)telefono.c_str());

					//Fecha de nacimiento
					DateTime_SetSystemtime(GetDlgItem(hwnd, DTP_NACIMIENTO_E), GDT_VALID, (LPARAM)&aPersona->dtmNacimiento);

					//Foto de la persona
					HBITMAP hbImagenUser = (HBITMAP)LoadImage(NULL, aPersona->fotoPath.c_str(), IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
					HWND hPbImagen = GetDlgItem(hwnd, BTM_EPSNA);
					SendMessage(hPbImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbImagenUser);

					//Foto de su documento
					HBITMAP hbImagenUser2 = (HBITMAP)LoadImage(NULL, aPersona->docId.c_str(), IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
					HWND hPbImagen2 = GetDlgItem(hwnd, BTM_DDI_E);
					SendMessage(hPbImagen2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbImagenUser2);

					//Botones radio de si tiene postracion
					if (aPersona->postracion == true) {
						SendMessage(GetDlgItem(hwnd, RB_SI_E), BM_SETCHECK, BST_CHECKED, 0);
					}else{
						SendMessage(GetDlgItem(hwnd, EB_NO_E), BM_SETCHECK, BST_CHECKED, 0);
					}

					//Se verifica si fue ingresado algo
					MessageBox(hwnd, "Listo.", "Encontrado", MB_OK);
					ingresado = true;
					aPersona = oPersona;
					break;
				}
				else {
					MessageBox(hwnd, "La clave ingresada no coincide con ninguna de las registradas.", "Not found", MB_OK);
					ingresado = false;
					aPersona = oPersona;
					break;
				}
			}
			else {
				MessageBox(hwnd, "No hay ninguna persona registrada.", "Not found", MB_OK);
				ingresado = false;
			}
		
		}break;
		case ID_EDITAR_OK: {
			if (ingresado) {

				string ocupaciones[12] = {  "Gerente", "Profesional", "Técnico y profesionale asociado",
											"Trabajador de apoyo administrativo",
											"Trabajador de servicios y ventas",
											"Trabajador de la agricultura, la silvicultura o la pesca",
											"Trabajador de artesanú} u oficios conexos",
											"Operadores de plantas o máquinas y ensambladores",
											"Ocupaciones elementales", "Ocupaciones de las Fuerzas Armadas",
											"Estudiante", "Otro" };
				string perfiles[8] = {		"Personal sanitario", "Persona de 80 años o mas",
											"Personas de 65 a 79 años",
											"Personas con comorbilidades",
											"Personas institucionalizadas",
											"Persona escencial", "Mayores a 55 años",
											"Publico general" };

				
				
				string claveUnica = getText(GetDlgItem(hwnd, EB_CU_E));
				
				aPersona = oPersona;

				if (!(validaciones.curpValida(claveUnica))) {
					MessageBox(hwnd, "Curp no valida.", "Not found", MB_OK);
					aPersona = oPersona;
					break;
				}




				//-----------------------------------
				string nombre = getText(GetDlgItem(hwnd, EB_E_NOMBRE));
				string appelidoP = getText(GetDlgItem(hwnd, EB_E_AP));
				string appelidoM = getText(GetDlgItem(hwnd, EB_E_AM));
				string claveU = getText(GetDlgItem(hwnd, EB_CU_E));
				string estado = getText(GetDlgItem(hwnd, EB_E_ESDO));
				string municipio = getText(GetDlgItem(hwnd, EB_E_CIUDAD));
				string postal = getText(GetDlgItem(hwnd, EB_E_POSTAL));
				string direccion = getText(GetDlgItem(hwnd, EB_E_DIRECCION));
				string telefono = getText(GetDlgItem(hwnd, EB_TELEFONO_E));
				string sexo = getText(GetDlgItem(hwnd, CMB_SEXO_E));
				string gOcp = getText(GetDlgItem(hwnd, CMB_GOCP_E));
				string pRiesgo = getText(GetDlgItem(hwnd, CMB_PRSGO_E));
				string vacuna = getText(GetDlgItem(hwnd, CMB_VCC_E));

				if (nombre.compare("") == 0 || appelidoP.compare("") == 0 || appelidoM.compare("") == 0 || estado.compare("") == 0 || municipio.compare("") == 0
					|| postal.compare("") == 0 || direccion.compare("") == 0 || claveU.compare("") == 0 || telefono.compare("") == 0 || sexo.compare("") == 0
					|| gOcp.compare("") == 0 || pRiesgo.compare("") == 0 || vacuna.compare("") == 0 ) {
					MessageBox(NULL, "Datos incompletos/ Datos incorrectos", "Error", MB_OK);
					break;
				}


				string nomVcc = getText(GetDlgItem(hwnd, CMB_VCC_E));
				bool hayVcc = false;
				aVacuna = oVacuna;
				while (aVacuna != NULL) {
					if (nomVcc.compare(aVacuna->nombre) == 0) {
						hayVcc = true;
						break;
					}
					aVacuna = aVacuna->next;
				}
				aVacuna = oVacuna;

				if (!hayVcc) {
					MessageBox(NULL, "Nombre de vacuna incorrecto", "Error", MB_OK);
					break;
				}

				if (!(sexo.compare("Femenino") == 0 || sexo.compare("Masculino") == 0)) {
					MessageBox(NULL, "Datos incompletos/ Datos incorrectos", "Error", MB_OK);
					break;
				}

				string gpoRiesgo = getText(GetDlgItem(hwnd, CMB_PRSGO_E));
				bool bRiesgo = false;
				for (int i = 0; i < 8; i++) {
					if (perfiles[i].compare(gpoRiesgo) == 0) {
						bRiesgo = true;
						break;
					}
				}

				string oficio = getText(GetDlgItem(hwnd, CMB_GOCP_E));
				bool bOficio = false;
				for (int i = 0; i < 12; i++) {
					if (ocupaciones[i].compare(oficio) == 0) {
						bOficio = true;
						break;
					}
				}

				if (bRiesgo == false) {
					MessageBox(NULL, "Datos incompletos/ Datos incorrectos", "Error", MB_OK);
					break;
				}

				if (bRiesgo == false || bOficio == false) {
					MessageBox(NULL, "Grupo o perfil incorrecto.", "Error", MB_OK);
					break;
				}
				
				if (!(validaciones.curpValida(claveU))) {
					MessageBox(NULL, "Esa curp no es valida.", "Error", MB_OK);
					break;

				}

				if (IsDlgButtonChecked(hwnd, RB_SI_E) && IsDlgButtonChecked(hwnd, EB_NO_E)) {
					MessageBox(NULL, "Solo selecciona una opcion para la postracio.", "Error", MB_OK);
					aPersona = oPersona;
					aCarnet = oCarnet;
					aVacuna = oVacuna;
					HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_EPERSONAS), NULL, editarPersonas);
					ShowWindow(vntnDatos, SW_SHOW);
					hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
					SetMenu(vntnDatos, hMenu);
					DestroyWindow(hwnd);
				}

				if (!(IsDlgButtonChecked(hwnd, RB_SI_E)) && !(IsDlgButtonChecked(hwnd, EB_NO_E))) {
					MessageBox(NULL, "Selecciona un estado de postracion.", "Error", MB_OK);
					aPersona = oPersona;
					aCarnet = oCarnet;
					aVacuna = oVacuna;
					HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_EPERSONAS), NULL, editarPersonas);
					ShowWindow(vntnDatos, SW_SHOW);
					hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
					SetMenu(vntnDatos, hMenu);
					DestroyWindow(hwnd);
				}
				//-----------------------------------


				bool repetido = false;
				int vecesRep = 0;
				while (aPersona != NULL) {
					if (aPersona->claveUnica.compare(claveUnica) == 0) {
						if (vecesRep > 1) {
							MessageBox(hwnd, "No se pueden repetir claves.", "Not found", MB_OK);
							repetido = true;
							aPersona = oPersona;
							break;
						}
						vecesRep++;
					}
					aPersona = aPersona->next;
				}

				if (repetido) {
					break;
				}

				
				
				aPersona = oPersona;
				while (aPersona != NULL) {
					if (aPersona->claveUnica.compare(claveTemp) == 0) {
						break;
					}
					aPersona = aPersona->next;
				}

				//Bitmaps
				if (!(rutaDeImg.compare("") == 0)) {
					aPersona->fotoPath = rutaDeImg;
				}

				if (!(rutaDeImg2.compare("") == 0)) {
					aPersona->docId = rutaDeImg2;
				}

				//Datos generales
				aPersona->nombre = getText(GetDlgItem(hwnd, EB_E_NOMBRE));
				aPersona->apellidoPaterno = getText(GetDlgItem(hwnd, EB_E_AP));
				aPersona->apellidoMaterno = getText(GetDlgItem(hwnd, EB_E_AM));
				aPersona->estado = getText(GetDlgItem(hwnd, EB_E_ESDO));
				aPersona->municipio = getText(GetDlgItem(hwnd, EB_E_CIUDAD));

				//Codigo postal
				int cPostal = stoi(getText(GetDlgItem(hwnd, EB_E_POSTAL)));
				aPersona->postal = cPostal;

				//Datos generales
				aPersona->claveUnica = getText(GetDlgItem(hwnd, EB_CU_E));
				aPersona->direccion = getText(GetDlgItem(hwnd, EB_E_DIRECCION));
				aPersona->sexo = getText(GetDlgItem(hwnd, CMB_SEXO_E));
				aPersona->vacuna = getText(GetDlgItem(hwnd, CMB_VCC_E));
				aPersona->gpoOcpal = getText(GetDlgItem(hwnd, CMB_GOCP_E));
				aPersona->gpoRiesgo = getText(GetDlgItem(hwnd, CMB_PRSGO_E));

				//Numero de telefono
				string numeroCel = getText(GetDlgItem(hwnd, EB_TELEFONO_E));
				aPersona->telefono = stoi(numeroCel);

				//Fecha
				HWND hDtpTiempo = GetDlgItem(hwnd, DTP_NACIMIENTO_E);
				int dtpSize = GetWindowTextLength(hDtpTiempo);
				char cadenaDelDTP[80];
				GetWindowText(hDtpTiempo, cadenaDelDTP, ++dtpSize);
				string fechaDTP(cadenaDelDTP);
				aPersona->nacimiento = fechaDTP;
				SendDlgItemMessage(hwnd, DTP_NACIMIENTO_E, DTM_GETSYSTEMTIME,
				NULL, (LPARAM)& (aPersona->dtmNacimiento));

				
				//Botones radio de si esta en postracion o no
				if (IsDlgButtonChecked(hwnd, RB_SI_E))
					aPersona->postracion = true;
				else
					aPersona->postracion = false;

				//Salida de la ejecucion
				rutaDeImg = "";
				rutaDeImg2 = "";
				aPersona = oPersona;
				ingresado = false;
				MessageBox(hwnd, "Persona editada con exito.", "Listo", MB_OK);
				break;

			}
			else {
				MessageBox(hwnd, "Ingresa una clave nuevamente.", "No encontrado", MB_OK);

			}
			//Se reinicia el bool 
			ingresado = false;
		
		}break;
		case BTN_CANCELAR_E: {
			HWND vntnHome = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_PERSONAS), NULL, personas);
			hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(vntnHome, hMenu);
			ShowWindow(vntnHome, SW_SHOW);
			DestroyWindow(hwnd);

		}break;
		}
	}break;
	
	case WM_CLOSE: {

	}break;
	case WM_DESTROY: {

	}break;
	default: {
		DefWindowProc(hwnd, msg, wParam, lParam);
	}break;
	}
	return FALSE;


}
//Funcion para el inicio de las vacunas
BOOL CALLBACK vacunasHome(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_INITDIALOG: {
		SetWindowText(GetDlgItem(hwnd, NAME_VACINI), nombreUserActual.c_str());
		aVacuna = oVacuna;
		if (aVacuna != NULL) {
			while (aVacuna != NULL) {
				string vacuna = aVacuna->nombre + " | Clave: " + aVacuna->clave + " | Numero de dosis: " + to_string(aVacuna->numeroDeDosis);
				SendMessageA(GetDlgItem(hwnd, LISTA_VACUNAS), LB_ADDSTRING, NULL, (LPARAM)vacuna.c_str());
				aVacuna = aVacuna->next;
			}
			aVacuna = oVacuna;
		}
		aVacuna = oVacuna;
	}break;
	case WM_COMMAND: {
		opcionesDeMenu(hwnd, wParam, lParam);
		switch (LOWORD(wParam)) {
		case ID_AGREGAR_VACUNAS: {
			HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(DLG_AGG_VACUNA), NULL, altaVacunas);
			ShowWindow(vntnDatos, SW_SHOW);
			hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(vntnDatos, hMenu);
			DestroyWindow(hwnd);
		
		}break;
		case ID_EDITAR_VACUNAS: {
			HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(DLG_ED_VACUNA), NULL, editarVacunas);
			ShowWindow(vntnDatos, SW_SHOW);
			hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(vntnDatos, hMenu);
			DestroyWindow(hwnd);
		}break;
		case ID_BORRAR_VACUNAS: {
			HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(DLG_BORR_VAC), NULL, bajaVacunas);
			ShowWindow(vntnDatos, SW_SHOW);
			hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(vntnDatos, hMenu);
			DestroyWindow(hwnd);
		}break;
		}
	}break;
	case WM_CLOSE: {

	}break;
	case WM_DESTROY: {

	}break;
	default: {
		DefWindowProc(hwnd, msg, wParam, lParam);
	}break;
	}
	return FALSE;
}
//Funcion ara dar vacunas de alta
BOOL CALLBACK altaVacunas(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_INITDIALOG: {
		SetWindowText(GetDlgItem(hwnd, NAME_AV), nombreUserActual.c_str());

	}break;
	case WM_COMMAND: {
		opcionesDeMenu(hwnd, wParam, lParam);
		switch (LOWORD(wParam)) {
		case BNT_AGG_VAC: {	
			HWND txtNombre = GetDlgItem(hwnd, EB_NOM_VAC);
			HWND txtClave = GetDlgItem(hwnd, EB_CLAVE_VAC);
			HWND fltPrecio = GetDlgItem(hwnd, EB_PRECIO_VAC);
			HWND intDosis = GetDlgItem(hwnd, EB_DOSIS_VAC);
			HWND txtDescripcion = GetDlgItem(hwnd, EB_DESC_VAC);

			string nombre = getText(txtNombre);
			string clave = getText(txtClave);
			string precio = getText(fltPrecio);
			string dosis = getText(intDosis);
			string descripcion = getText(txtDescripcion);
			

			if (nombre.compare("") == 0 || clave.compare("") == 0 || precio.compare("") == 0 || dosis.compare("") == 0 
				|| descripcion.compare("") == 0) {
				MessageBox(NULL, "Datos incompletos/ Datos incorrectos", "Error", MB_OK);
				break;
			}
			
			if (!(validaciones.sonDigitos(precio))) {
				MessageBox(NULL, "Solo digitos en el precio.", "Error", MB_OK);
				break;

			}

			if (validaciones.hayEspacios(clave)) {
				MessageBox(NULL, "No debe de haber espacios en la clave.", "Error", MB_OK);
				break;
			}

			if (!(validaciones.sonDigitos(dosis))) {
				MessageBox(NULL, "Las doses deben ser digitos.", "Error", MB_OK);
				break;
			
			}

			if (stoi(dosis) > 4 ) {
				MessageBox(NULL, "No puede haber mas de 4 dosis.", "Error", MB_OK);
				break;
			
			}

			if (stoi(dosis) <= 0) {
				MessageBox(NULL, "Debe haber almenos una dosis.", "Error", MB_OK);
				break;

			}

			

			if (stof(precio) <= 0) {
				MessageBox(NULL, "El precio debe de ser mayor a 0.", "Error", MB_OK);
				break;
			}



			bool yaexiste = false;
			aVacuna = oVacuna;
			while (aVacuna != NULL) {
				if (aVacuna->clave.compare(clave) == 0 || aVacuna->nombre.compare(nombre) == 0) {
					MessageBox(NULL, "No se puede repetir la vacuna.", "Error", MB_OK);
					yaexiste = true;
					break;

				}
				
				aVacuna = aVacuna->next;
			}
			aVacuna = oVacuna;

			if (yaexiste == true) {
				break;
			}

			if (oVacuna == NULL) {
				oVacuna = new Vacuna;
				oVacuna->nombre.append(nombre);
				oVacuna->clave.append(clave);
				oVacuna->numeroDeDosis = stoi(dosis);
				oVacuna->descripcion.append(descripcion);
				oVacuna->precio = stof(precio);
				oVacuna->next = NULL;
				oVacuna->prev = NULL;
				aVacuna = oVacuna;
				MessageBox(NULL, "Vacuna registrada exitosamente.", "Listo", MB_OK);
			}
			else if (oVacuna != NULL) {
				while (aVacuna->next != NULL) {
					aVacuna = aVacuna->next;
				}
				aVacuna->next = new Vacuna;
				aVacuna->next->prev = aVacuna;
				aVacuna = aVacuna->next;
				aVacuna->nombre.append(nombre);
				aVacuna->clave.append(clave);
				aVacuna->numeroDeDosis = stoi(dosis);
				aVacuna->descripcion.append(descripcion);
				aVacuna->precio = stof(precio);
				aVacuna->next = NULL;
				aVacuna = oVacuna;
				MessageBox(NULL, "Vacuna registrada exitosamente.", "Listo", MB_OK);
			
			}
		
		}break;

		case ID_C_A_VAC: {
			HWND vntnHome = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_VACUNAS), NULL, vacunasHome);
			hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(vntnHome, hMenu);
			ShowWindow(vntnHome, SW_SHOW);
			DestroyWindow(hwnd);
		}break;
		}
	}break;
	case WM_CLOSE: {

	}break;
	case WM_DESTROY: {

	}break;
	default: {
		DefWindowProc(hwnd, msg, wParam, lParam);
	}break;
	}
	return FALSE;
}
//Funciones para dar vacunas de baja
BOOL CALLBACK bajaVacunas(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_INITDIALOG: {
		SetWindowText(GetDlgItem(hwnd, NAME_BV), nombreUserActual.c_str());

	}break;
	case WM_COMMAND: {
		opcionesDeMenu(hwnd, wParam, lParam);
		switch (LOWORD(wParam)) {
		case BTN_BORR_VAC:{
			string clave = getText(GetDlgItem(hwnd, EB_CLAVE_VAC_BR));
			if (clave.compare("") == 0) {
				MessageBox(hwnd, "No has ingresado una clave.", "Listo", MB_OK);
				break;
			}

			bool enUso = false;;
			aVacuna = oVacuna;
			while (aVacuna != NULL) {
				if (aVacuna->clave.compare(clave) == 0) {
					//Buscamos entre todas las personas si usan esa vacuna
					aPersona = oPersona;
					while (aPersona != NULL) {
						if (aPersona->nombre.compare(aPersona->vacuna) == 0) {
							enUso = true;
							break;
						}
						aPersona = aPersona->next;
					}
					aPersona = oPersona;
					if (enUso) {
						break;
					}
				}
				aVacuna = aVacuna->next;
			}
			aVacuna = oVacuna;

			if (enUso) {
				MessageBox(hwnd, "Borra las personas registradas con esta vacuna o modifica la que van a tomar.", "Listo", MB_OK);
				break;
			}

			bool encontrado = false;
			if (oVacuna != NULL) {
				while (aVacuna != NULL) {
					if (aVacuna->clave.compare(clave) == 0) {
						encontrado = true;
						break;
					}
					aVacuna = aVacuna->next;
				}

				if (encontrado) {
					if (aVacuna->next == NULL && aVacuna->prev == NULL) {
						Vacuna* temp = new Vacuna;
						temp = aVacuna;
						delete temp;
						MessageBox(hwnd, "La vacuna fue eliminada.", "Listo", MB_OK);
						oVacuna = NULL;
						aVacuna = oVacuna;
						break;
					}
					else if (aVacuna->next == NULL && aVacuna->prev != NULL) {
						Vacuna* temp = new Vacuna;
						temp = aVacuna;
						temp->prev->next = NULL;
						delete temp;
						aVacuna = NULL;
						aVacuna = oVacuna;
						MessageBox(hwnd, "La vacuna fue eliminada.", "Listo", MB_OK);
						break;

					}
					else if (aVacuna->next != NULL && aVacuna->prev == NULL) {
						Vacuna* temp = new Vacuna;
						temp = aVacuna;
						aVacuna->next->prev = NULL;
						oVacuna = oVacuna->next;
						delete temp;
						aVacuna = NULL;
						aVacuna = oVacuna;
						MessageBox(hwnd, "La vacuna fue eliminada.", "Listo", MB_OK);
						break;
					}
					else if (aVacuna->next != NULL && aVacuna->prev != NULL) {
						Vacuna* temp = new Vacuna;
						temp = aVacuna;
						aVacuna->prev->next = aVacuna->next;
						aVacuna->next->prev = aVacuna->prev;
						delete temp;
						aVacuna = NULL;
						aVacuna = oVacuna;
						MessageBox(hwnd, "La vacuna fue eliminada.", "Listo", MB_OK);
						break;

					}
				}
				else {
					MessageBox(hwnd, "No hay ninguna vacuna registrada con esa clave.", "Error", MB_OK);
					break;
				}

			}
			else {
				MessageBox(hwnd, "No hay ninguna vacuna registrada", "Error", MB_OK);
				break;
			
			}
		}break;

		case BTN_CANCEL_BOR: {
			HWND vntnHome = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_VACUNAS), NULL, vacunasHome);
			hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(vntnHome, hMenu);
			ShowWindow(vntnHome, SW_SHOW);
			DestroyWindow(hwnd);
		}break;
		}
	}break;
	case WM_CLOSE: {

	}break;
	case WM_DESTROY: {

	}break;
	default: {
		DefWindowProc(hwnd, msg, wParam, lParam);
	}break;
	}
	return FALSE;
}
//Funcion para editar la informacion de las vacunas
BOOL CALLBACK editarVacunas(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_INITDIALOG: {
		SetWindowText(GetDlgItem(hwnd, NAME_EV), nombreUserActual.c_str());

	}break;
	case WM_COMMAND: {
		opcionesDeMenu(hwnd, wParam, lParam);
		switch (LOWORD(wParam)) {
			case BTN_BSCR_VAC : {
				claveTemporal = "";
				claveTemporal = getText(GetDlgItem(hwnd, EB_CLAVED_VAC));
				if (claveTemporal.compare("") == 0) {
					MessageBox(NULL, "Campo vacio.", "Error", MB_OK);
					break;
				}
				bool encontrar = false;

				if (oVacuna != NULL) {
					while (aVacuna != NULL) {
						if (aVacuna->clave.compare(claveTemporal) == 0) {
							encontrar = true;
							break;
						}
						aVacuna = aVacuna->next;
					}
					if (encontrar) {
						SendMessage(GetDlgItem(hwnd, EB_ED_NOMBRE), WM_SETTEXT, NULL, (LPARAM)aVacuna->nombre.c_str());
						SendMessage(GetDlgItem(hwnd, EB_ED_CLAVE), WM_SETTEXT, NULL, (LPARAM)aVacuna->clave.c_str());
						string numeroDe = to_string(aVacuna->numeroDeDosis);
						SendMessage(GetDlgItem(hwnd, EB_VED_CDD), WM_SETTEXT, NULL, (LPARAM)numeroDe.c_str());
						string precioDe = to_string(aVacuna->precio);
						SendMessage(GetDlgItem(hwnd, EB_V_ED_PRECIO), WM_SETTEXT, NULL, (LPARAM)precioDe.c_str());
						SendMessage(GetDlgItem(hwnd, EB_VED_DESC), WM_SETTEXT, NULL, (LPARAM)aVacuna->descripcion.c_str());
						acceder = true;
						aVacuna = oVacuna;
						break;
					}
					else {
						MessageBox(hwnd, "La clave ingresada no coincide con ninguna de las registradas.", "Not found", MB_OK);
						acceder = false;
						aVacuna = oVacuna;
						break;
					}
				}
				else {
					MessageBox(hwnd, "No hay ninguna persona regsitrada.", "Not found", MB_OK);
					acceder = false;
				}
			
			}break;

			case BTN_EDITA_VAC: {
				if (acceder) {
					string nombre = getText(GetDlgItem(hwnd, EB_ED_NOMBRE));
					bool enUso = false;
					while (aVacuna != NULL) {
						if (aVacuna->clave.compare(claveTemporal) == 0) {
							aPersona = oPersona;
							while (aPersona != NULL) {
								if (aPersona->vacuna.compare(nombre) == 0) {
									enUso = true;
									break;
								}
								aPersona = aPersona->next;
							}
							aPersona = oPersona;

							aCarnet = oCarnet;
							while (aCarnet != NULL) {
								if (aCarnet->vacuna.compare(nombre) == 0) {
									enUso = true;
									break;
								}
								aCarnet = aCarnet->next;
							}
							aCarnet = oCarnet;

							if (enUso) {
								break;
							}
						}
						aVacuna = aVacuna->next;
					}
					aVacuna = oVacuna;

					if (enUso) {
						MessageBox(hwnd, "Elimina los carnets y las personas relacionados a la vacuna primero.", "Error", MB_OK);
						acceder = false;
						break;
					}

					while (aVacuna != NULL) {
						if (aVacuna->clave.compare(claveTemporal) == 0) {
							break;
						}
						aVacuna = aVacuna->next;
					}
					
					aVacuna->nombre = getText(GetDlgItem(hwnd, EB_ED_NOMBRE));
					aVacuna->clave = getText(GetDlgItem(hwnd, EB_ED_CLAVE));
					aVacuna->numeroDeDosis = stoi(getText(GetDlgItem(hwnd, EB_VED_CDD)));
					aVacuna->precio = stof(getText(GetDlgItem(hwnd, EB_V_ED_PRECIO)));
					aVacuna->descripcion = getText(GetDlgItem(hwnd, EB_VED_DESC));
					aVacuna = oVacuna;
					MessageBox(hwnd, "Persona editada con exito.", "No encontrado", MB_OK);
					break;

				}
				else {
					MessageBox(hwnd, "No has ingresado ninguna clave valida/ Ingresala de nuevo", "No encontrado", MB_OK);

				}
				acceder = false;
			
			
			}break;


			case BTN_CANCEL_EDVA: {
				HWND vntnHome = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_VACUNAS), NULL, vacunasHome);
				hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
				SetMenu(vntnHome, hMenu);
				ShowWindow(vntnHome, SW_SHOW);
				DestroyWindow(hwnd);
			}break;
		}
	}break;
	case WM_CLOSE: {

	}break;
	case WM_DESTROY: {

	}break;
	default: {
		DefWindowProc(hwnd, msg, wParam, lParam);
	}break;
	}
	return FALSE;
}
//Funcion para mostar un panel general de los carnet
BOOL CALLBACK carnetsHome(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_INITDIALOG: {
		aCarnet = oCarnet;
		if (aCarnet != NULL) {
			while (aCarnet != NULL) {
				string carnet = aCarnet->nombre + " | Clave Unica: " + aCarnet->claveUnica + " | ID: " + to_string(aCarnet->ID);
				SendMessageA(GetDlgItem(hwnd, IDC_CARNETS), LB_ADDSTRING, NULL, (LPARAM)carnet.c_str());
				aCarnet = aCarnet->next;
			}
			aCarnet = oCarnet;
		}
		aCarnet = oCarnet;

	}break;
	case WM_COMMAND: {
		opcionesDeMenu(hwnd, wParam, lParam);
		switch (LOWORD(wParam)) {
			//Buscar
		case BTN_CNT_SR: {
			HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_BUSCAR_CARNET), NULL, buscarCarnet);
			ShowWindow(vntnDatos, SW_SHOW);
			hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(vntnDatos, hMenu);
			DestroyWindow(hwnd);
		}break;
			//Crear
		case BTM_CNTR: {
			HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_ALTA_CARNET), NULL, altaCarnet);
			ShowWindow(vntnDatos, SW_SHOW);
			hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(vntnDatos, hMenu);
			DestroyWindow(hwnd);
		}break;
			//Editar
		case BTN_CNT_C: {
			HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_CARNET_CU), NULL, buscarCarnetEditar);
			ShowWindow(vntnDatos, SW_SHOW);
			hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(vntnDatos, hMenu);
			DestroyWindow(hwnd);
		}break;
			//Borrar
		case BTN_CNT_B: {
			HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_ELIMINAR_CARNET), NULL, bajaCarnet);
			ShowWindow(vntnDatos, SW_SHOW);
			hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(vntnDatos, hMenu);
			DestroyWindow(hwnd);
		}break;
		}
	}break;
	case WM_CLOSE: {

	}break;
	case WM_DESTROY: {

	}break;
	default: {
		DefWindowProc(hwnd, msg, wParam, lParam);
	}break;
	}
	return FALSE;
}
//Funcion para dar de alta un carnet utilizando el nombre de la persona y si es necesaria su Clave Unica
BOOL CALLBACK altaCarnet(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_INITDIALOG: {


	}break;
	case WM_COMMAND: {
		opcionesDeMenu(hwnd, wParam, lParam);
		switch (LOWORD(wParam)) {
		case BTN_CUGENERAR: {

			if (oPersona == NULL) {
				MessageBox(NULL, "No existe niguna persona registrada.", "Error", MB_OK);
				break;
			}

			if (oVacuna == NULL) {
				MessageBox(NULL, "No existe niguna vacuna registrada.", "Error", MB_OK);
				break;
			}

			string nombre = getText(GetDlgItem(hwnd, EB_NM_SR));
			string apellidoP = getText(GetDlgItem(hwnd, EB_AP_SR));
			string apellidoM = getText(GetDlgItem(hwnd, EB_AM_SR));
			string curp = getText(GetDlgItem(hwnd, EB_CU_SR_ECU));

			if (nombre.compare("") == 0 || apellidoM.compare("") == 0
				|| apellidoP.compare("") == 0 || curp.compare("") == 0) {
				MessageBox(NULL, "Ningun campo debe de estar vacio.", "Error", MB_OK);
				break;
			}

			bool encontrado = false;
			int i = 0;
			while (aPersona != NULL) {
				if (aPersona->nombre.compare(nombre) == 0 && aPersona->apellidoPaterno.compare(apellidoP) == 0
					&& aPersona->apellidoMaterno.compare(apellidoM) == 0 && aPersona->claveUnica.compare(curp) == 0) {
					encontrado = true;
					break;
				}
				aPersona = aPersona->next;
			}

			aPersona = oPersona;
			if (!encontrado) {
				MessageBox(NULL, "No se encontro ninguna persona registrada a ese nombre.", "Error", MB_OK);
				break;
			}
			bool existe = false;
			while (aCarnet != NULL) {
				if (curp.compare(aCarnet->claveUnica) == 0) {
					existe = true;
					break;
				}
				aCarnet = aCarnet->next;
			}
			aCarnet = oCarnet;
			if (existe) {
				MessageBox(NULL, "Ya existe un carnet para esa persona.", "Error", MB_OK);
				existe = true;
				aCarnet = oCarnet;
				aPersona = oPersona;
				break;
			}

			aPersona = oPersona;

			while (aPersona != NULL) {
				if (aPersona->nombre.compare(nombre) == 0 && aPersona->apellidoPaterno.compare(apellidoP) == 0
					&& aPersona->apellidoMaterno.compare(apellidoM) == 0 && aPersona->claveUnica.compare(curp) == 0) {
					break;
				}
				aPersona = aPersona->next;
			}

			//Registro del carnet
			crearCarnet(hwnd);

			if (showCarnet == NULL) {
				MessageBox(NULL, "Error en la creacion de carnet.", "Error", MB_OK);
				break;
			}

			//Mostrar carnet
			HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_CARNET), NULL, mostrarCarnet);
			ShowWindow(vntnDatos, SW_SHOW);
			//hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
			//SetMenu(vntnDatos, hMenu);
			DestroyWindow(hwnd);

		}break;
		case BTN_CREAR: {
			aPersona = oPersona;
			if (oPersona == NULL) {
				MessageBox(NULL, "No existe niguna persona registrada.", "Error", MB_OK);
				break;
			}

			string nombre = getText(GetDlgItem(hwnd, EB_NM_SR));
			string apellidoP = getText(GetDlgItem(hwnd, EB_AP_SR));
			string apellidoM = getText(GetDlgItem(hwnd, EB_AM_SR));

			bool encontrado = false;
			int i = 0;
			while (aPersona != NULL) {
				if (aPersona->nombre.compare(nombre) == 0 && aPersona->apellidoPaterno.compare(apellidoP) == 0
					&& aPersona->apellidoMaterno.compare(apellidoM) == 0) {
					encontrado = true;
					i++;
				}
				aPersona = aPersona->next;
			}

			if (i > 1) {
				MessageBox(NULL, "Hay mas de una persona con ese nombre. Ingresa la clave unica tambien.", "Error", MB_OK);
				//EnableWindow(GetDlgItem(hwnd, BTN_SRCU), FALSE);
				ShowWindow(GetDlgItem(hwnd, BTN_CREAR), SW_HIDE);
				ShowWindow(GetDlgItem(hwnd, BTN_CUGENERAR), SW_SHOW);
				ShowWindow(GetDlgItem(hwnd, EB_CU_SR_ECU), SW_SHOW);
				ShowWindow(GetDlgItem(hwnd, TXT_CURP_BSED), SW_SHOW);
				ShowWindow(GetDlgItem(hwnd, GB_CU), SW_SHOW);
				/*HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_CARNET_CU), NULL, buscarCarnetCUAlta);
				ShowWindow(vntnDatos, SW_SHOW);
				DestroyWindow(hwnd);
				*/
				aCarnet = oCarnet;
				aPersona = oPersona;
				break;
			}

			aPersona = oPersona;
			while (aPersona != NULL) {
				if (aPersona->nombre.compare(nombre) == 0 && aPersona->apellidoPaterno.compare(apellidoP) == 0
					&& aPersona->apellidoMaterno.compare(apellidoM) == 0) {
					encontrado = true;
					break;
				}
				aPersona = aPersona->next;
			}

			aPersona = oPersona;
			if (!encontrado) {
				MessageBox(NULL, "No se encontro ninguna persona registrada a ese nombre.", "Error", MB_OK);
				break;
			}
			string curp;
			while (aPersona != NULL) {
				if (aPersona->nombre.compare(nombre) == 0 && aPersona->apellidoPaterno.compare(apellidoP) == 0
					&& aPersona->apellidoMaterno.compare(apellidoM) == 0) {
					curp.append(aPersona->claveUnica);
					break;
				}
				aPersona = aPersona->next;
			}
			bool existe = false;
			while (aCarnet != NULL) {
				if (curp.compare(aCarnet->claveUnica) == 0) {
					existe = true;
					break;
				}
				aCarnet = aCarnet->next;
			}

			aCarnet = oCarnet;

			if (existe) {
				MessageBox(NULL, "Ya existe un carnet para esa persona.", "Error", MB_OK);
				existe = true;
				aCarnet = oCarnet;
				aPersona = oPersona;
				break;
			}

			aPersona = oPersona;

			while (aPersona != NULL) {
				if (aPersona->nombre.compare(nombre) == 0 && aPersona->apellidoPaterno.compare(apellidoP) == 0
					&& aPersona->apellidoMaterno.compare(apellidoM) == 0) {
					break;
				}
				aPersona = aPersona->next;
			}

			//Registro del carnet
			crearCarnet(hwnd);
			aPersona = oPersona;

			if (showCarnet == NULL) {
				MessageBox(NULL, "Error en la creacion de carnet.", "Error", MB_OK);
				break;
			}

			//Mostrar carnet
			HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_CARNET), NULL, mostrarCarnet);
			ShowWindow(vntnDatos, SW_SHOW);
			//hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
			//SetMenu(vntnDatos, hMenu);
			DestroyWindow(hwnd);
		}break;
		case BTN_CANCELSRCH: {
			HWND vntnHome = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_CARNET_HOME), NULL, carnetsHome);
			hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(vntnHome, hMenu);
			ShowWindow(vntnHome, SW_SHOW);
			DestroyWindow(hwnd);
		}break;
		}
	}break;
	case WM_CLOSE: {

	}break;
	case WM_DESTROY: {

	}break;
	default: {
		DefWindowProc(hwnd, msg, wParam, lParam);
	}break;
	}
	return FALSE;
}//
//Funcion utilizada para mostrar el carnet registrado
BOOL CALLBACK mostrarCarnet(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_INITDIALOG: {
		//Se introducen todos los datos a la ventana
		SetWindowText(GetDlgItem(hwnd, TXT_NAME), showCarnet->nombre.c_str());
		SetWindowText(GetDlgItem(hwnd, TXT_AP), showCarnet->apellidoPaterno.c_str());
		SetWindowText(GetDlgItem(hwnd, TXT_AM), showCarnet->apellidoMaterno.c_str());
		SetWindowText(GetDlgItem(hwnd, TXT_FNC), showCarnet->nacimiento.c_str());
		SetWindowText(GetDlgItem(hwnd, TXT_ESTADO), showCarnet->estado.c_str());
		SetWindowText(GetDlgItem(hwnd, TXT_CIUDAD), showCarnet->ciudad.c_str());
		SetWindowText(GetDlgItem(hwnd, TXT_DRCC), showCarnet->direccion.c_str());
		SetWindowText(GetDlgItem(hwnd, TXT_VCC), showCarnet->vacuna.c_str());
		SetWindowText(GetDlgItem(hwnd, TXT_SX), showCarnet->sexo.c_str());
		string id = to_string(showCarnet->ID);
		SetWindowText(GetDlgItem(hwnd, TXT_ID), id.c_str());
		SetWindowText(GetDlgItem(hwnd, TXT_PVCC), showCarnet->primeraDosis.c_str());
		SetWindowText(GetDlgItem(hwnd, TXT_SVCC), "Prox.");
		SetWindowText(GetDlgItem(hwnd, TXT_TVCC), "Prox.");
		SetWindowText(GetDlgItem(hwnd, TXT_CVCC), "Prox.");

		//Fotos
		HBITMAP hbImagenUser = (HBITMAP)LoadImage(NULL, showCarnet->fotoDoc.c_str(), IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
		HWND hPbImagen = GetDlgItem(hwnd, BTM_FDC);
		SendMessage(hPbImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbImagenUser);

		HBITMAP hbImagenUser2 = (HBITMAP)LoadImage(NULL, showCarnet->fotoPerfil.c_str(), IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
		HWND hPbImagen2 = GetDlgItem(hwnd, BTM_FC);
		SendMessage(hPbImagen2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbImagenUser2);

	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case BTN_LISTO_CNT: {
			showCarnet = NULL;
			aPersona = oPersona;
			aCarnet = oCarnet;
			aPersona = oPersona;
			HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_ALTA_CARNET), NULL, altaCarnet);
			ShowWindow(vntnDatos, SW_SHOW);
			hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(vntnDatos, hMenu);
			DestroyWindow(hwnd);
		}break;
		}
	}break;
	case WM_CLOSE: {

	}break;
	case WM_DESTROY: {

	}break;
	default: {
		DefWindowProc(hwnd, msg, wParam, lParam);
	}break;
	}
	return FALSE;
}
//Funcion utilizada para editar los datos introducidos y cambiarlos al carnet
BOOL CALLBACK editarCarnet(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_INITDIALOG: {
		enviarDatosCarnet(hwnd);
		imgPsn = false;
		imgDDI = false;
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case BTN_C_ECNT: {
			HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_CARNET_HOME), NULL, carnetsHome);
			ShowWindow(vntnDatos, SW_SHOW);
			hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(vntnDatos, hMenu);
			DestroyWindow(hwnd);
		}break;
		case BTN_EDITAR_CANT: {
			string idCnt = getText(GetDlgItem(hwnd, TXT_ID_E));

			string vnVacuna = getText(GetDlgItem(hwnd, CB_EVC_CNT));
			aVacuna = oVacuna;
			bool hayVcc = false;
			while (aVacuna != NULL) {
				if (vnVacuna.compare(aVacuna->nombre) == 0) {
					hayVcc = true;
					break;
				}
				aVacuna = aVacuna->next;
			}
			aVacuna = oVacuna;

			if (!hayVcc) {
				MessageBox(NULL, "Ingresa una vacuna registrada.", "Error", MB_OK);
				break;
			}


			//Volvemos a encontrar el carnet que vamos a modificar. Utilizamos el ID
			showCarnet = NULL;
			aCarnet = oCarnet;
			while (aCarnet != NULL) {
				string cId = to_string(aCarnet->ID);
				if (cId.compare(idCnt) == 0) {
					break;
				}
				aCarnet = aCarnet->next;
			}

			//Datos generales
			aCarnet->nombre = getText(GetDlgItem(hwnd, EB_EN_CTN));
			aCarnet->apellidoPaterno = getText(GetDlgItem(hwnd, EB_EAP_CNT));
			aCarnet->apellidoMaterno = getText(GetDlgItem(hwnd, EB_EAM_CNT));
			aCarnet->sexo = getText(GetDlgItem(hwnd, CB_ESX_CNT));
			aCarnet->estado = getText(GetDlgItem(hwnd, EB_EDO_CTN));
			aCarnet->ciudad = getText(GetDlgItem(hwnd, EB_EC_CNT));
			aCarnet->direccion = getText(GetDlgItem(hwnd, EB_EDC_CNT));
			SendDlgItemMessage(hwnd, DTM_EFN_CNT, DTM_GETSYSTEMTIME, NULL, (LPARAM) & (aCarnet->stmNacimiento));
			//Fecha
			SendDlgItemMessage(hwnd, DTM_EP_CNT, DTM_GETSYSTEMTIME, NULL, (LPARAM) & (aCarnet->stmDosis));

			//Datos de vacuna
			aCarnet->vacuna = getText(GetDlgItem(hwnd, CB_EVC_CNT));

			aVacuna = oVacuna;
			//Encuentra el numero de dosis necesarias y envia los dos a los SYSTEMTIME
			int nDosis;
			while (aVacuna != NULL) {
				if (aVacuna->nombre.compare(aCarnet->vacuna) == 0) {
					nDosis = aVacuna->numeroDeDosis;
				}
				aVacuna = aVacuna->next;
			}

			aVacuna = oVacuna;
			if (nDosis == 2) {
				SendDlgItemMessage(hwnd, DTP_ESD_CNT, DTM_GETSYSTEMTIME, NULL, (LPARAM) & (aCarnet->stdDosisDos));
			}
			if (nDosis == 3) {
				SendDlgItemMessage(hwnd, DTP_ESD_CNT, DTM_GETSYSTEMTIME, NULL, (LPARAM) & (aCarnet->stdDosisDos));
				SendDlgItemMessage(hwnd, DTP_ETD_CNT, DTM_GETSYSTEMTIME, NULL, (LPARAM) & (aCarnet->stdDosisTres));
			}
			if (nDosis == 4) {
				SendDlgItemMessage(hwnd, DTP_ESD_CNT, DTM_GETSYSTEMTIME, NULL, (LPARAM) & (aCarnet->stdDosisDos));
				SendDlgItemMessage(hwnd, DTP_ETD_CNT, DTM_GETSYSTEMTIME, NULL, (LPARAM) & (aCarnet->stdDosisTres));
				SendDlgItemMessage(hwnd, DTP_ECD_CNT, DTM_GETSYSTEMTIME, NULL, (LPARAM) & (aCarnet->stdDosisCuatro));
			}

			//Fotos
			if (imgPsn) {
				aCarnet->fotoPerfil = rutaDeImg;
				imgPsn = false;
				rutaDeImg = "";
			}

			if (imgDDI) {
				aCarnet->fotoDoc = rutaDeImg2;
				imgDDI = false;
				rutaDeImg2 = "";
			}

			aCarnet = oCarnet;
			aVacuna = oVacuna;
			aPersona = oPersona;
			showCarnet = NULL;
			MessageBox(NULL, "Listo.", "Error", MB_OK);

		}break;
		case BTN_EF_CNT: {
			rutaDeImg = cargarImagen(hwnd, BTM_FC_E_CU);
			if (rutaDeImg.compare("") != 0) {
				imgPsn = true;
			}
		}break;
		case BTN_EID_CNT: {
			rutaDeImg2 = cargarImagen(hwnd, BTM_FDC_ED);
			if (rutaDeImg2.compare("") != 0) {
				imgDDI = true;
			}
		}break;
		}
	}break;
	case WM_CLOSE: {

	}break;
	case WM_DESTROY: {
		imgDDI = false;
		imgPsn = false;
		rutaDeImg = "";
		rutaDeImg2 = "";
		aCarnet = oCarnet;
		aVacuna = oVacuna;
		aPersona = oPersona;
		showCarnet = NULL;
	}break;
	default: {
		DefWindowProc(hwnd, msg, wParam, lParam);
	}break;
	}
	return FALSE;
}
//Funcion utilizada para buscar carnets y mostrar una ventana con edit box para editar
BOOL CALLBACK buscarCarnetEditar(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_INITDIALOG: {

	}break;
	case WM_COMMAND: {
		opcionesDeMenu(hwnd, wParam, lParam);
		switch (LOWORD(wParam)) {
		case BTN_CUC_CNC: {
			HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_CARNET_HOME), NULL, carnetsHome);
			ShowWindow(vntnDatos, SW_SHOW);
			hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(vntnDatos, hMenu);
			DestroyWindow(hwnd);
		}break;
		case BTN_SRCU: {
			string nombre = getText(GetDlgItem(hwnd, EB_NM_SR_ECU));
			string apellidoP = getText(GetDlgItem(hwnd, EB_AP_SR_ECU));
			string apellidoM = getText(GetDlgItem(hwnd, EB_AM_SR_ECU));

			//Si algun textBox esta vacio
			if (nombre.compare("") == 0 || apellidoP.compare("") == 0 || apellidoM.compare("") == 0) {
				MessageBox(NULL, "Hay un campo vacio.", "Error", MB_OK);
				break;
			}
			if (oCarnet == NULL) {
				MessageBox(NULL, "No hay ningun carnet registrado.", "Error", MB_OK);
				break;
			}

			if (oPersona == NULL) {
				MessageBox(NULL, "No hay ningun carnet registrado.", "Error", MB_OK);
				break;
			}

			//Buscamos si hay mas de una persona con ese nombre
			aCarnet = oCarnet;
			int i = 0;
			bool encontrado = false;
			while (aCarnet != NULL) {
				if (aCarnet->nombre.compare(nombre) == 0 && aCarnet->apellidoPaterno.compare(apellidoP) == 0
					&& aCarnet->apellidoMaterno.compare(apellidoM) == 0) 
				{
					encontrado = true;
					i++;
				}
				aCarnet = aCarnet->next;
			}
			aCarnet = oCarnet;

			//Si hay mas de una, habilitamos nuevas ventanas y nos salimos.
			if (i > 1) {
				MessageBox(NULL, "Hay mas de una persona con ese nombre. Ingresa la clave unica tambien.", "Error", MB_OK);
				ShowWindow(GetDlgItem(hwnd, BTN_SRCU), SW_HIDE);
				ShowWindow(GetDlgItem(hwnd, GB_CU), SW_SHOW);
				ShowWindow(GetDlgItem(hwnd, TXT_CURP_BSED), SW_SHOW);
				ShowWindow(GetDlgItem(hwnd, EB_CU_SR_ECU), SW_SHOW);
				ShowWindow(GetDlgItem(hwnd, BTN_BUS_CU), SW_SHOW);
				break;
			}

			//Si no se encuentra el nombre.
			if (!encontrado) {
				MessageBox(NULL, "No se encontro ninngun carnet a ese nombre", "Error", MB_OK);
				break;
			}

			bool encontrar = false;
			//Si se encuentra el nombre y es unico, lo buscamos de nuevo y damos luz verde a mostrar el carnet.
			while (aCarnet != NULL) {
				if (aCarnet->nombre.compare(nombre) == 0 && aCarnet->apellidoPaterno.compare(apellidoP) == 0
					&& aCarnet->apellidoMaterno.compare(apellidoM) == 0)
				{
					encontrar = true;
					showCarnet = aCarnet;
					break;
				}
				aCarnet = aCarnet->next;
			}
			aCarnet = oCarnet;

			if (encontrar) {
				HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_EDITARCNT), NULL, editarCarnet);
				ShowWindow(vntnDatos, SW_SHOW);
				DestroyWindow(hwnd);
			}

		}break;
		case BTN_BUS_CU: {
			showCarnet = NULL;
			string nombre = getText(GetDlgItem(hwnd, EB_NM_SR_ECU));
			string apellidoP = getText(GetDlgItem(hwnd, EB_AP_SR_ECU));
			string apellidoM = getText(GetDlgItem(hwnd, EB_AM_SR_ECU));
			string claveUnica = getText(GetDlgItem(hwnd, EB_CU_SR_ECU));

			//Si algun textBox esta vacio
			if (nombre.compare("") == 0 || apellidoP.compare("") == 0 || apellidoM.compare("") == 0 || claveUnica.compare("") == 0) {
				MessageBox(NULL, "Hay un campo vacio.", "Error", MB_OK);
				break;
			}

			//Buscamos si el carnet existe
			aCarnet = oCarnet;
			bool encontrado = false;
			while (aCarnet != NULL) {
				if (aCarnet->nombre.compare(nombre) == 0 && aCarnet->apellidoPaterno.compare(apellidoP) == 0 && aCarnet->apellidoMaterno.compare(apellidoM) == 0 && aCarnet->claveUnica.compare(claveUnica) == 0) {
					encontrado = true;
					showCarnet = aCarnet; //Puntero al carnet que queremos mostrar
					break;
				}
				aCarnet = aCarnet->next;
			}
			aCarnet = oCarnet;

			//Mostramos el carnet
			if (encontrado) {
				HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_EDITARCNT), NULL, editarCarnet);
				ShowWindow(vntnDatos, SW_SHOW);
				DestroyWindow(hwnd);
			}
			else {
				MessageBox(NULL, "No se encontro ningun carnet a ese nombre o curp.", "Error", MB_OK);
				aCarnet = oCarnet;
			}

		}break;
		}
	}break;
	case WM_CLOSE: {

	}break;
	case WM_DESTROY: {

	}break;
	default: {
		DefWindowProc(hwnd, msg, wParam, lParam);
	}break;
	}
	return FALSE;
}
//Funcion utilizada para dar de baja un carnet
BOOL CALLBACK bajaCarnet(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_INITDIALOG: {

	}break;
	case WM_COMMAND: {
		opcionesDeMenu(hwnd, wParam, lParam);
		switch (LOWORD(wParam)) {
		case BTN_BORRAR_CARNET: {

			if (oCarnet == NULL) {
				MessageBox(NULL, "No existe ningun carnet.", "Error", MB_OK);
				break;
			}

			string id = getText(GetDlgItem(hwnd, EB_ID_ELIM));
			aCarnet = oCarnet;
			bool existe = false;
			while (aCarnet != NULL) {
				string idCarnetLL = to_string(aCarnet->ID);
				if (idCarnetLL.compare(id) == 0) {
					existe = true;
					break;
				}
				aCarnet = aCarnet->next;
			}
			aCarnet = oCarnet;

			if (existe) {
				eliminarCarnet(hwnd, stoi(id));
			}
			else {
				MessageBox(NULL, "No se encontro ningun carnet con ese ID.", "Error", MB_OK);
				break;
			}
			
		}break;
		case BTN_C_BORRARCNT: {
			HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_CARNET_HOME), NULL, carnetsHome);
			ShowWindow(vntnDatos, SW_SHOW);
			hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(vntnDatos, hMenu);
			DestroyWindow(hwnd);
		}break;
		}
	}break;
	case WM_CLOSE: {

	}break;
	case WM_DESTROY: {

	}break;
	default: {
		DefWindowProc(hwnd, msg, wParam, lParam);
	}break;
	}
	return FALSE;
}
//Funcion utilizada para buscar un carnet y mostralo en una ventana llena de static text
BOOL CALLBACK buscarCarnet(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_INITDIALOG: {


	}break;
	case WM_COMMAND: {
		opcionesDeMenu(hwnd, wParam, lParam);
		switch (LOWORD(wParam)) {
		case BTN_BUSCAR_CNTPS: {
			string nombre = getText(GetDlgItem(hwnd, EB_BC_NOM));
			string apellidoP = getText(GetDlgItem(hwnd, EB_BC_AP));
			string apellidoM = getText(GetDlgItem(hwnd, EB_BC_AM));

			//Si algun textBox esta vacio
			if (nombre.compare("") == 0 || apellidoP.compare("") == 0 || apellidoM.compare("") == 0) {
				MessageBox(NULL, "Hay un campo vacio.", "Error", MB_OK);
				break;
			}
			if (oCarnet == NULL) {
				MessageBox(NULL, "No hay ningun carnet registrado.", "Error", MB_OK);
				break;
			}

			if (oPersona == NULL) {
				MessageBox(NULL, "No hay ningun carnet registrado.", "Error", MB_OK);
				break;
			}

			//Buscamos si hay mas de una persona con ese nombre
			aCarnet = oCarnet;
			int i = 0;
			bool encontrado = false;
			while (aCarnet != NULL) {
				if (aCarnet->nombre.compare(nombre) == 0 && aCarnet->apellidoPaterno.compare(apellidoP) == 0
					&& aCarnet->apellidoMaterno.compare(apellidoM) == 0)
				{
					encontrado = true;
					i++;
				}
				aCarnet = aCarnet->next;
			}
			aCarnet = oCarnet;

			//Si hay mas de una, habilitamos nuevas ventanas y nos salimos.
			if (i > 1) {
				MessageBox(NULL, "Hay mas de una persona con ese nombre. Ingresa la clave unica tambien.", "Error", MB_OK);
				ShowWindow(GetDlgItem(hwnd, BTN_BUSCAR_CNTPS), SW_HIDE);
				ShowWindow(GetDlgItem(hwnd, EB_BC_CU), SW_SHOW);
				ShowWindow(GetDlgItem(hwnd, BTN_BC_C_CU), SW_SHOW);
				ShowWindow(GetDlgItem(hwnd, TXT_BC_CU), SW_SHOW);
				ShowWindow(GetDlgItem(hwnd, GB_BC), SW_SHOW);
				break;
			}

			//Si no se encuentra el nombre.
			if (!encontrado) {
				MessageBox(NULL, "No se encontro ninngun carnet a ese nombre", "Error", MB_OK);
				break;
			}

			bool encontrar = false;
			//Si se encuentra el nombre y es unico, lo buscamos de nuevo y damos luz verde a mostrar el carnet.
			while (aCarnet != NULL) {
				if (aCarnet->nombre.compare(nombre) == 0 && aCarnet->apellidoPaterno.compare(apellidoP) == 0
					&& aCarnet->apellidoMaterno.compare(apellidoM) == 0)
				{
					encontrar = true;
					showCarnet = aCarnet;
					break;
				}
				aCarnet = aCarnet->next;
			}
			aCarnet = oCarnet;

			if (encontrar) {
				HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_CARNET), NULL, mostrarCarnet);
				ShowWindow(vntnDatos, SW_SHOW);
				DestroyWindow(hwnd);
			}
		
		}break;
		case BTN_BC_C_CU: {
			showCarnet = NULL;
			string nombre = getText(GetDlgItem(hwnd, EB_BC_NOM));
			string apellidoP = getText(GetDlgItem(hwnd, EB_BC_AP));
			string apellidoM = getText(GetDlgItem(hwnd, EB_BC_AM));
			string claveUnica = getText(GetDlgItem(hwnd, EB_BC_CU));

			//Si algun textBox esta vacio
			if (nombre.compare("") == 0 || apellidoP.compare("") == 0 || apellidoM.compare("") == 0 || claveUnica.compare("") == 0) {
				MessageBox(NULL, "Hay un campo vacio.", "Error", MB_OK);
				break;
			}

			//Buscamos si el carnet existe
			aCarnet = oCarnet;
			bool encontrado = false;
			while (aCarnet != NULL) {
				if (aCarnet->nombre.compare(nombre) == 0 && aCarnet->apellidoPaterno.compare(apellidoP) == 0 && aCarnet->apellidoMaterno.compare(apellidoM) == 0 && aCarnet->claveUnica.compare(claveUnica) == 0) {
					encontrado = true;
					showCarnet = aCarnet; //Puntero al carnet que queremos mostrar
					break;
				}
				aCarnet = aCarnet->next;
			}
			aCarnet = oCarnet;

			//Mostramos el carnet
			if (encontrado) {
				HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_CARNET), NULL, mostrarCarnet);
				ShowWindow(vntnDatos, SW_SHOW);
				DestroyWindow(hwnd);
			}
			else {
				MessageBox(NULL, "No se encontro ningun carnet a ese nombre o curp.", "Error", MB_OK);
				aCarnet = oCarnet;
			}
		
		}break;
		case BTN_CNR_BUS: {
			aCarnet = oCarnet;
			aPersona = oPersona;
			aVacuna = oVacuna;
			showCarnet = NULL;
			HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_CARNET_HOME), NULL, carnetsHome);
			ShowWindow(vntnDatos, SW_SHOW);
			hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(vntnDatos, hMenu);
			DestroyWindow(hwnd);
		}break;
		}
	}break;
	case WM_CLOSE: {

	}break;
	case WM_DESTROY: {
		
	}break;
	default: {
		DefWindowProc(hwnd, msg, wParam, lParam);
	}break;
	}
	return FALSE;
}
//Funcion de generacion de reportes
BOOL CALLBACK funcionReportes(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_INITDIALOG: {


	}break;
	case WM_COMMAND: {
		opcionesDeMenu(hwnd, wParam, lParam);
		switch (LOWORD(wParam)) {
		case BTN_HEAPSORT: {
			if (oCarnet == NULL) {
				MessageBox(NULL, "No existe ningun carnet.", "Error", MB_OK);
				break;
			}

			int totalCarnets = 0;
			while (aCarnet != NULL) {
				aCarnet = aCarnet->next;
				totalCarnets++;
			}
			aCarnet = oCarnet;/*
			heapSort(aCarnet, totalCarnets);*/
		}break;
		}
	}break;
	case WM_CLOSE: {

	}break;
	case WM_DESTROY: {

	}break;
	default: {
		DefWindowProc(hwnd, msg, wParam, lParam);
	}break;
	}
	return FALSE;
}

//Funcion para obtener el string de una textbox
string getText(HWND hwnd) {
	int length = GetWindowTextLength(hwnd);
	if (length < 1)
		return "";
	char cText[80];
	GetWindowText(hwnd, cText, ++length);
	string sText(cText);
	return sText;
}
//Guardar los usuarios
void guardarUsuarios(Usuario* ptrOrg) {
	fstream datos;
	datos.open("Usuarios.txt", fstream::out | ios::trunc);
	if (datos.is_open()) {
		while (ptrOrg != NULL) {
			datos << ptrOrg->nombre.c_str() << "\n";
			datos << ptrOrg->apellido.c_str() << "\n";
			datos << ptrOrg->usuario.c_str() << "\n";
			datos << ptrOrg->contrasena.c_str() << "\n";
			ptrOrg = ptrOrg->next;
		}
		datos.close();
	}

}
//Cargar los usuarios.
void cargarUsuarios() {
	fstream datos;
	datos.open("Usuarios.txt", ios::in | ios::ate);

	int totalCharUsuarios = datos.tellg();
	if (totalCharUsuarios < 1) {
		MessageBox(NULL, "No hay usuarios registrados", "Vacio", MB_ICONASTERISK);
		return;
	}

	datos.clear();
	datos.seekg(0);
	string linea = "i";
	int totalLineas = 0;
	while (getline(datos, linea)) {
		totalLineas++;

	}

	datos.clear();
	datos.seekg(0);
	int usuarios = totalLineas / 4;
	if (datos.is_open()) {
		
		int usuario = 0;
		for(int i = 0; i < usuarios; i++){
			
			if (usuario == 0) {
				oUsuario = new Usuario;
				getline(datos, linea);
				oUsuario->nombre.append(linea);
				getline(datos, linea);
				oUsuario->apellido.append(linea);
				getline(datos, linea);
				oUsuario->usuario.append(linea);
				getline(datos, linea);
				oUsuario->contrasena.append(linea);
				oUsuario->next = NULL;
				oUsuario->prev = NULL;
				aUsuario = oUsuario;
				usuario++;
			}
			else {
				while (aUsuario->next != NULL)
					aUsuario = aUsuario->next;
				aUsuario->next = new Usuario;
				aUsuario->next->prev = aUsuario;
				aUsuario = aUsuario->next;
				getline(datos, linea);
				aUsuario->nombre.append(linea);
				getline(datos, linea);
				aUsuario->apellido.append(linea);
				getline(datos, linea);
				aUsuario->usuario.append(linea);
				getline(datos, linea);
				aUsuario->contrasena.append(linea);
				aUsuario->next = NULL;
				aUsuario = oUsuario;
			}
		
		}
		datos.close();
	}


}
//Elimina la memoria del heap sobre usuario
void liberarMemoriaUsuarios() {
	if (oUsuario != NULL) {
		while (aUsuario != NULL) {
			Usuario* temp = aUsuario;
			aUsuario = aUsuario->next;
			delete temp;
		
		}
	
	}
	else
		return;

}
//Elimina la memoria del heap sobre personas
void liberarMemoriaPersonas() {
	if (oPersona != NULL) {
		while (aPersona != NULL) {
			Persona* temp = aPersona;
			aPersona = aPersona->next;
			delete temp;

		}
	}
	else
		return;
}
//Elimina la memoria del heap sobre carnets
void liberarMemoriaCarnets() {
	if (oCarnet != NULL) {
		while (aPersona != NULL) {
			Carnet* temp = aCarnet;
			aCarnet = aCarnet->next;
			delete temp;

		}
	}
	else
		return;
}
//Carga una imagen en un BTM
string cargarImagen(HWND hwnd, int bitmap) {
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	char rutaImagen[MAX_PATH] = "";
	string rutaImg = "";

	ofn.hwndOwner = hwnd;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFile = rutaImagen;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrDefExt = "txt";
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrFilter = "Imagenes bmp\0*.bmp\0";

	if (GetOpenFileName(&ofn)) {
		HBITMAP hbImagenUser = (HBITMAP)LoadImage(NULL, rutaImagen, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
		HWND hPbImagen = GetDlgItem(hwnd, bitmap);
		for (int i = 0; i < sizeof(rutaImagen); i++) {
			rutaImg += rutaImagen[i];
		}
		SendMessage(hPbImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbImagenUser);
	}
	return rutaImg;
}
//Inserta datos en lista lsigada de carnet
void crearCarnet(HWND hwnd) {
	if (oCarnet == NULL) {
		oCarnet = new Carnet;
		oCarnet->ID = rand() % 129000000 + 1;
		//Datos generales
		oCarnet->nombre = aPersona->nombre;
		oCarnet->apellidoPaterno = aPersona->apellidoPaterno;
		oCarnet->apellidoMaterno = aPersona->apellidoMaterno;
		oCarnet->telefono = aPersona->telefono;
		oCarnet->sexo = aPersona->sexo;
		oCarnet->direccion = aPersona->direccion;
		oCarnet->estado = aPersona->estado;
		oCarnet->ciudad = aPersona->municipio;
		oCarnet->nacimiento = aPersona->nacimiento;
		oCarnet->claveUnica = aPersona->claveUnica;
		oCarnet->stmNacimiento = aPersona->dtmNacimiento;

		//Datos de la vacunacion
		oCarnet->vacuna = aPersona->vacuna;
		HWND hDtpTiempo = GetDlgItem(hwnd, DTM_DOSIS);
		int dtpSize = GetWindowTextLength(hDtpTiempo);
		char cadenaDelDTP[80];
		GetWindowText(hDtpTiempo, cadenaDelDTP, ++dtpSize);
		string fechaDTP(cadenaDelDTP);
		oCarnet->primeraDosis.append(fechaDTP);
		SendDlgItemMessage(hwnd, DTM_DOSIS, DTM_GETSYSTEMTIME,NULL, (LPARAM) & (oCarnet->stmDosis));
		oCarnet->stmNacimiento = aPersona->dtmNacimiento;
		
		//Fotos
		oCarnet->fotoPerfil = aPersona->fotoPath;
		oCarnet->fotoDoc = aPersona->docId;

		//Lista ligada
		oCarnet->next = NULL;
		oCarnet->prev = NULL;
		aCarnet = oCarnet;
		showCarnet = oCarnet;
		MessageBox(NULL, "Carnet registrado exitosamente.", "Listo", MB_OK);

	}
	else {
		while (aCarnet->next != NULL) {
			aCarnet = aCarnet->next;
		}
		aCarnet->next = new Carnet;
		aCarnet->next->prev = aCarnet;
		aCarnet = aCarnet->next;

		aCarnet->ID = (aCarnet->prev->ID) + 1;
		//Datos generales
		aCarnet->nombre = aPersona->nombre;
		aCarnet->apellidoPaterno = aPersona->apellidoPaterno;
		aCarnet->apellidoMaterno = aPersona->apellidoMaterno;
		aCarnet->telefono = aPersona->telefono;
		aCarnet->sexo = aPersona->sexo;
		aCarnet->direccion = aPersona->direccion;
		aCarnet->estado = aPersona->estado;
		aCarnet->ciudad = aPersona->municipio;
		aCarnet->nacimiento = aPersona->nacimiento;
		aCarnet->stmNacimiento = aPersona->dtmNacimiento;
		aCarnet->claveUnica = aPersona->claveUnica;

		//Datos de la vacunacion
		aCarnet->vacuna = aPersona->vacuna;
		HWND hDtpTiempo = GetDlgItem(hwnd, DTM_DOSIS);
		int dtpSize = GetWindowTextLength(hDtpTiempo);
		char cadenaDelDTP[80];
		GetWindowText(hDtpTiempo, cadenaDelDTP, ++dtpSize);
		string fechaDTP(cadenaDelDTP);
		aCarnet->primeraDosis.append(fechaDTP);
		SendDlgItemMessage(hwnd, DTM_DOSIS, DTM_GETSYSTEMTIME, NULL, (LPARAM) & (aCarnet->stmDosis));

		//Fotos
		aCarnet->fotoPerfil = aPersona->fotoPath;
		aCarnet->fotoDoc = aPersona->docId;

		//Lista ligada
		aCarnet->next = NULL;
		showCarnet = aCarnet;
		aCarnet = oCarnet;
		MessageBox(NULL, "Carnet registrado exitosamente.", "Listo", MB_OK);
	}
	


	return;
}
//Se envian los datos de un carnet en especifico a una ventana
void enviarDatosCarnet(HWND hwnd) {
	//Enviar los datos

	SendMessage(GetDlgItem(hwnd, EB_EN_CTN), WM_SETTEXT, NULL, (LPARAM)showCarnet->nombre.c_str());
	SendMessage(GetDlgItem(hwnd, EB_EAP_CNT), WM_SETTEXT, NULL, (LPARAM)showCarnet->apellidoPaterno.c_str());
	SendMessage(GetDlgItem(hwnd, EB_EAM_CNT), WM_SETTEXT, NULL, (LPARAM)showCarnet->apellidoMaterno.c_str());
	SendMessage(GetDlgItem(hwnd, EB_EDO_CTN), WM_SETTEXT, NULL, (LPARAM)showCarnet->estado.c_str());
	SendMessage(GetDlgItem(hwnd, EB_EC_CNT), WM_SETTEXT, NULL, (LPARAM)showCarnet->ciudad.c_str());
	SendMessage(GetDlgItem(hwnd, EB_EDC_CNT), WM_SETTEXT, NULL, (LPARAM)showCarnet->direccion.c_str());
	SendMessage(GetDlgItem(hwnd, CB_EVC_CNT), WM_SETTEXT, NULL, (LPARAM)showCarnet->vacuna.c_str());
	SendMessage(GetDlgItem(hwnd, CB_ESX_CNT), WM_SETTEXT, NULL, (LPARAM)showCarnet->sexo.c_str());
	DateTime_SetSystemtime(GetDlgItem(hwnd, DTM_EFN_CNT), GDT_VALID, (LPARAM)&showCarnet->stmNacimiento);
	DateTime_SetSystemtime(GetDlgItem(hwnd, DTM_EP_CNT), GDT_VALID, (LPARAM)&showCarnet->stmDosis);
	string id = to_string(showCarnet->ID);
	SetWindowText(GetDlgItem(hwnd, TXT_ID_E), id.c_str());
	SetWindowText(GetDlgItem(hwnd, TXT_PVCC), showCarnet->primeraDosis.c_str());
	aVacuna = oVacuna;
	int dosis = 0;
	while (aVacuna != NULL) {
		if (showCarnet->vacuna.compare(aVacuna->nombre) == 0) {
			dosis = aVacuna->numeroDeDosis;
		}
		aVacuna = aVacuna->next;
	}
	if (dosis == 2) {
		ShowWindow(GetDlgItem(hwnd, DTP_ESD_CNT), SW_SHOW);
		DateTime_SetSystemtime(GetDlgItem(hwnd, DTP_ESD_CNT), GDT_VALID, (LPARAM)&showCarnet->stdDosisDos);
		SetWindowText(GetDlgItem(hwnd, TXT_SVCC), "Prox.");
	}
	else if (dosis == 3) {
		ShowWindow(GetDlgItem(hwnd, DTP_ESD_CNT), SW_SHOW);
		DateTime_SetSystemtime(GetDlgItem(hwnd, DTP_ESD_CNT), GDT_VALID, (LPARAM)&showCarnet->stdDosisDos);
		ShowWindow(GetDlgItem(hwnd, DTP_ETD_CNT), SW_SHOW);
		DateTime_SetSystemtime(GetDlgItem(hwnd, DTP_ETD_CNT), GDT_VALID, (LPARAM)&showCarnet->stdDosisTres);
		SetWindowText(GetDlgItem(hwnd, TXT_TVCC), "Prox.");
	}
	else if (dosis == 4) {
		ShowWindow(GetDlgItem(hwnd, DTP_ESD_CNT), SW_SHOW);
		DateTime_SetSystemtime(GetDlgItem(hwnd, DTP_ESD_CNT), GDT_VALID, (LPARAM)&showCarnet->stdDosisDos);
		ShowWindow(GetDlgItem(hwnd, DTP_ETD_CNT), SW_SHOW);
		DateTime_SetSystemtime(GetDlgItem(hwnd, DTP_ETD_CNT), GDT_VALID, (LPARAM)&showCarnet->stdDosisTres);
		ShowWindow(GetDlgItem(hwnd, DTP_ECD_CNT), SW_SHOW);
		DateTime_SetSystemtime(GetDlgItem(hwnd, DTP_ECD_CNT), GDT_VALID, (LPARAM)&showCarnet->stdDosisCuatro);
		SetWindowText(GetDlgItem(hwnd, TXT_CVCC), "Prox.");
	}


	rutaDeImg = rutaDeImg2 = "";
	imgPsn = false;
	imgDDI = false;
	int indice = 0;

	SendMessage(GetDlgItem(hwnd, CB_ESX_CNT), CB_ADDSTRING, 0, (LPARAM)"Femenino");
	SendMessage(GetDlgItem(hwnd, CB_ESX_CNT), CB_ADDSTRING, 1, (LPARAM)"Masculino");

	while (aVacuna != NULL) {
		SendMessage(GetDlgItem(hwnd, CB_EVC_CNT), CB_ADDSTRING, indice, (LPARAM)aVacuna->nombre.c_str());
		aVacuna = aVacuna->next;
		indice++;
	}

	HBITMAP hbImagenUser = (HBITMAP)LoadImage(NULL, showCarnet->fotoDoc.c_str(), IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
	HWND hPbImagen = GetDlgItem(hwnd, BTM_FDC_ED);
	SendMessage(hPbImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbImagenUser);

	HBITMAP hbImagenUser2 = (HBITMAP)LoadImage(NULL, showCarnet->fotoPerfil.c_str(), IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
	HWND hPbImagen2 = GetDlgItem(hwnd, BTM_FC_E_CU);
	SendMessage(hPbImagen2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbImagenUser2);


	aVacuna = oVacuna;
	return;

}
//Se borra un carnet en especifico de la lista ligada
void eliminarCarnet(HWND hwnd, int ID) {
	bool encontrado = false;
	if (oCarnet != NULL) {
		while (aCarnet != NULL) {

			if (aCarnet->ID == ID) {
				encontrado = true;
				break;
			}
			aCarnet = aCarnet->next;
		}

		if (encontrado) {
			if (aCarnet->next == NULL && aCarnet->prev == NULL) {
				Carnet* temp = new Carnet;
				temp = aCarnet;
				delete temp;
				MessageBox(hwnd, "El carnet fue elimiando.", "Listo", MB_OK);
				oCarnet = NULL;
				aCarnet = oCarnet;
				return;
			}
			else if (aCarnet->next == NULL && aCarnet->prev != NULL) {
				Carnet* temp = new Carnet;
				temp = aCarnet;
				temp->prev->next = NULL;
				delete temp;
				aCarnet = NULL;
				aCarnet = oCarnet;
				MessageBox(hwnd, "El carnet fue elimiando.", "Listo", MB_OK);
				return;

			}
			else if (aCarnet->next != NULL && aCarnet->prev == NULL) {
				Carnet* temp = new Carnet;
				temp = aCarnet;
				aCarnet->next->prev = NULL;
				oCarnet = oCarnet->next;
				delete temp;
				aCarnet = NULL;
				aCarnet = oCarnet;
				MessageBox(hwnd, "El carnet fue elimiando.", "Listo", MB_OK);
				return;
			}
			else if (aCarnet->next != NULL && aCarnet->prev != NULL) {
				Carnet* temp = new Carnet;
				temp = aCarnet;
				aCarnet->prev->next = aCarnet->next;
				aCarnet->next->prev = aCarnet->prev;
				delete temp;
				aCarnet = NULL;
				aCarnet = oCarnet;
				MessageBox(hwnd, "El carnet fue elimiando.", "Listo", MB_OK);
				return;

			}
		}
		else {
			MessageBox(hwnd, "No hay ningun carnet registrada con esa ID.", "Error", MB_OK);
			aCarnet = oCarnet;
			return;
		}

	}
	else {
		MessageBox(hwnd, "No hay ninguna vacuna registrada", "Error", MB_OK);
		return;

	}
	return;
}
//Switch dentro de los wm_command para hacer que el menu funcione
void opcionesDeMenu(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	switch (LOWORD(wParam)) {
	case MN_INICIO: {
		HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(DLG_INICIO), NULL, home);
		ShowWindow(vntnDatos, SW_SHOW);
		hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(vntnDatos, hMenu);
		DestroyWindow(hwnd);
	}break;
	case ID_CIUDADANO_REGISTRAS: {
		HWND vntnHome = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_PERSONAS), NULL, personas);
		hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(vntnHome, hMenu);
		ShowWindow(vntnHome, SW_SHOW);
		DestroyWindow(hwnd);
	}break;
	case ID_CIUDADANO_DA: {
		HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_RPERSONAS), NULL, registrarPersonas);
		ShowWindow(vntnDatos, SW_SHOW);
		hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(vntnDatos, hMenu);
		DestroyWindow(hwnd);
	}break;
	case ID_CIUDADANO_EDITAR: {
		HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_EPERSONAS), NULL, editarPersonas);
		ShowWindow(vntnDatos, SW_SHOW);
		hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(vntnDatos, hMenu);
		DestroyWindow(hwnd);
	}break;
	case MN_CIU_BAJA: {
		HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_BORRAR_PERSONA), NULL, borrarPersonas);
		ShowWindow(vntnDatos, SW_SHOW);
		hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(vntnDatos, hMenu);
		DestroyWindow(hwnd);
	}break;
	case ID_VACUNAS_INICIO: {
		HWND vntnHome = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_VACUNAS), NULL, vacunasHome);
		hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(vntnHome, hMenu);
		ShowWindow(vntnHome, SW_SHOW);
		DestroyWindow(hwnd);
	}break;
	case ID_VACUNAS_REGISTRAR: {
		HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(DLG_AGG_VACUNA), NULL, altaVacunas);
		ShowWindow(vntnDatos, SW_SHOW);
		hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(vntnDatos, hMenu);
		DestroyWindow(hwnd);
	}break;
	case ID_VACUNAS_DARDEBAJA: {
		HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(DLG_BORR_VAC), NULL, bajaVacunas);
		ShowWindow(vntnDatos, SW_SHOW);
		hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(vntnDatos, hMenu);
		DestroyWindow(hwnd);
	}break;
	case ID_VACUNAS_EDITAR: {
		HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(DLG_ED_VACUNA), NULL, editarVacunas);
		ShowWindow(vntnDatos, SW_SHOW);
		hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(vntnDatos, hMenu);
		DestroyWindow(hwnd);
	}break;
		//Menu: carnet
	case MN_H_GNL: {
		HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_CARNET_HOME), NULL, carnetsHome);
		ShowWindow(vntnDatos, SW_SHOW);
		hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(vntnDatos, hMenu);
		DestroyWindow(hwnd);
	}break;
	case MN_A_CNT: {
		HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_ALTA_CARNET), NULL, altaCarnet);
		ShowWindow(vntnDatos, SW_SHOW);
		hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(vntnDatos, hMenu);
		DestroyWindow(hwnd);
	}break;
	case MN_E_CNT: {
		HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_CARNET_CU), NULL, buscarCarnetEditar);
		ShowWindow(vntnDatos, SW_SHOW);
		hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(vntnDatos, hMenu);
		DestroyWindow(hwnd);
	}break;
	case MN_B_CNT: {
		HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_ELIMINAR_CARNET), NULL, bajaCarnet);
		ShowWindow(vntnDatos, SW_SHOW);
		hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(vntnDatos, hMenu);
		DestroyWindow(hwnd);
	}break;
	case MN_SR_CNT: {
		HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_BUSCAR_CARNET), NULL, buscarCarnet);
		ShowWindow(vntnDatos, SW_SHOW);
		hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(vntnDatos, hMenu);
		DestroyWindow(hwnd);
	}break;
	case ID_REPORTES: {
		HWND vntnDatos = CreateDialog(hgInst, MAKEINTRESOURCE(IDD_ORDENAMIENTO), NULL, funcionReportes);
		ShowWindow(vntnDatos, SW_SHOW);
		hMenu = LoadMenu(hgInst, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(vntnDatos, hMenu);
		DestroyWindow(hwnd);
	
	}break;
	case MN_SALIR: {
		if (HIWORD(wParam) == BN_CLICKED) {
			int i = MessageBox(hwnd, "¿Estas seguro de que quieres salir?", "Salir", MB_YESNO);
			if (i == IDYES) {
				PostQuitMessage(0);
			}
			else {
				break;
			}
		}
	}break;
	}
	return;
}

////HEAPSORT!!!
//void ejecutar() {
//	Carnet* origen;
//	Carnet* aux;
//	origen = aux = NULL;
//
//	agregarNodo(origen, aux, 4);
//	agregarNodo(origen, aux, 3);
//	agregarNodo(origen, aux, 2);
//	agregarNodo(origen, aux, 1);
//
//	encuentraNodo(origen, 2);
//
//	node_swap(origen->next, aux->next->next->next);
//	while (origen->prev != NULL) {
//		origen = origen->prev;
//	}
//
//	int totalNodos = 0;
//	while (aux != NULL) {
//		aux = aux->next;
//		totalNodos++;
//	}
//	heapSort(origen, totalNodos);
//	while (origen->prev != NULL) {
//		origen = origen->prev;
//	}
//	int p = 2;
//}
//
//void agregarNodo(Carnet*& org, Carnet*& aux, int numero) {
//	if (org == NULL) {
//		org = new Carnet;
//		org->ID = numero;
//		org->next = NULL;
//		org->prev = NULL;
//		aux = org;
//	}
//	else {
//		while (aux->next != NULL) {
//			aux = aux->next;
//		}
//		aux->next = new Carnet;
//		aux->next->prev = aux;
//		aux = aux->next;
//		aux->ID = numero;
//		aux->next = NULL;
//		aux = org;
//	}
//}
//
//void estanJuntos(struct Carnet* left, struct Carnet* right) {
//	struct Carnet* tmp;
//
//	tmp = left->prev;
//	if (tmp)
//		tmp->next = right;
//	right->prev = tmp;
//
//	left->prev = right;
//	left->next = right->next;
//	right->next = left;
//	if (left->next != NULL)
//		left->next->prev = left;
//
//}
//
//void node_swap(struct Carnet* i, struct Carnet* j) {
//	struct Carnet* tmp;
//	struct Carnet* tmp2;
//	struct Carnet* tmp3;
//
//	//Esto es en caso de que esten juntos!
//	//Apunta hacia atras de i y si no es nulo, redirije hacia el nuevo nodo
//	//Despues conecta el nuevo nodo con el prev anterior de i!
//	tmp2 = j->prev;
//	tmp3 = j->next;
//	tmp = i->prev;
//
//	if (i->next == j) {
//		estanJuntos(i, j);
//		return;
//	}
//
//	if (tmp)
//		tmp->next = j;
//	j->prev = tmp;
//
//	//Ahora tenemos que hacerlo desde la derecha!!!
//	tmp = i->next;
//	if (tmp)
//		tmp->prev = j;
//	j->next = tmp;
//
//
//	//Repetimos el mismo paso pero en el otro nodo!
//	if (tmp2)
//		tmp2->next = i;
//	i->prev = tmp2;
//
//	if (tmp3)
//		tmp3->prev = i;
//	i->next = tmp3;
//
//}
//
//void heapify(Carnet* arr, int iteration) {
//	int par, node;
//	for (int i = 0; i <= iteration; i++) {
//		node = i;
//		par = (node == 0) ? 0 : (int)node / 2;
//		while (par >= 0 && node != 0) {
//			while (arr->prev != NULL) {
//				arr = arr->prev;
//			}
//			Carnet* tmpPar = encuentraNodo(arr, par);
//			while (arr->prev != NULL) {
//				arr = arr->prev;
//			}
//			Carnet* tmpNode = encuentraNodo(arr, node);
//			if (tmpPar->ID < tmpNode->ID)
//				node_swap(tmpPar, tmpNode);
//			node = par;
//			par = (node == 0) ? 0 : (int)node / 2;
//		}
//	}
//}
//
//void heapSort(Carnet* arr, int size) {
//	for (int i = size - 1; i >= 0; i--) {
//		while (arr->prev != NULL) {
//			arr = arr->prev;
//		}
//		heapify(arr, i);
//		while (arr->prev != NULL) {
//			arr = arr->prev;
//		}
//		Carnet* temp = encuentraNodo(arr, i);
//		while (arr->prev != NULL) {
//			arr = arr->prev;
//		}
//		node_swap(arr, temp);
//	}
//}
//
//Carnet* encuentraNodo(Carnet* org, int posicion) {
//	Carnet* temp = org;
//	for (int i = 0; i < posicion; i++) {
//		temp = temp->next;
//	}
//	return temp;
//}