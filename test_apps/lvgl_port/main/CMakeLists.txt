idf_component_register(
    SRCS "test_app_main.cpp" "test_lvgl_port.cpp" "lvgl_port_v8.cpp"
    WHOLE_ARCHIVE
)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-missing-field-initializers)
