#ifndef _UART_H_
#define _UART_H_

// Open uart device
int uart_open(char *path, int oflags);

// Close uart device
void uart_close(int fd);

#endif
