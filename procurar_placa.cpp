#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <opencv2/core/mat.hpp>

  using namespace cv;
  using namespace std;

  void encontrar_a_placa(String imagem_carro) {

    Mat img = imread(imagem_carro);

    int nova_largura = 1000;
    int nova_altura = 1000;

    cv::resize(img, img, cv::Size(nova_largura, nova_altura));

    Mat cinza;
    cvtColor(img, cinza, COLOR_BGR2GRAY);

    Mat bin;
    threshold(cinza, bin, 90, 230, THRESH_BINARY);

    Mat desfocar;
    GaussianBlur(bin, desfocar, Size(5,5), 0);
    imshow("Imagem_desfocada", desfocar);
    imwrite("/home/gabriel/Imagens/Imagem_desfocada.jpg", img);

    vector<vector<Point>> contornos;
    vector<Vec4i> hierarquia;
    findContours(desfocar, contornos, hierarquia, RETR_TREE, CHAIN_APPROX_NONE);

    for (int i = 0; i < (int) contornos.size() ; i++) {
      double perimetro = arcLength(contornos[i], true);
      vector<Point> aprox;
      approxPolyDP(contornos[i], aprox, 0.03*perimetro, true);

      if (perimetro > 600 ) {
        Rect rect = boundingRect(contornos[i]);
        rectangle(img, rect, Scalar(0,255,0), 2);

        Mat temporario = img(rect);
        imwrite("/home/gabriel/Imagens/Imagem_temporaria.jpg", temporario);
      }
    }

    imshow("Placa_contornada", img);
    waitKey(0);
    destroyAllWindows();

  }


int main () {
    encontrar_a_placa("/home/gabriel/Imagens/placa2.jpg");
    return 0;
}
