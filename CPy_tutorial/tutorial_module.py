# Filename tutorial_module.py

def sayHello():
	print('Hello world!')

def sum(a,b):
	print( 'add ', a, ' and ', b)
	return a+b

def max(arr):
	import numpy as np
	print('find maximum of the array: ', arr)
	return np.amax(arr)