#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/delay.h>

typedef struct{
    uint8_t salida_portd; //Semaforos
    uint8_t salida_portb; //Peatones
    uint8_t es_aten; //0=Salida fija | 1=Parpadea verde 1 | 2=Parpadea verde 2
    uint8_t sig[2];
} Estado_t;

volatile uint8_t solicitud = 0;

//Tablas de estados
const Estado_t FSM_TABLA[] PROGMEM = {
    {0b10010000, 0b00000000, 0, {1,1}}, //S0
    {0b00010000, 0b00000000, 1, {2,2}}, //S1
    {0b01100000, 0b00000000, 0, {3,3}}, //S2
    {0b01000000, 0b00000000, 2, {4,4}}, //S3
    {0b01010000, 0b00000000, 0, {0,5}}, //S4
    {0b01010000, 0b00000010, 0, {0,0}} //S5
};

#define NUM_ESTADOS (sizeof(FSM_TABLA) / sizeof(Estado_t))

//Velocidad
uint16_t obt_tiemp(void){
    uint8_t leer = ~PINC;
    uint8_t modo = (leer >> PC1) & 0x03;
    if(modo == 0b01){ 
        return 500; //Hora pico
     }else if( modo == 0b10){ 
        return 2000; //Hora baja
      }else{ 
       return 1000; //Hora normal
      } 
}

//Peaton S/N
void monitorear(void){
    if(!(PINC & (1 << PC0))){
        solicitud = 1;
    }
    _delay_ms(10);
}

void ejecutar(uint8_t out_d, uint8_t out_b, uint8_t es_aten){
    uint16_t tiempo = obt_tiemp();
    uint16_t paso = 0;
    if(es_aten == 0){
        PORTD = (PORTD & 0x0F) | out_d;
        if(out_b & (1 << PB1)){
            PORTB |= (1 << PB1);
        }else{
            PORTB &= ~(1 << PB1);
        }
        while(paso<tiempo){
            monitorear();
            paso += 10;
        }
    }else{
        uint8_t pin_ver = (es_aten == 1) ? PD7 : PD5;
        uint16_t parpadeo = 250;
        while(paso<tiempo){
            if((paso / (parpadeo / 2)) %2 == 0){
                PORTD = (PORTD & 0x0F) | out_d | (1 << pin_ver);
            }else{
                PORTD = (PORTD & 0x0F) | out_d;
            }
            if(out_b & (1 << PB1)){
                PORTB |= (1 << PB1);
            }else{
                PORTB &= ~(1 << PB1);
            }
            monitorear();
            paso += 10;
        }
    }
}

int main(void){
    DDRC &= ~((1 << PC0) | (1 << PC1) | (1 << PC2)); //Entradas
    DDRD |= (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7); //Salidas semaforos
    DDRB |= (1 << PB1); //Salida peaton
    PORTC |= (1 << PC0) | (1 << PC1) | (1 << PC2);
    uint8_t est_act = 0; //S0
    while(1){
        uint8_t out_d = pgm_read_byte(&(FSM_TABLA[est_act].salida_portd));
        uint8_t out_b = pgm_read_byte(&(FSM_TABLA[est_act].salida_portb));
        uint8_t es_aten = pgm_read_byte(&(FSM_TABLA[est_act].es_aten));
        ejecutar(out_d, out_b, es_aten);
        uint8_t sigui = pgm_read_byte(&(FSM_TABLA[est_act].sig[solicitud]));
        if(sigui < NUM_ESTADOS){
            est_act = sigui;
        }else{
            est_act = 0;
        }
        if(est_act == 0 || est_act == 5){
            solicitud = 0;
        }
    }
    return 0;
}
