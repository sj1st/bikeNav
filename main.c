/*----------------------------------------------------------------------*/
/* Foolproof FatFs sample project for AVR              (C)ChaN, 2014    */
/*----------------------------------------------------------------------*/

#include <avr/io.h>	/* Device specific declarations */
#include <util/delay.h>
#include "ff.h"		/* Declarations of FatFs API */
#include "uart.h"
#include "version.h"
#include "lcd.h"

FATFS FatFs;		/* FatFs work area needed for each volume */
FIL Fil;			/* File object needed for each open file */
DIR Dir;
FILINFO fno;

void ERROR(const char *str, uint32_t ecode)
{
    uart_sends(str);
    uart_sends("ECODE:");
    uart_sendc('0' + ecode);
    uart_sendc('\n');
    while(1)
    {
        _delay_ms(1000);
        uart_sendc('.');
    }
}


int main (void)
{
    lcd_init();
    lcd_clear();
    lcd_puts("Hello world!");
    lcd_setpos(0, 1);
    lcd_puts(FW_VERSION);
    
    FRESULT rc;
	
    uart_init(19200UL);
	
    uart_sends("=== GSP Logger ===\n");
    uart_sends("build: ");
    uart_sends(FW_VERSION);
    uart_sends("\n");

	if((rc = f_mount(&FatFs, "", 0)) != FR_OK)		/* Give a work area to the default drive */
	{
		ERROR("f_mount failed ", rc);	
	}
	uart_sends("f_mount [OK]\n");
    
    if((rc = f_stat("sub1",&fno)) == FR_OK)
    {
        uart_sends("sub1 already exists\n");
    }
	else if((rc = f_mkdir("sub1")) != FR_OK)
	{
		ERROR("f_mkdir failed ", rc);	
    }
	
		
	if((rc = f_open(&Fil, "sub1/newfile.txt", FA_WRITE | FA_CREATE_ALWAYS)) != FR_OK)
	{
		ERROR("f_open failed ", rc);	
	}
	
	f_printf(&Fil, "GPS Logger, fw_version=%s\n", FW_VERSION); 
	f_close(&Fil); /* Close the file */
	uart_sends("File write complete\n");
	
	while(1)
    {
        uart_sends("Program end...\n");
        _delay_ms(1000);
    }
}


