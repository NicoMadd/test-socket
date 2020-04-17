/*
 * client-test.h
 *
 *  Created on: Apr 16, 2020
 *      Author: madd
 */

#ifndef CLIENT_TEST_H_
#define CLIENT_TEST_H_

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

typedef enum {
	MENSAJE = 1,
}op_code;

typedef struct{
	int size;
	void* stream;
} t_buffer;

typedef struct{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;


#endif /* CLIENT_TEST_H_ */
