#include <alpr.h>
#include <iostream>

int main(int argc, char *argv[]) {
    // Carregar o ambiente do OpenALPR
    alpr::Alpr openalpr("br", "/etc/openalpr/openalpr.conf");

    if (!openalpr.isLoaded()) {
        std::cerr << "Erro ao carregar o ambiente do OpenALPR" << std::endl;
        return 1;
    }

    // Configurar opções (opcional)
    openalpr.setTopN(1);  // Retornar apenas a melhor correspondência

    // Ler a imagem da placa de carro
    std::string imagePath = "/home/gabriel/Imagens/placa3.jpg";
    alpr::AlprResults results = openalpr.recognize(imagePath);

    // Verificar se a leitura foi bem-sucedida
    if (results.plates.size() > 0) {
        std::cout << "Placa detectada: " << results.plates[0].topNPlates[0].characters << std::endl;
    } else {
        std::cout << "Nenhuma placa detectada." << std::endl;
    }

    return 0;
}
