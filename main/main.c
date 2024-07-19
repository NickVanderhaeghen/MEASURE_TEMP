//temp measuring with the AHT10


#include <stdio.h>
#include <driver/i2c_master.h>
#include <driver/i2c_slave.h>
#include <driver/i2c_types.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_err.h>
#include <esp_log.h>

#define SCL_IO      (22)
#define SDA_IO      (21)
#define SLAVE_BUF   (1024)
#define SLAVE_ADDR  (0x38)
#define FREQUENTY   (100000)


uint8_t data;


void app_main(void)
{

    i2c_master_bus_config_t master_bus_config = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = SDA_IO,
        .scl_io_num = SCL_IO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7
    };
    i2c_master_bus_handle_t master_bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&master_bus_config, &master_bus_handle));




    const i2c_device_config_t bus_device_config = {
        .dev_addr_length = I2C_ADDR_BIT_7,
        .device_address = SLAVE_ADDR,
        .scl_speed_hz = FREQUENTY,
        .scl_wait_us = 0
    };
    i2c_master_dev_handle_t master_dev_handle;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(master_bus_handle, &bus_device_config, &master_dev_handle));


    ESP_ERROR_CHECK(i2c_master_probe(master_bus_handle, SLAVE_ADDR, 10));



    while (1)
    {
        ESP_ERROR_CHECK(i2c_master_receive(master_dev_handle, &data, 2, 10));

        for (int i = 8 - 1; i >= 0; i--){
            printf("%d", (data >> i) & 1);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        printf("\n");
    }

    
    
} 