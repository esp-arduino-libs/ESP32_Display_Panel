idf_component_register(
    SRCS "test_app_main.c" "test_mipi_dsi_lcd.cpp"
    PRIV_REQUIRES esp_lcd driver esp_timer
    WHOLE_ARCHIVE
)
