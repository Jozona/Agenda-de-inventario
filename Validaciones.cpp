#include "Validaciones.h"

//Compara todos los caracteres del string para ver si tiene numeros o un punto
bool vld::sonDigitos(const std::string& str) {
	return str.find_first_not_of("0123456789.") == std::string::npos;
}

//Funcion que solo permite letras y espacios usando regex
bool vld::soloLetras(const std::string& str) {
	 return !regex_match(str, regex("^[A-Za-z ]+$"));
}

//Funcion para verificar si el texto tiene espacios
bool vld::hayEspacios(const std::string& str) {
	int count = 0;
	int length = str.length();
	for (int i = 0; i < length; i++) {
		int chr = str[i];
		if (isspace(chr))
			return true;

	}
	return false;
}

//Funcion para verificar si la curp es correcta
bool vld::curpValida(const std::string& str) {

	string posibles = "[A-Z]{1}[AEIOU]{1}[A-Z]{2}[0-9]{2}(0[1-9]|1[0-2])(0[1-9]|1[0-9]|2[0-9]|3[0-1])[HM]{1}(AS|BC|BS|CC|CS|CH|CL|CM|DF|DG|GT|GR|HG|JC|MC|MN|MS|NT|NL|OC|PL|QT|QR|SP|SL|SR|TC|TS|TL|VZ|YN|ZS|NE)[B-DF-HJ-NP-TV-Z]{3}[0-9A-Z]{1}[0-9]{1}$";
	regex rx(posibles);
	bool correcta = regex_match(str.c_str(), rx);
	if (correcta) {
		return true;
	}
	else {
		return false;
	}
}