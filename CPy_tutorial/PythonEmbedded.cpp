#include "pch.h"
#include "PythonEmbedded.h"

/* --------- CPyModule --------- */

CPyModule::CPyModule(const char * name ):ModuleName(name)
{
	this->Initial();
}

CPyModule::CPyModule(std::string name):ModuleName(name.c_str())
{
	this->Initial();
}

CPyModule::~CPyModule()
{
	// release function pointer
	std::map<std::string, PyObject *>::iterator iter;
	for (iter = pFuncLUT.begin(); iter != pFuncLUT.end(); iter++)
	{
		if ( iter->second )	// if the PyObject* is not NULL
		{
			Py_DECREF( iter->second );	// release PyObject*
			iter->second = NULL;
		}
	}
	// release module pointer
	if (pModule)
		Py_DECREF(pModule);
	pModule = NULL;
}

void CPyModule::Initial()
{
	pModule = PyImport_ImportModule(ModuleName);
	if (!pModule) // check the module
	{
		printf("[ERROR] Python can't get module: ");
		printf("%s\n", ModuleName);
	}
}

PyObject* CPyModule::loadFunction(const char * FuncName)
{
	if ( this->pModule ) 
	{
		// get Python function
		PyObject* pFunc = PyObject_GetAttrString(this->pModule, FuncName);

		// check the function
		if ( !pFunc || !PyCallable_Check(pFunc) )  
		{
			if (PyErr_Occurred())	PyErr_Print();
			printf("[ERROR] Python can't find fuction: %s\n", FuncName);
			return NULL;
		}

		// insert the function into LUT
		pFuncLUT[FuncName] = pFunc;

		return pFunc;
	}
	else
	{
		printf("[ERROR] Python can't get module: %s\n", ModuleName);
		return NULL;
	}
}

PyObject* CPyModule::getFunction(std::string FuncName)
{ 
	// ensure that LUT contains the specific key name
	if ( pFuncLUT.count(FuncName) )
		return pFuncLUT[FuncName];
	else {
		printf("[ERROR] Python can't find fuction: %s\n", FuncName);
		return NULL;
	}
}

bool CPyModule::Callable_Check(PyObject* pFunc)
{
	if ( pFunc )
	{
		if ( PyCallable_Check( pFunc ) )
		{
			return true;
		}
		else
		{
			PyErr_Print();
			printf("[ERROR] Python function can't be called\n");
			return false;
		}
	}
	else
	{
		printf("[ERROR] Python function can't be found\n");
		return false;
	}
}

bool CPyModule::Callable_Check(std::string FuncName)
{
	PyObject* pFunc = this->getFunction(FuncName);
	if (this->Callable_Check(pFunc))
		return true;
	else
	{
		std::cout << "[ERROR] error happend in '" << FuncName << "'\n";
		return false;
	}
}


PyObject* CPyModule::callObject(std::string FuncName, PyObject* pArgs)
{
	PyObject* pFunc = this->getFunction(FuncName);
	if (this->Callable_Check(pFunc)) {
		//if (!pArgs)	return NULL;

		PyObject* Res = PyObject_CallObject(pFunc, pArgs);	// call func by given argument
		if (!Res) {
			std::cout << "[ERROR] python function '" << FuncName << "' can't return value." << std::endl;
			return NULL;
		}
		return Res;
	}
	else {
		std::cout << "[ERROR] error happend in '" << FuncName << "'\n";
		return NULL;
	}
}

PyObject* CPyModule::callFunction(std::string name, const char *format, ...)
{
	PyObject* pFunc = this->getFunction(name);
	if (pFunc && PyCallable_Check(pFunc))
	{
		int count = 0;

		va_list args;			// create a list of variable arguments
		va_start(args, format);	// init the list using va_start()

		if (format != nullptr)
			std::cout << "format: " << format << std::endl;

		while (format != nullptr && *format != '\0')
		{
			if (*format == 'd' || *format == 'i') {
				int i = va_arg(args, int);
				std::cout << "input argument: " << i << '\n';
			}
			else if (*format == 's') {
				char * s = va_arg(args, char*);
				std::cout << "input argument: " << s << '\n';
			}
			++format;
			++count;
		}

		if (format != nullptr)
		{
			format -= count;
			std::cout << "format: " << format << std::endl;
		}

		PyObject* Res = PyObject_CallFunction(pFunc, format, args);	// call function with variadic argument
		va_end(args);			// clean variable list

		if (!Res) {
			if (PyErr_Occurred())	PyErr_Print();
			std::cout << "[ERROR] python function '" << name << "' can't return value." << std::endl;
			return NULL;
		}
		return Res;	
	}
	else
	{
		if (PyErr_Occurred())	PyErr_Print();
		std::cout << "[ERROR] python function '" << name << "' can't be found." << std::endl;
		return NULL;
	}
}


/* --------- CPyObect --------- */

std::string CPyObject::ToString(void)
{
	std::string str;
	PyArg_Parse(this->pObj, "s", str.c_str());
	return str;
}

float CPyObject::ToSingle(void)
{
	float fValue;
	PyArg_Parse(this->pObj, "f", &fValue);
	return fValue;
}

double CPyObject::ToDouble()
{
	double dValue;
	PyArg_Parse(this->pObj, "d", &dValue);
	return dValue;
}

short CPyObject::ToInt16(void)
{
	short hValue;
	PyArg_Parse(this->pObj, "h", &hValue);
	return hValue;
}

int CPyObject::ToInt32(void)
{
	int iValue;
	PyArg_Parse(this->pObj, "i", &iValue);
	return iValue;
}

long CPyObject::ToInt64(void)
{
	short lValue;
	PyArg_Parse(this->pObj, "l", &lValue);
	return lValue;
}

unsigned short CPyObject::ToUInt16(void)
{
	short HValue;
	PyArg_Parse(this->pObj, "H", &HValue);
	return HValue;
}

unsigned int CPyObject::ToUInt32(void)
{
	int IValue;
	PyArg_Parse(this->pObj, "I", &IValue);
	return IValue;
}

unsigned long CPyObject::ToUInt64(void)
{
	short kValue;
	PyArg_Parse(this->pObj, "k", &kValue);
	return kValue;
}
