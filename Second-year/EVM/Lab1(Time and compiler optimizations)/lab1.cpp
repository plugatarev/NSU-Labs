#include <iostream>
#include <string>
#include <stdlib.h>


using namespace std;

bool is_digit(char * s){
    for (size_t i = 0; s[i] != '\0'; i++){
	if (!(s[i] >= '0' && s[i] <= '9')) return 0;
    }
    return 1;
}

double exhibitor(double x, size_t eps){
   double a = 1, s = 1;
   for(size_t i = 1; i <= eps; i++) {
     a *= x / i;
     s += a;
   }
   return s;
}

void foo(){
    for (int i = 0; i < 100000; i++);
}

int main(int argc, char **argv){
    //1 - accuracy, 2 - argument
    if (!is_digit(argv[1]) || !is_digit(argv[2]) || argc != 3){
	cout << "incorect data!" << endl;
	return 0;
    }
    foo();
    long int n = atoll(argv[1]);
    long int x = atoll(argv[2]);
    cout << exhibitor(x,n) << endl;

    return 0;
}
