/*
 * FlashRW.c
 *
 * Created: 2018-01-29
 * Author: zhanglifu
 */
 
/*********************************************************************/
//                        头文件
/*********************************************************************/
#include "FlashRW.h"
#include "stdio.h"


// 不检查的写入
// WriteAddr:起始地址
// pBuffer:  数据指针
// NumToWrite:字节数数
void FlashWriteNoCheck( uint32_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite )
{
    uint16_t i;

    for( i=0; i<NumToWrite; i+=4 )
    {
        while( HAL_OK != HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, WriteAddr+i,*(uint32_t *)(pBuffer+i) ) );
    }

}

extern void FLASH_PageErase(uint32_t PageAddress);
void FlashWriteBuff( const uint32_t destination_address, uint8_t *const buffer,uint32_t length )
{
    uint16_t i = 0;
    // uint8_t FlashBuff[FMC_SECTOR_SIZE];
    uint32_t StartAddress = destination_address - destination_address % FMC_SECTOR_SIZE;
    // uint16_t Offset = destination_address - StartAddress;
    // uint8_t *pBuf = buffer;
    uint32_t remaindNum = length;


	printf("StartAddress = %#x\n", StartAddress);
    // 地址检查
    if( (destination_address < FMC_FLASH_BASE) || ( destination_address + length >= FMC_FLASH_END) || (length <= 0) )
        return;

	// printf("destination_address i = %u, Offset = %u\n", i, Offset);

    HAL_Delay(1000);
    HAL_FLASH_Unlock();	// 解锁
    do
    {
        // 读出一页数据
        // for(i=0; i < FMC_SECTOR_SIZE; i += 4 )
        //     *(uint32_t *)(FlashBuff+i) = *(uint32_t *)(StartAddress+i);

        // // 修改要改入的数据
        // for ( i=0; (i+Offset)<FMC_SECTOR_SIZE && i< remaindNum; i++ )
        //     *(FlashBuff+Offset+i) = *(pBuf+i);


        // 擦除一ROW数据
#if 0
        FLASH_PageErase( StartAddress );
        HAL_StatusTypeDef status = HAL_ERROR;

        // HAL库 FLASH_PageErase有BUFF,要加上下面三行代码
        while( status != HAL_OK )
            status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

        CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
#elif 1
	FLASH_EraseInitTypeDef FlashEraseInit;
	uint32_t SectorError=0;
				// printf("0\n");
				FlashEraseInit.TypeErase=FLASH_TYPEERASE_PAGES;       //擦除类型，扇区擦除 
				FlashEraseInit.PageAddress=StartAddress;   //要擦除的扇区
				FlashEraseInit.NbPages=1;                             //一次只擦除一个扇区
				// FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
				if(HAL_FLASHEx_Erase(&FlashEraseInit, &SectorError)!=HAL_OK) 
				{
					printf("1\n");
					// break;//发生错误了
				}
#endif

				// printf("2\n");
        // 写入数据
        FlashWriteNoCheck( StartAddress,buffer,960 );
				// printf("3\n");

        // 为下一页做准备
        // StartAddress +=  FMC_SECTOR_SIZE;
        remaindNum = 0;
        // pBuf += i;
        // Offset = 0;
		printf(" i = %u\n", i);

    } while( remaindNum > 0 );

    HAL_FLASH_Lock();  // 上锁
		
				printf("HAL_FLASH_Lock\n");
}



// 从FLASH中读指定长度数据
void FlashReadBuff(const uint32_t source_address,uint8_t *const buffer,uint16_t length)
{
    uint16_t i;
    uint8_t Offset = 0;
    uint32_t StartAddress = source_address;
    uint16_t data;

    // 地址检测
    if( source_address + length > FMC_FLASH_END ) return;

    // 如果没有对16齐
    if( source_address & 1 )
    {
        Offset = 1;
        StartAddress = source_address-1;
    }

    // flash的操作要求16对齐 最小读写操作16个比特
    if ( Offset )
    {
        data = *(uint16_t *)(StartAddress);
        buffer[0] = data >> 8;
        StartAddress += 2;
    }

    for ( i = 0; i < (length-Offset); i += 2)
    {
        data = *(uint16_t *)(StartAddress+i);
        buffer[i+Offset] = (data & 0xFF);
        if ( (i+Offset) < (length - 1) )
            buffer[i + Offset + 1] = (data >> 8);
    }

}



