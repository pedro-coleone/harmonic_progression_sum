#include <iostream>
#include <sstream>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace chrono;

void sum(char* output, const long unsigned int d, const long unsigned int n) {
    const int num_threads = omp_get_max_threads();

    long unsigned int digits[d + 11];
    for (long unsigned int digit = 0; digit < d + 11; ++digit) {
        digits[digit] = 0;
    }

    #pragma omp parallel for reduction(+:digits[:d+11])
    for (long unsigned int i = 1; i <= n; ++i) {
        long unsigned int remainder = 1;
        int t = omp_get_thread_num();
        for (long unsigned int digit = 0; digit < d + 11 && remainder; ++digit) {
            long unsigned int div = remainder / i;
            long unsigned int mod = remainder % i;
            #pragma omp atomic
            digits[digit] += div;
            remainder = mod * 10;
        }
    }
    for(long unsigned int i=d+11-1;i>0;--i) {
		digits[i-1]+=digits[i]/10;
		digits[i]%=10;
	}
	if(digits[d+1]>=5) {
		++digits[d];
	}
	for(long unsigned int i=d;i>0;--i) {
		digits[i-1]+=digits[i]/10;
		digits[i]%=10;
	}


	stringstream stringstreamA;
	stringstreamA<<digits[0]<<",";
	for(long unsigned int i=1;i<=d;++i) {
		stringstreamA<<digits[i];
	}

	string stringA=stringstreamA.str();
	stringA.copy(output,stringA.size());
}

int main() {
    long unsigned int d = 100, n = 10000000;

    char output[d + 10]; // extra precisão devido a possíveis erros

    cout << "Progressão harmônica com " << d << " casas decimais e com " << n << " elementos e " << omp_get_max_threads() << " threads." << endl;

    sum(output, d, n);

    cout << "Resultado: " << output << endl;

    return 0;
}