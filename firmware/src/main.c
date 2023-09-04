#include <asf.h>

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
#define START_PIO_ID			ID_PIOC
#define START_PIO_IDX			28
#define START_PIO_IDX_MASK		(1 << START_PIO_IDX)

// Selecao
#define SELECAO_PIO				PIOC
#define SELECAO_PIO_ID			ID_PIOC
#define SELECAO_PIO_IDX			31
#define SELECAO_PIO_IDX_MASK	(1 << SELECAO_PIO_IDX)

// Variaveis Globais


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
	t = ((int)1000000)/((int)(2*freq));
	
	for (int i=0; i<duration; i++){
		set_buzzer();
		delay_us(t);
		clear_buzzer();
		delay_us(t);
	}
}


void tempo_nota(){
	int tempo = 200;
	// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
	// there are two values per note (pitch and duration), so for each note there are four bytes
	int notes = sizeof(melody) / sizeof(melody[0]) / 2;

	// this calculates the duration of a whole note in ms
	int wholenote = (60000 * 4) / tempo;

	int divider = 0, noteDuration = 0;


	// iterate over the notes of the melody.
	// Remember, the array is twice the number of notes (notes + durations)
	for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

		// calculates the duration of each note
		divider = melody[thisNote + 1];
		if (divider > 0) {
			// regular note, just proceed
			noteDuration = (wholenote) / divider;
			} else if (divider < 0) {
			// dotted notes are represented with negative durations!!
			noteDuration = (wholenote) / abs(divider);
			noteDuration *= 1.5; // increases the duration in half for dotted notes
		}

		// we only play the note for 90% of the duration, leaving 10% as a pause
		//tone(buzzer, melody[thisNote], noteDuration * 0.9);
		tone(melody[thisNote],noteDuration * 0.5);

		// Wait for the specief duration before playing the next note.
		delay_ms(noteDuration);

		// stop the waveform generation before the next note.
		//noTone(buzzer);
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
	
}


// MAIN
int main (void)
{
	init();
	
	
  // Init OLED
	gfx_mono_ssd1306_init();
  
  // Escreve na tela um circulo e um texto
	gfx_mono_draw_filled_circle(20, 16, 16, GFX_PIXEL_SET, GFX_WHOLE);
  gfx_mono_draw_string("mundo", 50,16, &sysfont);

  /* Insert application code here, after the board has been initialized. */
	while(1) {
		// TESTE
		tempo_nota();
	}
}
