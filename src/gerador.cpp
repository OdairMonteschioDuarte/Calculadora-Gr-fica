#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

double potencia(double base, int exp) {
    double res = 1.0;
    for (int i = 0; i < exp; i++){
    	res = res * base;
    }
    return res;
}

double fatorial(int n) {
    double res = 1.0;
    for (int i = 1; i <= n; i++){
    	res = res * i;
    }
    return res;
}

// Seno calculado usando a Serie de Taylor
/*
	somoratorio de (-1)^i * (x^(2i + 1)) / (2i + 1)!
	
	Ex:     i=0 -> (-1)^0 * (x^(0 + 1)) / (0 + 1)! = x
		i=1 -> (-1)^1 * (x^(2 + 1)) / (2 + 1)! = -(x^3)/3!
	    	i=2 -> (-1)^2 * (x^(4 + 1)) / (4 + 1)! = (x^5)/5!
	    
	    sen(x) = x - x³ + x⁵ - x⁷ + ...
	    	         3!   5!   7!

	O laço itera 15 vezes, verifica se o i é par ou impar. Se for par multiplica o resultado da pontencia por 1 e se for impar por -1.


*/
double sen(double x) {
    double resultado = 0.0;
    for (int i = 0; i < 15; i++) {
        double sinal = (i % 2 == 0) ? 1.0 : -1.0;
        resultado = resultado + (sinal * potencia(x, 2 * i + 1) / fatorial(2 * i + 1));
    }
    return resultado;
}

// Cosseno calculado usando a Serie de Taylor
/*
	somoratorio de (-1)^i * (x^(2i)) / (2i)!
	
	Ex:     i=0 -> (-1)⁰ * (x⁰) / 0! = 1
		i=1 -> (-1)¹ * (x²) / 2! = -(x^2)/2!
	    	i=2 -> (-1)² * (x⁴) / 4! = (x^4)/4!
	    
	    sen(x) = 1 - x² + x⁴ - x⁶ + ...
	    	         2!   4!   6!

	O laço itera 15 vezes, verifica se o i é par ou impar. Se for par multiplica o resultado da pontencia por 1 e se for impar por -1.


*/
double cos(double x) {
    double resultado = 0.0;
    for (int i = 0; i < 15; i++) {
        double sinal = (i % 2 == 0) ? 1.0 : -1.0;
        resultado = resultado + sinal * potencia(x, 2 * i) / fatorial(2 * i);
    }
    return resultado;
}


//Resolvedor de sistema linear
/*
A = a00 a01 a02 ... a0m  b = b0
    a10 a11 a12 ... a1m      b1
    a20 a21 a22 ... a2m      b2
    ... ... ... ... ...      ..
    an0 an1 an2 ... anm      bn
*/
std::vector<double> resolverSL(std::vector<std::vector<double>> A, std::vector<double> b) {
    int n = b.size();
    for (int i = 0; i < n; i++) {
        int linhaPivo = i;
        //encontra qual linha tem a respectiva coluna (i) maior e guarda a sua posição
        for (int k = i + 1; k < n; k++) {
            if (((A[k][i] > 0) ? A[k][i] : -A[k][i]) > ((A[linhaPivo][i] > 0) ? A[linhaPivo][i] : -A[linhaPivo][i])) {
                linhaPivo = k;
            }
        }
        //troca a linha mais acima pela linha com o pivo
        std::vector<double> auxA = A[i];
        A[i] = A[linhaPivo];
        A[linhaPivo] = auxA;
        double auxb = b[i];
        b[i] = b[linhaPivo];
        b[linhaPivo] = auxb;
        
        //redução
        for (int k = i + 1; k < n; k++) {
            double m = A[k][i] / A[i][i];
            for (int j = i; j < n; j++) {
                A[k][j] = A[k][j] - m * A[i][j];
            }
            b[k] = b[k] - m * b[i];
        }
    }
    
    //substituição regressiva
    std::vector<double> x(n);
    for (int i = n - 1; i >= 0; i--) {
        double sum = 0.0;
        for (int j = i + 1; j < n; j++) {
            sum = sum + A[i][j] * x[j];
        }
        x[i] = (b[i] - sum) / A[i][i];
    }
    
    return x;
}

void gerarRemez(int grau, double iInicio, double iFim) {
    //Quantidade de variáveis desconhecidas
    int n = grau + 2;
    
    /*
    Nós de Chebyshev
    
    	A partir de um intervalo [a, b]
    		ponto_i =  (a+b) - (b-a)cos(angulo)
    			     2            2
    	Angulo (n sendo a qtd de pontos):
    		(2i - 1)pi
    		   2n
    */
    std::vector<double> pontos(n);
    for (int i = 1; i < n; i++) {
        double angulo = (2.0 * i - 1.0) * iFim*2 / (2.0 * n);
        pontos[i-1] = 0.5 * (iInicio + iFim) + 0.5 * (iFim - iInicio) * cos(angulo);
    }
    
    //ORDENAR MANUALMENTE
    std::sort(pontos.begin(), pontos.end());
    
    //Monta o Sistema Linear
    /*
    A = a00 a01 a02 ... a0m  b = vetorSen = vetorCos = b0
    	a10 a11 a12 ... a1n      		       b1
    	a20 a21 a22 ... a2n     		       b2
    	... ... ... ... ...      		       ..
    	an0 an1 an2 ... ann      		       bn
    	
    
    */
    std::vector<std::vector<double>> A(n, std::vector<double>(n, 0.0));
    std::vector<double> vetorSen(n, 0.0), vetorCos(n, 0.0);
    
    for (int i = 0; i < n; i++) {
        double x = pontos[i];
        for (int j = 0; j <= n; j++) {
            A[i][j] = potencia(x, j);
        }
        //O ultimo valor da matriz é um termo independente relacionado ao erro maximo, ele alterna entre E e -E dependendo do indice da linha na matriz
        A[i][n-1] = (i % 2 == 0) ? 1.0 : -1.0;
        vetorSen[i] = sen(x);
        vetorCos[i] = cos(x);
    }
    
    //Resolve o sistema de Álgebra Linear
    std::vector<double> resultadoSen = resolverSL(A, vetorSen);
    std::vector<double> resultadoCos = resolverSL(A, vetorCos);
    
    //Extração do valor do erro maximo
    std::vector<double> coefSen(resultadoSen.begin(), resultadoSen.end() - 1);
    double erroSen = resultadoSen.back();
    std::vector<double> coefCos(resultadoCos.begin(), resultadoCos.end() - 1);
    double erroCos = resultadoCos.back();
    
    std::cout << "Erro máximo estimado Seno: " << (erroSen > 0 ? erroSen : -erroSen) << "\n\n";
    std::cout << "Erro máximo estimado Cosseno: " << (erroCos > 0 ? erroCos : -erroCos) << "\n\n";
    
    std::cout << std::fixed << std::setprecision(10);
    for (size_t i = 0; i < coefSen.size(); i++) {
        //Ignoraa coeficientes muito próximos de zero (termos pares no Seno)
        if (std::abs(coefSen[i]) > 1e-10) {
            std::cout << "seno" << i << " = " << coefSen[i] << ";\n";
        }
    }
    std::cout << std::endl;
    std::cout << std::fixed << std::setprecision(10);
    for (size_t i = 0; i < coefCos.size(); i++) {
        //Ignora coeficientes muito próximos de zero (termos impares no Cosseno)
        if (std::abs(coefCos[i]) > 1e-10) {
            std::cout << "cosseno" << i << " = " << coefCos[i] << ";\n";
        }
    }
}

int main() {
    // Usando um polinómio de grau x e o pi como intervalo
    const double pi = 3.14159265358979323846;
    gerarRemez(20, -pi/2.0, pi/2.0);
    
    return 0;
}
