#include <iostream>
#include <alpr.h>
#include <ctime>
#include <curl/curl.h>

// Função para enviar mensagem via Telegram
void enviarMensagemTelegram(const std::string& token, const std::string& chatId, const std::string& mensagem) {
    CURL* curl = curl_easy_init();
    
    if (curl) {
        std::string apiUrl = "https://api.telegram.org/bot" + token + "/sendMessage";
        std::string postData = "chat_id=" + chatId + "&text=" + mensagem;

        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());

        // Execute a requisição
        CURLcode res = curl_easy_perform(curl);

        // Verifique se houve algum erro na requisição
        if (res != CURLE_OK) {
            std::cerr << "Erro ao enviar mensagem via Telegram: " << curl_easy_strerror(res) << std::endl;
        }

        // Libere os recursos
        curl_easy_cleanup(curl);
    }
}

int main() {
    // Substitua 'SEU_TOKEN' pelo token do seu bot no Telegram
    std::string TELEGRAM_TOKEN = "6882591641:AAEDYynluu6ORn95xyjdL_6OoK1lPqxCYEM";
    // Substitua 'SEU_CHAT_ID' pelo ID do chat para o qual você deseja enviar as mensagens
    std::string TELEGRAM_CHAT_ID = "991789281";

    // Inicialize o objeto ALPR
    alpr::Alpr openalpr("us", "/etc/openalpr/openalpr.conf");

    if (!openalpr.isLoaded()) {
        std::cerr << "Erro ao inicializar o ALPR" << std::endl;
        return 1;
    }

    // Carregue a imagem
    std::string imagemPath = "placa01.jpg";
    alpr::AlprResults resultados = openalpr.recognize(imagemPath);

    if (resultados.plates.size() > 0) {
        // Obtenha a placa e a data/hora atual
        std::string placa = resultados.plates[0].topNPlates[0].characters;
        std::time_t agora = std::time(0);
        std::tm* localTime = std::localtime(&agora);
        char dataHora[100];
        std::strftime(dataHora, sizeof(dataHora), "%Y-%m-%d %H:%M:%S", localTime);

        // Formate a mensagem
        std::string mensagem = "Placa: " + placa + "\nData e Hora: " + dataHora;

        // Envie a mensagem via Telegram
        //enviarMensagemTelegram(TELEGRAM_TOKEN, TELEGRAM_CHAT_ID, mensagem);
    try {
            enviarMensagemTelegram(TELEGRAM_TOKEN, TELEGRAM_CHAT_ID, mensagem);
        } catch (const std::exception& e) {
            std::cerr << "Erro ao enviar mensagem pelo Telegram: " << e.what() << std::endl;
        }
    } else {
	std::string mensagem = "Nenhuma placa.";
    }

    return 0;
}
