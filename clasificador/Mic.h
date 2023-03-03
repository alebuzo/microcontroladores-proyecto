/**
 * Adapted from https://github.com/DaleGia/nano-33-sense-serial-example
 */

#include <PDM.h>
#include <arm_math.h>

#define MICROPHONE_BUFFER_SIZE_IN_WORDS (256U)
#define MICROPHONE_BUFFER_SIZE_IN_BYTES (MICROPHONE_BUFFER_SIZE_IN_WORDS * sizeof(int16_t))


/**
 * Acceso al microfono del arduino nano ble 33 sense 
 */
class Mic {
public:

    /**
     * Constructor
     */
    Mic() :
            _ready(false) {

    }

    /**
     * Setup de la biblioteca PDM (Pulse Density Modulation)
     */
    bool begin(uint8_t gain = 20) {
        PDM.begin(1, 16000);
        PDM.setGain(gain);

        return true;
    }

    /**
     * Revisa si el micrófono tiene nueva información
     */
    bool hasData() {
        return _ready;
    }

    /**
     * Brinda el valor RMS 
     */
    int16_t data() {
        return _rms;
    }

    /**
     *
     * @return
     */
    int16_t pop() {
        int16_t rms = data();

        reset();

        return rms;
    }

    /**
     * se encarga de leer la informacion del audio a través del objeto PDM
     */
    void update() {
        int bytesAvailable = PDM.available();

        if(bytesAvailable == MICROPHONE_BUFFER_SIZE_IN_BYTES) {
            int16_t _buffer[MICROPHONE_BUFFER_SIZE_IN_WORDS];

            _ready = true;
            PDM.read(_buffer, bytesAvailable);
            arm_rms_q15((q15_t*)_buffer, MICROPHONE_BUFFER_SIZE_IN_WORDS, (q15_t*)&_rms);
        }
    }

    /**
     * marca la información como captada y leída
     */
    void reset() {
        _ready = false;
    }


protected:
    int16_t _rms;
    bool _ready;
};
