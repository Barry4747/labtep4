#include "CError.h"


CError::CError() 
{
	s_error = DEFAULT_ERROR_MESSAGE;
}

CError::CError(const std::string err) 
{
	s_error = err;
}

CError::CError(const CError& cOther) 
{
	s_error = cOther.s_error;
}

CError::~CError(){}


std::string CError::sGetError()
{
	return s_error;
}

void CError::sSetError(std::string err)
{
	s_error = err;
}
