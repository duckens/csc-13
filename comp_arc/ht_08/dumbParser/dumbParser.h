#ifndef DUMB_PARSER_H
#define DUMB_PARSER_H

#include <cstdint>
#include <string>
#include <map>

class dumbParser{
	
	private:

		dumbParser(dumbParser const &other);
		dumbParser& operator= (dumbParser const &other);
		
		/*
		- udp – используется транспорт UDP
		- tcp – используется транспорт TCP
		- server – программа работает как сервер, то есть ждет запроса и отвечает на него
		- client – программа работает как клиент, то есть первой шлет запрос на сервер и ожидает ответа.
		- local NUM – задает номер локального порта NUM
		- remote NUM – задает номер удаленного порта NUM
		- data FILE – имя файла для обработки, содержащего пересылаемые данные
		- result FILE – имя файла для записи результата обработки
		*/
		bool udp;
		bool tcp;
		bool server;
		bool client;
		uint32_t local;
		uint32_t remote;
		std::string data;
		std::string result;

		bool local_isset;
		bool remote_isset;
		bool data_isset;
		bool result_isset;

		std::map<std::string, bool>* possibleArgs;


		bool argOk(std::string arg);
		bool needNextValue(std::string arg);
		void treatArgument(std::string arg, std::string arg_value);

	public:

		dumbParser();
		~dumbParser();

		void parseArgs(int argc, char** argv);

		bool get_udp();
		bool get_tcp();
		bool get_server();
		bool get_client();
		uint32_t get_local();
		uint32_t get_remote();
		std::string get_data();
		std::string get_result();

		bool get_local_isset();
		bool get_remote_isset();
		bool get_data_isset();
		bool get_result_isset();

};

#endif
