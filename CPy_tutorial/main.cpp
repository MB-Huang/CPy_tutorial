// CPy_tutorial.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include "pch.h"
#include <iostream>
#include "PythonEmbedded.h"
#include <vector>
#include <string>

using namespace std;

class TutorialModule
{
private:
	CPyModule PythonModule;

public:
	void sayHello(void);
	float sum(float);
};

int main()
{
	// initialize the python environment
	static CPyEnvironment evi;

	// indicate .py file location
	evi.AddPath("./");

	// Load module
	std::string moduleName = "tutorial_module";
	CPyModule TutorialModule = CPyModule(moduleName);
	if (!TutorialModule.isLoaded())
	{
		PyErr_Print();
		printf("[ERROR] fail to import %s\n", moduleName);
		return 0;
	}

	// Load functions from Python file
	std::vector<std::string> FuncName_vec = { "sayHello", "sum", "max" };
	for (int i = 0; i < FuncName_vec.size(); i++)
	{
		TutorialModule.loadFunction(FuncName_vec[i].c_str());
	}

	// call function "sayHello"
	TutorialModule.callObject("sayHello", NULL);
	
	// call function "sum"
	if (TutorialModule.Callable_Check("sum"))
	{
		CPyObject result_int = PyObject_CallFunction(TutorialModule.getFunction("sum"), "i,i", 1, -2);
		std::cout << "result = " << result_int.ToInt16() << std::endl;
		std::cout << "result = " << result_int.ToInt32() << std::endl;
		std::cout << "result = " << result_int.ToInt64() << std::endl;

		CPyObject result_uint = PyObject_CallFunction(TutorialModule.getFunction("sum"), "i,i", 1, 2);
		std::cout << "result = " << result_uint.ToUInt16() << std::endl;
		std::cout << "result = " << result_uint.ToUInt32() << std::endl;
		std::cout << "result = " << result_uint.ToUInt64() << std::endl;

		CPyObject result_float = PyObject_CallFunction(TutorialModule.getFunction("sum"), "f,f", 1.5, 2.5);
		std::cout << "result = " << result_float.ToSingle() << std::endl;
		std::cout << "result = " << result_float.ToDouble() << std::endl;
	}

	// call function "max"
	// build NumPy array
	const unsigned int num = 6;
	int arr[num] = { 3,2,4,9,8,7 };
	const int ND = 1;	// the number of dimension
	npy_intp Dims[ND] = { num };	// indicate the dimension of numpy
	import_array();	// import to call before use numpy in C++
	PyObject* pNumpyArray = PyArray_SimpleNewFromData(ND, Dims, NPY_INT, reinterpret_cast<void*>(arr));

	// build Args from NumPy array
	PyObject* pArgs_array;
	pArgs_array = PyTuple_New(1);	// new a dynamic memory
	PyTuple_SetItem(pArgs_array, 0, pNumpyArray);
	CPyObject res_max = TutorialModule.callObject("max", pArgs_array);	// pass argument
	Py_DECREF(pArgs_array);			// relase dynamic memory
	std::cout << "result = " << res_max.ToInt32() << std::endl;
	
	return 0;
}