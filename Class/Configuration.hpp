#pragma once
#include <fstream>
#include <stddef.h>
#include <string>
#include <set>
#include <vector>
#include <sstream>
#include <iostream>
#include "Server.hpp"

// parse the config file by filling the servers vector
// each server in the vector represents a server {}; block
class Configuration {

	public:

		Configuration(const std::string config_file_path);
		~Configuration();

		// public methods

		// getters
		std::vector<Server> get_servers() const { return servers; }

		// public attributes

	private:

		// private data structures

		struct	token {
			std::string	content;
			size_t		line;

		};

		struct location_block {
			std::string			path;
			std::vector<token>	tokens;
		};

		struct server_block {
			std::vector<token>			tokens;
			std::vector<location_block>	location_blocks;
		};

		// private methods

		// ***testing***
		void						print_server_blocks(const std::vector<server_block>& servers);
		void						print_servers(const std::vector<Server>& servers);

		void						create_directive_bank();
		void						parse(std::ifstream& config_file);
		void						space_out_symbols(std::string& file_content);
		std::vector<token>			tokenize(std::string spaced_out_content);
		std::vector<server_block>	parse_server_blocks(std::vector<token> tokens);
		server_block				create_server_block(std::vector<token> tokens, size_t &i);
		void						is_valid_server_block(std::vector<token> tokens, size_t &i);
		location_block				create_location_block(std::vector<token> tokens, size_t &i);
		void						is_valid_location_block(Configuration::location_block &location_block, std::vector<token> tokens, size_t &);
		void						validate_directive(std::vector<token> tokens, size_t &i);
		int							count_directive_args(std::vector<token> tokens, size_t i);
		void						verify_end_of_line(std::vector<token> tokens, size_t &i);
		std::string					get_full_line(std::vector<token> tokens, size_t& i);
		Server						create_server(server_block server_blocks);
		void						fill_server_attributes(std::vector<token> server_block_tokens, Server &server);
		template <typename T> void	fill_shared_attributes(std::vector<token> server_block_tokens, T &obj);
		void						fill_location_attributes(std::vector<token> server_block_tokens, Server::Location &location);
		std::vector<std::string>	get_arguments(std::vector<token> tokens, size_t i);
		bool						validate_listen_argument_format(std::vector<std::string> arguments);
		bool						is_valid_ip_address(const std::string& ip_address);
		std::vector<std::string>	split(std::string s, char delimiter);


		// private attributes
		std::vector<Server>							servers;
		std::map<std::string, std::pair<int, int> >	directive_bank;

		// private exceptions
		class parsing_exception : public std::exception {
			public:
				parsing_exception(const std::string msg,  const size_t line, const std::string& token)
					: message(msg), file_line(line), error_token(token) {
					std::ostringstream stream;
					stream << "Config Error: "<< "'" << message << "'";
					stream << " \"" << error_token << "\"";
					stream << " at line " << file_line;
					message = stream.str();
				}

				virtual ~parsing_exception() throw() {}
				virtual const char* what() const throw() {
					return message.c_str();
				}

			protected:
				std::string message;
				int file_line;
				std::string error_token;
		};

		class unknown_block_type : public parsing_exception {
		public:
			unknown_block_type(const size_t line, const std::string& token)
				: parsing_exception("Unknown block type", line, token) {}
		};

		class unknown_directive : public parsing_exception {
		public:
			unknown_directive(const size_t line, const std::string& token)
				: parsing_exception("Unknown directive", line, token) {}
		};

		class unexpected_token : public parsing_exception {
		public:
			unexpected_token(const size_t line, const std::string& token)
				: parsing_exception("Unexpected token", line, token) {}
		};

		class end_of_line : public parsing_exception {
		public:
			end_of_line(const size_t line, const std::string& token)
				: parsing_exception("Line must finish with semicolon", line, token) {}
		};

		class multiple_directive_on_same_line : public parsing_exception {
		public:
			multiple_directive_on_same_line(const size_t line, const std::string& token)
				: parsing_exception("Multiple directives on same line", line, token) {}
		};

		class location_path_invalid : public parsing_exception {
		public:
			location_path_invalid(const size_t line, const std::string& token)
				: parsing_exception("Location path invalid", line, token) {}
		};

		class no_location_path : public parsing_exception {
		public:
			no_location_path(const size_t line, const std::string& token)
				: parsing_exception("No path for location", line, token) {}
		};

		class no_directive_arg : public parsing_exception {
		public:
			no_directive_arg(const size_t line, const std::string& token)
				: parsing_exception("No argument for directive", line, token) {}
		};

		class to_many_directive_args : public parsing_exception {
		public:
			to_many_directive_args(const size_t line, const std::string& token)
				: parsing_exception("To many arguments for directive", line, token) {}
		};

		class not_enough_arguments_for_directive : public parsing_exception {
		public:
			not_enough_arguments_for_directive(const size_t line, const std::string& token)
				: parsing_exception("Not enough arguments for directive", line, token) {}
		};

		class unable_to_open_file : public std::exception {
				virtual const char* what() const throw() {
					return "Unable to open configuration file";
				}
		};

		class no_server_blocks : public std::exception {
				virtual const char* what() const throw() {
					return "No server blocks in configuration file";
				}
		};
};