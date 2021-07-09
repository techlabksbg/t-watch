#include "micdemo.h"

#include <driver/i2s.h>
// TWATCH 2020 V3 PDM microphone pin
#define MIC_DATA            2
#define MIC_CLOCK           0
#define MIC_BUFFER_SIZE 240

#define SCOPE_BG LV_COLOR_BLUE
#define SCOPE_FG LV_COLOR_CYAN

bool MicDemo::create() {
    self = this;
    register_for_swipe_up(myScr);
    lv_obj_set_style_local_bg_color(myScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    return true;
}

bool MicDemo::show() {
    setup();
    micTask = lv_task_create(micLoop, 5, LV_TASK_PRIO_LOWEST, NULL);
    return true;
}

bool MicDemo::hide() {
    lv_task_del(micTask);
    micTask = nullptr;

    tearDown();
    return true;
}

bool MicDemo::destroy() {
    return true;
}

// from https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library/blob/master/examples/BasicUnit/TwatcV3Special/Microphone/Microphone.ino
void MicDemo::setup() {
    Serial.println("MicDemo::setup() start");
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
        .sample_rate =  44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 2,
        .dma_buf_len = 128,
    };

    i2s_pin_config_t i2s_cfg;
    i2s_cfg.bck_io_num   = I2S_PIN_NO_CHANGE;
    i2s_cfg.ws_io_num    = MIC_CLOCK;
    i2s_cfg.data_out_num = I2S_PIN_NO_CHANGE;
    i2s_cfg.data_in_num  = MIC_DATA;

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &i2s_cfg);
    i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);

    micBuffer  = new int16_t[MIC_BUFFER_SIZE];
    clearIt = false;
    Serial.println("MicDemo::setup() end");
}

size_t MicDemo::readData() {
    //Serial.println("MicDemo::readData() start");
    if (clearIt) {
        for (int x=0; x<240; x++) {
            if (micBuffer[x]>=0 && micBuffer[x]<240) {
                ttgo->tft->drawPixel(x, micBuffer[x], 0);
            }
            //lv_canvas_set_px(canvas, x, micBuffer[x], SCOPE_BG);
        }
    }
    size_t read_len = 0;
    i2s_read(I2S_NUM_0, micBuffer, MIC_BUFFER_SIZE*2, &read_len, portMAX_DELAY);
    for (int x=0; x<240; x++) {
        //int s = micBuffer[x] < 0 ? -1 : 1;
        micBuffer[x] = (int16_t)(micBuffer[x]/10+240);
        //lv_canvas_set_px(canvas, x, micBuffer[x], SCOPE_BG);
        if (micBuffer[x]>=0 && micBuffer[x]<240) {
            ttgo->tft->drawPixel(x, micBuffer[x], 0xfff);
        }
    }
    clearIt = true;
    //Serial.println("MicDemo::readData() end");
    return read_len;
}

// from https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2s.html
void MicDemo::tearDown() {
    if (micBuffer!=nullptr) {
        delete[] micBuffer;
        micBuffer = nullptr;
    }
    i2s_driver_uninstall(I2S_NUM_0);
}

MicDemo* MicDemo::self = nullptr;