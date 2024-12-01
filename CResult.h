#ifndef CRESULT_H
#define CRESULT_H

#include <iostream>
#include <vector>

#include "CError.h"

using namespace std;

template <typename T, typename E>
class CResult
{
public:
	CResult() {

	}
	CResult(const T& cValue) {
		pc_value = new T(cValue);
	}
	CResult(E* pcError) {
		pc_value = NULL;
		v_errors.push_back(new E(*pcError));
	}
	CResult(vector<E*>& vErrors) {
		for (E* e : vErrors) {
			v_errors.push_back(new E(*e));
		}
	}
	CResult(const CResult<T, E>& cOther) {
		copyCommon(cOther);
	}

	~CResult() {
		delete pc_value;
	}

	static CResult<T, E> cOk(const T& cValue) {
		return CResult<T, E>(cValue);
	}
	static CResult<T, E> cFail(E* pcError) {
		return CResult<T, E>(pcError);
	}
	static CResult<T, E> cFail(vector<E*>& vErrors) {
		return CResult<T, E>(vErrors);
	}

	CResult<T, E>& operator=(const CResult<T, E>& cOther) {
		if (this != &cOther)
		{
			copyCommon(cOther);
		}
		return *this;
		
	}

	bool bIsSuccess()
	{
		return v_errors.empty();
	}

	T cGetValue() {
		if (pc_value != NULL) {
			return *pc_value;
		}
		else {
			//do przemyslenia
			return NULL;
		}
	}
	vector<E*>& vGetErrors() {
		return v_errors;
	}

private:
	T* pc_value;
	vector<E*> v_errors;

	void copyCommon(const CResult<T, E>& cOther) {
		
		clear();

		if (cOther.pc_value!=NULL)
		{
			pc_value = new T(*cOther.pc_value);
		}

		for (E* e : cOther.v_errors) {
			v_errors.push_back(new E(*e));
		}
	}
	void clear() {
		delete pc_value;
		pc_value = NULL;
		for (E* e : v_errors)
		{
			delete e;
		}
		v_errors.clear();
	}

};


template <typename E>
class CResult<void, E>
{
public:
	CResult();
	CResult(E* pcError) {
		v_errors.push_back(new E(*pcError));
	}
	CResult(vector<E*>& vErrors) {
		for (E* e : vErrors) {
			v_errors.push_back(new E(*e));
		}
	}
	CResult(const CResult<void, E>& cOther) {
		copyCommon(cOther);
	}

	~CResult();

	static CResult<void, E> cOk() {
		return NULL;
	}
	static CResult<void, E> cFail(E* pcError) {
		return CResult<void, E>(pcError);
	}
	static CResult<void, E> cFail(vector<E*>& vErrors) {
		return CResult<void, E>(vErrors);
	}

	CResult<void, E>& operator=(const CResult<void, E>& cOther)
	{
		if (this != &cOther)
		{
			copyCommon(cOther);
		}
		return *this;

	}

	bool bIsSuccess() 
	{
		return v_errors.size() == 0;
	}

	vector<E*>& vGetErrors() {
		return v_errors;
	}
private:
	vector<E*> v_errors;

	void copyCommon(const CResult<void, E>& cOther) {

		clear();

		for (E* e : cOther.v_errors) {
			v_errors.push_back(new E(*e));
		}
	}
	void clear() {
		for (E* e : v_errors)
		{
			delete e;
		}
		v_errors.clear();
	}

};




#endif // !CRESULT_CPP
