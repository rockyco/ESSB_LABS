#include <avr/io.h>
#include <util/delay.h>

int main() {
  DDRB |= (1 << PB0); // Set pin 14 as output
  while (1) {
    PORTB ^= (1 << PB0); // Toggle pin 14
    _delay_ms(200); // Wait for 0.5 second
  }
}
 
