#include "hardware/spi.h"
#include "pico/stdlib.h"

#define CS_PIN 9
#define RES_PIN 22
#define CLK_PIN 26
#define MOSI_PIN 27
#define DC_PIN 28

spi_inst_t *spi = spi1;

void Write_command(uint8_t command) {
    gpio_put(CS_PIN, 0);
    spi_write_blocking(spi, &command, 2);
    gpio_put(CS_PIN, 1);
}

void Initial_SSD1305Z() {
    Write_command(0xAE);  // Display Off

    Write_command(0xAD);  // Master Configuration
    Write_command(0x8E);  // Select external VCC supply

    Write_command(0xA8);  // Select Multiplex Ratio
    Write_command(0x1F);  // Default => 0x3F (1/64 Duty) 0x1F(1/32 Duty)

    Write_command(0xD3);  // Setting Display Offset
    Write_command(0x00);  // 00H Reset

    Write_command(0x00);  // Set Column Address LSB

    Write_command(0x10);  // Set Column Address MSB

    Write_command(0x40);  // Set Display Start Line

    Write_command(0x00);  //;Set Memory Addressing Mode Default => 0x02
                          // 0x00 => Horizontal Addressing Mode
    Write_command(0xA6);  // Set Normal Display

    Write_command(0xDB);  // Set Deselect Vcomh level
    Write_command(0x3C);  //~0.83xVCC

    Write_command(0xA4);  // Entire Display ON

    Write_command(0x81);  // Set Contrast Control
    Write_command(0x6F);

    Write_command(0xD5);  // SET DISPLAY CLOCK
    Write_command(0x71);  // 105HZ

    Write_command(0xD8);  // Select Area color ON/OFF
    Write_command(0x00);  // MONO Mode

    Write_command(0xA1);  // Set Segment Re-Map Default => 0xA0

    Write_command(0xC8);  // Set COM Output Scan Direction Default => 0xC0

    Write_command(0xDA);  // Set COM Hardware Configuration
    Write_command(0x12);  // Alternative COM Pin

    Write_command(0xD9);  // Set Pre-Charge period
    Write_command(0x22);  // Refer to SPEC 34PAGE

    Write_command(0xAF);  // Display ON
}

int main() {
    stdio_init_all();

    gpio_init(CS_PIN);
    gpio_set_dir(CS_PIN, GPIO_OUT);
    gpio_put(CS_PIN, 1);  // CS Off

    gpio_init(DC_PIN);
    gpio_set_dir(DC_PIN, GPIO_OUT);
    gpio_put(DC_PIN, 0);  // Default to Command mode

    gpio_init(RES_PIN);
    gpio_set_dir(RES_PIN, GPIO_OUT);
    gpio_put(RES_PIN, 1);  // Turn on Display

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);

    // Initialize SPI port at 1 MHz
    spi_init(spi, 1000 * 1000);

    // Set SPI format
    spi_set_format(spi,         // SPI instance
                   8,           // Number of bits per transfer
                   SPI_CPOL_1,  // Polarity (CPOL)
                   SPI_CPHA_1,  // Phase (CPHA)
                   SPI_MSB_FIRST);

    // Initialize SPI pins
    gpio_set_function(CLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MOSI_PIN, GPIO_FUNC_SPI);

    // Initialize OLED
    Initial_SSD1305Z();

    // Draw rectangle
    gpio_put(DC_PIN, 1);  // Data mode

    for (int i = 0; i < 96 * 32; i++) {
        uint8_t data = 0xFF;
        spi_write_blocking(spi, &data, 1);
    }

    gpio_put(DC_PIN, 0);  // Command mode

    while (true) {
        // Blink LED
        gpio_put(PICO_DEFAULT_LED_PIN, !gpio_get(PICO_DEFAULT_LED_PIN));
        sleep_ms(1000);
    }
}