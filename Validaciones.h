#ifndef _VALIDACIONES_
#define _VALIDACIONES_
#include <string>
#include <stdlib.h>
#include <regex>
#include <cstring>
#include <cctype>
using namespace std;
class vld {
public:
	bool sonDigitos(const std::string& str);
	bool hayEspacios(const std::string& str);
	bool soloLetras(const std::string& str);
	bool curpValida(const std::string& str);
};

#endif