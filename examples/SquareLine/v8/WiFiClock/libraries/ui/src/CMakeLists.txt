SET(SOURCES screens/ui_ScreenClock.c
    screens/ui_ScreenSet.c
    screens/ui_ScreenWifiList.c
    screens/ui_ScreenPassword.c
    screens/ui_ScreenAla.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_settings_icon_png.c
    images/ui_img_cloudy_png.c
    images/ui_img_light_rain_png.c
    images/ui_img_sunny_png.c
    images/ui_img_snow_png.c
    images/ui_img_wifi_png.c
    images/ui_img_thunderstorm_png.c
    images/ui_img_atmosphere_png.c
    images/ui_img_drizzle_png.c
    images/ui_img_return_png.c
    fonts/ui_font_AliShuHeTi16sbpp4.c
    fonts/ui_font_FontNumber28bp4.c
    fonts/ui_font_FontNumber36bp4.c
    fonts/ui_font_FontNumber48bp4.c
    fonts/ui_font_FontPuHui20bp4.c)

add_library(ui ${SOURCES})
