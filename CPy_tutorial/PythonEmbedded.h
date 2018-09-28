#ifndef PYTHONEMBEDDED_H_
#define PYTHONEMBEDDED_H_

#include <Python.h>				// location: ${PythonPath}\include
#include "numpy/arrayobject.h"	// location: ${PythonPath}\Lib\site-packages\numpy\core\include
#include <iostream>
#include <string>
#include <map>

class CPyEnvironment
{
public:
	CPyEnvironment()
	{
		Py_Initialize();	// initialize the python environment
		PyRun_SimpleString("import sys");
		PyRun_SimpleString("sys.path.append('./')");	// the location of .py file
	}

	~CPyEnvironment()
	{
		Py_Finalize();	// release the environment
	}

	void AddPath(std::string filePath)
	{
		PyRun_SimpleString( ("sys.path.append('" + filePath + "')").c_str() );
	}
};

class CPyModule
{
private:
	PyObject * pModule;  
	const char* ModuleName;

	// one to one map look up table
	std::map<std::string, PyObject *> pFuncLUT;

	void Initial();
public:
	CPyModule(void) { pModule = NULL; };
	CPyModule(const char*);
	CPyModule(std::string name);
	~CPyModule();

	bool isLoaded(void) { return pModule ? true : false; };

	PyObject* loadFunction(const char*);
	PyObject* loadFunction(std::string FuncName) { loadFunction(FuncName.c_str()); }
	PyObject* getFunction(std::string FuncName);
	bool Callable_Check(PyObject* pFunc);
	bool Callable_Check(std::string FuncName);
	PyObject* callObject(std::string FuncName, PyObject* pArgs);
	PyObject* callFunction(std::string name, const char *format, ...);
	
};

class CPyObject
{
private:
	PyObject* pObj;
public:
	CPyObject():pObj(NULL){}
	CPyObject(PyObject* InputObj) :pObj(InputObj) {}
	~CPyObject()
	{
		if (pObj)
			Py_DECREF(pObj);
		pObj = NULL;
	}

	/* --- data conversion ( C <==> Py ) ---*/
	std::string ToString(void);
	float ToSingle();
	double ToDouble();
	short ToInt16();
	int ToInt32();
	long ToInt64();
	unsigned short ToUInt16();
	unsigned int ToUInt32();
	unsigned long ToUInt64();

	/* ------------- operator ------------- */
	operator bool() {return pObj ? true : false;}
	void operator =(PyObject* another) { pObj = another; }	// copy assignment
	PyObject* operator =(CPyObject another) { return pObj; }
};

#endif // !PYTHONEMBEDDED_H_
