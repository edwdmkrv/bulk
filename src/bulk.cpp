#include <cstdlib>

#include <iostream>

#include "bulk.hpp"

int main() try {
	enum: unsigned {bulk = 3};

	usr::Logger logger;
	usr::Filer filer;
	usr::Interpreter interpreter{bulk};
	usr::Reader reader;

	reader.subscribe(interpreter);
	interpreter.subscribe(logger);
	interpreter.subscribe(filer);
	reader.run();

	return EXIT_SUCCESS;
} catch (std::exception const e) {
	std::cerr << "Error: " << e.what() << std::endl;
	return EXIT_FAILURE;
} catch (...) {
	std::cerr << "Error: " << "unidentified exception" << std::endl;
	return EXIT_FAILURE;
}
