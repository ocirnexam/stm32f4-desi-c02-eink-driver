#include "goodisplay_eink_driver.hpp"
#include "systick/systick.hpp"

#define EPD_WIDTH   176U
#define EPD_HEIGHT  264U
#define EPD_ARRAY_SIZE  (EPD_WIDTH * EPD_HEIGHT)

GOODISPLAY_EINK_Driver::GOODISPLAY_EINK_Driver(): spiInstance(SPI1), 
                                                  cs(IOPin(GPIOB, GPIO6)), 
                                                  mosi(IOPin(GPIOA, GPIO6)),
                                                  miso(IOPin(GPIOA, GPIO7)), 
                                                  clock(IOPin(GPIOA, GPIO5)),
                                                  busy_pin(IOPin(GPIOA, GPIO0)),
                                                  dc_pin(IOPin(GPIOA, GPIO1)),
                                                  reset_pin(IOPin(GPIOA, GPIO4)) { 
}

void GOODISPLAY_EINK_Driver::InitSPI() {
	rcc_periph_clock_enable(RCC_SPI1);
    rcc_periph_reset_pulse(RST_SPI1);
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);

    // setup io pins for spi
    gpio_mode_setup(mosi.port, 
                  GPIO_MODE_AF,
                  GPIO_PUPD_PULLDOWN, 
                  mosi.pin
    );
	gpio_set_af(mosi.port, GPIO_AF5, mosi.pin);
    gpio_mode_setup(miso.port, 
                  GPIO_MODE_AF,
                  GPIO_PUPD_PULLDOWN, 
                  miso.pin
    );
	gpio_set_af(miso.port, GPIO_AF5, miso.pin);
    gpio_mode_setup(clock.port, 
                  GPIO_MODE_AF,
                  GPIO_PUPD_PULLDOWN, 
                  clock.pin
    );
	gpio_set_af(clock.port, GPIO_AF5, clock.pin);
	gpio_mode_setup(cs.port, 
                  GPIO_MODE_OUTPUT, 
                  GPIO_PUPD_NONE, 
                  cs.pin
    );
	gpio_set(cs.port, cs.pin);

    // setup spi
    spi_init_master(spiInstance, 
                  SPI_CR1_BAUDRATE_FPCLK_DIV_4, 
                  SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
                  SPI_CR1_CPHA_CLK_TRANSITION_1, 
                  SPI_CR1_DFF_8BIT, 
                  SPI_CR1_LSBFIRST);
    spi_enable(spiInstance);
}

void GOODISPLAY_EINK_Driver::InitGPIOS() {

    // setup io pins for busy, reset and dc bit
    gpio_mode_setup(busy_pin.port,
                    GPIO_MODE_INPUT,
                    GPIO_PUPD_PULLDOWN,
                    busy_pin.pin
    );
    gpio_mode_setup(reset_pin.port,
                    GPIO_MODE_OUTPUT,
                    GPIO_PUPD_NONE,
                    reset_pin.pin
    );
    gpio_mode_setup(dc_pin.port,
                    GPIO_MODE_OUTPUT,
                    GPIO_PUPD_NONE,
                    dc_pin.pin
    );
}

bool GOODISPLAY_EINK_Driver::IsBusy() {
    return (bool) gpio_get(busy_pin.port, busy_pin.pin);
}

void GOODISPLAY_EINK_Driver::HW_InitDisplay() {
    gpio_clear(reset_pin.port, reset_pin.pin);  // Module reset   
    delay(10);//At least 10ms delay 
    gpio_set(reset_pin.port, reset_pin.pin);  // Module reset   
    delay(10); //At least 10ms delay 
    
    while(IsBusy());   
    SendCMD(0x12);  //SWRESET
    while(IsBusy());
}

void GOODISPLAY_EINK_Driver::Init() {
    InitSPI();
    InitGPIOS();
    HW_InitDisplay();
}

void GOODISPLAY_EINK_Driver::SendCMD(uint8_t command) {
    gpio_clear(cs.port, cs.pin); /* CS* select */
    gpio_clear(dc_pin.port, dc_pin.pin); /* dc = 0 is command */
    spi_send(spiInstance, command);
    (void) spi_read(spiInstance); // wait for transaction finished!
    gpio_set(cs.port, cs.pin); /* CS* deselect */
}

void GOODISPLAY_EINK_Driver::SendSingleDataByte(uint8_t data) {
    gpio_clear(cs.port, cs.pin); /* CS* select */
    gpio_set(dc_pin.port, dc_pin.pin); /* dc = 1 is data */
    spi_send(spiInstance, data);
    (void) spi_read(spiInstance); // wait for transaction finished!
    gpio_set(cs.port, cs.pin); /* CS* deselect */
}

void GOODISPLAY_EINK_Driver::SendData(uint8_t data[EPD_ARRAY_SIZE]) {
    for (volatile uint32_t i = 0; i < EPD_ARRAY_SIZE; i++) {
        SendSingleDataByte(data[i]);
    }
}

void GOODISPLAY_EINK_Driver::Update() {
    SendCMD(0x22); //Display Update Control
    SendSingleDataByte(0xF7);   
    SendCMD(0x20); //Activate Display Update Sequence
    while(IsBusy()); 
}

void GOODISPLAY_EINK_Driver::ClearScreenWhite()
{
    volatile unsigned int i;
    SendCMD(0x24U);
    for(i = 0; i < EPD_ARRAY_SIZE; i++)
    {
        SendSingleDataByte(0xffU);
    }
    Update();
    DeepSleep();
}

void GOODISPLAY_EINK_Driver::DeepSleep() {
    SendCMD(0x10); //Enter deep sleep
    SendSingleDataByte(0x01); 
}