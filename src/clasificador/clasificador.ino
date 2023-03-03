//IE-0624 Laboratorio de Microcontroladores
//Alexa Carmona Buzo B91643
//Raquel Corrales Marin B92378
//Laboratorio 4
//Sismógrafo


#include "Mic.h"
#include "Classifier.h"
#include "ClassifierRandomForest.h"

// se definen los parámetros de ganacia del micrófono y threshold del sonido, así como samples del PDM
#define SAMPLES 64
#define GAIN (1.0f/50)
#define SOUND_THRESHOLD 1000

// Con ELoquentTinyML se importa el modelo, en este caso del RandomForest con el objeto clf
float features[SAMPLES];
Mic mic;
Eloquent::ML::Port::RandomForest clf;

// se inicia el recibimiento de audios desde el micrófono
void setup() {
    Serial.begin(115200);
    PDM.onReceive(onAudio);
    mic.begin();
    delay(3000);
}

// si se detecta sonido se llama a la función clf.predictLabel(features) 
// enviando los features, que están definidos más abajo como los pulsos del sonido
void loop() {
    if (recordAudioSample()) {
        Serial.print("Palabra Detectada: ");
        Serial.println(clf.predictLabel(features));

        delay(1000);
    }

    delay(20);
}


/**
 * llamado al PDM para hacer un update del microfono
 */
void onAudio() {
    mic.update();
}


/**
 * se leen los valores de los pulsos que se obtienen a través del micrófono y se 
 * amplifican por decirlo de una forma por la ganancia del microfono
 * se guardan en features[i] para luego pasar a la clasificación en 
 * clf.predictLabel(features) del loop
 */
bool recordAudioSample() {
    if (mic.hasData() && mic.data() > SOUND_THRESHOLD) {

        for (int i = 0; i < SAMPLES; i++) {
            while (!mic.hasData())
                delay(1);

            features[i] = mic.pop() * GAIN;
            
        }

        return true;
    }

    return false;
}
