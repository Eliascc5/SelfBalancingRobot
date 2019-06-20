#define I2C_READ 0x01
#define I2C_WRITE 0x00
void i2c_init(uint32_t F_SCL);
uint8_t i2c_start(uint8_t address);
uint8_t i2c_write(uint8_t data);
uint8_t i2c_read(uint8_t ack);
void i2c_stop(void);
