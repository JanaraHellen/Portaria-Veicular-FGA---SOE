#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <opencv2/core/mat.hpp>

void processamento(){

    cv::Mat imagem_placa = cv::imread("/home/gabriel/Imagens/Imagem_temporaria.jpg");

    if (imagem_placa.empty())
        return;

    // Aumentar a imagem para um melhor ajuste no tesseract

    cv::Mat img_aumentada;
    cv::resize(imagem_placa, img_aumentada, cv::Size(), 4, 4, cv::INTER_CUBIC);

    // O 50 e o 255 são colocados na placa, é a margem do quanto de branco e cinza serão adicionados na imagem para realçar as letras 

    cv::Mat img_cinza;
    cv::cvtColor(img_aumentada, img_cinza, cv::COLOR_BGR2GRAY);

    //img_cinza = cv2.cvtColor(img_aumentada, cv2.COLOR_BGR2GRAY)

    cv::Mat img_binaria;
    cv::threshold(img_cinza, img_binaria, 65, 200, cv::THRESH_BINARY);

    // Salvar a imagem binarizada e em cinza

    cv::imwrite("/home/gabriel/Imagens/placa_binarizada.jpg", img_binaria);
    //cv::imwrite("../Imagem/placa_binarizada_png.png", img_binaria);

    cv::imshow("ler_placa", imagem_placa);
    cv::imshow("placa_cinza", img_cinza);
    cv::imshow("placa_binaria", img_binaria);
    cv::waitKey(0);
    cv::destroyAllWindows();

}

int main () {
    processamento();
    return 0;
}
