/*
 * server-test.h
 *
 *  Created on: Apr 16, 2020
 *      Author: madd
 */

#ifndef SERVER_TEST_H_
#define SERVER_TEST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>

typedef enum
{
	MENSAJE = 1,
}op_code;

typedef struct{
	uint32_t size;
	void* stream;
} t_buffer;

typedef struct{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;



pthread_t pthread;



#endif /* SERVER_TEST_H_ */
