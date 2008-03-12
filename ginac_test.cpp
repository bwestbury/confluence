// #include <iostream> 
// #include <ginac/ginac.h> 
// 
// using namespace std; 
// using namespace GiNaC;
// 
// int main() 
// { 
// 	symbol x("x"), y("y"); 
// 	ex poly; 
// 	for (int i=0; i<3; ++i) 
// 	poly += factorial(i+16)*pow(x,i)*pow(y,2-i); 
// 	cout << poly << endl; 
// 	return 0; 
// }

#include <iostream> 
#include <string> 
#include <stdexcept> 
#include <ginac/ginac.h>

using namespace std; 
using namespace GiNaC; 

int main() 
{ 
	symbol x("x"); 
	string s; 
	cout << "Enter an expression containing ’x’: "; 
	getline(cin, s); 
	try { 
		ex e(s, lst(x)); 
		cout << "The derivative of " << e << " with respect to x is "; 
		cout << e.diff(x) << ".\n"; 
	} catch (exception &p) { 
		cerr << p.what() << endl; 
	} 
}