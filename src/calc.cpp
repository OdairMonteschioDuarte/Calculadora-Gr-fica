#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <stdexcept>
#include <cctype>    //Para o isdigit()
#include "calc.h"

//Erro:  n(expressao)
//Erros com explicação

//Nomeia os tipos de variaveis presentes na struct
enum Tipo{
    numero, operador, variavel
};

//Cria a struct e os construtores
struct Termo{
    Tipo tipo;
    double valor;
    char operacao;

    Termo(double v) : tipo(numero), valor(v), operacao(0) {}   // Numero   = (0, x, 0)
    Termo(char op) : tipo(operador), valor(0), operacao(op) {} // Operador = (1, 0, x)
    Termo(Tipo t) : tipo(variavel), valor(0), operacao(0) {}   // X        = (2, 0, 0)
};

//Para verificar o dominio da função
struct Resultado{
	double res;
	bool val;
};

//Função para converter os termos da entrada (char) para o tipo <Termo> e armazenar em um vetor
std::vector<Termo> converter(std::string entrada){

	std::vector<Termo> expressao;			//Vetor para guardar a função com seus respectivos tipos
	std::string aux = "";				//String para guardar o valor parcial do numeros, tipo um buffer
	int ponto = 0, parentese = 0, menos = 0;	//Verificadores (se é um número tem decimal, a presenta de parenteses e quantos, menos)
	char v;						//Variavel auxiliar ara facilitar escrita pegando o termo atual da entrada
	
	//Laço para verificar a autenticidade da função inserida ao identificar se cada termo e a ordem deles é valida (não verifica matematica como divisão por 0)
	for(int i = 0; i < entrada.length(); i++){
		v = entrada[i];
		
		//Verifica se v é número ou ponto
		//Se for ponto é adicionado um na variavel e caso essa varivale já for diferente de 0, manda o erro pq não pode ter mais de um ponto em um número
		//Se for um digito ou o primeiro ponto, adiciona a informação no buffer
		if(std::isdigit(v) or v == '.'){
			if(v == '.'){
				if(ponto == 0 and aux != ""){
					ponto++;
				}
				else{
					throw std::runtime_error("Pontos multiplos ou no inicio do valor");
				}
			}
			aux = aux + v;
		}
		
		//Verifica se v é uma variavel
		//Caso o buffer auxiliar não esteja vazio, o aux é inserido no vector resultado (caso aux não termine com ponto), então o operador de multiplicação também é inserido e por fim a variavél em si é inserida
		//Caso esteja vazio, apenas a variavel é inserida no vetor saida
		else if(v == 'x'){
			if(aux != "" and aux[aux.length()-1] != '.'){
				expressao.push_back(Termo(std::stod(aux)));
				aux = "";
				ponto = 0;
			}
			else if(aux != "" and aux[aux.length() - 1] == '.'){
				throw std::runtime_error("Termina com ponto");
			}
			if(!expressao.empty()){
				if(expressao.back().tipo == numero or expressao.back().tipo == variavel or expressao.back().operacao == ')'){
					expressao.push_back(Termo('*'));
				}
			}
			expressao.push_back(Termo(variavel));
			
			//O operador - é convertido em "(-1 * ", ent esse é o fechamento da multiplicação
			if(menos != 0){
				expressao.push_back(Termo(')'));
				menos = 0;
			}
		}
		
		//Verifica se v é uma operação utilizando os simbolos préselecionados
		else if(v == 's' or v == 'c' or v == 't' or v == 'l' or v == 'm' or v == '/' or v == '*' or v == '-' or v == '+' or v == '^' or v == '(' or v == ')'){
			
			//Verificação de valores inseridos anteriormente
			//Caso o buffer não esteja vazio e o ultimo termo dele seja diferente de ponto, o buffer será inserido no vetor saida
			if(aux != "" and aux[aux.length() - 1] != '.'){
				//Caso a expressão não esteja vazia e o termo anterior seja uma variavel ("x") ou um fecha aprenteses (")"), ele insere uma multiplicação antes de inserir o buffer
				if(!expressao.empty() and (expressao.back().tipo == variavel or expressao.back().operacao == ')')){
					expressao.push_back(Termo('*'));
				}
				expressao.push_back(Termo(std::stod(aux)));
				
				//O operador - é convertido em "(-1 * ", ent esse é o fechamento da multiplicação
				if(menos != 0){
					expressao.push_back(Termo(')'));
				}
				//Zera as variaveis relacionadas aos números
				menos = 0;
				ponto = 0;
				aux = "";
			}
			
			//Caso o buffer terminar com ponto, lança o erro
			else if(aux != "" and aux[aux.length() - 1] == '.'){
				throw std::runtime_error("Termina com ponto");
			}
			
			//Caso o vetor saida não esteja vazio começa a verificação dos operadores
			if(!expressao.empty()){
				char ant = expressao.back().operacao; //Termo anterior (ant) no vetor saida
				
				//Verifica se a operação inserida é compativel com o inicio da expressão ou com outros operadores
				if(v == '(' or v == ')' or v == '-'){
					//Se for um abre parenteses, verifica se o termo anterior é um numero, uma variavel ou um fecha parentes.
					//Assim insere o operador de multiplicação e depois adiciona 1 na variavel parentese
					if(v == '('){
						if(expressao.back().tipo == numero or expressao.back().tipo == variavel or ant == ')'){
							expressao.push_back(Termo('*'));
						}
						parentese++;
					}
					
					//Se for um fecha parenteses, verifica se um abre parenteses foi inserido anteriormente (variavel parentese) 
					// e se o ultimo termo é uma operação, se for, verifica se é diferente de outro fecha parenteses
					if(v == ')'){
						if(parentese == 0 or (ant != 0 and ant != ')')){
							throw std::runtime_error("Erro no fechamento dos parenteses");
						}
						parentese = parentese - 1;
					}
					
					//Se for um operador de subtração, faz uma troca pela expressão: "+( -1.0 * " 
					if(v == '-'){
						if(expressao.back().tipo != operador){
							expressao.push_back('+');
						}
						expressao.push_back('(');
						expressao.push_back(Termo(-1.0));
						expressao.push_back(Termo('*'));
						menos++;
					}
				}
				
				//Verifica os operadores anteriores, dois sinais juntos resulta em um erro
				else if(ant == '/' or ant == '*' or ant == '+' or ant == '^' or ant == '(' or ant == 's' or ant == 'c' or ant == 't' or ant == 'l' or ant == 'm'){
					throw std::runtime_error("Padrão de sinais invalido: " + std::string(1, ant) + " " + std::string(1, v));
				}
				
				//Depois das verificações, caso a operação não seja de subtração insere o operador no vetor saida
				if(v != '-'){
					expressao.push_back(Termo(v));	
				}
			}
			
			//Se a saida estiver vazia, só seram aceitos os operadores de subtração de o abre parentese
			else{
				if(v == '('){
					parentese = parentese + 1;
					expressao.push_back(Termo(v));
				}
				else if(v == '-'){
					expressao.push_back(Termo(-1.0));
					expressao.push_back(Termo('*'));
				}
				else{
					throw std::runtime_error("Sinais invalidos no começo da expressão: " + std::string(1, v));
				}
			}
		}
		
		//Caso algum caracter invalido tenha sido inserido
		else{
			throw std::runtime_error("Caracteres invalidos inseridos");
		}
	}

	//Depois de verificar todos os termos da entrada, verifica se o buffer de numeros esta vazio e se não estiver esvazia
	if(aux != ""){
		if(aux[aux.length() - 1] == '.'){
			throw std::runtime_error("Termina com ponto");
		}
		if(!expressao.empty() and (expressao.back().tipo == variavel or expressao.back().operacao == ')')){
			expressao.push_back(Termo('*'));
		}
		expressao.push_back(Termo(std::stod(aux)));
		if(menos != 0){
			expressao.push_back(Termo(')'));
			menos = 0;
		}
	}
	
	//Verifica se a expressão terminou com sinal e se algum parentese não foi fechado
	if(expressao.back().operacao != 0 and expressao.back().operacao != ')'){
		throw std::runtime_error("Expressão terminada em sinal");
	}
	if(parentese != 0){
		throw std::runtime_error("Parenteses abertos");
	}
    return expressao;
}

//Função para ordenar o vetor em notação posfixada
std::vector<Termo> RPN(std::vector<Termo>& funcao){
   std::vector<Termo> posfixa; //Vetor para guardar a expressao ordenada
   std::stack<char> pilha;     //Pilha de operadores
   
   /*
   Notação posfixada pode ser obtida da seguinte forma:
        se a entrada for um numero, passa ela para o vetor
        se a entrada for operador, verifica a "intensidade" do operador.
            se a intensidade for maior do que a do operador anterior, insere o mesmo na pilha
            se for menor, insere os operadores da pilha no vetor e depois coloca o operador atual na pilha novamente.
            se for "(", ele serve como barreira, então quando a pilha colapsar, ela só insere os termos até ele.
            "(" soh sera retirado da pilha caso o operador inserido for ")".
       
        Ex: (2+3)*5 -> 23+5*
       
        |i| e | p  |  r  |
        |0| ( | (  |     |
        |1| 2 | (  |2    |
        |2| + | (+ |2    |
        |3| 3 | (+ |23   |
        |4| ) |    |23+  |
        |5| * | *  |23+  |
        |6| 5 | *  |23+5 |
        |7|   |    |23+5*|
   */
   
    for(int i = 0; i<funcao.size(); i++){
   
       //Se for numero ou variavel, insere no vetor
       if(funcao[i].tipo == numero){
           posfixa.push_back(Termo(funcao[i].valor));
       }
       else if(funcao[i].tipo == variavel){
           posfixa.push_back(Termo(variavel));
       }
       
       //Se não, verifica o grau do operador. Caso o operador anterior for maior do que o atual, esvazia a pilha e insere o valor atual na pilha
       //Caso for um fecha parenteses, apenas esvazia a pilha até o abre parenteses
       else{
           char v = funcao[i].operacao;
           if(v == '+' or v == '-'){
               if(!pilha.empty() and pilha.top() != '(' and pilha.top() != ')'){
                   while(!pilha.empty() and pilha.top() != '('){
                       posfixa.push_back(Termo(pilha.top()));
                       pilha.pop();
                    }
                }
                pilha.push(v);
            }
            else if(v == '*' or v == '/'){
                if(!pilha.empty() and pilha.top() != '(' and pilha.top() != ')' and pilha.top() != '+' and pilha.top() != '-'){
                    while(!pilha.empty() and pilha.top() != '('){
                        posfixa.push_back(Termo(pilha.top()));
                        pilha.pop();
                    }
                }
                pilha.push(v);
            }
            else if(v == '^' or v == 's' or v == 'c' or v == 't' or v == 'm' or v == 'l'){
                if(!pilha.empty() and pilha.top() != '(' and pilha.top() != ')' and pilha.top() != '+' and pilha.top() != '-' and pilha.top() != '*' and pilha.top() != '/'){
                    while(!pilha.empty() and pilha.top() != '('){
                        posfixa.push_back(Termo(pilha.top()));
                        pilha.pop();
                    }
                }
                pilha.push(v);
            }
            else if(v == '('){
                pilha.push(v);
            }
            else if(v == ')'){
                while(pilha.top() != '('){
                    posfixa.push_back(Termo(pilha.top()));
                    pilha.pop();
               }
               pilha.pop();
            }
    	}
    }
    
    //Esvazia a pilha quando toda a entrada tiver acabado
    while(!pilha.empty()){
    	posfixa.push_back(pilha.top());
    	pilha.pop();
    }
    return posfixa;
}

//Usa a expressão posfixada para realizar o calculo da função
Resultado calculo(std::vector<Termo>& funcao, double x){
	/*Variaveis para armazenar:
		o resultado das operações
	        os operandos atuais
	        valores calculados de sen e cos
	        e a pilha com os operandos
	        qtde de coeficientes
	        coeficientes de remez
	*/
	double resultado, a, b, sen, cos;
	int t = 19;
	std::stack<double> pilha;
	const double c[t+1] = {1 , 1 , -0.5 , -0.166667 , 0.0416667 , 0.00833333 , -0.00138889 , -0.000198413 , 2.48016e-05 , 2.75573e-06 , -2.75573e-07 , -2.50528e-08 , 2.08726e-09 , 1.61151e-10 , -1.12754e-11 , -1.01665e-12 , -9.67891e-15 , 6.36588e-14 , 9.58634e-15 , -6.09549e-15};

   	
    
    /*
    A solução de operações envolvendo notações pósfixada é dada da seguinte forma:
        A leitura ocorre da esquerda pra direita, o algoritmo insere os numeros na pilha.
        Ao ler um operador, os dois primeiros numeros da pilha são retirados a operação é aplicada e o resultado volta para a pilha.
       
        Ex: 23+5*
       
        | i | e |  p  | r  |
        | 0 | 2 |  2  |    |
        | 1 | 3 | 2 3 |    |
        | 2 | + |  5  |    |
        | 3 | 5 | 5 5 |    |
        | 4 | * | 25  |    |
        | 5 |   |     | 25 |
    */
    for(int i = 0; i<funcao.size(); i++){
    	if(funcao[i].tipo == numero){
    		pilha.push(funcao[i].valor);
    	}
    	else if(funcao[i].tipo == variavel){
    		pilha.push(x);
    	}
    	else{
    		a = pilha.top();
    		pilha.pop();
    		b = pilha.top();
    		pilha.pop();
    		
    		//Variaveis auxiliares para as funções menos simples
    		double a2 = a*a, ac = a+1.57079632679489661923, ac2 = ac*ac, horner, z, z2, ln, expo;
    		int exp;
    		switch(funcao[i].operacao){
    			case '-':
    				resultado = b - a;
    				break;
    			case '+':
    				resultado = b + a;
    				break;
    			case '*':
    				resultado = b * a;
    				break;
    			case '/':
    				resultado = b / a;
    				break;
    				
    			case 'l':
    				pilha.push(b);
    				
    				//Correção de dominio
    				if(a < 0){
    					return {0.0, false};
    				}
    				z = (a-1.0)/(a+1.0), z2 = z*z;
    				horner = 1.0 + z2 * (0.3333333333 + z2 * (0.2 + z2 * (0.1428571428 + z2 * 0.1111111111)));
    				resultado = 2.0*z*horner;
    				break;
    				
    			case '^':
    				resultado = 1.0;
    				
    				//expoente inteiro
    				if(double(int(a)) == a){
    					exp = (int)(a);
    					if(exp < 0){
    						exp = exp * -1;
    					}
    					for(int i = 0; i<exp; i++){
    						resultado *= b;
    					}
    					
    					/*Expoente negativo
					  x⁻ʸ = 1
					        xʸ			
    					*/
    					if(a < 0){
    						resultado = 1.0/resultado;
    					}
    					
    				}
    				//expoente racional
    				else{
    					if(b>0.0){
    						z = (b-1.0)/(b+1.0); z2 = z*z;
    						ln = 2.0 * z * (1.0 + z2 * (0.3333333333 + z2 * (0.2 + z2 * (0.1428571428 + z2 * 0.1111111111))));
    						expo = a * ln;
    						bool negativo = (expo < 0);
    						if(negativo){
    							expo = -expo;
    						}
    						
    						horner = 1.0 + expo * (1.0 + expo * (0.5 + expo * (0.16666666666666666 + expo * (0.0416666666666666 + expo * (0.00833333333333333 + expo * (0.00138888888888888 + expo * (0.00019841269841269 + expo * (0.00002480158730158 + expo * 0.00000275573192239))))))));
    						if(negativo){
    							resultado = 1.0/horner;
    						}
    						else{
    							resultado = horner;
    						}
    					}
    					else{
    						resultado = 0.0;
    					}
    					
    				}
    				break;
    				
    			//P(x) = x.(c1+xx.(c3+xx(c5+ ... xx.cn)))
    		        case 's':
    				pilha.push(b);
    				resultado = c[((t % 2 == 0) ? t-1 : t)];
    				for(int i = ((t % 2 == 0) ? t-3 : t-2); i>0; i-=2){
    					resultado = c[i] + a2 * resultado;
    				}
    				resultado *= a;
    				break;
    				
    			//P(x) = c0 + xx.(c2+xx.(c4+xx(c6+ ... xx.cn)))
    			case 'c':
    				pilha.push(b);
    				resultado = c[((t % 2 == 0) ? t : t-1)];
    				for(int i = ((t % 2 == 0) ? t-2 : t-3); i>=0; i-=2){
    					resultado = c[i] + a2 * resultado;
    				}
    				break;
    			
    			//tan(x) = sen(x)/cos(x)
    			case 't':
    				pilha.push(b);
    				sen = c[((t % 2 == 0) ? t-1 : t)];
    				cos = c[((t % 2 == 0) ? t : t-1)];
    				for(int i = t; i>=0; i--){
    					(i % 2 == 0) ? (cos = cos * a2 + c[i]) : (sen = sen * a2 + c[i]);
    				}
    				resultado = (sen*a)/cos;
    				break;
    				
    			//se x > 0 -> x; se x < 0 -> -x
    			case 'm':
    				pilha.push(b);
    				(a < 0) ? (resultado = -a) : (resultado = a);
    				break;
    				
    		}
    		pilha.push(resultado);
    	}
    }

    return {pilha.top(), true};
}

std::vector<double> calcular(std::string entrada, int vx, int vy, double intervalo){
    if(entrada == ""){
    	throw std::runtime_error("Entrada vazia");
    }
    std::vector<Termo> saida = converter(entrada);
   
    std::vector<Termo> posfixa = RPN(saida);
    
    double escala = vx/intervalo, centroY = vy/2.0;
    Resultado valor;
    
    std::vector<double> resultado;
    for(int i = 0; i<vx; i++){
    	valor = calculo(posfixa, -intervalo/2 + i*(intervalo/vx));
    	if(valor.val == true){
    		double pontoY = valor.res;
    		resultado.push_back(i);
    		resultado.push_back(centroY - (int)(pontoY*escala));
    	}
    }    

    return resultado;
}
