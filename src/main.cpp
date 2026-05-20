#include "document/Document.h"
#include <iostream>

int main(int argc, char** argv) {

	if (argc < 2) {
		std::cerr << "Usage: dxf file_name" << std::endl;
		return 1;
	}
	try {
		dxf::Document d(argv[1]);
	} catch (const std::string& e) {
		std::cerr << "Error: " << e << std::endl;
		return 1;
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
