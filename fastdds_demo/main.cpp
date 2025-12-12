/**
 * FastDDS Demo - Main Entry Point
 * 
 * This is a simple entry point that demonstrates how to run either 
 * the publisher or subscriber based on command line arguments.
 * 
 * Usage:
 *   ./fastdds_demo publisher    # Run as publisher
 *   ./fastdds_demo subscriber   # Run as subscriber
 */

#include <iostream>
#include <string>

int main(int argc, char** argv) 
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " {publisher|subscriber}" << std::endl;
        std::cout << "  publisher   - Run as publisher" << std::endl;
        std::cout << "  subscriber  - Run as subscriber" << std::endl;
        return 1;
    }

    std::string mode = argv[1];
    
    if (mode == "publisher") {
        std::cout << "This would run the publisher demo." << std::endl;
        std::cout << "In a real application, this would call the publisher code." << std::endl;
        std::cout << "See src/publisher.cpp for the actual implementation." << std::endl;
    } 
    else if (mode == "subscriber") {
        std::cout << "This would run the subscriber demo." << std::endl;
        std::cout << "In a real application, this would call the subscriber code." << std::endl;
        std::cout << "See src/subscriber.cpp for the actual implementation." << std::endl;
    } 
    else {
        std::cout << "Invalid mode: " << mode << std::endl;
        std::cout << "Use 'publisher' or 'subscriber'" << std::endl;
        return 1;
    }

    return 0;
}