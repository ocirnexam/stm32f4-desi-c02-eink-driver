#ifndef __DESI_C02_EINK_H
#define __DESI_C02_EINK_H

#include <stdint.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>

struct IOPin {
    explicit IOPin(uint32_t GPIO_port, uint32_t GPIO_pin): pin(GPIO_pin), port(GPIO_port) {}
    uint32_t pin;
    uint32_t port;
};

class GOODISPLAY_EINK_Driver {
private:
    // spi
    uint32_t spiInstance;
    IOPin cs;
    IOPin mosi;
    IOPin miso;
    IOPin clock;

    // driver pins
    IOPin busy_pin;
    IOPin dc_pin;
    IOPin reset_pin;

    void InitGPIOS(void);
    void InitSPI(void);
    void HW_InitDisplay(void); 
    void SendSingleDataByte(uint8_t data);
    void SendCMD(uint8_t command);
    void SendData(uint8_t data[]);
    void DeepSleep(void);
    void Update(void);
    bool IsBusy(void);

public:
    explicit GOODISPLAY_EINK_Driver();
    ~GOODISPLAY_EINK_Driver() {}

    void Init();

    void WriteScreen(const unsigned char *datas);
    void ClearScreenWhite(void);
    void ClearScreenBlack(void);
};

#endif
