#include <iostream>
#include <string>
#include <cstdint>
#include <cstring>

#include <signal.h>

#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netinet/ip.h>

#include <dumbParser.h>

// #define DEBUG


#define ROLE_CLIENT 0
#define ROLE_SERVER 1

#define PROTOCOL_UDP 0
#define PROTOCOL_TCP 1

#define RESULT_STDOUT 0
#define RESULT_FILE 1


#define BYTES_TO_PRINT_LIMIT 100
#define BUFFER_SIZE_UDP 400
#define BUFFER_SIZE_TCP 0x400000


// struct sockaddr_in {
// 	sa_family_t    sin_family; /* address family: AF_INET */
// 	in_port_t      sin_port;   /* port in network byte order */
// 	struct in_addr sin_addr;   /* internet address */
// };

int s = -1;			// socket
int sock = -1;		// socket for TCP server only
FILE* f_input = NULL;
FILE* f_output = NULL;

char* buf_recv;	// receive buffer
char* buf_send;	// send buffer

void printFirstNBytes(char* buf, int n);
void allocateBuffers(int protocol);
void openSocket(int protocol);
sockaddr_in generateAddressStructure(uint32_t port);
void inverseRecvToSend(int bytes_read);
void printRecvToConsole(int bytes_read);
void printSendToConsole(int size);
void informOfReceiving(int protocol, sockaddr_in* from);
bool readInputFileToBuffer(int bufSize, int* bytes_read);
void writeOutputFileFromBuffer(int bytes_write);
void openFiles(std::string* input_file, std::string* output_file, bool openOutputFile);
void printInfo();
void printDebug();
void printData();
void printError();
void printServerHello();
void printClientHello();
void closeEverithing();
void my_terminate(int param);

void my_terminate(int param){
	printInfo();
	if(param == SIGTERM){
		printf("Got signal SIGTERM\n");
	} else if(param == SIGSEGV){
		printf("Got signal SIGSEGV\n");
	} else if(param == SIGKILL){
		printf("Got signal SIGKILL\n");
	} else if(param == SIGINT){
		printf("Got signal SIGINT\n");
	}
	closeEverithing();
	printError();
	printf("terminated\n");
	exit(5);

}

void closeEverithing(){
	printInfo();
	printf("closing everything\n");

	FILE* f;
	int sock_local_var;
	if(f_input != NULL){
		f = f_input;
		f_input = NULL;
		fclose(f);
	}
	if(f_output != NULL){
		f = f_output;
		f_output = NULL;
		fclose(f);
	}
	if(s >= 0){
		sock_local_var = s;
		s = -1;
		// close(sock_local_var);
	}
	if(s >= 0){
		sock_local_var = sock;
		sock = -1;
		// close(sock_local_var);
	}

}

void printServerHello(){
	printInfo();
	printf("Starting server\n");
}

void printClientHello(){
	printInfo();
	printf("Starting client\n");
}



void printData(){
	printf("[Data: ] ");
}

void printInfo(){
	printf("[Info: ] ");
}

void printDebug(){
	printf("[Debug:] ");
}

void printError(){
	printf("[Error:] ");
	fflush(stdout);
}

void openFiles(std::string* input_file, std::string* output_file, bool openOutputFile){
	#ifdef DEBUG
	printDebug();
	printf("opening input file: %s\n", input_file->c_str());
	#endif
	f_input = fopen(input_file->c_str(), "r");
	if(openOutputFile){
		#ifdef DEBUG
		printDebug();
		printf("opening output file: %s\n", output_file->c_str());
		#endif
		f_output = fopen(output_file->c_str(), "w");
		if(f_output == NULL){
			printError();
			perror("output fileopen result");
			exit(4);
		}
	}
	if(f_input == NULL){
		printError();
		perror("intput fileopen input");
		exit(4);
	}
}

void writeOutputFileFromBuffer(int bytes_write){
	fwrite(buf_recv, sizeof(char), bytes_write, f_output);
}

bool readInputFileToBuffer(int bufSize, int* bytes_read){
	*bytes_read = fread( buf_send, sizeof(char), bufSize, f_input );
	if(feof(f_input)){
		return true;
	}
	return false;
}

void informOfReceiving(int role, int protocol, sockaddr_in* from){
	if(from == NULL) return;

	printInfo();
	if(role == ROLE_SERVER){
		if(protocol == PROTOCOL_TCP){
			printf("Accepted connection with port: %d\n", from->sin_port);
		} else {
			printf("Got message from port: %d\n", from->sin_port);
		}
	} else {
		printf("Got answer from port: %d\n", from->sin_port);
	}
}

void printSendToConsole(int size){

	if(size > 0){
		printInfo();
		printf("Send buffer first %d bytes:\n", size);
		printFirstNBytes(buf_send, size);
	}
}

void printRecvToConsole(int bytes_read, bool friteFull){
	int bytes_to_print = bytes_read;
	if(!friteFull){
		bytes_to_print = std::min(bytes_read, BYTES_TO_PRINT_LIMIT);
	}
	printInfo();
	printf("Received %d bytes.\n", bytes_read);

	if(bytes_read > 0){
		printInfo();
		printf("First %d:\n", bytes_to_print);
		printFirstNBytes(buf_recv, bytes_to_print);
	}
}

void inverseRecvToSend(int bytes_read){
	for(int i = 0; i < bytes_read; i++){
		buf_send[i] = buf_recv[bytes_read - i - 1];
	}
}

sockaddr_in generateAddressStructure(uint32_t port){
	struct sockaddr_in remote_address;
	remote_address.sin_family = AF_INET;
	remote_address.sin_port = htons(port);
	remote_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	return remote_address;
}


void printFirstNBytes(char* buf, int n){
	printData();
	for(int i = 0; i < n; i++){
		char ch = buf[i];
		if(ch == '\0'){
			putchar('\\');
			putchar('0');
		} else if(ch == '\t'){
			putchar('\\');
			putchar('t');
		} else if(ch == '\n'){
			putchar('\\');
			putchar('n');
		} else if(ch == '\r'){
			putchar('\\');
			putchar('r');
		} else {
			putchar(ch);
		}
	}
	puts("\n");
}

void allocateBuffers(int protocol){
	if(protocol == PROTOCOL_TCP){
		buf_recv = new char[BUFFER_SIZE_TCP];
		buf_send = new char[BUFFER_SIZE_TCP];
	} else {
		buf_recv = new char[BUFFER_SIZE_UDP];
		buf_send = new char[BUFFER_SIZE_UDP];
	}
}

void openSocket(int protocol){

	if(protocol == PROTOCOL_TCP){
		s = socket(AF_INET, SOCK_STREAM, 0);
	} else {
		s = socket(AF_INET, SOCK_DGRAM, 0);
	}

	if(s < 0){
		printError();
		perror("socket");
		exit(1);
	}

}

int main(int argc, char** argv){

	void (*prev_fn)(int);
	
	prev_fn = signal(SIGTERM, my_terminate);
	prev_fn = signal(SIGSEGV, my_terminate);
	prev_fn = signal(SIGKILL, my_terminate);
	prev_fn = signal(SIGINT, my_terminate);
	


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
		input_file = parser->get_data();
		if(parser->get_result_isset() ){
			result = RESULT_FILE;
			output_file = parser->get_result();
		} else {
			result = RESULT_STDOUT;
		}
		
	}

	allocateBuffers(protocol);
	openSocket(protocol);
	sockaddr_in address = generateAddressStructure(port);

	if(role == ROLE_CLIENT){

		
		printClientHello();
		


		openFiles(&input_file, &output_file, (result == RESULT_FILE));
		int bytes_to_send;

		if(protocol == PROTOCOL_TCP){

			/* ============================================= *\
			===================================================
			===================================================
			===================  TPC CLIENT  ==================
			===================================================
			===================================================
			\* ============================================= */


			if( connect(s, (sockaddr*)&address, sizeof(address)) < 0 ){
				printError();
				perror("connect");
				exit(2);
			}

			while(1){
				bool finished = readInputFileToBuffer(BUFFER_SIZE_TCP, &bytes_to_send);

				send(s, buf_send, bytes_to_send, 0);

				int bytes_read = recv(s, buf_recv, BUFFER_SIZE_TCP, 0);

				informOfReceiving(role, protocol, &address);

				if(result == RESULT_FILE){
					printRecvToConsole(bytes_read, false);
					writeOutputFileFromBuffer(bytes_read);
				} else {
					printRecvToConsole(bytes_read, false);
				}


				if(finished) break;
			}

			
		} else {

			/* ============================================= *\
			===================================================
			===================================================
			===================  UDP CLIENT  ==================
			===================================================
			===================================================
			\* ============================================= */

			while(1){
				bool finished = readInputFileToBuffer(BUFFER_SIZE_UDP, &bytes_to_send);
				sendto(s, buf_send, bytes_to_send, 0, (struct sockaddr*)&address, sizeof(address));
				
				struct sockaddr_in from;
				socklen_t fromlen;
				
				int bytes_read = recvfrom(s, buf_recv, BUFFER_SIZE_UDP, 0, (struct sockaddr*)&from, &fromlen);
				
				informOfReceiving(role, protocol, &from);
				printRecvToConsole(bytes_read, true);

				if(result == RESULT_FILE){
					printRecvToConsole(bytes_read, false);
					writeOutputFileFromBuffer(bytes_read);
				} else {
					printRecvToConsole(bytes_read, false);
				}

				if(finished) break;
			}

		}

	} else {
		
		printServerHello();
		

		if( bind(s, (struct sockaddr*) &address, sizeof(sockaddr_in)) ){
			printError();
			perror("bind");
			exit(2);
		}


		struct sockaddr_in from;
		socklen_t fromlen;


    	

		if(protocol == PROTOCOL_TCP){

			/* ============================================= *\
			===================================================
			===================================================
			===================  TCP SERVER  ==================
			===================================================
			===================================================
			\* ============================================= */

			listen(s, 1);

			while(1){
				
				sock = accept(s, (struct sockaddr*)&from, &fromlen);
				

				if(sock < 0){
					printError();
					perror("accept");
					exit(3);
				}

				informOfReceiving(role, protocol, &from);

				while(1){

					int bytes_read = recv(sock, buf_recv, BUFFER_SIZE_TCP, 0);

					if(bytes_read <= 0) break;

					printRecvToConsole(bytes_read, false);

					inverseRecvToSend(bytes_read);
					#ifdef DEBUG
					//printSendToConsole(bytes_read);
					#endif
					send(sock, buf_send, bytes_read, 0);
				
				}
			}

		} else {

			/* ============================================= *\
			===================================================
			===================================================
			===================  UDP SERVER  ==================
			===================================================
			===================================================
			\* ============================================= */

			
			
			while(1){
				int bytes_read = recvfrom(s, buf_recv, BUFFER_SIZE_UDP, 0, (struct sockaddr*)&from, &fromlen);
				
				informOfReceiving(role, protocol, &from);

				if(bytes_read > 0){

					printRecvToConsole(bytes_read, false);

					inverseRecvToSend(bytes_read);
					
					#ifdef DEBUG
					// printSendToConsole(bytes_read);
					#endif
					
					#ifdef DEBUG
					printf("SENDING!\n");
					#endif
					
					sendto(s, buf_send, bytes_read, 0, (struct sockaddr*)&from, fromlen);
					
					#ifdef DEBUG
					printf("SENT!\n");
					#endif
				
				}
			}
		}
	}

	closeEverithing();
	return 0;
}