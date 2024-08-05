#pragma once
#include <iostream>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib") // Linka a biblioteca winmm.lib

struct tocar {
    tocar_som_bomba() {
        // Caminho para o arquivo WAV
        const char* soundFilePath = "C:\\ufpi\\9 periodo\\COMPUTACAO_GRAFICA\\atividades\\opengl_glut\\bomberman\\sons\\som_bomba.wav"; // Substitua pelo caminho do seu arquivo WAV

        // Reproduzir o som
        if (PlaySound(TEXT(soundFilePath), NULL, SND_FILENAME | SND_SYNC)) {
            std::cout << "Som reproduzido com sucesso!" << std::endl;
        }
        else {
            std::cerr << "Falha ao reproduzir o som." << std::endl;
        }

        // Manter o console aberto para ouvir o som (opcional)
        std::cin.get();

        return 0;
    }

    tocar_som_jogo() {
        // Caminho para o arquivo WAV
        const char* soundFilePath = "C:\\ufpi\\9 periodo\\COMPUTACAO_GRAFICA\\atividades\\opengl_glut\\bomberman\\sons\\som_jogo.wav"; // Substitua pelo caminho do seu arquivo WAV

        // Reproduzir o som
        if (PlaySound(TEXT(soundFilePath), NULL, SND_FILENAME | SND_SYNC)) {
            std::cout << "Som reproduzido com sucesso!" << std::endl;
        }
        else {
            std::cerr << "Falha ao reproduzir o som." << std::endl;
        }

        // Manter o console aberto para ouvir o som (opcional)
        std::cin.get();

        return 0;
    }
};