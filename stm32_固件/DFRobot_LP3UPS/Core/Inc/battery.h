// Thanks for https://github.com/bhagwat-99/BQ34Z100-G1
#ifndef BATTERY_H
#define BATTERY_H

#include "main.h"

typedef union
{   
    int    int_num;   /**<int data*/
    char   char_num;   /**<char data*/
    unsigned long data_long;/**<unsigned long data*/
    unsigned char uchar_num ;/**<unsigned char data*/
    float float_num ;/**<unsigned char data*/
    double double_num;             /**<double data*/
    unsigned char bnum[8];/**<unsigned char data*/
}uHexDouble_t;


//device address
#define SLAVE_ADDR              0x55

//commands
#define CONTROL                 0x00
#define STATE_OF_CHARGE         0x02
#define MAX_ERROR               0x03
#define REMAINING_CAPACITY      0x04
#define FULL_CHARGE_CAPACITY    0x06
#define VOLTAGE                 0x08
#define AVERAGE_CURRENT         0x0A
#define TEMPERATURE             0x0C
#define CURRENT                 0x10

//extended data commands
#define AVAILABLE_ENERGY        0x24    //10mW/h
#define AVERAGE_POWER           0X26    //10mW
#define INTERNAL_TEMPERATURE    0x2A    //0.1k
#define PACK_CONFIGURATION      0x3A
#define DESIGN_CAPACITY         0x3C    //0X3C



//control subcommands
#define CONTROL_STATUS          0x0000 //return the status of key features
#define DEVICE_TYPE             0x0001 //return the device type of 0x100(indicate BQ34Z100-G1)
#define CHEM_ID                 0x0008 // return the chemID value
#define BOARD_OFFSET            0X0009 // force the device to measure and store board offset
#define CC_OFFSET               0x000A // force the device to measure the internal cc offset
#define CC_OFFSET_SAVE          0x000B // force the device to store the internal cc offset
#define SEALED                  0x0020 // places the device in sealed mode
#define IT_ENABLE               0x0021 // enable the impedance track algorith
#define CAL_ENABLE              0x002D // toggle calibration mode enable
#define RESET                   0x0041 // reset the fuel guage
#define ENTER_CAL               0x0081 // enter the calibration mode
#define EXIT_CAL                0x0080 // exit the calibration mode
#define OFFSET_CAL              0x0082 // report internal cc offset in calibration mode



typedef unsigned char uint8_t;
typedef unsigned short uint16_t;


int gauge_seal(void);

static int gauge_unseal(void);

static int gauge_full_access(void);

int gauge_unlock(void);

int reset_guage(void);



//autocalibration fuctions

static int board_offset(void);

static int cc_offset(void);

static int offset_calibration(void);

static int enable_calibration(void);

static int enter_calibration(void);

static int calibration_exit(void);

static int it_enable(void);

int autocalibrate(void);


static int enable_block_data_control(void);

static int read_control(uint16_t control_subcommand);

static uint8_t checksum(uint8_t * checksum_data);

static unsigned char * read_flash_block(uint8_t sub_class, uint8_t offset);

static int write_flash_block(uint8_t sub_class, uint8_t offset, uint8_t * data);

static int readVDivider(void);

static int read_pack_configuration(void);

static int read_voltsel(void);

static int read_design_capacity(void);

static int read_design_energy(void);

static int read_flash_update_ok_voltage(void);

static int read_series_cell(void);

static int read_design_energy_scale(void);

static int set_vdivider(uint16_t v_divider);

static int set_series_cell(uint16_t series_cell);

static int set_design_capacity(uint16_t design_capacity);

static int set_design_energy_scale(uint16_t design_energy_scale);

static int set_design_energy(uint16_t design_energy);

static int set_voltsel(uint16_t dummy_value);

static int set_flash_update_ok_voltage(uint16_t flash_update_ok_voltage);

int gauge_verify_and_calibrate(void);

static int verify_calibrate_func(int (*read_func)(), int (*set_func)(uint16_t),uint16_t value);

static void failed_to_calibrate(uint16_t value);

static int calibrated_data(void);

uint8_t soc(void);



int control_status(void);

int device_type(void);


//battery parameter fuctions

int internal_temperature(void);

int temperature(void);

int voltage(void);

int current(void);

int average_current(void);

//int gauge_parameters();

int write_to_file(void);


#endif
