/*
	PIC18F27Q43でCLCとDMAを使ったプログラム。
	かなり拙い内容ですが一応動作させることができました。
	参考にした下記のサイト様に感謝です。
	
	Program using CLC and DMA with PIC18F27Q43. 
	It is quite poor, but I was able to make it work. 
	I would like to thank the following site for the reference.
	
	https://www.youtube.com/watch?v=hHLMnJs87VE
	https://github.com/sponkytoo/Moving-Colors
	https://www.microchip.com/forums/m1155783.aspx

*/

#include "mcc_generated_files/mcc.h"
#include "HardwareProfile.h"



//From left to right: R, G, B, end string.
uint8_t test_array[8][4] = {
    {255,  0,  0,  1},
    {  0,255,  0,  1},
    {  0,  0,255,  1},
    {255,255,  0,  1},
    {255,  0,255,  1},
    {  0,255,255,  1},
    {127,127,127,  1},
    {  0,  0,  0,  0}   
};


//Microchipのソースコードを少し細工しただけです。
//DMAで読ませる配列に入れるだけのプログラムです
//Just a little work on the Microchip source code.
//It's just a program to put in an array to be read by DMA.
void myLEDArray (uint8_t r_in,uint8_t g_in,uint8_t b_in , uint8_t num){   
    DMA_SrcVar[num*3    ]=g_in;
    DMA_SrcVar[num*3 +1 ]=r_in;
    DMA_SrcVar[num*3 +2 ]=b_in;
}



void main(void){
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t i;
    uint8_t x;
    uint8_t y;
    uint8_t z;
    uint8_t end_flag=1;
    uint8_t q;
    uint8_t p;
    uint8_t up_counter=1;
    uint8_t loop_count=0;
    
    
    SYSTEM_Initialize();
    
    //DMAの詳しい設定はあまりわかっていません
    //I don't know much about the detailed DMA settings.
    DMA1_SetSourceSize(RGB_LED_MAX);
    DMA1_SetDMAPriority(0);
    
    
    
    
    
    
    //タイマー割り込みを入れるときは下記の関数で指定します。
	//XC8のMCCで割り込みハンドラっていうのをつい最近まで理解していませんでした。
	//The following function is used to specify when to insert timer interrupts. 
	//I didn't understand what an interrupt handler was until recently.
	
	//TMR1_SetInterruptHandler(Timer1_Interrupt);
    
    
    
    
    
    INTERRUPT_GlobalInterruptEnable();
    //INTERRUPT_GlobalInterruptDisable();

    for(i=0;i<60;i++)myLEDArray(0,0,0,i);
    __delay_ms(10);

    q=0;
    p=0;
    x = 255; 
    y = 0; 
    z = 0;

    
    
    while (1){
        
        for (i=0;i<p;i++)myLEDArray(x,y,z,p);
        DMA1_StartTransferWithTrigger();
        
        
        q++;
        p++;


        if (q==60){
            q=0;
            p=0;
            up_counter++;
            end_flag=test_array[up_counter][3];
            if(!end_flag)up_counter=0;
            x=test_array[up_counter][0];
            y=test_array[up_counter][1];
            z=test_array[up_counter][2];
        }
        
        //Heartbeat LED
        IO_TEST_LED_LAT = loop_count >= 5 ? 1 : 0;
        if(++loop_count>10)loop_count=0;
        
        
        __delay_ms(50);
    }
}

/*

//	タイマー割り込みを入れるときは下記の関数で指定します。
//	メインルーチンで指定した割り込みハンドラ設定で指定した関数を読ませるらしいです。
//	The following function is used to specify when to insert timer interrupts. 
//	It is said to read the function specified in the interrupt handler setting specified in the main routine

void Timer1_Interrupt(void){
    mills++;					//Of course, to use this, you must specify it in a global variable (uint32_t)
}

*/
