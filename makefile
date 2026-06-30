INCLUDES = -I imgui -I imgui/backends -I dependencias/glfw-3.4/include

SOURCES = main.cpp \
          src/calc.cpp \
          imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp \
          imgui/imgui_widgets.cpp imgui/imgui_demo.cpp \
          imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp

#Linux
CXX_LINUX = g++
OUTPUT_LINUX = calculadora
LIBS_LINUX = -L dependencias/glfw-3.4/build/src -lglfw3 -lGL -lX11 -lpthread -ldl

#Windows
CXX_WIN = x86_64-w64-mingw32-g++
OUTPUT_WIN = calculadora.exe
LIBS_WIN = -L dependencias/glfw-3.4.bin.WIN64/lib-mingw-w64 -lglfw3 -lopengl32 -lgdi32 -static-libgcc -static-libstdc++ -static


.PHONY: all linux windows clean

all: linux

linux:
	$(CXX_LINUX) $(SOURCES) -o $(OUTPUT_LINUX) $(INCLUDES) $(LIBS_LINUX)

windows:
	$(CXX_WIN) $(SOURCES) -o $(OUTPUT_WIN) $(INCLUDES) $(LIBS_WIN)
	
clean:
	rm -f $(OUTPUT_LINUX) $(OUTPUT_WIN)
