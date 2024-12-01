#ifndef CERROR_H
#define CERROR_H

#include <iostream>

const std::string DEFAULT_ERROR_MESSAGE = "DEFAULT ERROR";

class CError {

public:
	CError();
	CError(const std::string err);
	CError(const CError& cOther);
	~CError();

	std::string sGetError();
	void sSetError(std::string err);
private:
	std::string s_error;

};

#endif // !CERROR_CPP
