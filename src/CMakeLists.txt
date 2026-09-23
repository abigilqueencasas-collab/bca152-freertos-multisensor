idf_component_register(
    SRCS
        "main.cpp"
        "alarm.cpp"
        "alarm_logic.cpp"
        "dht22.cpp"
        "display.cpp"
        "display_mode.cpp"
        "input.cpp"
        "motion.cpp"
        "oled_ssd1306.cpp"
        "rtos_objects.cpp"
        "sensors.cpp"
        "state_logic.cpp"
        "system_state.cpp"
    INCLUDE_DIRS "../include"
    REQUIRES driver log freertos esp_timer
)