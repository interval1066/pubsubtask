#include <iostream>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <experimental/random>

/**
 * This is a stupid program, but I think it works to spec.
 */
int
main(int argc, char** argv)
{
	std::ostringstream oss;
	std::string line, totline;
	int m = 0;

	if(argv[1] == nullptr) {
		std::cerr << "Missing file arg" << std::endl;
		return EXIT_FAILURE;
	}

	std::ofstream outputfile(argv[1]);

	for(int n = 0; n < 1000; n++) {
		int data = std::experimental::randint(1,
			std::numeric_limits<int>::max());
		oss << data;

		m = line.append(oss.str()).append(" ").length();
		if(m > 100) {
			outputfile << line << "\n";

			oss.str("");
			oss.clear();
			line.clear();
		}
	}

	outputfile.flush();
	outputfile.close();

	return EXIT_SUCCESS;
}
