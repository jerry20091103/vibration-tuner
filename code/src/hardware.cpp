#include "Hardware.h"
#include "controls.h"
#include "pages.h"


HardwareRotaryEncoder *enc0;
Button2 btnEnc;
Adafruit_DRV2605 haptic;
Preferences preferences;

//* Display settings
#define TFT_HOR_RES   320
#define TFT_VER_RES   240
#define TFT_ROTATION  LV_DISPLAY_ROTATION_0

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

#if LV_USE_LOG != 0
void my_print( lv_log_level_t level, const char * buf )
{
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif

/*use Arduinos millis() as tick source*/
static uint32_t my_tick(void)
{
    return millis();
}

void updateGui()
{
    lv_timer_handler();
}

void hardwareSetup() {

    // setup display
    lv_init();
    /*Set a tick source so that LVGL will know how much time elapsed. */
    lv_tick_set_cb(my_tick);
    /* register print function for debugging */
#if LV_USE_LOG != 0
    lv_log_register_print_cb( my_print );
#endif
    lv_display_t * disp;
    /*TFT_eSPI can be enabled lv_conf.h to initialize the display in a simple way*/
    disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, sizeof(draw_buf));
    lv_display_set_rotation(disp, TFT_ROTATION);

    // setup Pages
    PageManager.init();

    // setup rotary encoder
    Serial.println("Setting up hardware...");
    switches.init(ioUsingArduino(), SWITCHES_NO_POLLING, INPUT_PULLUP);
    enc0 = new HardwareRotaryEncoder(ENC_A, ENC_B, Enc0Callback, HWACCEL_SLOWER);
    switches.setEncoder(0, enc0);
    enc0->changePrecision(0, 0); // use encoder in direction mode
    
    // setup button
    btnEnc.begin(ENC_BTN);
    btnEnc.setClickHandler(BtnPressCallback);
    btnEnc.setLongClickHandler(BtnHoldCallback);
    btnEnc.setDoubleClickHandler(BtnDoubleClickCallback);

    // setup haptic driver
    if (!Wire.begin(DRV_SDA, DRV_SCL, 400000))
    {
        Serial.println("Failed to start I2C bus");
    }

    if (!haptic.begin()) {
        Serial.println("Failed to start haptic driver");
    }
    haptic.selectLibrary(6);
    haptic.setMode(DRV2605_MODE_INTTRIG);
    haptic.useLRA();
    // test first waveform
    haptic.setWaveform(0, 24);
    haptic.setWaveform(1, 0);

    // setup buzzer
    pinMode(BUZZER_PIN, OUTPUT);
}