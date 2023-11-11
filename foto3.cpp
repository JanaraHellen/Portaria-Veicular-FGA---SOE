#include <opencv2/opencv.hpp>

int main() {
    // Inicializar a webcam
    cv::VideoCapture cap(0); // 0 representa a câmera padrão, pode ser necessário ajustar dependendo do sistema

    // Verificar se a webcam foi aberta com sucesso
    if (!cap.isOpened()) {
        std::cerr << "Erro ao abrir a webcam." << std::endl;
        return -1;
    }

    // Definir a resolução desejada
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);  // Largura desejada
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);  // Altura desejada

    // Verificar se a configuração da resolução foi bem-sucedida
    double width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    double height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    std::cout << "Resolucao: " << width << "x" << height << std::endl;

    // Capturar uma imagem da webcam
    cv::Mat frame;
    cap >> frame;

    // Verificar se a captura foi bem-sucedida
    if (frame.empty()) {
        std::cerr << "Erro ao capturar imagem da webcam." << std::endl;
        return -1;
    }

    // Salvar a imagem capturada
    cv::imwrite("captured_image.jpg", frame);

    // Liberar recursos
    cap.release();

    return 0;
}
