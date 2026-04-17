# Makefile simples para Calculadora Grafica

# Compilador
CXX = g++

# Flags de inclusão
INCLUDES = -I imgui -I imgui/backends -I dependencias/glfw-3.4/include

# Flags de biblioteca
LIBS = -L dependencias/glfw-3.4/build/src -lglfw3 -lGL -lX11 -lpthread -ldl

# Arquivos fonte
SOURCES = main.cpp \
	  src/calc.cpp \
          dependencias/imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp \
          dependencias/imgui/imgui_widgets.cpp imgui/imgui_demo.cpp \
          dependencias/imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp

# Nome do executável final
OUTPUT = calculadora

# Regra principal
all:
	$(CXX) $(SOURCES) -o $(OUTPUT) $(INCLUDES) $(LIBS)
