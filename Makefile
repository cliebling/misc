CC=g++

cpptest:
	$(CC) shunt.cpp infix_test.cpp -o infix_test_cpp

ctest:
	$(CC) shunt.cpp infix_test.c -o infix_test_c
