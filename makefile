# Makefile simples para Calculadora Grafica

# Compilador
CXX = g++

# Flags de inclusão (Onde estão os .h)
INCLUDES = -I imgui -I imgui/backends -I dependencias/glfw-3.4/include

# Flags de biblioteca (Onde estão os .a e quais bibliotecas do sistema usar)
LIBS = -L dependencias/glfw-3.4/build/src -lglfw3 -lGL -lX11 -lpthread -ldl

# Arquivos fonte (Seu main + arquivos do ImGui)
SOURCES = main.cpp \
	  src/calc.cpp \
          imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp \
          imgui/imgui_widgets.cpp imgui/imgui_demo.cpp \
          imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp

# Nome do executável final
OUTPUT = calculadora

# Regra principal (o que acontece quando digita 'make')
all:
	$(CXX) $(SOURCES) -o $(OUTPUT) $(INCLUDES) $(LIBS)

# Regra de limpeza (para apagar o executável)
clean:
	rm -f $(OUTPUT)
