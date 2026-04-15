#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <cstring> //.find() não estava funcionando
#include "src/calc.h"

struct Funcao {
	std::string nome;
	std::vector<double> valores;
	ImU32 corReta;
};

struct ControleTeclado {
    int movimento;
};

int main(int, char**)
{
    //Inicializa o glfw
    if(!glfwInit()){
    	return 1;
    } 

    //Configura OpenGL 3.0, versão retirada de imgui/backends/imgui_impl_opengl3.cpp (linha 66)
    const char* glsl_version = "#version 150";
    //Para inicializar a janela maximizada
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    //Cria a Janela
    GLFWwindow* janela = glfwCreateWindow(1280, 720, "Teste", NULL, NULL);
    
    if(janela== NULL){
    	return 1;
    }
    //Fala para o glfw qual janela é a atual
    glfwMakeContextCurrent(janela);
    glfwSetWindowSizeLimits(janela, 800, 600, GLFW_DONT_CARE, GLFW_DONT_CARE);


    //Inicialização do imgui
    IMGUI_CHECKVERSION();
    	//Cria o ambiente do imgui
    ImGui::CreateContext();

    //Conecta imgui com glfw e opengl
    ImGui_ImplGlfw_InitForOpenGL(janela, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    //Loop princiapl
    while(!glfwWindowShouldClose(janela)){
    	//Processa a fila de eventos
        glfwPollEvents();
        
        //Tamanho da janela do glfw
        double larguraGLFW = ImGui::GetIO().DisplaySize.x, alturaGLFW = ImGui::GetIO().DisplaySize.y;
        

        //Inicializa um novo quadro
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        //Configur a a janela do teclado
        ImGui::SetNextWindowSize(ImVec2(4*larguraGLFW/5, alturaGLFW/6));
        ImGui::SetNextWindowPos(ImVec2(larguraGLFW/5, 5*alturaGLFW/6));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
        ImGui::Begin("Teclado", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
        static char termos[256];
        int vx = 4*larguraGLFW/5, vy = 5*alturaGLFW/6;
        static const char* botoes = "6789+-(p2345*/)_.x01^<>=";
        static char valor = 0;
	static ControleTeclado ctrl = {0};
        static bool foco = false;
        static bool enter = false;
        const double botaoX = 4*larguraGLFW/100, botaoY = alturaGLFW/42; 
        ImVec2 btnSize(botaoX, botaoY);
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(187, 187, 187, 255));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(127, 127, 127, 255));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(87, 87, 87, 255));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
        for(int i = 0; i<3; i++){
        	for(int j = 0; j<8; j++){
        		ImGui::SetCursorPos(ImVec2((5.5+j)*botaoX + j*botaoX/7, (1.5+i)*botaoY + i*botaoY/3));
        		char aux[2];
        		aux[0] = botoes[j+i*8];
        		aux[1] = '\0';
        		if(ImGui::Button(aux, btnSize)){
        			if(botoes[j+i*8] == '_'){
        				termos[0] = '\0';
        			}
        			else if(botoes[j+i*8] == '<'){
        				ctrl.movimento = -1;
        			}
        			else if(botoes[j+i*8] == '>'){
        				ctrl.movimento = 1;
        			}
        			else if(botoes[j+i*8] == '='){
					enter = true;
        			}
        			else{
        				valor = botoes[j+i*8];
        			}
        			foco = true;
        		}
        	}
        }
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar();
        ImGui::End();
        ImGui::PopStyleColor();
        
        //Configura a janela das equações/pontos salvos
        ImGui::SetNextWindowSize(ImVec2(larguraGLFW/5, 5*alturaGLFW/6));
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.1f, 0.1f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
        
        static std::vector<Funcao> eq;
        ImGui::Begin("##Lista", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
        if(!eq.empty()){
        	for(int i = eq.size()-1; i>=0; i--){
        		ImGui::PushID(&eq[i]);
        		ImGui::AlignTextToFramePadding();
        		if(ImGui::Selectable(eq[i].nome.c_str(), false, 0, ImVec2(ImGui::GetContentRegionAvail().x, 0))){
        			//opções cor, funcao e deleção
        			ImGui::OpenPopup("edicao");
        		}
        		
        		if(ImGui::BeginPopup("edicao")){
        			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
        			ImGui::SeparatorText("Edição");
        			if(ImGui::Selectable("Colorir", false, ImGuiSelectableFlags_DontClosePopups)){
        				ImGui::OpenPopup("edicaoCor");
        			}
        			if(ImGui::Selectable("Editar", false, ImGuiSelectableFlags_DontClosePopups)){
        				ImGui::OpenPopup("edicaoValor");
        			}
        			if(ImGui::Selectable("Deletar")){
        				eq.erase(eq.begin() + i);
        				ImGui::PopStyleColor(2);
        				ImGui::EndPopup();
        				ImGui::PopID();
        				break;
        			}
        			if(ImGui::BeginPopup("edicaoCor")){
        				ImGui::SeparatorText("Escolha a cor:");
        				ImVec4 corAux = ImGui::ColorConvertU32ToFloat4(eq[i].corReta);
        				ImGui::ColorPicker4("##picker", &corAux.x, ImGuiColorEditFlags_None);
        				eq[i].corReta = ImGui::ColorConvertFloat4ToU32(corAux);
        				if(ImGui::Button("Pronto", ImVec2(-1,0))){
        					ImGui::CloseCurrentPopup();
        				}
        				ImGui::EndPopup();
        			}
        			
        			if(ImGui::BeginPopup("edicaoValor")){
        				ImGui::SeparatorText("Digite a alteração na função:");
        				ImGui::InputText("##ValorEquação" , termos, IM_ARRAYSIZE(termos), ImGuiInputTextFlags_CallbackCharFilter | ImGuiInputTextFlags_CallbackAlways,
        [] (ImGuiInputTextCallbackData* data) -> int{

        	if(data->EventFlag == ImGuiInputTextFlags_CallbackCharFilter){
        		const char* permitidos = "0123456789x-+/*^().";
        	
        		if(data->EventChar < 256 && strchr(permitidos, (char)data->EventChar)){
        			return 0;
        		}
        		data->EventChar = 0;
        		return 1;
        	}
        	
        	if(data->EventFlag == ImGuiInputTextFlags_CallbackAlways){
        		ControleTeclado* c = (ControleTeclado*)data->UserData;
        		if(c->movimento != 0){
        			data->CursorPos += c->movimento;
        			if(data->CursorPos < 0){
        				data->CursorPos = 0;
        			}
        			if(data->CursorPos > data->BufTextLen){
        				data->CursorPos = data->BufTextLen;
        			}
        			c->movimento = 0;
        			data->BufDirty = true;
        		}
        	}
        	return 0;
        }, &ctrl);
        				eq[i].nome = termos;
        				if(ImGui::Button("Pronto##2", ImVec2(-1,0))){
        					eq[i].valores = calcular(eq[i].nome, vx);
        					ImGui::CloseCurrentPopup();
        				}
        				ImGui::EndPopup();
        			}
        			
        			ImGui::PopStyleColor(2);
        			ImGui::EndPopup();
        		}
        		/*ImGui::SameLine();
        		if(ImGui::Button("x###delete", ImVec2(20, 20))){
        			eq.erase(eq.begin() + i);
        			ImGui::PopID();
        			break;
        		}*/
        		
        		ImGui::PopID();
        	}
       	
        }
        ImGui::End();
        ImGui::PopStyleVar(1);
        ImGui::PopStyleColor(8);
        
        //Configura a janela do texto da equação atual
        ImGui::SetNextWindowSize(ImVec2(larguraGLFW/5, alturaGLFW/6));
        ImGui::SetNextWindowPos(ImVec2(0, 5*alturaGLFW/6));
        ImGui::SetNextItemWidth(larguraGLFW/6);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
        ImGui::Begin("Equação", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
        static bool reescrita;
        static std::string msg = "";
        static ImVec4 cor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        reescrita = false;
        ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(127, 127, 127, 255));
        ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.9f, 0.9f, 0.9f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
        if(foco){
        	ImGui::SetKeyboardFocusHere(0);
        	foco = false;
        }
	ImGui::InputText("##ValorEquação" , termos, IM_ARRAYSIZE(termos), ImGuiInputTextFlags_CallbackCharFilter | ImGuiInputTextFlags_CallbackAlways,
        [] (ImGuiInputTextCallbackData* data) -> int{

        	if(data->EventFlag == ImGuiInputTextFlags_CallbackCharFilter){
        		const char* permitidos = "0123456789x-+/*^().";
        	
        		if(data->EventChar < 256 && strchr(permitidos, (char)data->EventChar)){
        			return 0;
        		}
        		data->EventChar = 0;
        		return 1;
        	}
        	
        	if(data->EventFlag == ImGuiInputTextFlags_CallbackAlways){
        		ControleTeclado* c = (ControleTeclado*)data->UserData;
        		if(c->movimento != 0){
        			data->CursorPos += c->movimento;
        			if(data->CursorPos < 0){
        				data->CursorPos = 0;
        			}
        			if(data->CursorPos > data->BufTextLen){
        				data->CursorPos = data->BufTextLen;
        			}
        			c->movimento = 0;
        			data->BufDirty = true;
        		}
        	}
        	return 0;
        }, &ctrl);
        
        if(valor != 0){
        	if(ImGui::IsItemActive()){
        		if(valor != 'p'){
        			ImGui::GetIO().AddInputCharacter(valor);
        		}
        		else{
        			ImGui::GetIO().AddInputCharactersUTF8("3.1415");
        		}
        		valor = 0;
        	}
        	else{
        		foco = true;
        	}
        }
        if(ImGui::IsItemDeactivatedAfterEdit()){
        	reescrita = true;
        }
        
       	std::string equacao = std::string(termos);
       	ImGui::SameLine();
       	ImGui::ColorEdit4("Cor##", (float*)&cor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
       	if(msg != ""){
        	ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), msg.c_str());
        }
       	ImGui::PopStyleColor(3);
       	ImGui::PopStyleVar();
       	ImGui::End();
       	ImGui::PopStyleColor();
        
        //COnfigura a janela do grafico
        ImGui::SetNextWindowSize(ImVec2(4*larguraGLFW/5, 5*alturaGLFW/6));
        ImGui::SetNextWindowPos(ImVec2(larguraGLFW/5, 0));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::Begin("Grafico", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
        ImDrawList* lista = ImGui::GetWindowDrawList();
        ImVec2 p = ImGui::GetCursorScreenPos();
        std::string entrada1 = equacao;
        static std::string entrada2;
        static std::vector<double> coordenadas;
        if(entrada1 != entrada2 and reescrita == true){
        	try{
        		if(ImGui::IsKeyPressed(ImGuiKey_Enter) or enter == true){ //teste
        			enter = false;
				coordenadas = calcular(entrada1, vx);
				msg = "";
				eq.push_back({entrada1, coordenadas, ImGui::GetColorU32(cor)}); //teste
			} //teste
        	}
        	catch(const std::exception& e){
        		msg = e.what();
        	}
        	if(msg.empty()){
        		entrada2 = entrada1;
        	}
        	//Retirar
        	else{
        		std::cout<<msg<<std::endl;
        		try{
        			coordenadas = calcular(entrada2, vx);
        		}
        		catch(std::exception& e){}
        	}
        	
        }
        enter = false; //teste
        if(!eq.empty()){ //teste
        for(int j = 0; j<eq.size(); j++){ //teste
        for(int i = 2; i<coordenadas.size(); i+=2){
        	//lista->AddLine(ImVec2(p.x + coordenadas[i-2] + vx/2, p.y - coordenadas[i-1] + vy/2), ImVec2(p.x + coordenadas[i] + vx/2, p.y - coordenadas[i+1] + vy/2), ImGui::GetColorU32(cor), 3.0f);
        	lista->AddLine(ImVec2(p.x + eq[j].valores[i-2] + vx/2, p.y - eq[j].valores[i-1] + vy/2), ImVec2(p.x + eq[j].valores[i] + vx/2, p.y - eq[j].valores[i+1] + vy/2), eq[j].corReta, 3.0f);

        }
        }} //teste
        ImGui::End();
        ImGui::PopStyleColor();
        
        bool show_demo = true;
        ImGui::ShowDemoWindow(&show_demo);
        

        // Renderização
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(janela, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        
        // Limpa a tela
        glClearColor(0.00f, 0.00f, 0.00f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Desenha os dados do ImGui
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(janela);
    }

    //Limpeza
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(janela);
    glfwTerminate();

    return 0;
}
