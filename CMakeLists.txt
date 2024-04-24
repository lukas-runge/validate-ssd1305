cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico-sdk/pico_sdk_init.cmake)

project(my_project)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

if (TARGET tinyusb_device)

    # rest of your project
    add_executable(main
        src/main.cpp
    )

    # Add pico_stdlib library which aggregates commonly used features
    target_link_libraries(main pico_stdlib hardware_spi)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(main 1)
    pico_enable_stdio_uart(main 0)

    # create map/bin/hex/uf2 file in addition to ELF.
    pico_add_extra_outputs(main)

elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()