#include <iostream>
#include <string>
#include <cstdint>
#include <cstring>
#include <dumbParser.h>

#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netinet/ip.h>


#define ROLE_CLIENT 0
#define ROLE_SERVER 1

#define PROTOCOL_UDP 0
#define PROTOCOL_TCP 1

#define RESULT_STDOUT 0
#define RESULT_FILE 1

int s;

#define BYTES_TO_PRINT 100
#define BUFFER_SIZE_UDP 400
#define BUFFER_SIZE_TCP 0x400000

// struct sockaddr_in {
// 	sa_family_t    sin_family; /* address family: AF_INET */
// 	in_port_t      sin_port;   /* port in network byte order */
// 	struct in_addr sin_addr;   /* internet address */
// };

char* buf_recv;
char* buf_send;

void printFirstNBytes(char* buf, int n);

void printFirstNBytes(char* buf, int n){
	for(int i = 0; i < n; i++){
		char ch = buf[i];
		if(ch == '\0'){
			puts("\\0");
		} else if(ch == '\t'){
			puts("\\t");
		} else if(ch == '\n'){
			puts("\\n");
		} else if(ch == '\r'){
			puts("\\r");
		} else {
			putchar(ch);
		}
	}
	puts("\n");
}

int main(int argc, char** argv){

	int role;
	int protocol;
	uint32_t port;
	std::string input_file;
	int result;
	std::string output_file;

	dumbParser* parser = new dumbParser();
	parser->parseArgs(argc, argv);



	if(! (parser->get_udp() ^ parser->get_tcp()) ){
		std::cerr << "specify protocol udp or tcp (but not both)" << std::endl;
		exit(1);
	}
	if(parser->get_tcp()){
		protocol = PROTOCOL_TCP;
	} else {
		protocol = PROTOCOL_UDP;
	}



	if(! (parser->get_server() ^ parser->get_client()) ){
		std::cerr << "specify role client or server (but not both)" << std::endl;
		exit(1);
	}
	if(parser->get_server()){
		role = ROLE_SERVER;
	} else {
		role = ROLE_CLIENT;
	}



	if( ( (role == ROLE_SERVER) && !parser->get_local_isset() ) || ( (role == ROLE_CLIENT) && !parser->get_remote_isset() ) ){
		std::cerr << "specify port" << std::endl;
		exit(1);
	}
	if(role == ROLE_SERVER){
		port = parser->get_local();
	} else {
		port = parser->get_remote();
	}


	if(role == ROLE_CLIENT){
		if( !parser->get_data_isset() ){
			std::cerr << "specify data file for client" << std::endl;
			exit(1);
		}
		if(parser->get_result_isset() ){
			result = RESULT_FILE;
			output_file = parser->get_result();
		} else {
			result = RESULT_STDOUT;
		}
		
	}


	if(role == ROLE_CLIENT){

		std::cout << "starting client" << std::endl;

		if(protocol == PROTOCOL_TCP){
			s = socket(AF_INET, SOCK_STREAM, 0);
		} else {
			s = socket(AF_INET, SOCK_DGRAM, 0);
		}

		if(s < 0){
			perror("socket");
			exit(1);
		}

		struct sockaddr_in remote_address;
		remote_address.sin_family = AF_INET;
		remote_address.sin_port = htons(port);
		remote_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		
		if(protocol == PROTOCOL_TCP){
			if( connect(s, (struct sockaddr *)&remote_address, sizeof(remote_address)) < 0 ){
				perror("connect");
				exit(2);
			}


			char message[] = "Hello there!\n";
			char buf_recv[sizeof(message)];

			send(s, message, sizeof(message), 0);
			int bytes_read = recv(s, buf_recv, sizeof(message), 0);
			int bytes_to_print = std::min(bytes_read, BYTES_TO_PRINT);
			printf("Received %d bytes.\n", bytes_read);
			if(bytes_read > 0){	
				printf("First %d:\n", bytes_read, bytes_to_print);
				printFirstNBytes(buf_recv, bytes_to_print);
				
			}
			// close(s);
		} else {
			
		}

		return 0;

	} else {
		std::cout << "starting server" << std::endl;
		
		if(protocol == PROTOCOL_TCP){
			s = socket(AF_INET, SOCK_STREAM, 0);
		} else {
			s = socket(AF_INET, SOCK_DGRAM, 0);
		}

		if(s < 0){
			perror("socket");
			exit(1);
		}
		// struct sockaddr_in {
		// 	sa_family_t    sin_family; /* address family: AF_INET */
		// 	in_port_t      sin_port;   /* port in network byte order */
		// 	struct in_addr sin_addr;   /* internet address */
		// };

		struct sockaddr_in my_address;
		my_address.sin_family = AF_INET;
		my_address.sin_port = htons(port);
		my_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

		if( bind(s, (struct sockaddr *) &my_address, sizeof(struct sockaddr_in)) ){
			std::cerr << "failed to bind socket" << std::endl;
			exit(2);
		} else {
			std::cout << "opened socket" << std::endl;
		}


    	

		if(protocol == PROTOCOL_TCP){
			buf_recv = new char[BUFFER_SIZE_TCP];
			buf_send = new char[BUFFER_SIZE_TCP];
			
			listen(s, 1);

			while(1){
				
				int sock = accept(s, NULL, NULL);

				if(sock < 0){
					perror("accept");
					exit(3);
				}

				while(1){

					int bytes_read = recv(sock, buf_recv, BUFFER_SIZE_TCP, 0);
					int bytes_to_print = std::min(bytes_read, BYTES_TO_PRINT);
					printf("Received %d bytes.\n", bytes_read);
					if(bytes_read <= 0) break;
					printf("First %d:\n", bytes_read, bytes_to_print);
					printFirstNBytes(buf_recv, bytes_to_print);
					for(int i = 0; i < bytes_read; i++){
						buf_send[i] = buf_recv[bytes_read - i - 1];
					}
					send(sock, buf_send, bytes_read, 0);
				}

				// close(sock);
			}

		} else {
			buf_recv = new char[BUFFER_SIZE_UDP];
			buf_send = new char[BUFFER_SIZE_UDP];
			
			
			while(1){
				int bytes_read = recvfrom(s, buf_recv, BUFFER_SIZE_UDP, 0, NULL, NULL);
				if(bytes_read > 0){
					// char* buf_send = new char[bytes_read];
					for(int i = 0; i < bytes_read; i++){
						buf_send[bytes_read] = buf_recv[bytes_read - i - 1];
					}
					sendto(s, buf_send, bytes_read, 0, NULL, 0);
					// int sendto(int sockfd, const void *msg, int len, unsigned int flags, const struct sockaddr *to, int tolen);
					// int sendto(int sockfd, const void *msg, int len, unsigned int flags, const struct sockaddr *to, int tolen);
					buf_recv[bytes_read] = '\0';
					printf("%s", buf_recv);
					
				}

			}

		}

	}


	return 0;
}