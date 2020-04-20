/*
 ============================================================================
 Name        : utils.c
 Author      : Madd
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

//CHEQUEAR DONDE SE CIERRA EL SOCKET_CLIENTE

#include "Servidor.h"

#define IP "127.0.0.1"
#define PUERTO "25445"

/* recibir_cliente
 * socket_servidor = socket del cual se esperara la solicitud de conexion
 */

void recibir_cliente(int socket_servidor){
	while(1){
		esperar_cliente(socket_servidor);


	}
}

/* deserializar_buffer
 * codigo_operacion = codigo sobre el cual se decidira que accion tomar
 * buffer = donde esta contenida la informacion
 */

void deserializar_buffer(int codigo_operacion, t_buffer* buffer){
	void* mensaje = malloc(buffer->size);
	switch(codigo_operacion){
		case MENSAJE:
			puts("Entra a MENSAJE");
			memcpy(mensaje,buffer->stream, buffer->size);
			puts((char *)mensaje);
			break;
		default:
			puts("default");
			break;
	}

}

/* recibir_cliente
 * socket_cliente = socket de la cual se recibiran datos
 */

void recibir_mensaje(int *socket_cliente){

	int codigo_operacion;

	if(recv(*socket_cliente, &(codigo_operacion),sizeof(uint32_t), MSG_WAITALL)==-1){
		perror("Falla recv() op_code");
	}

	int size;

	if(recv(*socket_cliente, &(size), sizeof(uint32_t), MSG_WAITALL) == -1){
		perror("Falla recv() buffer->size");
	}

	char* stream = malloc(size);

	if(recv(*socket_cliente, stream, size, MSG_WAITALL) == -1){
		perror("Falla recv() buffer->stream");
	}

	t_buffer* buffer= malloc(sizeof(t_buffer));
	buffer->size=size;
	buffer->stream=stream;
    deserializar_buffer(codigo_operacion,buffer);

}

/* esperar_cliente
 * socket_servidor = socket en la cual se aceptaran comunicaciones
 */

void esperar_cliente(int socket_servidor){
	struct sockaddr_in dir_cliente;
	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	pthread_create(&pthread, NULL, (void*)recibir_mensaje, &socket_cliente);
	pthread_detach(pthread);

}

/* listen_to
 * ip = ip del socket a conectarse
 * puerto = puerto del socket a conectarse
 */

int listen_to(char* ip,char* puerto){

	int socket_servidor;
	struct addrinfo hints, *servinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &servinfo);


	if ((socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1){
		perror("No se pudo crear socket");
		return -1;
	}

	if (bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
		perror("No se pudo bindear el socket");
		close(socket_servidor);
		return -1;
	}
	puts("Empieza listening");
	if(listen(socket_servidor, 10)==-1){
		perror("No se pudo poner en listen al socket");
		close(socket_servidor);
		return -1;
	}
	freeaddrinfo(servinfo);

	return socket_servidor;
}




int main(void) {

	int socket_de_escucha = listen_to(IP, PUERTO);
	recibir_cliente(socket_de_escucha );

}

