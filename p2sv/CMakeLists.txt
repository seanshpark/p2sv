set(P2SV_DEV_FILES
    dev/p2sv_sys.cpp
    dev/p2sv_adc.cpp
    dev/p2sv_i2c.cpp
    dev/p2sv_gpio.cpp
)
if (P2SV_DISPLAY_LCD1602)
    list(APPEND P2SV_DEV_FILES
        dev/p2sv_lcd1602.cpp
    )
    add_definitions(-DP2SV_DISPLAY_LCD1602)
endif()
if (P2SV_DISPLAY_OLED12864)
    list(APPEND P2SV_DEV_FILES
        dev/p2sv_gfxlib.cpp
        dev/p2sv_oled12864.cpp
    )
    add_definitions(-DP2SV_DISPLAY_OLED12864)
endif()
if (P2SV_DISPLAY_LEDDOT328)
    list(APPEND P2SV_DEV_FILES
        dev/p2sv_leddot328.cpp
    )
    add_definitions(-DP2SV_DISPLAY_LEDDOT328)
endif()

set(P2SV_SRC_FILES
    src/p2sv.cpp
    src/p2sv_fft.cpp
    src/p2sv_bars.cpp
    src/p2sv_proc.cpp
)

add_executable(p2sv ${P2SV_SRC_FILES} ${P2SV_DEV_FILES})
add_definitions(-DP2SV_VERSION_STRING="0.3.0")

pico_set_program_name(p2sv "p2sv")
pico_set_program_version(p2sv "0.3.0")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(p2sv 1)
pico_enable_stdio_usb(p2sv 0)

target_link_libraries(p2sv pico_stdlib)
target_include_directories(p2sv PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(p2sv PRIVATE ${CMAKE_CURRENT_LIST_DIR}/dev)
target_link_libraries(p2sv hardware_i2c)
target_link_libraries(p2sv hardware_dma)
target_link_libraries(p2sv hardware_adc)
target_link_libraries(p2sv hardware_pio)
target_link_libraries(p2sv hardware_timer)
target_link_libraries(p2sv hardware_clocks)
target_link_libraries(p2sv kissfft)

pico_add_extra_outputs(p2sv)

set(P2SV_COPY_ELF_OPT ${CMAKE_COMMAND} -E copy p2sv.elf ${CMAKE_CURRENT_BINARY_DIR}/..)

add_custom_command(TARGET p2sv 
                   POST_BUILD
                   COMMAND ${P2SV_COPY_ELF_OPT})
