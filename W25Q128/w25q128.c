/**********************
*       INCLUDE        
**********************/
#include "w25q128.h"
/**********************
*       DEFINE
**********************/
/*
这些宏是为了方便移植
*/
#define spi_start() SYS_spi_start()
#define spi_end()   SYS_spi_end()
#define spi_swap(data) SYS_spi_swap_byte(data) 

#define W25Q128_READ_DID 0x90
#define W25Q128_READ_DATA 0x03
#define W25Q128_WRITE_ENABLE 0x06
#define W25Q128_PAGE_PROGRAM 0x02
#define W25Q128_ERASE_SECTOR 0x20
#define W25Q128_READ_STATUS_REG 0x05
/**********************
*   GLOBAL VARIABLE
**********************/
static uint8_t w25q128_sector_buf[4096];
/**********************
*     FUNCTIONS
**********************/

static void BSP_w25q128_write_enable(void)
{
    spi_start();
    spi_swap(W25Q128_WRITE_ENABLE);
    spi_end();
}
static uint8_t BSP_w25q128_read_status_reg_1(void)
{
    uint8_t status_reg_value;
    spi_start();
    spi_swap(W25Q128_READ_STATUS_REG);
    status_reg_value = spi_swap(0XFF);
    //若需要还可读取状态寄存器2
    spi_end();
    return status_reg_value;

}
static void BSP_w25q128_wait_busy(void)
{
    uint8_t is_busy = BSP_w25q128_read_status_reg_1();
    while (is_busy & 0x01)
    {
        is_busy = BSP_w25q128_read_status_reg_1();
    }
}
uint16_t BSP_w25q128_read_device_ID(void)
{
    uint16_t DID;
    spi_start();
    spi_swap(W25Q128_READ_DID);
    spi_swap(0);
    spi_swap(0);
    spi_swap(0);
    DID = spi_swap(0xFF);
    DID <<= 8;
    DID |= spi_swap(0xFF);
    spi_end();
    return DID;
}
void BSP_w25q128_read_data(uint8_t *buf,uint32_t addr,size_t readlength)
{
    if(readlength == 0){
        return;
    }
    spi_start();
    spi_swap(W25Q128_READ_DATA);
    spi_swap(addr>>16);
    spi_swap(addr>>8);
    spi_swap(addr);
    for (int i = 0; i < readlength; i++)
    {
        buf[i] = spi_swap(0XFF);
    }
    spi_end();
}
static void BSP_w25q128_erase_sector(uint32_t addr)
{
    BSP_w25q128_write_enable();

    uint32_t sector_num = addr/4096;
    uint32_t sector_pos = sector_num * 4096;
    spi_start();
    spi_swap(W25Q128_ERASE_SECTOR);
    spi_swap(sector_pos>>16);
    spi_swap(sector_pos>>8);
    spi_swap(sector_pos);
    spi_end();

    BSP_w25q128_wait_busy();
}
static void BSP_w25q128_write_page(uint8_t *buf,uint32_t addr,size_t writelength)
{
    BSP_w25q128_write_enable();

    spi_start();
    spi_swap(W25Q128_PAGE_PROGRAM);
    spi_swap(addr>>16);
    spi_swap(addr>>8);
    spi_swap(addr);
    for (int i = 0; i < writelength; i++)
    {
        spi_swap(buf[i]);
    }
    spi_end();

    BSP_w25q128_wait_busy();
}
static void BSP_w25q128_write_no_erase(uint8_t *buf,uint32_t addr,size_t length)
{
    uint8_t pageremain;
    pageremain = 256 - addr%256;
    if(length <= pageremain){
        pageremain = length;
    }
    while (1)
    {
        BSP_w25q128_write_page(buf,addr,pageremain);
        if(pageremain == length){
            break;
        }
        else{
            buf += pageremain;
            addr += pageremain;
            length -= pageremain;
            if(length <= 256){
                pageremain = length;
            }
            else
                pageremain = 256;
        }
    }
}

void BSP_w25q128_write_data(uint8_t *buf,uint32_t addr,size_t length)
{
    int i;
    uint32_t sector_num = addr/4096;
    uint32_t sector_offops = addr%4096;
    uint32_t sector_remain = 4096-sector_offops;
    if(sector_remain >= length){
        sector_remain = length;
    }
    while (1)
    {
        BSP_w25q128_read_data(w25q128_sector_buf,addr,sector_remain);
        for (i = 0; i < sector_remain; i++)
        {
            if(w25q128_sector_buf[i]!=0xff){
                break;
            }
        }
        if(i != sector_remain){
            BSP_w25q128_erase_sector(sector_num * 4096);
        }
        BSP_w25q128_write_no_erase(buf,addr,sector_remain);

        if(sector_remain == length){
            break;
        }
        else{
            buf += sector_remain;
            addr += sector_remain;
            length -= sector_remain;
            sector_num += 1;

            if(length <= 4096){
                sector_remain = length;
            }
            else
                sector_remain = 4096;
        }
    }
}