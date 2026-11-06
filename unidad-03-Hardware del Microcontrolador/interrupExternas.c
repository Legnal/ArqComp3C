include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

ISR(INT0_vect) {
    _delay_ms(30);
    PORTD ^= (1 << PORTD1);
    EIFR |= (1 << INTF0);
}

int main(void) {
    UCSR0B &= ~(1 << TXEN0);
    DDRD |= (1 << DDD1);
    DDRD &= ~(1 << DDD2);
    PORTD |= (1 << PORTD2);
    // MODO 1: Nivel Bajo (Low Level)
     EICRA &= ~((1 << ISC01) | (1 << ISC00));

    // MODO 2: Cualquier Cambio Lógico (Any Logical Change)
    //EICRA |= (1 << ISC00);
    //EICRA &= ~(1 << ISC01);

    // MODO 3: Flanco de Bajada (Falling Edge) - [Al presionar]
    // EICRA = (1 << ISC01);

    // MODO 4: Flanco de Subida (Rising Edge) - [Al soltar]
    // EICRA |= (1 << ISC01) | (1 << ISC00);

    EIMSK |= (1 << INT0);
    sei();
    while (1) {
    }
}
