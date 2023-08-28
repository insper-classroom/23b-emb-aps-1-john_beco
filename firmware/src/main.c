#include <asf.h>

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


// FUNCOES
void set_buzzer(Pio *p_pio, const uint32_t ul_mask){
	p_pio->PIO_SODR = ul_mask;
}

void clear_buzzer(Pio *p_pio, const uint32_t ul_mask) {
	p_pio->PIO_CODR = ul_mask;
}

int get_startstop(){
	if (pio_get(START_PIO, PIO_INPUT, START_PIO_IDX_MASK)){
		return 1;
	}
	else {
		return 0;
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
	pmc_enable_all_periph_clk(BUZZER_PIO_ID);
	pio_set_output(BUZZER_PIO, BUZZER_PIO_IDX_MASK, PIO_DEFAULT);
	
	// START
	pmc_enable_all_periph_clk(START_PIO_ID);
	pio_set_output(START_PIO, START_PIO_IDX_MASK, PIO_DEFAULT);
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

	}
}
