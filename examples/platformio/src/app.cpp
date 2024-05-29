#include <Arduino.h>
#include <ESP_Panel_Library.h>
#include <lvgl.h>
#include <demos/lv_demos.h>
#include "lvgl_port_v8.h"
void setup()
{
    Serial.begin(115200);

    Serial.println("Initialize panel device");
    ESP_Panel *panel = new ESP_Panel();
    panel->init();
#if LVGL_PORT_AVOID_TEAR
    // When avoid tearing function is enabled, configure the RGB bus according to the LVGL configuration
    ESP_PanelBus_RGB *rgb_bus = static_cast<ESP_PanelBus_RGB *>(panel->getLcd()->getBus());
    rgb_bus->configRgbFrameBufferNumber(LVGL_PORT_DISP_BUFFER_NUM);
    rgb_bus->configRgbBounceBufferSize(LVGL_PORT_RGB_BOUNCE_BUFFER_SIZE);
#endif

    panel->begin();

    Serial.println("Initialize LVGL");
    lvgl_port_init(panel->getLcd(), panel->getTouch());

    Serial.println("Create UI");
    /* Lock the mutex due to the LVGL APIs are not thread-safe */
    lvgl_port_lock(-1);

    lv_demo_music();
    /* Release the mutex */
    lvgl_port_unlock();
}

void loop()
{

    delay(0x10);
}
