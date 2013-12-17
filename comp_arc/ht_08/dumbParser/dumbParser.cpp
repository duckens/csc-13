#include <dumbParser.h>
#include <iostream>


// #define DEBUG





dumbParser::dumbParser(dumbParser const &other){

}

dumbParser& dumbParser::operator= (dumbParser const &other){
	return *this;
}

dumbParser::dumbParser(){
	possibleArgs = new std::map<std::string, bool>();

	possibleArgs->insert( std::pair<std::string, bool> ( std::string("-udp"), false ) );
	possibleArgs->insert( std::pair<std::string, bool> ( std::string("-tcp"), false ) );
	possibleArgs->insert( std::pair<std::string, bool> ( std::string("-server"), false ) );
	possibleArgs->insert( std::pair<std::string, bool> ( std::string("-client"), false ) );
	possibleArgs->insert( std::pair<std::string, bool> ( std::string("-local"), true ) );
	possibleArgs->insert( std::pair<std::string, bool> ( std::string("-remote"), true ) );
	possibleArgs->insert( std::pair<std::string, bool> ( std::string("-data"), true ) );
	possibleArgs->insert( std::pair<std::string, bool> ( std::string("-result"), true ) );
}

dumbParser::~dumbParser(){
	delete possibleArgs;
}

void dumbParser::parseArgs(int argc, char** argv){

	udp = false;
	tcp = false;
	server = false;
	client = false;
	local_isset = false;
	remote_isset = false;
	data_isset = false;
	result_isset = false;

	for(int i = 1; i < argc; i++){
		
		std::string arg = std::string(argv[i]);
		
		if(!argOk(arg)) continue;
		
		std::string arg_value = std::string("");
		if(needNextValue(arg) && (i < argc - 1)){
			arg_value = std::string(argv[i + 1]);
			i++;
		}

		treatArgument(arg, arg_value);
	}
}

void dumbParser::treatArgument(std::string arg, std::string arg_value){
	#ifdef DEBUG
	std::cout << "[Debug:] treatArgument " << arg << ", " << arg_value << std::endl;
	#endif

	if(arg == "-tcp"){
		tcp = true;
	} else if(arg == "-udp"){
		udp = true;
	} else if(arg == "-server"){
		server = true;
	} else if(arg == "-client"){
		client = true;
	} else if(arg == "-local"){
		local_isset = true;
		sscanf(arg_value.c_str(), "%d", &local);
	} else if(arg == "-remote"){
		remote_isset = true;
		sscanf(arg_value.c_str(), "%d", &remote);
	} else if(arg == "-data"){
		data_isset = true;
		data = arg_value;
	} else if(arg == "-result"){
		result_isset = true;
		result = arg_value;
	} else {
		std::cout << "[Error:] unexpected argument " << arg << std::endl;
	}
}

bool dumbParser::argOk(std::string arg){
	if(possibleArgs->find(arg) != possibleArgs->end()) return true;
	return false;
}

bool dumbParser::needNextValue(std::string arg){
	return (*(possibleArgs->find(arg))).second;
}

bool dumbParser::get_udp(){
	return udp;
}

bool dumbParser::get_tcp(){
	return tcp;
}

bool dumbParser::get_server(){
	return server;
}

bool dumbParser::get_client(){
	return client;
}

uint32_t dumbParser::get_local(){
	return local;
}

uint32_t dumbParser::get_remote(){
	return remote;
}

std::string dumbParser::get_data(){
	return data;
}

std::string dumbParser::get_result(){
	return result;
}

bool dumbParser::get_local_isset(){
	return local_isset;
}

bool dumbParser::get_remote_isset(){
	return remote_isset;
}

bool dumbParser::get_data_isset(){
	return data_isset;
}

bool dumbParser::get_result_isset(){
	return result_isset;
}




