#include "imu_sensor.h"
#include "app.h"

static imu_sensor_data_t sensor_data = {0,0,0};
static sensor_selsection_t sensor_selection;

static imu_status_t lsm6ds3_fifo_sensor_enable(void);
static imu_status_t imu_sensor_config_acc(uint8_t data_rate, uint8_t scale);
static imu_status_t imu_sensor_config_gyro(uint8_t data_rate, uint8_t scale);
static imu_status_t imu_sensor_acc_output_status_config(uint8_t status);
static imu_status_t imu_sensor_gyro_output_status_config(uint8_t status);
static imu_status_t imu_sensor_fifo_threshold_interrupt(void);
static imu_status_t imu_sensor_fifo_threshold_disable_interrupt(void);
static imu_status_t imu_sensor_fifo_threshold_level(uint16_t fifo_level);
static imu_status_t imu_sensor_lsm6ds3_soft_reset(void);
static imu_status_t imu_sensor_lsm303agr_soft_rest(void);
static imu_status_t imu_sensor_acc_get_sensitivity( float *pfData );
static imu_status_t  imu_sensor_gyro_get_sensitivity( float *pfData );


/*reset sensors*/
imu_status_t imu_sensor_reset(void)
{
    uint8_t tmp1 = 0;
    uint8_t tempReg[2] = {0, 0};
    uint32_t p_data_rate = 0;

    if(LSM6DS3_IO_Init() != IMU_6AXES_OK)
    {
        printf("lsm6ds3 io init error\n");
        return imu_status_fail;
    }
    /* Configure interrupt lines */
    LSM6DS3_IO_ITConfig();
    printf("IT IO Config\n");

    /*soft reset*/
    if(imu_sensor_lsm6ds3_soft_reset() != imu_status_ok)
    {
        printf("lsm6ds3 reset error\n");
        return imu_status_fail;
    }

    /*soft reset*/
    if(imu_sensor_lsm303agr_soft_rest() != imu_status_ok)
    {
        printf("lsm303agr reset error\n");
        return imu_status_fail;
    }

    printf("sensor reset\n");

    imu_sensor_read_fifo_status();

    /*set fifo water mark level*/
    if(imu_sensor_fifo_threshold_level(2046) != imu_status_ok) { //1.2kBytes
        printf("sensor fifo water mark setting error\n");
        return imu_status_fail;
    }

    printf("fifo water mark level set over\n");

    /*enable fifo interrupt*/
    if(imu_sensor_fifo_threshold_interrupt() != imu_status_ok ) {
        printf("sensor fifo interrupt setting error\n");
        return imu_status_fail;
    }

    printf("fifo threshold interrupt set over\n");

    if(lsm6ds3_fifo_sensor_enable() != IMU_6AXES_OK)
    {
        printf("sensor fifoenable error\n");
        return imu_status_fail;
    }
    printf("sensor enable\n");
    return imu_status_ok;

}



/*active sensor*/
imu_status_t imu_sensor_select_features(sensor_selsection_t features)
{

    sensor_selection = features;

    printf("sensor features : %x\n", sensor_selection);

    return imu_status_ok;
}

/*set data rate*/
imu_status_t imu_sensor_set_data_rate(uint32_t* p_data_rate)
{

    uint8_t tmp1 = 0x00;
    uint8_t new_odr = 0x00;
    uint8_t mag_odr = 100;
    /*lsm6ds3*/
    {
        if(LSM6DS3_IO_Read(&tmp1, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_FIFO_CTRL5, 1) != IMU_6AXES_OK)
        {
            return imu_status_fail;
        }

        /* FIFO ODR selection */
        switch(* p_data_rate) {
        case 0:
            tmp1 |= LSM6DS3_XG_FIFO_ODR_NA;
        case 10:
            tmp1 |= LSM6DS3_XG_FIFO_ODR_10HZ;
            break;
        case 25:
            tmp1 |= LSM6DS3_XG_FIFO_ODR_25HZ;
            break;
        case 50:
            tmp1 |= LSM6DS3_XG_FIFO_ODR_50HZ;
            break;
        case 100:
            tmp1 |= LSM6DS3_XG_FIFO_ODR_100HZ;
            break;
        case 200:
            tmp1 |= LSM6DS3_XG_FIFO_ODR_200HZ;
            break;
        case 400:
            tmp1 |= LSM6DS3_XG_FIFO_ODR_400HZ;
            break;
        case 800:
            tmp1 |= LSM6DS3_XG_FIFO_ODR_800HZ;
            break;
        case 1600:
            tmp1 |= LSM6DS3_XG_FIFO_ODR_1600HZ;
            break;

        default:
            break;
        }

        /* FIFO mode selection */
        tmp1 &= ~(LSM6DS3_XG_FIFO_MODE_MASK);
        tmp1 |= LSM6DS3_XG_FIFO_MODE_FIFO;

        if(LSM6DS3_IO_Write(&tmp1, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_FIFO_CTRL5, 1) != IMU_6AXES_OK)
        {
            return imu_status_fail;
        }
        printf("fifo odr:%x\n",* p_data_rate);
    }
    /*acc and gyro odr */
    {
        new_odr = ( * p_data_rate <= 0    )     ? LSM6DS3_XL_ODR_PD          /* Power Down */
                  : ( * p_data_rate <= 13   )  ? LSM6DS3_XL_ODR_13HZ
                  : ( * p_data_rate <= 26   )  ? LSM6DS3_XL_ODR_26HZ
                  : ( * p_data_rate <= 52   )  ? LSM6DS3_XL_ODR_52HZ
                  : ( * p_data_rate <= 104  )  ? LSM6DS3_XL_ODR_104HZ
                  : ( * p_data_rate <= 208  )  ? LSM6DS3_XL_ODR_208HZ
                  : ( * p_data_rate <= 416  )  ? LSM6DS3_XL_ODR_416HZ
                  : ( * p_data_rate <= 833  )  ? LSM6DS3_XL_ODR_833HZ
                  :                          LSM6DS3_XL_ODR_1660HZ;
        if((sensor_selection == ACC_ENABLE) || (sensor_selection == ACC_AND_GYRO_ENABLE)) {

            if(imu_sensor_config_acc(new_odr, LSM6DS3_XL_FS_2G) != imu_status_ok)
                return imu_status_fail;
        }
        if((sensor_selection == GYRO_ENABLE) || (sensor_selection == ACC_AND_GYRO_ENABLE)) {

            if(imu_sensor_config_gyro(new_odr, LSM6DS3_G_FS_125_ENABLE) != imu_status_ok)
                return imu_status_fail;
        }

        printf("acc and gyro odr set over\n");
    }
    /*lsm303agr*/
    {
        if(LSM303AGR_MAG_W_ODR(mag_odr) != imu_status_ok)
        {
            return imu_status_fail;
        }
        printf("mag odr set over\n");
    }


    return imu_status_ok;
}

/*start get sensor data*/
imu_status_t imu_sensor_start(void)
{

    /*lsm6ds3*/
    {
        if((sensor_selection == ACC_ENABLE) || (sensor_selection == ACC_AND_GYRO_ENABLE)) {
            if(imu_sensor_acc_output_status_config(OUTPUT_ENABLE) != imu_status_ok)
            {
                return imu_status_fail;
            }
            printf("acc output enable\n");
        }
        if((sensor_selection == GYRO_ENABLE) || (sensor_selection == ACC_AND_GYRO_ENABLE)) {
            if(imu_sensor_gyro_output_status_config(OUTPUT_ENABLE) != imu_status_ok)
            {
                return imu_status_fail;
            }
            printf("gyro output enable\n");
        }

    }
    /*lsm303agr odr*/
    {
        if(LSM303AGR_MAG_W_MD(LSM303AGR_MAG_MD_CONTINUOS_MODE) != imu_status_ok)
        {
            return imu_status_fail;
        }
        printf("mag output enable\n");
    }
    return imu_status_ok;
}

/*stop get sensor data*/
imu_status_t imu_sensor_stop(void)
{
    /*lsm6ds3*/
    {

        if((sensor_selection == ACC_ENABLE) || (sensor_selection == ACC_AND_GYRO_ENABLE)) {
            if(imu_sensor_acc_output_status_config(OUTPUT_DISABLE) != imu_status_ok)
            {
                return imu_status_fail;
            }
            printf("acc output disable\n");
        }
        if((sensor_selection == GYRO_ENABLE) || (sensor_selection == ACC_AND_GYRO_ENABLE)) {
            if(imu_sensor_gyro_output_status_config(OUTPUT_DISABLE) != imu_status_ok)
            {
                return imu_status_fail;
            }
            printf("gyro output disable\n");
        }

    }
    /*lsm303agr odr*/
    {
        if(LSM303AGR_MAG_W_MD(LSM303AGR_MAG_MD_IDLE1_MODE) != imu_status_ok)
        {
            return imu_status_fail;
        }
        printf("mag output disable\n");
    }

    return imu_status_ok;
}

/*received data callback*/
imu_status_t on_imu_sensor_data(uint8_t flag, int32_t* data)
{
// printf("flag : %x,short :%x, sensor data: %x,%x,%x\n",
//           flag, data[0], data[1],data[2]);

    if(flag == TYPE_ACC_DATA) {
        sensor_data.acc[0] = data[0];
        sensor_data.acc[1] = data[1];
        sensor_data.acc[2] = data[2];
    }
    if(flag == TYPE_GYRO_DATA) {
        sensor_data.gyro[0] = data[0];
        sensor_data.gyro[1] = data[1];
        sensor_data.gyro[2] = data[2];
        app_pitch_roll(&sensor_data);
    }
    if(flag == TYPE_MAG_DATA) {
        sensor_data.mag[0] = data[0];
        sensor_data.mag[1] = data[1];
        sensor_data.mag[2] = data[2];
        app_pitch_roll(&sensor_data);
    }

}

/*fifo config*/
static imu_status_t lsm6ds3_fifo_sensor_enable(void)
{
    uint8_t tmp1 = 0x00;
    /*lsm6ds3*/
    {

        if(LSM6DS3_IO_Read(&tmp1, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_CTRL3_C, 1) != IMU_6AXES_OK)
        {
            return imu_status_fail;
        }
        /* Enable register address automatically incremented during a multiple byte
           access with a serial interface (I2C or SPI) */
        tmp1 &= ~(LSM6DS3_XG_IF_INC_MASK);
        tmp1 |= LSM6DS3_XG_IF_INC;

        if(LSM6DS3_IO_Write(&tmp1, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_CTRL3_C, 1) != IMU_6AXES_OK)
        {
            return imu_status_fail;
        }

    }

    return imu_status_ok;

}

/*active acc*/
static imu_status_t imu_sensor_config_acc(uint8_t data_rate, uint8_t scale)
{
    uint8_t tempReg = 0x00;

    /*acc rate and scale*/
    if(LSM6DS3_IO_Read( &tempReg, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_CTRL1_XL, 1 ) != IMU_6AXES_OK)
    {
        return imu_status_fail;
    }

    tempReg &= ~(LSM6DS3_XL_ODR_MASK);
    tempReg |= data_rate;
    /* Full scale selection */
    tempReg &= ~(LSM6DS3_XL_FS_MASK);
    tempReg |= scale;

    if(LSM6DS3_IO_Write(&tempReg, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_CTRL1_XL, 1) != IMU_6AXES_OK)
    {
        return imu_status_fail;
    }

    return imu_status_ok;
}

/*active gyro*/
static imu_status_t imu_sensor_config_gyro(uint8_t data_rate, uint8_t scale)
{
    uint8_t tempReg = 0x00;

    /*gyro rate and scale*/
    if(LSM6DS3_IO_Read( &tempReg, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_CTRL2_G, 1 ) != IMU_6AXES_OK)
    {
        return imu_status_fail;
    }

    tempReg &= ~(LSM6DS3_G_ODR_MASK);
    tempReg |= data_rate;
    /* Full scale selection */
    tempReg &= ~(LSM6DS3_G_FS_125_MASK);
    tempReg |= scale;

    if(LSM6DS3_IO_Write(&tempReg, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_CTRL2_G, 1) != IMU_6AXES_OK)
    {
        return imu_status_fail;
    }

    return imu_status_ok;
}


/*set acc output state*/
static imu_status_t imu_sensor_acc_output_status_config(uint8_t status)
{
    uint8_t tmp1 = 0x00;
    uint8_t eX = 0x00;
    uint8_t eY = 0x00;
    uint8_t eZ = 0x00;
    if(status == OUTPUT_ENABLE) {

        eX = LSM6DS3_XL_XEN_ENABLE;
        eY = LSM6DS3_XL_YEN_ENABLE;
        eZ = LSM6DS3_XL_ZEN_ENABLE;
    }

    if(LSM6DS3_IO_Read(&tmp1, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_CTRL9_XL, 1) != IMU_6AXES_OK)
    {
        return imu_status_fail;
    }

    /* Enable X axis selection */
    tmp1 &= ~(LSM6DS3_XL_XEN_MASK);
    tmp1 |= eX;

    /* Enable Y axis selection */
    tmp1 &= ~(LSM6DS3_XL_YEN_MASK);
    tmp1 |= eY;

    /* Enable Z axis selection */
    tmp1 &= ~(LSM6DS3_XL_ZEN_MASK);
    tmp1 |= eZ;

    if(LSM6DS3_IO_Write(&tmp1, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_CTRL9_XL, 1) != IMU_6AXES_OK)
    {
        return imu_status_fail;
    }

    return imu_status_ok;
}

/*set gyro output state*/
static imu_status_t imu_sensor_gyro_output_status_config(uint8_t status)
{
    uint8_t tmp1 = 0x00;
    uint8_t eX = 0x00;
    uint8_t eY = 0x00;
    uint8_t eZ = 0x00;

    if(status == OUTPUT_ENABLE) {

        eX = LSM6DS3_G_XEN_ENABLE;
        eY = LSM6DS3_G_YEN_ENABLE;
        eZ = LSM6DS3_G_ZEN_ENABLE;
    }

    if(LSM6DS3_IO_Read(&tmp1, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_CTRL10_C, 1) != IMU_6AXES_OK)
    {
        return imu_status_fail;
    }

    /* Enable X axis selection */
    tmp1 &= ~(LSM6DS3_G_XEN_MASK);
    tmp1 |= eX;

    /* Enable Y axis selection */
    tmp1 &= ~(LSM6DS3_G_YEN_MASK);
    tmp1 |= eY;

    /* Enable Z axis selection */
    tmp1 &= ~(LSM6DS3_G_ZEN_MASK);
    tmp1 |= eZ;

    if(LSM6DS3_IO_Write(&tmp1, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_CTRL10_C, 1) != IMU_6AXES_OK)
    {
        return imu_status_fail;
    }

    return imu_status_ok;
}

/*fifo read*/
imu_status_t imu_sensor_read_data_from_fifo(void)
{
    float sensitivity = 0.0f;
    int32_t data[3]= {0};
    uint16_t fifo_threthold = 2046;
    int16_t pData[3] = {0};
    uint8_t tempReg[2] = {0, 0}, tmp1;
    sensor_data_type_t flag;

    printf("read io\n");

    /*mag data*/
    LSM303AGR_MAG_Get_Magnetic(data);
    flag = TYPE_MAG_DATA;
    on_imu_sensor_data(flag, data);

    while(fifo_threthold > 3) {
        if(LSM6DS3_IO_Read(&tempReg[0], LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_FIFO_DATA_OUT_L, 2) != IMU_6AXES_OK)
        {
            printf("read sensor error\n");
            return imu_status_fail;
        }
//        printf("ok\n");
        pData[0] = ((((int16_t)tempReg[1]) << 8) + (int16_t)tempReg[0]);

        if(LSM6DS3_IO_Read(&tempReg[0], LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_FIFO_DATA_OUT_L, 2) != IMU_6AXES_OK)
        {
            printf("read sensor error\n");
            return imu_status_fail;
        }

        pData[1] = ((((int16_t)tempReg[1]) << 8) + (int16_t)tempReg[0]);

        if(LSM6DS3_IO_Read(&tempReg[0], LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_FIFO_DATA_OUT_L, 2) != IMU_6AXES_OK)
        {
            printf("read sensor error\n");
            return imu_status_fail;
        }

        pData[2] = ((((int16_t)tempReg[1]) << 8) + (int16_t)tempReg[0]);

        if((flag == TYPE_ACC_DATA) || (flag == TYPE_MAG_DATA)) {
            /*acc data*/
            if(imu_sensor_acc_get_sensitivity( &sensitivity ) != imu_status_ok)
            {
                return imu_status_fail;
            }

            data[0] = (int32_t)(pData[0] * sensitivity);
            data[1] = (int32_t)(pData[1] * sensitivity);
            data[2] = (int32_t)(pData[2] * sensitivity);

            on_imu_sensor_data(flag, data);
            flag = TYPE_GYRO_DATA;
        } else {
            /*gyro data*/
            if(imu_sensor_acc_get_sensitivity( &sensitivity ) != imu_status_ok)
            {
                return imu_status_fail;
            }

            data[0] = (int32_t)(pData[0] * sensitivity);
            data[1] = (int32_t)(pData[1] * sensitivity);
            data[2] = (int32_t)(pData[2] * sensitivity);

            on_imu_sensor_data(flag, data);
            flag = TYPE_ACC_DATA;
        }

        fifo_threthold-=3;
    }

    return imu_status_ok;
}

/*fifo threshold interrupt*/
static imu_status_t imu_sensor_fifo_threshold_interrupt(void)
{

    uint8_t tmp1;

    if(LSM6DS3_IO_Read(&tmp1, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_INT1_CTRL, 1) != IMU_6AXES_OK)
    {
        return imu_status_fail;
    }
    /*enable fifo threshold interrupt*/
    tmp1 &= ~(LSM6DS3_XG_FIFO_INT_THRESHOLD_MASK);
    tmp1 |= LSM6DS3_XG_FIFO_INT_THRESHOLD_MASK;

    if(LSM6DS3_IO_Write(&tmp1, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_INT1_CTRL, 1) != IMU_6AXES_OK)
    {
        return imu_status_fail;
    }

    return imu_status_ok;

}


/*fifo threshold disable interrupt*/
static imu_status_t imu_sensor_fifo_threshold_disable_interrupt(void)
{

    uint8_t tmp1;

    if(LSM6DS3_IO_Read(&tmp1, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_INT1_CTRL, 1) != IMU_6AXES_OK)
    {
        return imu_status_fail;
    }
    /*enable fifo threshold interrupt*/
    tmp1 &= ~(LSM6DS3_XG_FIFO_INT_THRESHOLD_MASK);

    if(LSM6DS3_IO_Write(&tmp1, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_INT1_CTRL, 1) != IMU_6AXES_OK)
    {
        return imu_status_fail;
    }

    printf("disable fifo interrupt on int1\n");
    return imu_status_ok;

}

/*fifo threshold level setting*/
static imu_status_t imu_sensor_fifo_threshold_level(uint16_t fifo_level)
{
    uint8_t tmp1,count;
    {
        if(LSM6DS3_IO_Read(&tmp1, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_FIFO_CTRL1, 1) != IMU_6AXES_OK)
        {
            return imu_status_fail;
        }
        /*set water mark low 8bit*/
        tmp1 &= ~(0xFF);
        tmp1 |= (fifo_level & 0xFF);
        if(LSM6DS3_IO_Write(&tmp1, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_FIFO_CTRL1, 1) != IMU_6AXES_OK)
        {
            return imu_status_fail;
        }

    }

    {
        if(LSM6DS3_IO_Read(&tmp1, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_FIFO_CTRL2, 1) != IMU_6AXES_OK)
        {
            return imu_status_fail;
        }
        /*set water mark high 4bit*/
        tmp1 &= ~(LSM6DS3_XG_FIFO_THRESHOLD_MASK);
        tmp1 |= (fifo_level >> 8) & 0x0F;
        if(LSM6DS3_IO_Write(&tmp1, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_FIFO_CTRL2, 1) != IMU_6AXES_OK)
        {
            return imu_status_fail;
        }

    }

    {
        if(LSM6DS3_IO_Read(&tmp1, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_FIFO_CTRL3, 1) != IMU_6AXES_OK)
        {
            return imu_status_fail;
        }
        /*set */
        tmp1 &= ~(0x3F);
        tmp1 |= 0x09;
        if(LSM6DS3_IO_Write(&tmp1, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_FIFO_CTRL3, 1) != IMU_6AXES_OK)
        {
            return imu_status_fail;
        }

    }

    return imu_status_ok;

}
/*lsm6ds3 soft reset*/
static imu_status_t imu_sensor_lsm6ds3_soft_reset(void)
{
    uint8_t tmp1;

    if(LSM6DS3_IO_Read(&tmp1, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_CTRL3_C, 1) != IMU_6AXES_OK)
    {
        return imu_status_fail;
    }

    /*soft reset*/
    tmp1 &= ~(0x01);
    tmp1 |= 0x01;

    if(LSM6DS3_IO_Write(&tmp1, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_CTRL3_C, 1) != IMU_6AXES_OK)
    {
        return imu_status_fail;
    }
    HAL_Delay(1);

    return imu_status_ok;

}

/*lsm303agr soft reset*/
static imu_status_t imu_sensor_lsm303agr_soft_rest(void)
{
    uint8_t tmp1;

    if( LSM303AGR_MAG_ReadReg(LSM303AGR_MAG_CFG_REG_A, &tmp1) != imu_status_ok)
    {
        return imu_status_fail;
    }

    tmp1 &= ~(0x20);
    tmp1 |= (0x20);

    if( LSM303AGR_MAG_WriteReg(LSM303AGR_MAG_CFG_REG_A, tmp1) != imu_status_ok)
    {
        return imu_status_fail;
    }

    HAL_Delay(1);
    return imu_status_ok;
}

imu_status_t imu_sensor_read_fifo_status(void)
{
    uint8_t tmp1;

    if(LSM6DS3_IO_Read(&tmp1, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_FIFO_STATUS1, 1) != IMU_6AXES_OK)
    {
        return imu_status_fail;
    }

    printf("number of unread words: %d\n",tmp1);

    if(LSM6DS3_IO_Read(&tmp1, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_FIFO_STATUS2, 1) != IMU_6AXES_OK)
    {
        return imu_status_fail;
    }

    printf("fifo status: %x\n",tmp1);

    return imu_status_ok;

}

static imu_status_t imu_sensor_acc_get_sensitivity( float *pfData )
{
    /*Here we have to add the check if the parameters are valid*/

    uint8_t tempReg = 0x00;


    if(LSM6DS3_IO_Read( &tempReg, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_CTRL1_XL, 1 ) != imu_status_ok)
    {
        return imu_status_fail;
    }

    tempReg &= LSM6DS3_XL_FS_MASK;

    switch( tempReg )
    {
    case LSM6DS3_XL_FS_2G:
        *pfData = 0.061f;
        break;
    case LSM6DS3_XL_FS_4G:
        *pfData = 0.122f;
        break;
    case LSM6DS3_XL_FS_8G:
        *pfData = 0.244f;
        break;
    case LSM6DS3_XL_FS_16G:
        *pfData = 0.488f;
        break;
    default:
        break;
    }

    return imu_status_ok;
}

static imu_status_t  imu_sensor_gyro_get_sensitivity( float *pfData )
{
    /*Here we have to add the check if the parameters are valid*/

    uint8_t tempReg = 0x00;

    if(LSM6DS3_IO_Read( &tempReg, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_CTRL2_G, 1 ) != imu_status_ok)
    {
        return imu_status_fail;
    }

    tempReg &= LSM6DS3_G_FS_125_MASK;

    if(tempReg == LSM6DS3_G_FS_125_ENABLE)
    {
        *pfData = 4.375f;
    }
    else
    {
        if(LSM6DS3_IO_Read( &tempReg, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_CTRL2_G, 1 ) != imu_status_ok)
        {
            return imu_status_fail;
        }

        tempReg &= LSM6DS3_G_FS_MASK;

        switch( tempReg )
        {
        case LSM6DS3_G_FS_245:
            *pfData = 8.75f;
            break;
        case LSM6DS3_G_FS_500:
            *pfData = 17.50f;
            break;
        case LSM6DS3_G_FS_1000:
            *pfData = 35.0f;
            break;
        case LSM6DS3_G_FS_2000:
            *pfData = 70.0f;
            break;
        default:
            break;
        }
    }

    return imu_status_ok;
}