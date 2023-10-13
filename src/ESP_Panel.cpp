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

#ifndef ESP_PANEL_CONF_IGNORE
static const char *TAG = "ESP_Panel";

#define _CREATE_BUS_INIT_HOST(name, host_config, io_config, host_id)  ESP_PanelBus_##name(host_config, io_config, host_id)
#define CREATE_BUS_INIT_HOST(name, host_config, io_config, host_id)   _CREATE_BUS_INIT_HOST(name, host_config, io_config, host_id)
#define _CREATE_BUS_SKIP_HOST(name, io_config, host_id)   ESP_PanelBus_##name(io_config, host_id)
#define CREATE_BUS_SKIP_HOST(name, io_config, host_id)    _CREATE_BUS_SKIP_HOST(name, io_config, host_id)

#define _CREATE_LCD(name, bus, cfg)                    ESP_PanelLcd_##name(bus, cfg)
#define CREATE_LCD(name, bus, cfg)                     _CREATE_LCD(name, bus, cfg)

#define _CREATE_LCD_TOUCH(name, bus, cfg)              ESP_PanelLcdTouch_##name(bus, cfg)
#define CREATE_LCD_TOUCH(name, bus, cfg)               _CREATE_LCD_TOUCH(name, bus, cfg)

#define _LCD_PANEL_IO_3WIRE_SPI_CONFIG(name, line_config, scl_active_edge) \
                                                       name##_PANEL_IO_3WIRE_SPI_CONFIG(line_config, scl_active_edge)
#define LCD_PANEL_IO_3WIRE_SPI_CONFIG(name, line_config, scl_active_edge)  \
                                                       _LCD_PANEL_IO_3WIRE_SPI_CONFIG(name, line_config, scl_active_edge)

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
    CHECK_NULL_RETURN(expander);
    this->expander = expander;
}

void ESP_Panel::init(void)
{
    /* LCD-related configuration */
#if ESP_PANEL_USE_LCD
#if ESP_PANEL_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C

#error "The function is not implemented and will be implemented in the future."

#elif ESP_PANEL_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI
    // SPI bus
#if !ESP_PANEL_LCD_BUS_SKIP_INIT_HOST
    spi_bus_config_t lcd_bus_host_config = {
        .mosi_io_num = ESP_PANEL_LCD_SPI_IO_MOSI,
        .miso_io_num = ESP_PANEL_LCD_SPI_IO_MISO,
        .sclk_io_num = ESP_PANEL_LCD_SPI_IO_SCK,
        .quadwp_io_num = GPIO_NUM_NC,
        .quadhd_io_num = GPIO_NUM_NC,
        .data4_io_num = GPIO_NUM_NC,
        .data5_io_num = GPIO_NUM_NC,
        .data6_io_num = GPIO_NUM_NC,
        .data7_io_num = GPIO_NUM_NC,
        .max_transfer_sz = SPI_MAX_TRANSFER_SIZE,
        .flags = SPICOMMON_BUSFLAG_MASTER,
        .intr_flags = 0,
    };
#endif /* ESP_PANEL_LCD_BUS_SKIP_INIT_HOST */

    // SPI panel IO
    esp_lcd_panel_io_spi_config_t lcd_panel_io_config = {
        .cs_gpio_num = ESP_PANEL_LCD_SPI_IO_CS,
        .dc_gpio_num = ESP_PANEL_LCD_SPI_IO_DC,
        .spi_mode = ESP_PANEL_LCD_SPI_MODE,
        .pclk_hz = ESP_PANEL_LCD_SPI_CLK_HZ,
        .trans_queue_depth = ESP_PANEL_LCD_SPI_TRANS_QUEUE_SZ,
        .on_color_trans_done = NULL,
        .user_ctx = NULL,
        .lcd_cmd_bits = ESP_PANEL_LCD_SPI_CMD_BITS,
        .lcd_param_bits = ESP_PANEL_LCD_SPI_PARAM_BITS,
        .flags = {
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
            .dc_as_cmd_phase = 0,
            .dc_low_on_data = 0,
            .octal_mode = 0,
            .lsb_first = 0,
#else
            .dc_low_on_data = 0,
            .octal_mode = 0,
            .quad_mode = 0,
            .sio_mode = 0,
            .lsb_first = 0,
            .cs_high_active = 0,
#endif
        },
    };
#elif ESP_PANEL_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_I80

#error "This function is not implemented and will be implemented in the future."

#elif ESP_PANEL_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB
    // 3-wire SPI panel IO
#if !ESP_PANEL_LCD_BUS_SKIP_INIT_HOST
    spi_line_config_t line_config = {
        .cs_io_type = (panel_io_type_t)ESP_PANEL_LCD_3WIRE_SPI_CS_USE_EXPNADER,
        .cs_gpio_num = BIT64(ESP_PANEL_LCD_3WIRE_SPI_IO_CS),
        .scl_io_type = (panel_io_type_t)ESP_PANEL_LCD_3WIRE_SPI_SCL_USE_EXPNADER,
        .scl_gpio_num = BIT64( ESP_PANEL_LCD_3WIRE_SPI_IO_SCL),
        .sda_io_type = (panel_io_type_t)ESP_PANEL_LCD_3WIRE_SPI_SDA_USE_EXPNADER,
        .sda_gpio_num = BIT64(ESP_PANEL_LCD_3WIRE_SPI_IO_SDA),
        .io_expander = expander->getHandle(),
    };
    esp_lcd_panel_io_3wire_spi_config_t lcd_bus_host_config = LCD_PANEL_IO_3WIRE_SPI_CONFIG(ESP_PANEL_LCD_NAME,
                                                                line_config, ESP_PANEL_LCD_3WIRE_SPI_SCL_ACTIVE_EDGE);
#endif /* ESP_PANEL_LCD_BUS_SKIP_INIT_HOST */

    // RGB panel
    esp_lcd_rgb_panel_config_t lcd_panel_io_config = {
        .clk_src = LCD_CLK_SRC_DEFAULT,
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
        .bits_per_pixel = ESP_PANEL_LCD_RGB_PIXEL_BITS,
        .num_fbs = ESP_PANEL_LCD_RGB_FRAME_BUF_NUM,
        .bounce_buffer_size_px = ESP_PANEL_LCD_RGB_BOUNCE_BUF_SIZE,
        .sram_trans_align = 4,
        .psram_trans_align = 64,
        .hsync_gpio_num = ESP_PANEL_LCD_RGB_IO_HSYNC,
        .vsync_gpio_num = ESP_PANEL_LCD_RGB_IO_VSYNC,
        .de_gpio_num = ESP_PANEL_LCD_RGB_IO_DE,
        .pclk_gpio_num = ESP_PANEL_LCD_RGB_IO_PCLK,
        .disp_gpio_num = ESP_PANEL_LCD_RGB_IO_DISP,
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
        .flags = {
            .fb_in_psram = 1,
        },
    };
#endif /* ESP_PANEL_LCD_BUS_TYPE */

    // Panel vendor config
    lcd_vendor_config_t lcd_vendor_config = {
        .init_cmds = ESP_PANEL_LCD_INIT_CMD,
        .init_cmds_size = ESP_PANEL_LCD_INIT_CMD_SIZE,
#if ESP_PANEL_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB && !ESP_PANEL_LCD_BUS_SKIP_INIT_HOST
        .rgb_config = &lcd_panel_io_config,
        .flags = {
            .auto_del_panel_io = ESP_PANEL_LCD_3WIRE_SPI_AUTO_DEL_PANEL_IO,
        },
#endif
    };

    // Panel device config
    esp_lcd_panel_dev_config_t lcd_config = {
            .reset_gpio_num = ESP_PANEL_LCD_IO_RST,
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
            .color_space = (esp_lcd_color_space_t)ESP_PANEL_LCD_RGB_ORDER,
#else
            .rgb_ele_order = (lcd_rgb_element_order_t)ESP_PANEL_LCD_RGB_ORDER,
            .data_endian = LCD_RGB_DATA_ENDIAN_BIG,
#endif
            .bits_per_pixel = ESP_PANEL_LCD_COLOR_BITS,
            .flags = {
                .reset_active_high = ESP_PANEL_LCD_RST_LEVEL,
            },
            .vendor_config = &lcd_vendor_config,
    };
#endif /* ESP_PANEL_USE_LCD */

    /* LCD_Touch-related configuration */
#if ESP_PANEL_USE_LCD_TOUCH
#if ESP_PANEL_LCD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C
    // I2C bus
#if !ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST
    i2c_config_t lcd_touch_host_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = ESP_PANEL_LCD_TOUCH_I2C_IO_SDA,
        .scl_io_num = ESP_PANEL_LCD_TOUCH_I2C_IO_SCL,
        .sda_pullup_en = ESP_PANEL_LCD_TOUCH_I2C_SDA_PULLUP,
        .scl_pullup_en = ESP_PANEL_LCD_TOUCH_I2C_SCL_PULLUP,
        .master = {
            .clk_speed = ESP_PANEL_LCD_TOUCH_I2C_CLK_HZ,
        },
        .clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL,
    };
#endif /* ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST */

    // I2C panel IO
    esp_lcd_panel_io_i2c_config_t lcd_touch_panel_io_config = LCD_TOUCH_PANEL_IO_I2C_CONFIG(ESP_PANEL_LCD_TOUCH_NAME);
#elif ESP_PANEL_LCD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI
    // SPI bus configuration
#if !ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST
    spi_bus_config_t lcd_touch_host_config = {
        .mosi_io_num = ESP_PANEL_LCD_TOUCH_SPI_IO_MOSI,
        .miso_io_num = ESP_PANEL_LCD_TOUCH_SPI_IO_MISO,
        .sclk_io_num = ESP_PANEL_LCD_TOUCH_SPI_IO_SCK,
        .quadwp_io_num = GPIO_NUM_NC,
        .quadhd_io_num = GPIO_NUM_NC,
        .max_transfer_sz = SPI_MAX_TRANSFER_SIZE,
    };
#endif /* ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST */

    // SPI panel IO
    esp_lcd_panel_io_spi_config_t lcd_touch_panel_io_config = LCD_TOUCH_PANEL_IO_SPI_CONFIG(ESP_PANEL_LCD_TOUCH_NAME,
                                                                                            ESP_PANEL_LCD_TOUCH_SPI_IO_CS);
#endif /* ESP_PANEL_LCD_TOUCH_BUS_TYPE */

    // Panel device
    esp_lcd_touch_config_t lcd_touch_config = {
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

    /* Backlight-related configuration */
#if ESP_PANEL_USE_BL
    ESP_PanelBacklightConfig_t bl_config = {
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
#endif /* ESP_PANEL_USE_BL */

    ESP_PanelBus *lcd_bus = NULL;
    ESP_PanelBus *lcd_touch_bus = NULL;

    // Create LCD bus
#if ESP_PANEL_USE_LCD
#if ESP_PANEL_LCD_BUS_SKIP_INIT_HOST
    lcd_bus = new CREATE_BUS_SKIP_HOST(ESP_PANEL_LCD_BUS_NAME, &lcd_panel_io_config, ESP_PANEL_LCD_BUS_HOST);
#else
    lcd_bus = new CREATE_BUS_INIT_HOST(ESP_PANEL_LCD_BUS_NAME, &lcd_bus_host_config, &lcd_panel_io_config,
                                       ESP_PANEL_LCD_BUS_HOST);
    CHECK_NULL_GOTO(lcd_bus, err);
#endif /* ESP_PANEL_LCD_BUS_SKIP_INIT_HOST */

    // Create and initialize LCD
    lcd = new CREATE_LCD(ESP_PANEL_LCD_NAME, lcd_bus, &lcd_config);
    CHECK_NULL_GOTO(lcd, err);
#endif /* ESP_PANEL_USE_LCD */

    // Create LCD Touch bus
#if ESP_PANEL_USE_LCD_TOUCH
#if ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST
    lcd_touch_bus = new CREATE_BUS_SKIP_HOST(ESP_PANEL_LCD_TOUCH_BUS_NAME, &lcd_touch_panel_io_config,
                                             ESP_PANEL_LCD_TOUCH_BUS_HOST);
#else
    lcd_touch_bus = new CREATE_BUS_INIT_HOST(ESP_PANEL_LCD_TOUCH_BUS_NAME, &lcd_touch_host_config,
                                             &lcd_touch_panel_io_config, ESP_PANEL_LCD_TOUCH_BUS_HOST);
#endif /* ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST */
    CHECK_NULL_GOTO(lcd_touch_bus, err);

    // Create LCD Touch
    lcd_touch = new CREATE_LCD_TOUCH(ESP_PANEL_LCD_TOUCH_NAME, lcd_touch_bus, &lcd_touch_config);
    CHECK_NULL_GOTO(lcd_touch, err);
#endif /* ESP_PANEL_USE_LCD_TOUCH */

    // Create backlight
#if ESP_PANEL_USE_BL
    backlight = new ESP_PanelBacklight(&bl_config);
    CHECK_NULL_GOTO(backlight, err);
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
    return;

err:
    delete lcd_bus;
    delete lcd;
    delete lcd_touch_bus;
    delete lcd_touch;
    delete backlight;
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
