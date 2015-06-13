#include <iostream>
#include <functional>
#include "source/base/RandomForest.hpp"

#include "Program.hpp"
#include "base/RFParameter.hpp"
#include "base/RandomForest.hpp"
#include "base/Histogram.hpp"
#include "cells/CenterPixelNodeFactory.hpp"


//#define BOOST_ALL_DYN_LINK 

/**
 * Returns the message of an unknown exception in a catch context.
 */
const std::string find_exception_msg() {
    try {
        throw;
    } catch (const std::exception& e) {
        return e.what();
    } catch (const std::string& e) {
        return e;
    } catch (...) {
        return "Unknown exception";
    }
}

int main(int argc, char** argv) {
    try {
        Program program;
        return program.run(argc, argv);
    } catch (...) {
        std::cerr << "Unexpected exception:" << find_exception_msg() << std::endl;
        return EXIT_FAILURE;
    }
}
