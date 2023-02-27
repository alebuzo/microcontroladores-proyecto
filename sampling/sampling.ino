#include "Mic.h"

// Se definen los parametros para el muestreo
#define SAMPLES 64
#define GAIN (1.0f/50)
#define SOUND_THRESHOLD 500


float features[SAMPLES];
Mic mic;


void setup() {
    Serial.begin(115200);
    PDM.onReceive(onAudio);
    mic.begin();
    delay(3000);
}


void loop() {
    // await for a word to be pronounced
    if (recordAudioSample()) {
        // print features to serial monitor
        for (int i = 0; i < SAMPLES; i++) {
            Serial.print(features[i], 6);
            Serial.print(i == SAMPLES - 1 ? '\n' : ',');
        }

        delay(1000);
    }

    delay(20);
}


/**
 * PDM callback para actualizar el objeto
 */
void onAudio() {
    mic.update();
}


/**
 * se realiza el sampling con el microfono
 */
bool recordAudioSample() {
    if (mic.hasData() && mic.pop() > SOUND_THRESHOLD) {

        for (int i = 0; i < SAMPLES; i++) {
            while (!mic.hasData())
                delay(1);

            features[i] = mic.pop() * GAIN;
        }

        return true;
    }

    return false;
}
