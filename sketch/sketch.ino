#include <Wire.h>

/* I2C Address */

#define AMG_I2C_ADDRESS 0x69


/* AMG8833 Register Address */
#define AMG8833_PIXEL_DATA_REGISTER_ADDRESS_START 0x80

#define  AMG_PCTL 	0x00
#define  AMG_RST 	0x01
#define  AMG_FPSC  	0x02
#define  AMG_INTC  	0x03
#define  AMG_STAT  	0x04
#define  AMG_SCLR  	0x05
#define  AMG_AVE  	0x07
#define  AMG_INTHL  0x08
#define  AMG_INTHH  0x09
#define  AMG_INTLL  0x0A
#define  AMG_INTLH  0x0B
#define  AMG_IHYSL  0x0C
#define  AMG_IHYSH  0x0D
#define  AMG_TTHL  	0x0E
#define  AMG_TTHH  	0x0F
#define  AMG_INT_OFFSET  	0x010

/* AMG8833 Operation Modes */
#define AMG_NORMAL_MODE  		0x00
#define AMG_SLEEP_MODE  		0x01
#define AMG_STAND_BY_MODE_60  	0x20
#define AMG_STAND_BY_MODE_10  	0x21

#define AMG_FLAG_RESET			0x30
#define AMG_INITIAL_RESET		0x3F

/* Frames Per Second */
#define AMG_FPS_10 0x00 
#define AMG_FPS_1  0x01

void i2c_init()
{
    Wire.begin();   
}

void i2c_write16(uint16_t data)
{
    uint8_t msb = (data >> 8) & 0xFF;
    uint8_t lsb = data & 0xFF;
    Wire.write(msb);
    Wire.write(lsb);
}

void i2c_write8(uint8_t data)
{
    Wire.write(data);
}

uint8_t i2c_read8(uint8_t address)
{
    Wire.requestFrom(address, 1);
    return Wire.read();
}


void AMG8833_INIT_REGISTERS()
{
    AMG8833_WRITE_BYTE(AMG_I2C_ADDRESS, AMG_PCTL, AMG_NORMAL_MODE);
    AMG8833_WRITE_BYTE(AMG_I2C_ADDRESS, AMG_RST, AMG_INITIAL_RESET);

    /* Disable Interrupt */
    AMG8833_WRITE_BYTE(AMG_I2C_ADDRESS, AMG_INTC, 0x0);

    /*Frame Rate 10 frames per second*/
    AMG8833_WRITE_BYTE(AMG_I2C_ADDRESS, AMG_FPSC, AMG_FPS_10);
    
    delay(250);

}

void AMG8833_WRITE_BYTE(uint8_t i2c_slave_address, uint8_t register_address, uint8_t data) 
{
    Wire.beginTransmission(i2c_slave_address);
    i2c_write8(register_address);
    i2c_write8(data);
    Wire.endTransmission();
}


/*  arr[0] = lsb
 *  arr[1] = msb
 */
uint16_t merge_lsb_n_msb(uint8_t lsb, uint8_t msb)
{
    uint16_t value = (uint16_t) (((uint16_t) lsb) | (((uint16_t) msb) << 8));
    return value;
}

void zero_buffer_double(double *buffer, int size)
{
    int i;
    
    if(!buffer){
        return;
    }

    for(i=0; i<size; i++){
        buffer[i] = 0x0;
    }
}


void zero_buffer8(uint8_t *buffer, int size)
{
    int i;
    
    if(!buffer){
        return;
    }

    for(i=0; i<size; i++){
        buffer[i] = 0x0;
    }
}


/* Convert 12 bit integer twos compliment number stored in uin16_t  to double
 */
double convert_temp_value(uint16_t val){
  int16_t n = (val << 4);
  double  d = (n >> 4);
  return d;
}

void AMG8833_READ_PIXELS(double *pixels)
{
    if(!pixels){
        return;
    }

    zero_buffer_double(pixels, 64);

    uint8_t buffer[128];
    zero_buffer8(buffer, 128);
    int i;
    uint8_t register_address = AMG8833_PIXEL_DATA_REGISTER_ADDRESS_START;

    for(i=0; i<128; i++){
        Wire.beginTransmission((uint8_t)AMG_I2C_ADDRESS);
        register_address = register_address+1;
        i2c_write8(register_address);
        Wire.endTransmission();
        Wire.requestFrom((uint8_t)AMG_I2C_ADDRESS, 1);
        buffer[i] = Wire.read();
    }

    uint16_t v;
    double t;
    /*append msb and lsb*/
    for(i=0; i<64; i++){
        uint8_t lsb = buffer[(i*2)];
        uint8_t msb = buffer[(i*2)+1];
        v = merge_lsb_n_msb(lsb, msb);
        t = convert_temp_value(v);
#define TEMP_CONVERSION_CONSTANT 0.25        
        pixels[i] = t * TEMP_CONVERSION_CONSTANT;   
        
    }
    
}

void setup() {
    delay(2000);
    Serial.begin(9600);   
    i2c_init();
    Serial.println("setup begin"); 
    delay(100);
    AMG8833_INIT_REGISTERS();
    delay(2000);
	Serial.println("setup complete"); 
}


void loop() {
    double pixels[64];
    AMG8833_READ_PIXELS(pixels);
    Serial.print("Pixel Value : ");
    Serial.print(pixels[40]);
    Serial.println(" c");
    delay(500);
}
