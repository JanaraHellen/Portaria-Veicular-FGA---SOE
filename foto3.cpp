#include <cstdlib>
#include <iostream>
#include <string> // Adicionado para incluir a biblioteca string

int main() {
    // Nome do arquivo de saída
    const char* nomeArquivo = "foto.jpg";

    // Comando para capturar imagem usando fswebcam
    const char* comando = "fswebcam --no-banner -r 640x480 --jpeg 85 -D 1 -S 2 ";

    // Concatenando o nome do arquivo ao comando
    std::string comandoCompleto = std::string(comando) + nomeArquivo;

    // Executando o comando
    int resultado = system(comandoCompleto.c_str());

    // Verificando se a captura foi bem-sucedida
    if (resultado == 0) {
        std::cout << "Foto capturada com sucesso e salva como " << nomeArquivo << std::endl;
    } else {
        std::cerr << "Erro ao capturar a foto da webcam" << std::endl;
    }

    return 0;
}

