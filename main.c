#include "main.h"

TIM_HandleTypeDef timer_ayarlari ; // Timer ayarlarini yapacagimi belirttim.
GPIO_InitTypeDef  buton_ayarlari;  // Buton ayarlarini yapacagimi belirttim.
GPIO_InitTypeDef  yedi_segment_cikis_ayarlari ; // Segment'in çikis ayarlarini yapacagimi belirttim.
GPIO_InitTypeDef  led ; // kart uzerindeki ledler icin


GPIO_PinState sureyi_hesaplayan_buton; // PinState türünden degiskeni tanimladim. 
                                       // Butondan okuyacagimiz degeri bu degiskende kaydetmek için( SET/RESET)
GPIO_PinState harfi_goster				;    // PinState türünden degiskeni tanimladim.

uint8_t sayac = 0 ; //Kullanicinin butona basma süresini kaydeder.
uint8_t bilgi = 0 ; // Kullanicinin girilen bilgilerini kaydeder.
uint8_t girilen_bilgiler = 0 ; // Kullanicinin kaç bilgi girdigini kydeder. 


/*
BU KISIMDA HARFLERIN DOGRULUK TABLOSUNU OLUSTURDUM:

  g,f,a,b,e,d,c
A 1,1,1,1,1,0,1
B 1,1,1,1,1,1,1
C 0,1,1,0,1,1,0
d 1,0,0,1,1,1,1
E 1,1,1,0,1,1,0
F 1,1,1,0,1,0,0
G 0,1,1,0,1,1,1
H 1,1,0,1,1,0,1
I 0,1,0,0,1,0,0
J 0,0,0,1,1,1,1
k 1,1,1,0,1,0,1
L 0,1,0,0,1,1,0
M 0,0,1,0,1,0,1
n 0,1,1,1,1,0,1
O 0,1,1,1,1,1,1
P 1,1,1,1,1,0,0
q 1,1,1,1,0,1,0
R 0,1,1,1,1,0,0
S 1,1,1,0,0,1,1
t 1,1,0,0,1,1,0
U 0,1,0,1,1,1,1
v 0,0,0,0,1,1,1
y 1,1,0,1,0,1,1
x 1,1,0,1,1,0,1
Z 1,0,1,1,1,1,0

	YEDI_segment_ledler ; // sirasiyla g,f,a,b,e,d,c
	
	  W YOK
	
		YEDI_segment_ledler (1,1,1,1,1,0,1) ; // A
		YEDI_segment_ledler (1,1,1,1,1,1,1 ) ; //B
		YEDI_segment_ledler (0,1,1,0,1,1,0 ) ; //C
		YEDI_segment_ledler (1,0,0,1,1,1,1 ) ; //d
		YEDI_segment_ledler (1,1,1,0,1,1,0  ) ; //E
		YEDI_segment_ledler (1,1,1,0,1,0,0 ) ; //F
		YEDI_segment_ledler (0,1,1,0,1,1,1 ) ; //G
		YEDI_segment_ledler (1,1,0,1,1,0,1 ) ; //H
		YEDI_segment_ledler (0,1,0,0,1,0,0 ) ; //I
		YEDI_segment_ledler (0,0,0,1,1,1,1) ; //J
		YEDI_segment_ledler (1,1,1,0,1,0,1) ; //K
		YEDI_segment_ledler (0,1,0,0,1,1,0 ) ; //L
		YEDI_segment_ledler (0,0,1,0,1,0,1) ; //M
		YEDI_segment_ledler (0,1,1,1,1,0,1) ; //n
		YEDI_segment_ledler (0,1,1,1,1,1,1 ) ; //O
		YEDI_segment_ledler (1,1,1,1,1,0,0 ) ; //P
		YEDI_segment_ledler (1,1,1,1,0,1,0 ) ; //q
		YEDI_segment_ledler (0,1,1,1,1,0,0 ) ; //R
		YEDI_segment_ledler (1,1,1,0,0,1,1) ; //S
		YEDI_segment_ledler (1,1,0,0,1,1,0 ) ; //t
		YEDI_segment_ledler (0,1,0,1,1,1,1) ; //U
		YEDI_segment_ledler (0,0,0,0,1,1,1) ; //v
		YEDI_segment_ledler (1,1,0,1,0,1,1) ;// y
		YEDI_segment_ledler (1,1,0,1,1,0,1) ;//x
		YEDI_segment_ledler (1,0,1,1,1,1,0) ; //z

*/

// harfler 
// sirasiyla g,f,a,b,e,d,c
	
void YEDI_segment_ledler (GPIO_PinState g , GPIO_PinState f , GPIO_PinState	a , GPIO_PinState	b , GPIO_PinState	e ,GPIO_PinState d , GPIO_PinState c){

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0 , g ); //g
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1 , f ); //f
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2 , a );	//a
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3 , b );	//b
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4 , e ); //e
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5 , d ); //d
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6 , c ); //c
	
}

void buton_tanimlari (void){ // void içine tanimladigimiz fonksiyonlari kolaylikla int maine 
	                           // çagirmak için kullaniyoruz.
	__HAL_RCC_GPIOB_CLK_ENABLE();	// B portunun clock'unu aktiflestirdim.			 
	
	
	buton_ayarlari.Mode = GPIO_MODE_INPUT;	// 0-1. pini giris olarak ayarladim.
	buton_ayarlari.Pin = GPIO_PIN_0 | GPIO_PIN_1 ;			  	
	buton_ayarlari.Pull = GPIO_NOPULL;	// PULL-DOWN baglantisini breadbord üzerinde gösterdigim için koda dahil etmedim.
																	
	
	HAL_GPIO_Init(GPIOB, &buton_ayarlari);	// B portuna yukaridaki buton ayarlarini tanimladim.

}

void yedi_segment_GPIO_output_tanimlari (void){
	
	__HAL_RCC_GPIOD_CLK_ENABLE();	// D portundaki pinlerin clock'unu aktiflestirdim.						 
	
	
	yedi_segment_cikis_ayarlari.Mode = GPIO_MODE_OUTPUT_PP;	//  GPIO 0,1,2,3,4,5,6 çikis olarak tanimladim.
	yedi_segment_cikis_ayarlari.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2  | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 ;			  	
	yedi_segment_cikis_ayarlari.Pull = GPIO_NOPULL;		// çikis oldugundan pull down ya da pull up gerek olmadigindan NOPULL
	yedi_segment_cikis_ayarlari.Speed = GPIO_SPEED_FREQ_HIGH;	//Frekans hizini ayarladim.
	HAL_GPIO_Init(GPIOD, &yedi_segment_cikis_ayarlari);	// 7-segmentte yaptigim ayarlari D portuna tanimladim.
	
	
	


}

void LED_tanimlari  (void) {
	
		// kart uzerindeki ledler icin tanimlamalar
	// kartin data sheet'e gore yesil led G13  kirmizi led G14
	
	__HAL_RCC_GPIOG_CLK_ENABLE();	// G portundaki pinlerin clock'unu aktiflestirdim.		
 
	led.Mode = GPIO_MODE_OUTPUT_PP;	
	led.Pin = GPIO_PIN_13 | GPIO_PIN_14  ;			  	
	led.Pull = GPIO_NOPULL;		// çikis oldugundan pull down ya da pull up gerek olmadigindan NOPULL
	led.Speed = GPIO_SPEED_FREQ_HIGH;	//Frekans hizini ayarladim.
	HAL_GPIO_Init(GPIOG, &led);	// ledler icin yaptigim ayarlari G portuna tanimladim.
}

void timer_tanimlari (void) {  
	
		__HAL_RCC_TIM2_CLK_ENABLE();	//TIM2 kullandim, clock'unu aktiflestirdim.
	
	timer_ayarlari.Instance 						= TIM2;					 	//	Timer 2 kullaniyorum.
	timer_ayarlari.Init.Prescaler   		= 1000;						// 16.000.000 / 1000 = 16000
	timer_ayarlari.Init.CounterMode 		= TIM_COUNTERMODE_UP;
	timer_ayarlari.Init.Period 					= 8000;					// 16000/ 8000 = 2 
																											// 2 hz => 0.5s
															
	while (HAL_TIM_Base_Init(&timer_ayarlari)!= HAL_OK); // Siteden aldim.
	
HAL_TIM_Base_Start_IT(&timer_ayarlari); //picproje.org'dan aldim.

	HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0); // Siteden aldim.
	HAL_NVIC_EnableIRQ(TIM2_IRQn); 
	HAL_TIM_Base_Start(&timer_ayarlari); //TIM2'yi baslat.
	
}



int main(void)
{
  HAL_Init();
  SystemClock_Config();

	buton_tanimlari () ;
	timer_tanimlari () ;
	yedi_segment_GPIO_output_tanimlari () ; // int main'de tüm tanimlama fonksiyonlarini çagirdim.
	LED_tanimlari () ;

	while (1) {

}
	
}

void TIM2_IRQHandler (void){
	
	HAL_TIM_IRQHandler ( &timer_ayarlari );	
	
	// F = 2
	// Her 0.5 saniye burdaki kodlari okuyacak.
	
	sureyi_hesaplayan_buton = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0); // Butondan basili olup olmadigini okuyoruz.
	harfi_goster 						= HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);   
	
	
	if ( sureyi_hesaplayan_buton == GPIO_PIN_SET) {
		
													sayac ++ ;
													
													// ÇIZGI VE NOKTA GÖSTEREN LEDLERIN KODUNU YAZDIM.
															if ( sayac == 1 ) {
																
												HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13 , GPIO_PIN_SET );

															}
															
															else if ( sayac == 2) {
																	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13 , GPIO_PIN_RESET );
																	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14 , GPIO_PIN_SET );

															}
															
															// Buraya kadar.
	}
	
	else if ( sayac == 1 && sureyi_hesaplayan_buton == GPIO_PIN_RESET ) {//kullanici butona basti ve 0.5 sn sonra birakti.
		                                                                   // sayacin degeri 1 yani nokta
		
															HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13 | GPIO_PIN_14 , GPIO_PIN_RESET );
		
															girilen_bilgiler++;
															bilgi =	bilgi << 1 ;
															sayac = 0 ;
	}
	
	else 	if ( sayac > 1 && sureyi_hesaplayan_buton == GPIO_PIN_RESET ) {// kullanici butona basti ve 1'den büyük
		                                                                   // yani çizgi
														HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13| GPIO_PIN_14 , GPIO_PIN_RESET );
														girilen_bilgiler++;
		
														bilgi = bilgi +1 ;
														bilgi =	bilgi << 1 ;
														sayac = 0 ;
	}
	
	
	
		if ( harfi_goster == GPIO_PIN_SET) {
		
							if ( girilen_bilgiler == 1 ) {
								
												// E T
												switch (bilgi) {
												
												case 0x00 : //	 0 				0
													YEDI_segment_ledler (1,1,1,0,1,1,0  ) ; //E
												break ;

												case 0x02 : //	 1 				0
															YEDI_segment_ledler (1,1,0,0,1,1,0 ) ; //t
												break ;
												
												}
								}	
							
							
							else if ( girilen_bilgiler == 2 ) {

								
												// A I M  N 			
												switch (bilgi) {
												
												case 0x02 : //	 01 				0
													YEDI_segment_ledler 	(1,1,1,1,1,0,1) ; //A
												break ;
												
												case 0x00 : //	 00 				0
													YEDI_segment_ledler (0,1,0,0,1,0,0 ) ; //I
												break ;
												
												case 0x06 : //	 11				0
													YEDI_segment_ledler (0,0,1,0,1,0,1) ; //M
												break ;
												
												case 0x04 : //	 10				0
													YEDI_segment_ledler (0,1,1,1,1,0,1) ; //n
												break ;
												
												}								
								}
							
								
								else if ( girilen_bilgiler == 3 ) {

								
												// D G K O R S U 		
												switch (bilgi) {
												
												case 0x08 : //	 100 				0
													YEDI_segment_ledler (1,0,0,1,1,1,1 ) ; //d
												break ;
												
												case 0x0C : //	 110 				0
													YEDI_segment_ledler (0,1,1,0,1,1,1 ) ; //G
												break ;
												
												case 0x0A : //	 101			0
													YEDI_segment_ledler (1,1,1,0,1,0,1) ; //K
												break ;
												
												case 0x0E : //	 111			0
													YEDI_segment_ledler (0,1,1,1,1,1,1 ) ; //O
												break ;
												
												case 0x04 : //	 010				0
													YEDI_segment_ledler (0,1,1,1,1,0,0 ) ; //R
												break ;
												
												case 0x00 : //	 000				0
													YEDI_segment_ledler (1,1,1,0,0,1,1) ; //S
												break ;
												
												case 0x02 : //	 001				0
													YEDI_segment_ledler (0,1,0,1,1,1,1) ; //U
												break ;
												
												}								
								}
								
									else if ( girilen_bilgiler == 4 ) {

												//  B C F H J L P Q V X Y Z		
												switch (bilgi) {
												// 
												case 0x10 : //	 1000 		0
															YEDI_segment_ledler (1,1,1,1,1,1,1 ) ; //B
												break ;
												
												case 0x14 : //	 1010 		0
															YEDI_segment_ledler (0,1,1,0,1,1,0 ) ; //C
												break ;
												
												case 0x04 : //	 0010			0
															YEDI_segment_ledler (1,1,1,0,1,0,0 ) ; //F
												break ;
												
												case 0x00 : //	 0000			0
															YEDI_segment_ledler (1,1,0,1,1,0,1 ) ; //H
												break ;
												
												case 0x0E : //	 0111			0
															YEDI_segment_ledler (0,0,0,1,1,1,1) ; //J
												break ;
												
												case 0x08 : //	 0100			0
															YEDI_segment_ledler (0,1,0,0,1,1,0 ) ; //L
												break ;
												
												case 0x0C : //	 0110			0
															YEDI_segment_ledler (1,1,1,1,1,0,0 ) ; //P
												break ;
												
												case 0x1A : //	 1101 		0
															YEDI_segment_ledler (1,1,1,1,0,1,0 ) ; //q
												break ;
												
												case 0x02 : //	 0001 		0
															YEDI_segment_ledler (0,0,0,0,1,1,1) ; //v
												break ;
												
												case 0x12 : //	 1001			0
															YEDI_segment_ledler (1,1,0,1,1,0,1) ;//x
												break ;
												
												case 0x16 : //	 1011			0
															YEDI_segment_ledler (1,1,0,1,0,1,1) ;// y
												break ;
												
												case 0x18 : //	 1100			0
															YEDI_segment_ledler (1,0,1,1,1,1,0) ; //z
												break ;
												
												}								
								}
			
				bilgi = 0 ;
				girilen_bilgiler = 0 ;
	}
	}

