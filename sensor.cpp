#include <wiringPi.h>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#define PIR_PIN 21 // Use o pino GPIO que voc� escolheu

int main(void) {
	int resultado;

    if (wiringPiSetupGpio() == -1) {
        printf("Erro na inicializa��o do wiringPi.\n");
        return 1;
    }


    while (1) {
        if (digitalRead(PIR_PIN) == HIGH) {
            printf("Movimento detectado!\n");
            resultado = system("./programa2");

            if (resultado != 0) {
                std::cerr << "Erro ao chamar o codigo da c�mera" << std::endl;
            }
            // Fa�a o que voc� deseja quando o movimento for detectado
            // Por exemplo, acione um alarme, tire uma foto, etc.
            delay(1000); // Evite m�ltiplas detec��es em r�pida

        }
        delay(10000);


    }

    return 0;
}
