#include "m5stack_core_s3.h"

#include "esphome/core/log.h"

namespace esphome {
using namespace aw9523;
namespace m5stack {

static const char *TAG = "core_s3";

/*
    // In CL,DA, EX CL,DA
    { board_t::board_M5StackCoreS3, GPIO_NUM_11,GPIO_NUM_12 , GPIO_NUM_1 ,GPIO_NUM_2  },
    //pB p1,p2, pC p1,p2
    { board_t::board_M5StackCoreS3, GPIO_NUM_8 ,GPIO_NUM_9 , GPIO_NUM_18,GPIO_NUM_17 },
    //pD p1,p2, pE p1,p2
    { board_t::board_M5StackCoreS3, 14,10, 18,17 },
    // clk,mosi,miso,cs
    { board_t::board_M5StackCoreS3, GPIO_NUM_36, GPIO_NUM_37, GPIO_NUM_35, GPIO_NUM_4  },



    M5.begin();

        // Power Hold pin for Capsule/Dial/DinMeter
        // I think this is cam related
        // GPIO46 : CAM_VSYNC
        m5gfx::gpio_hi(GPIO_NUM_46);
        m5gfx::pinMode(GPIO_NUM_46, m5gfx::pin_mode_t::output);

        _setup_pinmap(board);

        _setup_i2c(board);

        _begin(cfg);
            Power.begin()
            Power.setExtOutput(cfg.output_power);

            /// Slightly lengthen the acceptance time of the AXP192 power button multiclick.
            BtnPWR.setHoldThresh(BtnPWR.getHoldThresh() * 1.2);

            // setup Hardware Buttons
            m5gfx::pinMode(GPIO_NUM_41, m5gfx::pin_mode_t::input);



        // Module Display / Unit OLED / Unit LCD is determined after _begin (because it must be after external power supply)
        M5ModuleDisplay dsp(cfg.module_display);
        dsp.init()
            // Wechseln Sie die Rolle von GPIO35 während des CS-Betriebs (MISO oder D/C);

                // FSPIQ_IN_IDX==FSPI MISO / SIG_GPIO_OUT_IDX==GPIO OUT
                // *(volatile uint32_t*)GPIO_FUNC35_OUT_SEL_CFG_REG = flg ? FSPIQ_OUT_IDX : SIG_GPIO_OUT_IDX;

                // Wenn CS HIGH, GPIO-Ausgabe deaktivieren und als MISO-Eingabe fungieren lassen.
                 //Wenn CS NIEDRIG ist, GPIO-Ausgang aktivieren und als D/C fungieren.


            /// Wenn der SPI-Bus-Pullup nicht wirksam ist, aktivieren Sie den VBUS 5V-Ausgang.
            /// (Denn es gibt Kombinationen wie USB-Hostmodule, die Strom aus der Signalleitung absorbieren, wenn keine 5V ausgegeben werden)
            auto result = lgfx::gpio::command(
              (const uint8_t[]) {
              lgfx::gpio::command_mode_input_pullup, GPIO_NUM_35,
              lgfx::gpio::command_mode_input_pullup, GPIO_NUM_36,
              lgfx::gpio::command_mode_input_pullup, GPIO_NUM_37,
              lgfx::gpio::command_read             , GPIO_NUM_35,
              lgfx::gpio::command_read             , GPIO_NUM_36,
              lgfx::gpio::command_read             , GPIO_NUM_37,
              lgfx::gpio::command_end
              }
            );

            uint8_t reg0x02 = (result == 0) ? 0b00000111 : 0b00000101;
            uint8_t reg0x03 = (result == 0) ? 0b10000011 : 0b00000011;
            m5gfx::i2c::bitOn(i2c_port, aw9523_i2c_addr, 0x02, reg0x02); //port0 output ctrl
            m5gfx::i2c::bitOn(i2c_port, aw9523_i2c_addr, 0x03, reg0x03); //port1 output ctrl
            m5gfx::i2c::writeRegister8(i2c_port, aw9523_i2c_addr, 0x04, 0b00011000);  // CONFIG_P0
            m5gfx::i2c::writeRegister8(i2c_port, aw9523_i2c_addr, 0x05, 0b00001100);  // CONFIG_P1
            m5gfx::i2c::writeRegister8(i2c_port, aw9523_i2c_addr, 0x11, 0b00010000);  // GCR P0 port is Push-Pull mode.
            m5gfx::i2c::writeRegister8(i2c_port, aw9523_i2c_addr, 0x12, 0b11111111);  // LEDMODE_P0
            m5gfx::i2c::writeRegister8(i2c_port, aw9523_i2c_addr, 0x13, 0b11111111);  // LEDMODE_P1

            m5gfx::i2c::writeRegister8(i2c_port, axp_i2c_addr, 0x90, 0xBF); // LDOS ON/OFF control 0
            m5gfx::i2c::writeRegister8(i2c_port, axp_i2c_addr, 0x95, 0x28); // ALDO3 set to 3.3v // for TF card slot

        // Speaker selection is performed after the Module Display has been determined.
        _begin_spk(cfg);
            auto mic_cfg = Mic.config();
            mic_cfg.i2s_port = I2S_NUM_0;

            mic_cfg.magnification = 2;
            mic_cfg.over_sampling = 1;
            mic_cfg.pin_mck = GPIO_NUM_0;
            mic_cfg.pin_bck = GPIO_NUM_34;
            mic_cfg.pin_ws = GPIO_NUM_33;
            mic_cfg.pin_data_in = GPIO_NUM_14;
            mic_cfg.i2s_port = I2S_NUM_1;
            mic_cfg.stereo = true;

            auto spk_cfg = Speaker.config();
            spk_cfg.magnification = 16;



    auto cfg = M5.Speaker.config();
    cfg.task_priority = 15;
    M5.Speaker.config(cfg);
    M5.Speaker.begin();
        _cb_set_enabled(_cb_set_enabled_args, true)
            this->setup_aw88298

        _setup_i2s()


    M5.Speaker.setVolume(200);
    */



void M5StackCoreS3::setup(){
    i2c::I2CDevice aw9523;
    aw9523.set_i2c_bus( this->bus_);
    aw9523.set_i2c_address(0x58);

    i2c::I2CDevice axp2101;
    axp2101.set_i2c_bus( this->bus_);
    axp2101.set_i2c_address(0x34);

    static constexpr std::uint8_t reg_data_array[] =
      { 0x90, 0xBF  // LDOS ON/OFF control 0
      , 0x92, 18 -5 // ALDO1 set to 1.8v // for AW88298
      , 0x93, 33 -5 // ALDO2 set to 3.3v // for ES7210
      , 0x94, 33 -5 // ALDO3 set to 3.3v // for camera
      , 0x95, 33 -5 // ALDO3 set to 3.3v // for TF card slot
      , 0x27, 0x00 // PowerKey Hold=1sec / PowerOff=4sec
      , 0x69, 0x11 // CHGLED setting
      , 0x10, 0x30 // PMU common config
      };

    //Axp2101.writeRegister8Array(reg_data_array, sizeof(reg_data_array));
    for (size_t i = 0; i < sizeof(reg_data_array); i+=2)
    {
      axp2101.reg(reg_data_array[i]) = reg_data_array[i+1];
    }

    #define XPOWERS_AXP2101_LDO_ONOFF_CTRL0                  (0x90)
    uint8_t reg = axp2101.reg( XPOWERS_AXP2101_LDO_ONOFF_CTRL0 ).get();
    ESP_LOGCONFIG(TAG, "read XPOWERS_AXP2101_LDO_ONOFF_CTRL0 :0x%x", reg );

    reg = axp2101.reg( 0x92 ).get();
    ESP_LOGCONFIG(TAG, "read XPOWERS_AXP2101_ALDO_VOLTAGE :0x%x", reg );

    uint8_t result = 0;
    uint8_t reg0x02 = (result == 0) ? 0b00000111 : 0b00000101;
    uint8_t reg0x03 = (result == 0) ? 0b10000011 : 0b00000011;
    aw9523.reg( 0x02 ) = reg0x02; //port0 output ctrl
    aw9523.reg( 0x03 ) = reg0x03; //port1 output ctrl

    /*
    m5gfx::i2c::writeRegister8(i2c_port, aw9523_i2c_addr, 0x04, 0b00011000);  // CONFIG_P0
    m5gfx::i2c::writeRegister8(i2c_port, aw9523_i2c_addr, 0x05, 0b00001100);  // CONFIG_P1
    m5gfx::i2c::writeRegister8(i2c_port, aw9523_i2c_addr, 0x11, 0b00010000);  // GCR P0 port is Push-Pull mode.
    m5gfx::i2c::writeRegister8(i2c_port, aw9523_i2c_addr, 0x12, 0b11111111);  // LEDMODE_P0
    m5gfx::i2c::writeRegister8(i2c_port, aw9523_i2c_addr, 0x13, 0b11111111);  // LEDMODE_P1
    */
    aw9523.reg(0x04) = 0b00011000;  // CONFIG_P0
    aw9523.reg(0x05) = 0b00001100;  // CONFIG_P1
    aw9523.reg(0x11) = 0b00010000;  // GCR P0 port is Push-Pull mode.
    aw9523.reg(0x12) = 0b11111111;  // LEDMODE_P0
    aw9523.reg(0x13) = 0b11111111;  // LEDMODE_P1

    if(this->aw9523_ != nullptr){
        this->write_bus_out_en(this->bus_out_en_);
        this->write_usb_otg_en(this->usb_otg_en_);
    }
}

void M5StackCoreS3::write_usb_otg_en(bool flag){
    this->aw9523_->set_pin_value( AW9523Port::PORT_0, 5, flag);
}

void M5StackCoreS3::write_bus_out_en(bool flag){
    this->aw9523_->set_pin_value( AW9523Port::PORT_0, 1, flag);
}



}
}
