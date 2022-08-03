// Thanks for https://github.com/bhagwat-99/BQ34Z100-G1
#include "battery.h"
#include "i2c.h"
// #include "stdio.h"
// #include <string.h>

// const char * DATA_OUT_FORMAT = "{\n\t\"Voltage\":\"%d\", \n\t\"Internal_temperature(C)\":\"%.2f\", \n\t\"Average_current\":\"%d\"\n}\n";

// seal the gauge - don't execute the function if device already in sealed state
int gauge_seal()
{
    if(read_control(SEALED) == -1)
    {
        return -1;
    }
    return 0;
}

//unseal the gauge before read/write
static int gauge_unseal()
{
        uint8_t reg_addr    =   0x00;
        uint8_t n_bytes     =   0x02;

        uint8_t reg_data[2];
        reg_data[0]         =   0x14;   //lsb
        reg_data[1]         =   0x04;   //msb

        int ret_val = i2c_write(SLAVE_ADDR,reg_addr, reg_data,n_bytes);
        HAL_Delay(250);

        if (ret_val < 0 )
        {
            printf("gauge unseal failed\n");
            return -1;
        }

        reg_addr =0x00;
        reg_data[0]=0x72;//lsb
        reg_data[1]=0x36;//msb

        ret_val = i2c_write(SLAVE_ADDR,reg_addr, reg_data,n_bytes);
        HAL_Delay(250);
        if (ret_val < 0 )
        {
            printf("gauge unseal failed\n");
            return -1;
        }
        return 0;
}

//full access mode of gauge. gauge must be in unsealed mode to full access mode
static int gauge_full_access()
{
    uint8_t n_bytes = 2;
    for(uint8_t i=0;i<3;i++)
    {
        uint8_t reg_addr;
        uint8_t reg_data[2];

        reg_addr =0x00;
        reg_data[0]=0xFF;//lsb
        reg_data[1]=0xFF;//msb

        int ret_val = i2c_write(SLAVE_ADDR,reg_addr, reg_data,n_bytes);
        HAL_Delay(250);
        if (ret_val < 0 )
        {
            printf("gauge unseal failed\n");
            return -1;
        }


        reg_addr =0x00;
        reg_data[0]=0xFF;//lsb
        reg_data[1]=0xFF;//msb
        ret_val = i2c_write(SLAVE_ADDR,reg_addr, reg_data,n_bytes);
        HAL_Delay(250);
        if (ret_val < 0 )
        {
            printf("gauge unseal failed\n");
            return -1;
        }
    }
    return 0;

}

//unlock the gauge
int gauge_unlock()
{
    int ret_val;
    ret_val = gauge_unseal();
    if(ret_val == -1)
    {
        printf("Error gauge unlock : gauge unseal\n");
        return -1;
    }
    ret_val = gauge_full_access();
    if(ret_val == -1)
    {
        printf("Error gauge unlock : gauge full access\n");
        return -1;
    }
    return 0;
}

//reset the guage
int reset_guage()
{
    int ret_val = read_control(RESET);
    if(ret_val == -1)
    {
        printf("failed to reset gauge\n");
        return -1;
    }
    return 0;

}

//control status
int control_status()
{
    int ret_val = read_control(CONTROL_STATUS);
    if(ret_val == -1)
    {
        printf("failed control status\n");
        return -1;
    }

    return (uint16_t)ret_val;
}

//get device type 0x0100 for bq34z100-g1
int device_type()
{
    int ret_val = read_control(DEVICE_TYPE);
    if(ret_val == -1)
    {
        printf("failed device type\n");
        return -1;
    }

    return (uint16_t)ret_val;
}

///////////////////////////// data read write functions ///////////////////////////////////////////////////

//enable accessing block data
static int enable_block_data_control()
{
    uint8_t n_bytes     =   1;
    uint8_t reg_addr    =   0x61;

    uint8_t reg_data[1];
    reg_data[0]=0x00;

    int ret_val = i2c_write(SLAVE_ADDR,reg_addr, reg_data,n_bytes);
    HAL_Delay(250);
    if (ret_val < 0 )
    {
        printf("Error enable_block_data_control\n");
        return -1;
    }
    return 0;
    
}

//read control command
static int  read_control(uint16_t control_subcommand)
{
    uint8_t n_bytes = 2;
    uint8_t reg_data[2];

    reg_data[0] = control_subcommand & 0x00FF; //lsb
    reg_data[1] = control_subcommand >> 8; //msb

    int ret_val = i2c_write(SLAVE_ADDR, CONTROL, reg_data, n_bytes);
    HAL_Delay(250);
    if (ret_val < 0 )
    {
        printf("read_control failed \n");
        return -1;
    }


    uint8_t * p_ret_val = i2c_read(SLAVE_ADDR, CONTROL, n_bytes);
    HAL_Delay(250);
    if(p_ret_val)
    {
        printf("read control failed \n");
        return -1;
    }

    return ((uint16_t)(*(p_ret_val + 1)) << 8 | * p_ret_val) ;

}



static uint8_t checksum(uint8_t * checksum_data)
{
        uint8_t checksum_value = 0x00;

        for (uint8_t n = 0; n < 32; n++)
        {
            checksum_value += *(checksum_data+n);
        }

        checksum_value = 0xFF - checksum_value;

        return checksum_value;

}


//read flash block
static uint8_t * read_flash_block(uint8_t sub_class, uint8_t offset)
{
    //enable block data
    int ret_val = enable_block_data_control();
    if(ret_val == -1)
    {
            return NULL;
    }

    //write subclass id to 0x3e
    uint8_t reg_addr = 0x3e;
    uint8_t n_bytes = 1;
    uint8_t reg_data[1];
    reg_data[0] =  sub_class ;
    ret_val = i2c_write(SLAVE_ADDR, reg_addr, reg_data, n_bytes);
    HAL_Delay(50);
    if(ret_val == -1)
    {
        printf("Error read_flash_block : 3e\n");
        return NULL;
    }

    //write offset to 0x3f register
    reg_addr = 0x3f;
    n_bytes = 1;
    reg_data[0] =  offset/32 ; //msb
    ret_val = i2c_write(SLAVE_ADDR, reg_addr, reg_data, n_bytes);
    HAL_Delay(50);
    if(ret_val == -1)
    {
        printf("Error read_flash_block : 3f\n");
        return NULL;
    }

    //read data from 0x40 address
    reg_addr = 0x40;
    n_bytes = 32;
    uint8_t * p_ret_val = i2c_read(SLAVE_ADDR, reg_addr, n_bytes);
    HAL_Delay(50);
    if(p_ret_val == NULL)
    {
        printf("Error read_flash_block : 0x40\n");
        return NULL;
    }

    return p_ret_val;
}


// write 32 byte flash data block
static int write_flash_block(uint8_t sub_class, uint8_t offset, uint8_t * data)
{
    //enable block data
    int ret_val = enable_block_data_control();
    if(ret_val == -1)
    {
        return -1;
    }
   
    //write subclass id to 0x3e
    uint8_t reg_addr = 0x3e;
    uint8_t n_bytes = 1;
    uint8_t reg_data[1];
    reg_data[0] =  sub_class ;
    ret_val = i2c_write(SLAVE_ADDR, reg_addr, reg_data, n_bytes);
    HAL_Delay(500);
    if(ret_val == -1)
    {
        printf("Error write_flash_block : 3e\n");
        return -1;
    }

    //write offset to 0x3f register
    reg_addr = 0x3f;
    n_bytes = 1;
    reg_data[0] =  offset/32 ; //msb
    ret_val = i2c_write(SLAVE_ADDR, reg_addr, reg_data, n_bytes);
    HAL_Delay(500);
    if(ret_val == -1)
    {
        printf("Error write_flash_block : 3f\n");
        return -1;
    }

    //write data to 0x40 address
    reg_addr = 0x40;
    n_bytes = 32;
    ret_val = i2c_write(SLAVE_ADDR, reg_addr, data, n_bytes);
    HAL_Delay(250);
    if(ret_val == -1)
    {
        printf("Error read_flash_block : 3f\n");
        return -1;
    }


    //write checksum to address 0x60
    reg_addr = 0x60;
    n_bytes = 0x01;
    reg_data[0] = checksum(data);

    ret_val = i2c_write(SLAVE_ADDR,reg_addr, reg_data,n_bytes);
    HAL_Delay(500);
    if(ret_val == -1 )
    {
        printf("failed to write checksum\n");
        return -1;
    }

    return 0;

}



/////////////////////////////// autocalibration fuctions ///////////////////////////////////////////////////


//enable calibration
static int enable_calibration()
{
    int ret_val = read_control(CAL_ENABLE);
    if(ret_val == -1)
    {
        printf("failed CAL ENABLE\n");
        return -1;
    }

    return (uint16_t)ret_val;
}


//enter calibration
static int enter_calibration()
{
    int ret_val = read_control(ENTER_CAL);
    if(ret_val == -1)
    {
        printf("failed enter_calibration\n");
        return -1;
    }

    return (uint16_t)ret_val;
}

//exit calibration
static int calibration_exit()
{
    int ret_val = read_control(EXIT_CAL);
    if(ret_val == -1)
    {
        printf("failed exit calibration\n");
        return -1;
    }

    return (uint16_t)ret_val;
    
}


// force the device to measure and store board offset
static int board_offset()
{
    int ret_val = read_control(BOARD_OFFSET);
    if(ret_val == -1)
    {
        printf("failed board_offset\n");
        return -1;
    }

    return (uint16_t)ret_val;
    
}

 // force the device to measure the internal cc offset
static int cc_offset()
{
    int ret_val = read_control(CC_OFFSET);
    if(ret_val == -1)
    {
        printf("failed cc_offset\n");
        return -1;
    }

    return (uint16_t)ret_val;

    
}


//report internal cc offset in calibration mode
static int offset_calibration()
{
    int ret_val =  read_control(OFFSET_CAL);
    if(ret_val == -1)
    {
        printf("failed offset_calibration\n");
        return -1;
    }

    return (uint16_t)ret_val;

   
}

//enable IT
static int it_enable()
{
    int ret_val =  read_control(IT_ENABLE);
    if(ret_val == -1)
    {
        printf("failed it_enable\n");
        return -1;
    }

    return (uint16_t)ret_val;

    
}

int autocalibrate()
{
    int ret_val = board_offset();
    if(ret_val == -1)
    {
        printf("failed autocalibrate : board offset\n");
        return -1;
    }

    ret_val = cc_offset();
    if(ret_val == -1)
    {
        printf("failed autocalibrate : cc offset\n");
        return -1;
    }

    ret_val = offset_calibration();
    if(ret_val == -1)
    {
        printf("failed autocalibrate : offset calibration\n");
        return -1;
    }

    ret_val = enable_calibration();
    if(ret_val == -1)
    {
        printf("failed autocalibrate : enable calibration\n");
        return -1;
    }

    ret_val = enter_calibration();
    if(ret_val == -1)
    {

        ret_val = calibration_exit();
        if(ret_val == -1)
        {
            printf("failed autocalibrate : exit calibration\n");
            return -1;
        }

        printf("failed autocalibrate : enter calibration\n");
        return -1;
    }

    ret_val = it_enable();
    if(ret_val == -1)
    {
        printf("failed autocalibrate : it enable\n");
        return -1;
    }

    HAL_Delay(250);
    return 0;

}




////////////////////////// gauge parameter functions //////////////////////////////////////////


//get internal temperature 0.1K
int internal_temperature()
{
    uint8_t * p_ret_val = (i2c_read(SLAVE_ADDR,INTERNAL_TEMPERATURE,0x02));
    if(p_ret_val == NULL)
    {
        printf("Error internal_temperature\n");
        return -1;
    }

    return ((uint16_t)(*(p_ret_val+1)) << 8 | *p_ret_val) ;
}

//get temperature 0.1K
int temperature()
{
    uint8_t * p_ret_val = (i2c_read(SLAVE_ADDR,TEMPERATURE,0x01));
    if(p_ret_val == NULL)
    {
        printf("Error temperature\n");
        return -1;
    }
    return ((uint16_t)(*(p_ret_val+1)) << 8 | *p_ret_val );
}

//get voltage mV
int voltage()
{
    uint8_t * p_ret_val = (i2c_read(SLAVE_ADDR,VOLTAGE,0x02));
    if(p_ret_val == NULL)
    {
        printf("Error voltage\n");
        return -1;
    }

    uint16_t voltage_value = *(p_ret_val+1) << 8 | *p_ret_val;
    return voltage_value;
}

//get current in mA
int current()
{
    uint8_t * p_ret_val = (i2c_read(SLAVE_ADDR,CURRENT,0x02));
    if(p_ret_val == NULL)
    {
        printf("Error current\n");
        return -1;
    }

    int16_t current_value = *(p_ret_val+1) << 8 | *p_ret_val;
    return current_value;

}


//get average current mA
int average_current()
{
    uint8_t * p_ret_val = (i2c_read(SLAVE_ADDR,AVERAGE_CURRENT,0x02));
    if(p_ret_val == NULL)
    {
        printf("Error average_current\n");
        return -1;
    }

    int16_t current_value = *(p_ret_val+1) << 8 | *p_ret_val;
    return current_value;
}

//get soc
uint8_t soc()
{
    return *(i2c_read(SLAVE_ADDR,STATE_OF_CHARGE,0x01));
}

/////////////////////// gauge calibration function //////////////////////////////


//read voltsel bit in pack configuration register
static int read_voltsel()
{   
    uint16_t ret_val = read_pack_configuration();
    if((int16_t)ret_val == -1)
    {
        printf("Error read_voltsel\n");
        return -1;
    }
    return (ret_val & 0x0800) >> 11;
  
}

//read voltage divider
static int readVDivider()
{
    uint8_t * p_ret_val = read_flash_block(0x68, 0x0e);
    if(p_ret_val == NULL)
    {
        printf("Error readVDivider\n");
        return -1;
    }

    return (uint16_t)(*(p_ret_val + 14)) << 8 | *(p_ret_val + 15);

}

//read pack_configuration
static int read_pack_configuration()
{
    uint8_t * p_ret_val = read_flash_block(0x40, 0x00);
    if(p_ret_val == NULL)
    {
        printf("Error read_pack_configuration\n");
        return -1;
    }
    return (uint16_t)(*(p_ret_val + 0)) << 8 | *(p_ret_val + 1);

}

//read design capacity
static int read_design_capacity()
{
    uint8_t * p_ret_val = read_flash_block(0x30, 0x0b);
    if(p_ret_val == NULL)
    {
        printf("Error read_design_capacity\n");
        return -1;
    }

    return (uint16_t)(*(p_ret_val + 11)) << 8 | *(p_ret_val + 12);

}

//read design energy
static int read_design_energy()
{
    uint8_t * p_ret_val = read_flash_block(0x30, 0x0d);
    if(p_ret_val == NULL)
    {
        printf("Error read_design_energy\n");
        return -1;
    }

    return (uint16_t)(*(p_ret_val + 13)) << 8 | *(p_ret_val + 14);

}

//read flash_update_ok_voltage
static int read_flash_update_ok_voltage()
{
    uint8_t * p_ret_val = read_flash_block(0x44, 0x00);
    if(p_ret_val == NULL)
    {
        printf("Error read_flash_update_ok_voltage\n");
        return -1;
    }

    return (uint16_t)(*(p_ret_val + 0)) << 8 | *(p_ret_val + 1);

}

//read no. of cell
static int read_series_cell()
{
    uint8_t * p_ret_val = read_flash_block(0x40, 0x07);
    if(p_ret_val == NULL)
    {
        printf("Error read_series_cell\n");
        return -1;
    }

    return  *(p_ret_val+7);

}

//read design energy scale
static int read_design_energy_scale()
{
    uint8_t * p_ret_val = read_flash_block(0x30, 0x1e);
    if(p_ret_val == NULL)
    {
        printf("Error read_design_energy_scale\n");
        return -1;
    }

    return  *(p_ret_val+30);

}

//set voltage divider value
static int set_vdivider(uint16_t v_divider)
{
    uint8_t data[32];
    uint8_t msb = v_divider >> 8 ;
    uint8_t lsb = v_divider & 0x00FF;

    uint8_t * p_ret_val = read_flash_block(0x68,0x0e);
    //HAL_Delay(250);
    if(p_ret_val == NULL)
    {
        printf("Error setv_divider : read flash block\n");
        return -1;
    }
    
    for(uint8_t i = 0; i < 32; i++)
    {
        data[i] = *(p_ret_val + i);
        // printf("data[i] = %u\n", data[i]);
    }

    // 默认10.124 mΩ
    // data[0] = 127;
    // data[1] = 113;
    // data[2] = 32;
    // data[3] = 92;
    // 修改10.000 mΩ 相当于写入 0.476800
    data[0] = 127;
    data[1] = 116;
    data[2] = 31;
    data[3] = 33;
    data[14] = msb; //msb
    data[15] = lsb; //lsb

    int ret_val = write_flash_block(0x68, 0x0e,data);
    if(ret_val == -1)
    {
        printf("Error setting voltage divider\n");
        return -1;
    }

    return 0;
}

//set series cell
static int set_series_cell(uint16_t series_cell)
{
    uint8_t data[32];
    uint8_t * p_ret_val = read_flash_block(0x40,0x07);
    //HAL_Delay(250);
    if(p_ret_val == NULL)
    {
        printf("Error set_series_cell : read flash block\n");
        return -1;
    }

    for(uint8_t i = 0; i < 32; i++)
    {
        data[i] = *(p_ret_val + i);
    }

    data[7] = (uint8_t)series_cell; //number of series cell
    

    int ret_val = write_flash_block(0x40, 0x07,data);
    if(ret_val == -1)
    {
        printf("Error setting series cell\n");
        return -1;
    }
    return 0;
    
}

//set design capacity
static int set_design_capacity(uint16_t design_capacity)
{
    uint8_t data[32];
    uint8_t msb = design_capacity >> 8 ;
    uint8_t lsb = design_capacity & 0x00FF;

    uint8_t * p_ret_val = read_flash_block(0x30,0x0b);
    HAL_Delay(250);
    if(p_ret_val  == NULL)
    {
        printf("Error set_design_capacity : read_flash_block\n");
        return -1;
    }
    
    for(uint8_t i = 0; i < 32; i++)
    {
        data[i] = *(p_ret_val + i);
    }

    data[11] = msb; //msb
    data[12] = lsb; //lsb

    int ret_val = write_flash_block(0x30, 0x0b,data);
    if(ret_val == -1)
    {
        printf("Error set_design_capacity\n");
        return -1;
    }
    return 0;
}

//set design energy scale
static int set_design_energy_scale(uint16_t design_energy_scale)
{
    uint8_t data[32];
   
    uint8_t * p_ret_val = read_flash_block(0x30,0x1e);
    HAL_Delay(250);
    if(p_ret_val == NULL)
    {
        printf("Error set_design_energy_scale : read_flash_block\n");
        return -1;
    }
    
    for(uint8_t i = 0; i < 32; i++)
    {
        data[i] = *(p_ret_val + i);
    }

    data[30] = (uint8_t)design_energy_scale;//design_energy_scale = 10

    int ret_val = write_flash_block(0x30, 0x1e,data);
    if(ret_val == -1)
    {
        printf("Error set_design_energy_scale\n");
        return -1;
    }

    return 0;
}

//set design energy
static int set_design_energy(uint16_t design_energy)
{
    uint8_t data[32];
    uint8_t msb = design_energy >> 8 ;
    uint8_t lsb = design_energy & 0x00FF;

    uint8_t * p_ret_val = read_flash_block(0x30,0x0d);
    //HAL_Delay(250);
    if(p_ret_val == NULL)
    {
        printf("Error set_design_energy : read_flash_block\n");
        return -1;
    }
    
    for(uint8_t i = 0; i < 32; i++)
    {
        data[i] = *(p_ret_val+i);
    }

    data[13] = msb; //msb
    data[14] = lsb; //lsb

    int ret_val = write_flash_block(0x30, 0x0d,data);
    if(ret_val == -1)
    {
        printf("Error set_design_energy\n");
        return -1;
    }

    return 0;
}

//set VOLTSEL BIT in pack_configuration register
static int set_voltsel(uint16_t dummy_value)
{   
    uint8_t data[32];
    uint16_t pack_configuration = read_pack_configuration();
    //HAL_Delay(250);
    if((int16_t)pack_configuration == -1)
    {
        printf("Error set voltsel : read pack configuration\n");
        return -1;
    }

    dummy_value &= 1;
    pack_configuration = pack_configuration & ~0x0800;
    printf("pack_configuration = %#x\n", pack_configuration);
    pack_configuration = pack_configuration | (dummy_value<<11);
    printf("pack_configuration = %#x\n", pack_configuration);

    uint8_t msb = pack_configuration >> 8 ;
    uint8_t lsb = pack_configuration & 0x00FF;

    uint8_t * p_ret_val = read_flash_block(0x40,0x00);
    //HAL_Delay(250);
    if(p_ret_val == NULL)
    {
        printf("Error set_voltsel : read_flash_block\n");
        return -1;
    }


    for(uint8_t i = 0; i < 32; i++)
    {
        data[i] = *(p_ret_val+i);
    }

    data[0] = msb; //msb
    data[1] = lsb; //lsb

    int ret_val = write_flash_block(0x40, 0x00,data);
    //HAL_Delay(250);
    if(ret_val == -1)
    {
        printf("Error set_voltsel : write_flash_block\n");
        return -1;
    }

    return 0;

}

//set flash update ok cell volt
static int set_flash_update_ok_voltage(uint16_t flash_update_ok_voltage)
{
    uint8_t data[32];
    uint8_t msb = flash_update_ok_voltage >> 8 ;
    uint8_t lsb = flash_update_ok_voltage & 0x00FF;

    uint8_t * p_ret_val = read_flash_block(0x44,0x00);
    //HAL_Delay(250);
    if(p_ret_val == NULL)
    {
        printf("Error set_flash_update_ok_voltage : read_flash_block\n");
        return -1;
    }
    
    for(uint8_t i = 0; i < 32; i++)
    {
        data[i] = *(p_ret_val+i);
    }

    data[0] = msb; //msb
    data[1] = lsb; //lsb

    int ret_val = write_flash_block(0x44, 0x00,data);
    //HAL_Delay(250);
    if(ret_val == -1)
    {
        printf("Error set_flash_update_ok_voltage : write_flash_block\n");
        return -1;
    }

    return 0;

}

int gauge_verify_and_calibrate()
{
    int ret_val;
    gauge_unlock();

    // setting flash update ok voltage
    ret_val =  verify_calibrate_func(read_flash_update_ok_voltage,set_flash_update_ok_voltage,3300);
    if(ret_val == -1)
    {
        return -1;
    }

    // set voltsel bit
    ret_val =  verify_calibrate_func(read_voltsel,set_voltsel,1);
    if(ret_val == -1)
    {
        return -1;
    }

    // set series cell
    ret_val =  verify_calibrate_func(read_series_cell,set_series_cell,3);
    if(ret_val == -1)
    {
        return -1;
    }

    // set voltage divider
    ret_val =  verify_calibrate_func(readVDivider,set_vdivider,15752);
    // set_vdivider(15752);   // 为了跳过是否改值检测
    if(ret_val == -1)
    {
        return -1;
    }

    // set design energy scale
    ret_val =  verify_calibrate_func(read_design_energy_scale,set_design_energy_scale,1);
    if(ret_val == -1)
    {
        return -1;
    }

    // set design capacity
    ret_val =  verify_calibrate_func(read_design_capacity,set_design_capacity,2500);
    if(ret_val == -1)
    {
        return -1;
    }

    // set design energy
    ret_val =  verify_calibrate_func(read_design_energy,set_design_energy,27750);
    if(ret_val == -1)
    {
        return -1;
    }

    printf("Successfully done calibration\n");

    ret_val =  calibrated_data();
    if(ret_val == -1)
    {
        return -1;
    }

    return 0;

}

static int verify_calibrate_func(int (*read_func)(), int (*set_func)(uint16_t),uint16_t value)
{

    uint8_t attempt = 0;
    while(read_func() != value && attempt < 3)
    {
        set_func(value);
        if(read_func() != value)
        {
            attempt++;
        }
        
    }
    if(attempt>=3)
    {
        failed_to_calibrate(value);
        return -1;
    }
    return 0;
}

static void failed_to_calibrate(uint16_t value)
{
    if(value == 1)
    {
        printf("Failed to set voltsel\n");
    }
    else if (value == 3)
    {
        printf("Failed to set series cell\n");
    }
    else if (value == 1)
    {
        printf("Failed to set design energy scale\n");
    }
    else if (value == 2500)
    {
        printf("Failed to set design capacity\n");
    }
    else if (value == 15752)
    {
        printf("Failed to set voltage divider\n");
    }
    else if (value == 27750)
    {
        printf("Failed to set design energy\n");
    }
    else if (value == 3300)
    {
        printf("Failed to set flash update ok voltage\n");
    }
}

static int calibrated_data()
{
    int ret_val = read_flash_update_ok_voltage();
    if(ret_val == -1)
    {
        printf("Error calibrated dada : read_flash_update_ok_voltage\n");
        return -1;
    }
    printf("flash update voltage : %d\n",ret_val);

    ret_val = read_voltsel();
    if(ret_val == -1)
    {
        printf("Error calibrated dada : read_voltsel\n");
        return -1;
       
    }
    printf("voltsel : %x\n",ret_val);

    ret_val = read_series_cell();
    if(ret_val == -1)
    {
        printf("Error calibrated dada : read_series_cell\n");
        return -1;
    }
    printf("series cell : %x\n",ret_val);

    ret_val = readVDivider();
    if(ret_val == -1)
    {
        printf("Error calibrated dada : readVdivider\n");
        return -1;
        
    }
    printf("vdivider : %d\n",ret_val );
    
    ret_val = read_design_energy_scale();
    if(ret_val == -1)
    {
        printf("Error calibrated dada : read_design_energy_scale\n");
        return -1;
        
    }
    printf("design energy scale: %d\n",ret_val);

    ret_val = read_design_capacity();
    if(ret_val == -1)
    {
        printf("Error calibrated dada : read_design_capacity\n");
        return -1;
        
    }
    printf("design capacity : %d\n",ret_val);

    ret_val = read_design_energy();
    if(ret_val == -1)
    {
        printf("Error calibrated dada : read_design_energy\n");
        return -1;
        
    }
    printf("design energy: %d\n",ret_val);

    return 0;

}

/////////////////////////////////////////////////////////////////////////////////
