/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <iostream>
#include <stdint.h>

#include "driver/gpio.h"
#include "esp_log.h"

#include "private/CheckResult.h"
#include "ESP_Panel_Conf_Internal.h"
#include "ESP_Panel.h"

static const char *TAG = "ESP_Panel";

#define _CREATE_BUS_INIT_HOST(name, io_cfg, host_cfg, host_id)  ESP_PanelBus_##name(io_cfg, host_cfg, host_id)
#define CREATE_BUS_INIT_HOST(name, io_cfg, host_cfg, host_id)   _CREATE_BUS_INIT_HOST(name, io_cfg, host_cfg, host_id)
#define _CREATE_BUS_SKIP_HOST(name, io_cfg, host_id)   ESP_PanelBus_##name(io_cfg, host_id)
#define CREATE_BUS_SKIP_HOST(name, io_cfg, host_id)    _CREATE_BUS_SKIP_HOST(name, io_cfg, host_id)

#define _CREATE_LCD(name, bus, cfg)                    ESP_PanelLcd_##name(bus, cfg)
#define CREATE_LCD(name, bus, cfg)                     _CREATE_LCD(name, bus, cfg)

#define _CREATE_LCD_TOUCH(name, bus, cfg)              ESP_PanelLcdTouch_##name(bus, cfg)
#define CREATE_LCD_TOUCH(name, bus, cfg)               _CREATE_LCD_TOUCH(name, bus, cfg)

#define _LCD_TOUCH_PANEL_IO_I2C_CONFIG(name)           ESP_LCD_TOUCH_IO_I2C_##name##_CONFIG()
#define LCD_TOUCH_PANEL_IO_I2C_CONFIG(name)            _LCD_TOUCH_PANEL_IO_I2C_CONFIG(name)
#define _LCD_TOUCH_PANEL_IO_SPI_CONFIG(name, cs)       ESP_LCD_TOUCH_IO_SPI_##name##_CONFIG(cs)
#define LCD_TOUCH_PANEL_IO_SPI_CONFIG(name, cs)        _LCD_TOUCH_PANEL_IO_SPI_CONFIG(name, cs)

ESP_Panel::ESP_Panel(void):
    lcd(NULL),
    lcd_touch(NULL),
    backlight(NULL),
    expander(NULL)
{
}

ESP_Panel::~ESP_Panel(void)
{
    del();
}

void ESP_Panel::addIOExpander(ESP_IOExpander *expander)
{
    this->expander = expander;
}

void ESP_Panel::init(void)
{
#if ESP_PANEL_USE_LCD
#if ESP_PANEL_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C
#elif ESP_PANEL_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI
#if !ESP_PANEL_LCD_BUS_SKIP_INIT_HOST
    spi_bus_config_t lcd_bus_host_cfg = {
        .mosi_io_num = ESP_PANEL_LCD_SPI_IO_MOSI,
        .miso_io_num = ESP_PANEL_LCD_SPI_IO_MISO,
        .sclk_io_num = ESP_PANEL_LCD_SPI_IO_SCK,
        .quadwp_io_num = GPIO_NUM_NC,
        .quadhd_io_num = GPIO_NUM_NC,
        .max_transfer_sz = SPI_MAX_TRANSFER_SIZE,
    };
#endif /* ESP_PANEL_LCD_BUS_SKIP_INIT_HOST */
    esp_lcd_panel_io_spi_config_t lcd_panel_io_cfg = {
        .cs_gpio_num = ESP_PANEL_LCD_SPI_IO_CS,
        .dc_gpio_num = ESP_PANEL_LCD_SPI_IO_DC,
        .spi_mode = ESP_PANEL_LCD_SPI_MODE,
        .pclk_hz = ESP_PANEL_LCD_SPI_CLK_HZ,
        .trans_queue_depth = ESP_PANEL_LCD_SPI_TRANS_QUEUE_SZ,
        .on_color_trans_done = NULL,
        .lcd_cmd_bits = ESP_PANEL_LCD_SPI_CMD_BITS,
        .lcd_param_bits = ESP_PANEL_LCD_SPI_PARAM_BITS,
    };
#elif ESP_PANEL_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_I80
#error "This function is not implemented and will be implemented in the future."
#elif ESP_PANEL_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB
#if !ESP_PANEL_LCD_BUS_SKIP_INIT_HOST
    esp_lcd_panel_io_3wire_spi_config_t lcd_bus_host_cfg = {
        .line_config = {
            .cs_io_type = (panel_io_type_t)ESP_PANEL_LCD_SPI_CS_USE_EXPNADER,
            .cs_gpio_num = BIT64(ESP_PANEL_LCD_SPI_IO_CS),
            .scl_io_type = (panel_io_type_t)ESP_PANEL_LCD_SPI_SCL_USE_EXPNADER,
            .scl_gpio_num = BIT64( ESP_PANEL_LCD_SPI_IO_SCL),
            .sda_io_type = (panel_io_type_t)ESP_PANEL_LCD_SPI_SDA_USE_EXPNADER,
            .sda_gpio_num = BIT64(ESP_PANEL_LCD_SPI_IO_SDA),
        },
        .expect_clk_speed = ESP_PANEL_LCD_SPI_CLK_HZ,
        .spi_mode = ESP_PANEL_LCD_SPI_MODE,
        .lcd_cmd_bytes = ESP_PANEL_LCD_SPI_CMD_BYTES,
        .lcd_param_bytes = ESP_PANEL_LCD_SPI_PARAM_BYTES,
        .flags = {
            .use_dc_bit = ESP_PANEL_LCD_SPI_USE_DC_BIT,
            .dc_zero_on_data = ESP_PANEL_LCD_SPI_DC_ZERO_ON_DATA,
            .lsb_first = ESP_PANEL_LCD_SPI_LSB_FIRST,
            .cs_high_active = ESP_PANEL_LCD_SPI_CS_HIGH_ACTIVE,
            .del_keep_cs_inactive = ESP_PANEL_LCD_SPI_DEL_KEEP_CS,
        },
    };
#endif /* ESP_PANEL_LCD_BUS_SKIP_INIT_HOST */
    esp_lcd_rgb_panel_config_t lcd_panel_io_cfg = {
        .clk_src = LCD_CLK_SRC_PLL160M,
        .timings =  {
            .pclk_hz = ESP_PANEL_LCD_RGB_CLK_HZ,
            .h_res = ESP_PANEL_LCD_H_RES,
            .v_res = ESP_PANEL_LCD_V_RES,
            .hsync_pulse_width = ESP_PANEL_LCD_RGB_HPW,
            .hsync_back_porch = ESP_PANEL_LCD_RGB_HBP,
            .hsync_front_porch = ESP_PANEL_LCD_RGB_HFP,
            .vsync_pulse_width = ESP_PANEL_LCD_RGB_VPW,
            .vsync_back_porch = ESP_PANEL_LCD_RGB_VBP,
            .vsync_front_porch = ESP_PANEL_LCD_RGB_VFP,
            .flags = {
                .pclk_active_neg = ESP_PANEL_LCD_RGB_PCLK_ACTIVE_NEG,
            },
        },
        .data_width = ESP_PANEL_LCD_RGB_DATA_WIDTH,
        .psram_trans_align = 64,
        .hsync_gpio_num = ESP_PANEL_LCD_RGB_IO_HSYNC,
        .vsync_gpio_num = ESP_PANEL_LCD_RGB_IO_VSYNC,
        .de_gpio_num = ESP_PANEL_LCD_RGB_IO_DE,
        .pclk_gpio_num = ESP_PANEL_LCD_RGB_IO_PCLK,
        .data_gpio_nums = {
            ESP_PANEL_LCD_RGB_IO_DATA0,
            ESP_PANEL_LCD_RGB_IO_DATA1,
            ESP_PANEL_LCD_RGB_IO_DATA2,
            ESP_PANEL_LCD_RGB_IO_DATA3,
            ESP_PANEL_LCD_RGB_IO_DATA4,
            ESP_PANEL_LCD_RGB_IO_DATA5,
            ESP_PANEL_LCD_RGB_IO_DATA6,
            ESP_PANEL_LCD_RGB_IO_DATA7,
            ESP_PANEL_LCD_RGB_IO_DATA8,
            ESP_PANEL_LCD_RGB_IO_DATA9,
            ESP_PANEL_LCD_RGB_IO_DATA10,
            ESP_PANEL_LCD_RGB_IO_DATA11,
            ESP_PANEL_LCD_RGB_IO_DATA12,
            ESP_PANEL_LCD_RGB_IO_DATA13,
            ESP_PANEL_LCD_RGB_IO_DATA14,
            ESP_PANEL_LCD_RGB_IO_DATA15,
        },
        .disp_gpio_num = ESP_PANEL_LCD_RGB_IO_DISP,
        .on_frame_trans_done = NULL,
        .flags = {
            .fb_in_psram = 1,
        },
    };
#endif /* ESP_PANEL_LCD_BUS_TYPE */
    ESP_PanelLcdConfig_t lcd_cfg = {
        .dev_config = {
            .reset_gpio_num = ESP_PANEL_LCD_IO_RST,
            .color_space = (esp_lcd_color_space_t)ESP_PANEL_LCD_COLOR_SPACE,
            .bits_per_pixel = ESP_PANEL_LCD_COLOR_BITS,
            .flags = {
                .reset_active_high = ESP_PANEL_LCD_RST_LEVEL,
            },
        },
    };
#endif /* ESP_PANEL_USE_LCD */

#if ESP_PANEL_USE_LCD_TOUCH
#if ESP_PANEL_LCD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C
#if !ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST
    i2c_config_t lcd_touch_host_cfg = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = ESP_PANEL_LCD_TOUCH_I2C_IO_SDA,
        .scl_io_num = ESP_PANEL_LCD_TOUCH_I2C_IO_SCL,
        .sda_pullup_en = ESP_PANEL_LCD_TOUCH_I2C_SDA_PULLUP,
        .scl_pullup_en = ESP_PANEL_LCD_TOUCH_I2C_SCL_PULLUP,
        .master = {
            .clk_speed = ESP_PANEL_LCD_TOUCH_I2C_CLK_HZ,
        },
    };
#endif /* ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST */
    esp_lcd_panel_io_i2c_config_t lcd_touch_panel_io_cfg = LCD_TOUCH_PANEL_IO_I2C_CONFIG(ESP_PANEL_LCD_TOUCH_NAME);
#elif ESP_PANEL_LCD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI
#if !ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST
    spi_bus_config_t lcd_touch_host_cfg = {
        .mosi_io_num = ESP_PANEL_LCD_TOUCH_SPI_IO_MOSI,
        .miso_io_num = ESP_PANEL_LCD_TOUCH_SPI_IO_MISO,
        .sclk_io_num = ESP_PANEL_LCD_TOUCH_SPI_IO_SCK,
        .quadwp_io_num = GPIO_NUM_NC,
        .quadhd_io_num = GPIO_NUM_NC,
        .max_transfer_sz = SPI_MAX_TRANSFER_SIZE,
    };
#endif /* ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST */
    esp_lcd_panel_io_spi_config_t lcd_touch_panel_io_cfg = LCD_TOUCH_PANEL_IO_SPI_CONFIG(ESP_PANEL_LCD_TOUCH_NAME, ESP_PANEL_LCD_TOUCH_SPI_IO_CS);
#endif /* ESP_PANEL_LCD_TOUCH_BUS_TYPE */
    esp_lcd_touch_config_t lcd_touch_cfg = {
        .x_max = ESP_PANEL_LCD_TOUCH_H_RES,
        .y_max = ESP_PANEL_LCD_TOUCH_V_RES,
        .rst_gpio_num = (gpio_num_t)ESP_PANEL_LCD_TOUCH_IO_RST,
        .int_gpio_num = (gpio_num_t)ESP_PANEL_LCD_TOUCH_IO_INT,
        .levels = {
            .reset = ESP_PANEL_LCD_TOUCH_RST_LEVEL,
            .interrupt = ESP_PANEL_LCD_TOUCH_INT_LEVEL,
        },
        .flags = {
            .swap_xy = ESP_PANEL_LCD_TOUCH_SWAP_XY,
            .mirror_x = ESP_PANEL_LCD_TOUCH_MIRROR_X,
            .mirror_y = ESP_PANEL_LCD_TOUCH_MIRROR_Y,
        },
    };
#endif /* ESP_PANEL_USE_LCD_TOUCH */

#if ESP_PANEL_USE_BL
    ESP_PanelBacklightConfig_t bl_cfg = {
        .io_num = ESP_PANEL_LCD_IO_BL,
#if ESP_PANEL_LCD_BL_USE_PWM
        .pwm = {
            .timer = (ledc_timer_t)ESP_PANEL_LCD_BL_PWM_TIMER,
            .channel = (ledc_channel_t)ESP_PANEL_LCD_BL_PWM_CHANNEL,
            .resolution = (ledc_timer_bit_t)ESP_PANEL_LCD_BL_PWM_RESOLUTION,
            .freq_hz = ESP_PANEL_LCD_BL_PWM_FREQ_HZ,
        },
#endif
        .flags = {
            .use_pwm = ESP_PANEL_LCD_BL_USE_PWM,
            .light_on_level = ESP_PANEL_LCD_BL_ON_LEVEL,
        },
    };
#endif /* ESP_PANEL_LCD_IO_BL */

    ESP_PanelBus *lcd_bus = NULL;
#if ESP_PANEL_USE_LCD
#if (ESP_PANEL_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB) && \
    (ESP_PANEL_LCD_SPI_CS_USE_EXPNADER || ESP_PANEL_LCD_SPI_SCL_USE_EXPNADER || ESP_PANEL_LCD_SPI_SDA_USE_EXPNADER)
    // For `3-wire SPI + RGB` bus, we may need IO expander to control pins
    CHECK_NULL_RETURN(expander);
    lcd_bus_host_cfg.line_config.io_expander = expander->getHandle();
#endif
#if ESP_PANEL_LCD_BUS_SKIP_INIT_HOST
    lcd_bus = new CREATE_BUS_SKIP_HOST(ESP_PANEL_LCD_BUS_NAME, &lcd_panel_io_cfg, ESP_PANEL_LCD_BUS_HOST);
#else
    lcd_bus = new CREATE_BUS_INIT_HOST(ESP_PANEL_LCD_BUS_NAME, &lcd_panel_io_cfg, &lcd_bus_host_cfg, ESP_PANEL_LCD_BUS_HOST);
#endif /* ESP_PANEL_LCD_BUS_SKIP_INIT_HOST */
    CHECK_NULL_RETURN(lcd_bus);
    lcd = new CREATE_LCD(ESP_PANEL_LCD_NAME, lcd_bus, &lcd_cfg);
    CHECK_NULL_RETURN(lcd);
#endif /* ESP_PANEL_USE_LCD */

    ESP_PanelBus *lcd_touch_bus = NULL;
#if ESP_PANEL_USE_LCD_TOUCH
#if ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST
    lcd_touch_bus = new CREATE_BUS_SKIP_HOST(ESP_PANEL_LCD_TOUCH_BUS_NAME, &lcd_touch_panel_io_cfg, ESP_PANEL_LCD_TOUCH_BUS_HOST);
#else
    lcd_touch_bus = new CREATE_BUS_INIT_HOST(ESP_PANEL_LCD_TOUCH_BUS_NAME, &lcd_touch_panel_io_cfg, &lcd_touch_host_cfg, ESP_PANEL_LCD_TOUCH_BUS_HOST);
#endif /* ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST */
    CHECK_NULL_RETURN(lcd_touch_bus);
    lcd_touch = new CREATE_LCD_TOUCH(ESP_PANEL_LCD_TOUCH_NAME, lcd_touch_bus, &lcd_touch_cfg);
    CHECK_NULL_RETURN(lcd_touch);
#endif /* ESP_PANEL_USE_LCD_TOUCH */

#if ESP_PANEL_USE_BL
    if (!backlight) {
        backlight = new ESP_PanelBacklight(&bl_cfg);
        CHECK_NULL_RETURN(backlight);
    }
#endif /* ESP_PANEL_LCD_IO_BL */

    runExtraBoardInit();

    if (lcd_bus) {
        lcd_bus->init();
    }
    if (lcd) {
        lcd->init();
    }
    if (lcd_touch_bus) {
        lcd_touch_bus->init();
    }
    if (backlight) {
        backlight->init();
    }
}

void ESP_Panel::begin(void)
{
    if (lcd) {
        lcd->reset();
#if ESP_PANEL_LCD_BUS_TYPE != ESP_PANEL_BUS_TYPE_RGB
        lcd->swapAxes(ESP_PANEL_LCD_SWAP_XY);
        lcd->mirror(ESP_PANEL_LCD_MIRROR_X, ESP_PANEL_LCD_MIRROR_Y);
#endif
        lcd->invertColor(ESP_PANEL_LCD_INEVRT_COLOR);
        lcd->begin();
#if (ESP_PANEL_LCD_BUS_TYPE != ESP_PANEL_BUS_TYPE_RGB) || (ESP_PANEL_LCD_RGB_IO_DISP != -1)
        lcd->displayOn();
#endif
    }
    if (lcd_touch) {
        lcd_touch->begin();
    }
    if (backlight) {
        backlight->on();
    }
}

void ESP_Panel::del(void)
{
    if (lcd) {
        delete lcd->getBus();
        delete lcd;
        lcd = NULL;
    }
    if (lcd_touch) {
        delete lcd_touch->getBus();
        delete lcd_touch;
        lcd_touch = NULL;
    }
    if (backlight) {
        delete backlight;
        backlight = NULL;
    }
}

ESP_PanelLcd *ESP_Panel::getLcd(void)
{
    CHECK_NULL_GOTO(lcd, err);
    return lcd;

err:
    return NULL;
}

ESP_PanelLcdTouch *ESP_Panel::getLcdTouch(void)
{
    CHECK_NULL_GOTO(lcd_touch, err);
    return lcd_touch;

err:
    return NULL;
}

ESP_PanelBacklight *ESP_Panel::getBacklight(void)
{
    CHECK_NULL_GOTO(backlight, err);
    return backlight;

err:
    return NULL;
}

void ESP_Panel::runExtraBoardInit(void)
{
    /* For ESP32-S3-LCD-EV-Board */
#ifdef ESP_PANEL_BOARD_ESP32_S3_LCD_EV_BOARD
    /* For the newest version sub board, need to set `ESP_PANEL_LCD_RGB_IO_VSYNC` to high before initialize LCD */
    gpio_set_direction((gpio_num_t)ESP_PANEL_LCD_RGB_IO_VSYNC, GPIO_MODE_OUTPUT);
    gpio_set_level((gpio_num_t)ESP_PANEL_LCD_RGB_IO_VSYNC, 1);
#endif

    /* For ESP32-S3-Korvo-2 */
#ifdef ESP_PANEL_BOARD_ESP32_S3_KORVO_2
    CHECK_NULL_RETURN(expander);
    expander->multiPinMode(IO_EXPANDER_PIN_NUM_1 | IO_EXPANDER_PIN_NUM_2 | IO_EXPANDER_PIN_NUM_3, OUTPUT);
    /* Reset LCD */
    expander->digitalWrite(2, LOW);
    vTaskDelay(pdMS_TO_TICKS(20));
    expander->digitalWrite(2, LOW);
    vTaskDelay(pdMS_TO_TICKS(120));
    expander->digitalWrite(2, HIGH);
    /* Turn on backlight */
    expander->digitalWrite(1, HIGH);
    /* Keep LCD CS low */
    expander->digitalWrite(3, LOW);
#endif

    /* For ESP32-S3-BOX-3 */
#ifdef ESP_PANEL_BOARD_ESP32_S3_BOX_3
    /* Maintain the touch INT signal in a low state during the reset process to set its I2C address to `0x5D` */
    gpio_set_direction((gpio_num_t)ESP_PANEL_LCD_TOUCH_IO_INT, GPIO_MODE_OUTPUT);
    gpio_set_level((gpio_num_t)ESP_PANEL_LCD_TOUCH_IO_INT, 0);
    usleep(100);
#endif
}
