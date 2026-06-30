#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <stdexcept>
#include <cctype>    //Para o isdigit()
#include <math.h>
#include "calc.h"
//Erro:  n(expressao)
//Nomeia os tipos de variaveis presentes na struct
enum Tipo{
    numero, operador, variavel
};

//Cria a struct e os construtores
struct Termo{
    Tipo tipo;
    double valor;
    char operacao;

    Termo(double v) : tipo(numero), valor(v), operacao(0) {}   // Numero = (0, x, 0)
    Termo(char op) : tipo(operador), valor(0), operacao(op) {} // Operador = (1, 0, x)
    Termo(Tipo t) : tipo(variavel), valor(0), operacao(0) {}   // X = (2, 0, 0)
};

//Função para converter os termos da entrada (char) para o tipo Termo e armazenar em um vetor
std::vector<Termo> converter(std::string entrada){

	std::vector<Termo> expressao;			//Vetor para guardar a função com seus respectivos tipos
	std::string aux = "";				//String para guardar o valor parcial do numeros
	int ponto = 0, parentese = 0, menos = 0;	//Verificadores
	char v;						//Para facilitar escrita
	for(int i = 0; i < entrada.length(); i++){
		v = entrada[i];
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
			if(menos != 0){
				expressao.push_back(Termo(')'));
				menos = 0;
			}
		}
		else if(v == 's' or v == 'c' or v == 't' or v == 'l' or v == 'm' or v == '/' or v == '*' or v == '-' or v == '+' or v == '^' or v == '(' or v == ')'){
			if(aux != "" and aux[aux.length() - 1] != '.'){
				if(!expressao.empty() and (expressao.back().tipo == variavel or expressao.back().operacao == ')')){
					expressao.push_back(Termo('*'));
				}
				expressao.push_back(Termo(std::stod(aux)));
				if(menos != 0){
					expressao.push_back(Termo(')'));
				}
				menos = 0;
				ponto = 0;
				aux = "";
			}
			else if(aux != "" and aux[aux.length() - 1] == '.'){
				throw std::runtime_error("Termina com ponto");
			}
			if(!expressao.empty()){
				char e = expressao.back().operacao;
				if(v == '(' or v == ')' or v == '-'){
					if(v == '('){
						if(expressao.back().tipo == numero or expressao.back().tipo == variavel or expressao.back().operacao == ')'){
							expressao.push_back(Termo('*'));
						}
						parentese = parentese + 1;
					}
					if(v == ')'){
						if(parentese == 0 or (expressao.back().operacao != 0 and expressao.back().operacao != ')')){
							throw std::runtime_error("Erro no fechamento dos parenteses");
						}
						parentese = parentese - 1;
					}
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
				else if(e == '/' or e == '*' or e == '+' or e == '^' or e == '(' or e == 's' or e == 'c' or e == 't' or e == 'l' or e == 'm'){
					throw std::runtime_error("Padrão de sinais invalido");
				}
				if(v != '-'){
					expressao.push_back(Termo(v));	
				}
			}
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
					throw std::runtime_error("Sinais invalidos no começo da expressão");
				}
			}
		}
		else{
			throw std::runtime_error("Caracteres invalidos inseridos");
		}
	}

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
	if(expressao.back().operacao != 0 and expressao.back().operacao != ')'){
		throw std::runtime_error("Expressão terminada em sinal");
	}
	if(parentese != 0){
		throw std::runtime_error("Parenteses abertos");
	}

	
    /*
    //Vetor para entrada
    std::vector<Termo> expressao;
    int op = 0;
    int parentese = 0;
    //String para guardar o numero para conversão (string pois se fosse char 12 viraria 1 e 2 ou 1.2 viraria 1, . e 2)
    std::string numeral = "";
   
    for(int i = 0; i<entrada.length(); i++){
        //Variavel para facilitar a leitura do codigo
        char v = entrada[i];
       
        //Se v == x, usa o construtor de variavel e insere o resultado no vetor
        if(v == 'x'){
            expressao.push_back(Termo(variavel));
        }
        //Se v != operador conhecido, concatena o numero
        else if(v == '0' or v == '1' or v == '2' or v == '3' or v == '4' or v == '5' or v == '6' or v == '7' or v == '8' or v == '9' or v == '.'){
            numeral = numeral + v;
            op = 0;
        }
        else if(v == '-' or v == '+' or v == '*' or v == '/' or v == '^' or v == '(' or v == ')'){

            if(v != '(' and v != ')' and v != '-'){
            	op++;
            }
            if(v == '('){
            	parentese = parentese + 1;
            }
            if(v == ')'){
            	parentese = parentese - 1;
            }
            
            if(op >= 2 or (i == entrada.length() -1 and i != ')')){
            	std::cout<<"O: "<<op<<std::endl<<"i: "<<i<<std::endl;
            	throw std::runtime_error("Padrão de termos invalido");
            }
            if(numeral != ""){
                expressao.push_back(Termo(std::stod(numeral)));
                numeral = "";
            }

            
            expressao.push_back(Termo(v));
        }
        else{
        	throw std::runtime_error("Valor invalido: " + v);
        }
    }
    //Como a inserção de numero soh ocorre depois de um operador, essa condição armazena um possivel ultimo valor que poderia se perder
    std::cout<<"P: "<<parentese<<std::endl;
    if(numeral != ""){
        expressao.push_back(Termo(std::stod(numeral)));
    }

    if(parentese != 0){
    	throw std::runtime_error("Parenteses aberto");
    }
    */
    
    return expressao;
}

//Função para ordenar o vetor em notação posfixada
std::vector<Termo> RPN(std::vector<Termo>& funcao){
    //Vetor para guardar a expressao ordenada
   std::vector<Termo> posfixa;
   //Pilha de operadores
   std::stack<char> pilha;
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
       else{
           char v = funcao[i].operacao;
           //Verifica a operação
           if(v == '+' or v == '-'){
               //Verifica o grau da operação
               if(!pilha.empty() and pilha.top() != '(' and pilha.top() != ')'){
                   //Se o grau atual for menor do que o anterior, esvazia a pilha
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
   while(!pilha.empty()){
       posfixa.push_back(pilha.top());
       pilha.pop();
   }

   return posfixa;
}

//Usa a expressão posfixada para realizar o calculo da função
double calculo(std::vector<Termo>& funcao, double x){
    double resultado, a, b, sen, cos;
    std::stack<double> pilha;
    /*for(int i = 0; i<funcao.size(); i++){
    	if(funcao[i].tipo == numero){
    		std::cout<<"Valor[ "<<funcao[i].valor<<" ] ";
    	}
    	else if(funcao[i].tipo == operador){
    		std::cout<<"Operação[ "<<funcao[i].operacao<<" ] ";
    	}
    	else{
    		std::cout<<"Variavel[x] ";
    	}
    }*/
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
    const double c0 = 1.0, c1 = 1.0, c2 = -0.5, c3 = -0.1666666667, c4 = 0.0416666667, c5 = 0.0083333333, c6 = -0.0013888889, c7 = -0.0001984127, c8 = 0.0000248016, c9 = 0.0000027557, c10 = -0.0000002756, c11 = -0.0000000251, c12 = 0.0000000021, c13 = 0.0000000002;
    
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
    			case '^':
    				/*resultado = 1;
    				if(std::floor(a) != a){
    					if(x < 0){
    						resultado = 0;
    						break;	
    					}
    					if(a < 0){
    						a = -a;
    						b = 1/b;
    					}
    					z = (b-1.0)/(b+1.0), z2 = z*z;
    					resultado = 1.0 + z2 * (0.3333333333 + z2 * (0.2 + z2 * (0.1428571428 + z2 * 0.1111111111)));
    					resultado = resultado * z * 2.0;
    					horner = 1 + (a*resultado) * (1.0 + (a*resultado) * (0.5 + (a*resultado) * (0.16666666666666666 + (a*resultado) * (0.0416666666666666 + (a*resultado) * (0.00833333333333333 + (a*resultado) * (0.00138888888888888 + (a*resultado) *(0.00019841269841269 + (a*resultado) *(0.00002480158730158 + (a*resultado) *0.00000275573192239))))))));
    					resultado = horner;
    					
    				}
    				else if(a>=0){
    					for(int i = 0; i<a; i++){
    						resultado = resultado*b;
    					}
    				}
    				else{
    					for(int i = 0; i<abs(a); i++){
    						resultado = resultado*b;
    					}
    					resultado = 1/resultado;
    				}*/
    				
    				resultado = 1.0;
    				if(std::floor(a) == a){
    					exp = (int)(a);
    					if(exp < 0){
    						exp = exp * -1;
    					}
    					for(int i = 0; i<exp; i++){
    						resultado *= b;
    					}
    					if(a < 0){
    						resultado = 1.0/resultado;
    					}
    					
    				}
    				else{
    					if(b>0.0){
    						z = (b-1.0)/(b+1.0);
    						z2 = z*z;
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
    				
    			//P(x) = x.(c1+xx.(c3+xx(c5+ ... xx.cu)))
    		        case 's':
    				pilha.push(b);
    				
    				resultado = a * (c1 + a2 * (c3 + a2 * (c5 + a2 * (c7 + a2 * (c9 + a2 * (c11 + a2 * c13))))));
    				break;
    			//P(x) = c0 + x.(c2+xx.(c4+xx(c6+ ... xx.cu)))
    			case 'c':
    				pilha.push(b);
    				
    				//resultado = c0 + a * (c2 + a2 * (c4 + a2 * (c6 + a2 * (c8 + a2 * (c10 + a2 * c12)))));
    				resultado = ac * (c1 + ac2 * (c3 + ac2 * (c5 + ac2 * (c7 + ac2 * (c9 + ac2 * (c11 + ac2 * c13))))));
    				break;
    			case 't':
    				pilha.push(b);
    				sen = a * (c1 + a2 * (c3 + a2 * (c5 + a2 * (c7 + a2 * (c9 + a2 * (c11 + a2 * c13))))));
    				cos = c0 + a2 * (c2 + a2 * (c4 + a2 * (c6 + a2 * (c8 + a2 * (c10 + a2 * c12)))));
    				resultado = sen/cos;
    				break;
    			case 'm':
    				pilha.push(b);
    				
    				if(a < 0){
					resultado = a * -1;
				}
				else{
					resultado = a;
				}
    				break;
    			case 'l':
    				if(x < 0){
    					resultado = 0;
    					break;	
    				}
    				z = (a-1.0)/(a+1.0), z2 = z*z;
    				horner = 1.0 + z2 * (0.3333333333 + z2 * (0.2 + z2 * (0.1428571428 + z2 * 0.1111111111)));
    				pilha.push(b);
    				resultado = 2.0*z*horner;
    				break;
    				
    		}
    		pilha.push(resultado);
    	}
    }

    return pilha.top();
}

std::vector<double> calcular(std::string entrada, int vx, int vy, double intervalo){
    if(entrada == ""){
    	throw std::runtime_error("Entrada vazia");
    }
    std::vector<Termo> saida = converter(entrada);
   
    std::vector<Termo> posfixa = RPN(saida);
    
    double escala = vx/intervalo, centroY = vy/2.0;
    
    
    std::vector<double> resultado;
    for(int i = 0; i<vx; i++){
    	resultado.push_back(i);
    	double pontoY = calculo(posfixa, -intervalo/2 + i*(intervalo/vx));
    	resultado.push_back(centroY - (int)(pontoY*escala));
    }    

    return resultado;
}
