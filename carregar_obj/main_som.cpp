#include <iostream>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib") // Linka a biblioteca winmm.lib

int main() {
    // Caminho para o arquivo WAV
    const char* soundFilePath = "C:\\ufpi\\9 periodo\\COMPUTACAO_GRAFICA\\atividades\\opengl_glut\\carregar_obj\\som_bomba.wav"; // Substitua pelo caminho do seu arquivo WAV

    // Reproduzir o som
    if (PlaySound(TEXT(soundFilePath), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP)) {
        std::cout << "Som reproduzido com sucesso!" << std::endl;
    }
    else {
        std::cerr << "Falha ao reproduzir o som." << std::endl;
    }

    // Manter o console aberto para ouvir o som (opcional)
    std::cin.get();

    return 0;
}
