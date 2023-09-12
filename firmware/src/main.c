#include <asf.h>
#include <string.h>

#include "musica.h"

#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"

// DEFINES
// Buzzer
#define BUZZER_PIO				PIOC
#define BUZZER_PIO_ID			ID_PIOC
#define BUZZER_PIO_IDX			19
#define BUZZER_PIO_IDX_MASK		(1 << BUZZER_PIO_IDX)

// Button
#define START_PIO				PIOD
#define START_PIO_ID			ID_PIOD
#define START_PIO_IDX			28
#define START_PIO_IDX_MASK		(1 << START_PIO_IDX)

#define BUT3_PIO PIOA
#define BUT3_PIO_ID ID_PIOA
#define BUT3_PIO_IDX 19
#define BUT3_PIO_IDX_MASK (1u << BUT3_PIO_IDX)

// Selecao
#define SELECAO_PIO				PIOC
#define SELECAO_PIO_ID			ID_PIOC
#define SELECAO_PIO_IDX			31
#define SELECAO_PIO_IDX_MASK	((UINT32_C)(1 << SELECAO_PIO_IDX))

// GLOBALS
volatile char butflag1 = 0;
volatile char butflag2 = 0;
volatile char butflag3 = 0;



// FUNCOES
void set_buzzer(){
	pio_set(BUZZER_PIO, BUZZER_PIO_IDX_MASK);
}

void clear_buzzer() {
	pio_clear(BUZZER_PIO, BUZZER_PIO_IDX_MASK);
}

int get_startstop(){
	if (pio_get(START_PIO, PIO_INPUT, START_PIO_IDX_MASK)){
		return 0;
	}
	else {
		return 1;
	}
}

int get_selecao(){
	if (pio_get(SELECAO_PIO, PIO_INPUT, SELECAO_PIO_IDX_MASK)){
		return 0;
	}
	else {
		return 1;
	}
}

void buzzer_test(int freq){
	int t = 0;
	t = 1000000/(2*freq);
	
	for (int i=0; i<1000000; i++){
		set_buzzer();
		delay_us(t);
		clear_buzzer();
		delay_us(t);
	}
}

void tone(int freq, int duration){
	int t = 0;
	if(freq == 0){
		delay_ms(duration);
	}
	else{
	t = ((int)1000000)/((int)(2*freq));
	}
	
	for (int i=0; i<duration; i++){
		set_buzzer();
		delay_us(t);
		clear_buzzer();
		delay_us(t);
	}
}

void but_1(){
	butflag1 = 1;
}

void but_2(){
	butflag2 = 1;
}

void but_3(){
	butflag3 = 1;
}


void musica(int escolha) {
	int *melodia = NULL; // Initialize the pointer
	int notes = 0;
	int tempo = 200;
	
	char buffer[32];
	for (int i =0; i < 32; i++)
	{
		buffer[i] = ' ';
	}
	buffer[31] = '\0'; 
	gfx_mono_draw_string(buffer, 0, 0, &sysfont);

	// Select the melody based on escolha
	if (escolha == 0) {
		tempo = 156;
		melodia = melody;
		notes = sizeof(melody) / sizeof(melody[0]) / 2;
		strcpy(buffer, "Super Mario");
	} else if (escolha == 1) {
		tempo = 314;
		melodia = melody_harrypotter;
		notes = sizeof(melody_harrypotter) / sizeof(melody_harrypotter[0]) / 2;
		strcpy(buffer, "Harry Potter");
	} else if (escolha == 2) {
		melodia = melody_starwars;
		notes = sizeof(melody_starwars) / sizeof(melody_starwars[0]) / 2;
		strcpy(buffer, "Star Wars");
	} else if (escolha == 3){
		tempo = 175;
		melodia = melody_nggyu;
		notes = sizeof(melody_nggyu) / sizeof(melody_nggyu[0]) / 2;
		strcpy(buffer, "Never...");
	}
	else{
		// Handle an invalid escolha value here (e.g., print an error message)
		strcpy(buffer, "");
		return;
	}

	gfx_mono_draw_string(buffer, 0, 0, &sysfont);
	int wholenote = (60000 * 4) / tempo;

	int noteDuration = 0;
	
	for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
		if(butflag3 == 1){
			int apertou = 1;
			butflag3 = 0;
			while(apertou == 1){
				if(butflag3 == 1){
					butflag3 = 0;
					apertou = 0;
				}
			}
		}

		if(butflag2 == 1){
			if(escolha == 3){
				escolha = 0;
			}
			else{
				escolha++;
			}

			butflag2 = 0;

			musica(escolha);
			return;
		}

		int divider = melodia[thisNote + 1];		

		if (divider > 0) {
			noteDuration = (wholenote) / divider;
			} else if (divider < 0) {
			noteDuration = (wholenote) / abs(divider);
			noteDuration *= 1.5;
		}

		tone(melodia[thisNote],noteDuration * 0.9);
		delay_ms(noteDuration*0.1);
	}
}



// INIT
void init(){
	board_init();
	sysclk_init();
	delay_init();
	
	// Desativa WatchDog Timer
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	// Buzzer
	pmc_enable_periph_clk(BUZZER_PIO_ID);
	pio_set_output(BUZZER_PIO, BUZZER_PIO_IDX_MASK, 0,0,0);
	
	// START
	pmc_enable_periph_clk(START_PIO_ID);
	pio_set_input(START_PIO, START_PIO_IDX_MASK, PIO_PULLUP);
	
	// Selecao
	pmc_enable_periph_clk(SELECAO_PIO_ID);
	pio_set_input(SELECAO_PIO, SELECAO_PIO_IDX_MASK, PIO_PULLUP);

	pio_handler_set(
	START_PIO,
	START_PIO_ID,
	START_PIO_IDX_MASK,
	PIO_IT_RISE_EDGE,
	but_1);

	pio_handler_set(
	SELECAO_PIO,
	SELECAO_PIO_ID,
	SELECAO_PIO_IDX_MASK,
	PIO_IT_EDGE,
	but_2);

	pio_handler_set(
	BUT3_PIO,
	BUT3_PIO_ID,
	BUT3_PIO_IDX_MASK,
	PIO_IT_RISE_EDGE,
	but_3);


	// Ativa interrupção e limpa primeira IRQ gerada na ativacao
	pio_enable_interrupt(START_PIO, START_PIO_IDX_MASK);
	pio_enable_interrupt(SELECAO_PIO, SELECAO_PIO_IDX_MASK);
	pio_enable_interrupt(BUT3_PIO, BUT3_PIO_IDX_MASK);

	pio_get_interrupt_status(START_PIO);
	pio_get_interrupt_status(SELECAO_PIO);
	pio_get_interrupt_status(BUT3_PIO);
	
	// Configura NVIC para receber interrupcoes do PIO do botao
	// com prioridade 4 (quanto mais próximo de 0 maior)
	NVIC_EnableIRQ(START_PIO_ID);
	NVIC_EnableIRQ(SELECAO_PIO_ID);
	NVIC_EnableIRQ(BUT3_PIO_ID);
	
	NVIC_SetPriority(START_PIO_ID, 4);
	NVIC_SetPriority(SELECAO_PIO_ID, 4);
	NVIC_SetPriority(BUT3_PIO_ID, 4); // Prioridade 4
	
}


// MAIN
int main (void)
{
	init();

	// Init OLED
	gfx_mono_ssd1306_init();
	// gfx_mono_draw_string("Aperte o botao", 0, 0, &sysfont);
	// Escreve na tela um circulo e um texto
	//gfx_mono_draw_filled_circle(20, 16, 16, GFX_PIXEL_SET, GFX_WHOLE);
	

	/* Insert application code here, after the board has been initialized. */
	while(1) {
		 		
		if(butflag1){
			musica(2);
			//toca = !toca;
		}
	}
}
