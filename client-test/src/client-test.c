/*
 ============================================================================
 Name        : client-test.c
 Author      : Madd
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include "client-test.h"

#define IP "127.0.0.1"
#define PUERTO "25445"

/* connect_to
 * ip = ip a conectarse
 * puerto = puerto del socket a conectarse
 * wait_time = tiempo de espera entre reintentos de conexion (en segundos), en caso de fallo
 *
 *
 */

void* serializar_paquete(t_paquete* paquete, int tam_paquete){
	void* stream = malloc(tam_paquete);
	int offset = 0;

	memcpy(stream + offset, &(paquete->codigo_operacion), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &(paquete->buffer->size), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &(paquete->buffer->stream), paquete->buffer->size);

	return stream;
}

void* serializar_buffer(t_buffer* buffer){
	void* stream = malloc(buffer->size + 2*sizeof(uint32_t));
	int offset = 0;
	printf("%d", MENSAJE);
	op_code codigo_operacion = MENSAJE;

	memcpy(stream, &(codigo_operacion), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &(buffer->size), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &(buffer->stream), buffer->size);

	return stream;
}

void deserializar_buffer(int codigo_operacion, t_buffer* buffer){
	char* mensaje;
	puts("llega al switch");
	printf(" cod_op:%d\n",codigo_operacion);
	switch(codigo_operacion){
		case MENSAJE:
			puts("entra a MENSAJE");
			memcpy(&mensaje,&(buffer->stream), buffer->size);
			puts(mensaje);
			break;
		default:
			puts("default");
			break;
	}

}


void enviar_mensaje(int socket_cliente, char* mensaje){
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = strlen(mensaje) + 1;
	void* stream = malloc(buffer->size);
	memcpy(stream, mensaje, buffer->size);
	buffer->stream = stream;
	t_paquete* paquete = malloc(sizeof(t_paquete));

	void* data = serializar_buffer(buffer);
	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = buffer;

	int tam_paquete = paquete->buffer->size + 2*sizeof(uint32_t);
	void* data_a_enviar = serializar_paquete(paquete,tam_paquete);

	int bytes_enviados = send(socket_cliente, data_a_enviar, tam_paquete, 0);
	printf("bytes enviados: %d", bytes_enviados);

	int op_code;
	memcpy(&op_code,data_a_enviar,sizeof(uint32_t));
	memcp

}

int connect_to(char* ip, char* puerto,int wait_time){
	struct addrinfo hints;
	struct addrinfo* server_info;

	int socket_cliente;

	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags=AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	if((socket_cliente=socket(server_info->ai_family,server_info->ai_socktype,server_info->ai_protocol)) == -1){
		perror("No se pudo crear socket");
		return -1;
	}
	// INTENTA CONEXION INDEFINIDAMENTE
	while(connect(socket_cliente,server_info->ai_addr,server_info->ai_addrlen)== -1){
		perror("Fallo Conexion, reintentando...");
		usleep(wait_time*1000000);
	}

	printf("socket_servidor: %d", socket_cliente);

	freeaddrinfo(server_info);

	return socket_cliente;
}

int main(void) {
	int socket_servidor;

	socket_servidor = connect_to(IP, PUERTO,2);
	enviar_mensaje(socket_servidor, "Feliz cumple Fabian!");
	puts("Conectado con exito!");
	return EXIT_SUCCESS;


}
