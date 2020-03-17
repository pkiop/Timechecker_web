#python_class_example.py

class MyClass:
	"""A simple example class이건 독 스트링"""
	i = 12345

	def f(self):
		return 'hello world'

	def __init__(self, a):
		self.i = a

	# def __init__(self):
	# 	self.i = 22222

x = MyClass(3)
print(x.i)
print(x.f())