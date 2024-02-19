#include <avr/io.h>
#include <util/delay.h>

int main() {
  DDRB |= (1 << PB5); // Set pin 13 as output
  while (1) {
    PORTB ^= (1 << PB5); // Toggle pin 13
    _delay_ms(200); // Wait for 0.5 second
  }
}
 
