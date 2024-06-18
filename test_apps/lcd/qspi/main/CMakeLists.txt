idf_component_register(
    SRCS "test_app_main.c" "test_qspi_lcd.cpp"
    PRIV_REQUIRES esp_lcd driver
    WHOLE_ARCHIVE
)
