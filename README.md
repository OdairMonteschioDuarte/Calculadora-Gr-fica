# Sobre o Projeto
Uma calculadora gráfica 2D interativa de código aberto, desenvolvida em C++ utilizando OpenGL/GLFW para a renderizaçao e Dear ImGui para a interface de usuario.

Um dos principais objetivos do projeto foi não utilizar nenhuma biblioteca matematica, sendo necessario aplicar metodos manuais para o calculo de coisas como modulo, funçoes trigonometricas, potencia, radiciaçao, etc.

# Instalação e Uso

## 1 - Clone o Repositorio

    git clone https://github.com/OdairMonteschioDuarte/Calculadora-Gr-fica

## 2 - Compilação (opcional)

### 2.1 - Linux

    cd calculadora
    make linux

### 2.2 - Windows

A compilação deste programa no Windows tem como pré-requisito a instalação do MSYS2, ela pode ser feita da seguinte forma:

1. Acessar o site oficial: https://www.msys2.org/
2. Baixar o executavel correspondente à arquitetura do seu processador e executar.
3. Dentro do terminal do MSYS2, rode o seguinte comando para instalar o compilador g++ e Make e depois utilize o make para compilar:

        pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain

        cd calculadora
        make windows

# Uso

Para utilizar basta clicar no executavel criado pela compilação ou rodar via terminal através do comando:

    ./[nomeDoExecutavel]

# Funcionalidades

 - Interpretador RPN
 - Funções 
    - Polinomiais
    - Racionais
    - Irracionais
    - Modulares
    - Exponenciais
    - Logaritmicas Naturais
    - Trigonometricas Basicas
 - Multiplas Plotagens Simultaneas
 - Alteração dos Parametros da Função em Tempo Real

# Tecnologias Utilizadas

* **Linguagem:** C/C++
* **Renderização:** [OpenGL 3.3+](https://www.opengl.org/)
* **Gerenciamento de Janelas e Contexto:** [GLFW 3.4](https://www.glfw.org/)
* **Interface Gráfica:** [Dear ImGui](https://github.com/ocornut/imgui)

# Estrutura do Projeto

```text
├── dependencias/
│   ├── glfw-3.4/             # Binários/Código do GLFW para Linux
│   └── glfw-3.4.bin.WIN64/   # Binários pré-compilados do GLFW para Windows
├── imgui/                    # Código-fonte da biblioteca Dear ImGui
├── src/
│   ├── gerador.cpp           # Implementação do gerador de Coeficinetes de Remez
│   ├── calc.h
│   └── calc.cpp              # Implementação da parte matematica
├── imgui.ini
├── LICENSE
├── README.md
├── main.cpp                  # Loop principal da aplicação e interface ImGui
└── makefile                  # Automatizador de build
```

# Licença

Este projeto está licenciado sob a **MIT License**.

# Autores

* [Odair Monteschio Duarte](https://github.com/OdairMonteschioDuarte)
* [Gabriel de Paula Gaspar Pinto](https://github.com/gpgp2006)
* [Yasmin Cabrini Brancaleone]()
