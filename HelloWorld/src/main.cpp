#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

// +-----------------------------------------------------------------------+ //
// | ATmega328p Baudrate values for UBRRn for external crystal 16MHz  | //
// +-----------------------------------------------------------------------+ //

#define BAUD_RATE_4800_BPS  206 // 4800bps
#define BAUD_RATE_9600_BPS  103  // 9600bps

#define BAUD_RATE_14400_BPS  52 // 14.4k bps
#define BAUD_RATE_19200_BPS  35 // 19.2k bps  
#define BAUD_RATE_28800_BPS  23 // 28.8k bps
#define BAUD_RATE_38400_BPS  17 // 38.4k bps
#define BAUD_RATE_57600_BPS  11 // 57.6k bps
#define BAUD_RATE_76800_BPS   8 // 76.8k bps

#define BAUD_RATE_115200_BPS  5 // 115.2k bps
#define BAUD_RATE_230400_BPS  2 // 230.4k bps


void uart_init(uint16_t ubrr) {
  // Set baud rate in UBRR0
  UBRR0H = (uint8_t)(ubrr >> 8);
  UBRR0L = (uint8_t)(ubrr & 0xFF); 
  // Enable receiver and transmitter
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  //UCSR0B = (1 << TXEN0);
  // Set frame format: 8 data, 1 stop bit
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
}

// Transmitting a character
void uart_putc(unsigned char data) {
  // Wait for empty transmit buffer
  while (!(UCSR0A & (1 << UDRE0)));
  // Put data into buffer, sends the data
  UDR0 = data;
}

// Transmitting a string
void uart_puts(const char *str) {
  // Transmit each character in the string
  for (int i = 0; str[i] != '\0'; i++) {
    uart_putc(str[i]);
  }
}

// Transmitting hex value
void uart_puthex8(uint8_t value) {
  // extract the upper and lower nibbles
  uint8_t upper = (value >> 4) & 0x0F;
  uint8_t lower = value & 0x0F;

  // Convert the upper nibble to ASCII
  upper += (upper < 10) ? '0' : 'A' - 10;
  // Convert the lower nibble to ASCII
  lower += (lower < 10) ? '0' : 'A' - 10;

  // Transmit the value
  uart_putc(upper);
  uart_putc(lower);
}

void uart_puthex16(uint16_t value) {
  // Transmit the upper byte
  uart_puthex8((value >> 8) & 0xFF);
  // Transmit the lower byte
  uart_puthex8(value & 0xFF);
}

void uart_putU8(uint8_t value) {
  uint8_t digit1, digit2;

  // count value in 100s place
  while (value >= 100)
  {
    value -= 100;
    digit1++;
  }
  // count value in 10s place
  while (value >= 10)
  {
    value -= 10;
    digit2++;
  }
  // print the first digit if it is not 0
  if (digit1 != 0)  {uart_putc(digit1 + '0');}
  // print the second digit if it is not 0
  if (digit1 != 0 || digit2 != 0)  {uart_putc(digit2 + '0');}
  // print the last digit
  uart_putc(value + '0');
}

void uart_putU16(uint16_t value) {
  uint16_t digit1, digit2, digit3, digit4;

  // count value in 1000s place
  while (value >= 1000)
  {
    value -= 1000;
    digit1++;
  }
  // count value in 100s place
  while (value >= 100)
  {
    value -= 100;
    digit2++;
  }
  // count value in 10s place
  while (value >= 10)
  {
    value -= 10;
    digit3++;
  }
  // print the first digit if it is not 0
  if (digit1 != 0)  {uart_putc(digit1 + '0');}
  // print the second digit if it is not 0
  if (digit1 != 0 || digit2 != 0)  {uart_putc(digit2 + '0');}
  // print the third digit if it is not 0
  if (digit1 != 0 || digit2 != 0 || digit3 != 0)  {uart_putc(digit3 + '0');}
  // print the last digit
  uart_putc(value + '0');
}

void uart_putS8(int8_t value) {
  if (value < 0) {
    uart_putc('-');
    value = -value;
  }
  uart_putU8(value);
}

void uart_putS16(int16_t value) {
  if (value < 0) {
    uart_putc('-');
    value = -value;
  }
  uart_putU16(value);
}

char uart_getc() {
  // Wait for data to be received
  while (!(UCSR0A & (1 << RXC0)));
  // Get and return received data from buffer
  return UDR0;
}

void uart_getline(char *buf, uint8_t len) {
  uint8_t bufIdex = 0;
  char c;

  // while received character is not carriage return
  // and end of buffer has not been reached
  do {
    // get the received character
    c = uart_getc();
    // store the received character in the buffer
    buf[bufIdex++] = c;
  } while (c != '\r' && bufIdex < len);
  // add a null terminator to the string
  buf[bufIdex] = '\0';
}

int main() {
  uint16_t ubrr = BAUD_RATE_9600_BPS;
  uart_init(ubrr);
  // allocate a buffer to store the received string
  char buffer[32];
  while (1) {
    uart_puts("Enter a string: ");
    // get a line from UART
    uart_getline(buffer, 10);
    // echo the received string
    uart_puts("You entered: ");
    uart_puts(buffer);
    uart_putc('\n');
  } 
  return 0; 
}