#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

const uint16_t VELOCIDADES[8] PROGMEM = {
    1000,
     800,
     600,
     400,
     200,
     100,
      50,
      20
};

void delay_ms_variable(uint16_t ms) {
    while (ms > 0) {
        _delay_ms(1);
        ms--;
    }
}

int main(void) {
    DDRC &= ~((1 << DDC0) | (1 << DDC1) | (1 << DDC2));
    PORTC |= (1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2);
    DDRB |= (1 << DDB0);

    uint8_t indice_velocidad = 0;
    uint16_t retardo_ms = 0;

    while (1) {
        indice_velocidad = (~PINC) & 0x07; 
        retardo_ms = pgm_read_word(&(VELOCIDADES[indice_velocidad]));

        PORTB |= (1 << PORTB0);
        delay_ms_variable(retardo_ms);

        PORTB &= ~(1 << PORTB0);
        delay_ms_variable(retardo_ms);
    }

    return 0;
}