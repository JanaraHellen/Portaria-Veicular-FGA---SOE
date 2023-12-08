#include <wiringPi.h>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#define PIR_PIN 21 // Use o pino GPIO que você escolheu

int main(void) {
	int resultado;

    if (wiringPiSetupGpio() == -1) {
        printf("Erro na inicialização do wiringPi.\n");
        return 1;
    }


    while (1) {
        if (digitalRead(PIR_PIN) == HIGH) {
            printf("Movimento detectado!\n");
            resultado = system("./programa2");

            if (resultado != 0) {
                std::cerr << "Erro ao chamar o codigo da câmera" << std::endl;
            }
            // Faça o que você deseja quando o movimento for detectado
            // Por exemplo, acione um alarme, tire uma foto, etc.
            delay(1000); // Evite múltiplas detecções em rápida

        }
        delay(10000);


    }

    return 0;
}
