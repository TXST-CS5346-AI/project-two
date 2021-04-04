#include <iostream> 
#include <string.h>  // used by strcmp method

/**
* Main function, which serves as an entry point to the Checkers application. 
* User may invoke a help menu by passing in either -h or -help as a CLI parameter when launching the program. 
* 
* @param int argc - count of the number of CLI arguments provided 
* @param char* argv[] - char array of the CLI arguments
* 
* @return EXIT_SUCCESS if the program completes successfully 
*/ 
int main(int argc, char* argv[])
{
	std::cout << "Welcome to the Checkers AI Program." << std::endl;
    std::cout << "Authors: David Torrente (dat54@txstate.edu), Randall Henderson (rrh93@txstate.edu), Borislav Sabotinov (bss64@txstate.edu)." << std::endl;
    std::cout << std::endl;

    // display help menu
    if (argc == 2 && (strcmp(argv[1], "-h") || strcmp(argv[1], "-help"))) 
    {
        std::cout << "To use this program, please read the instructions below and re-launch." << std::endl;
        std::cout << "Additional details for building and execution are also available in the README.md file." << std::endl;
        std::cout << std::endl;

        // TODO: additional help instructions for the user
        

        return EXIT_SUCCESS;
    }
	
	return EXIT_SUCCESS;
}