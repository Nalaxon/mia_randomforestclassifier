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

std::vector<Histogram<Label, cv::Mat>> histogram;
Histogram<Label, cv::Mat> ensemble(const std::vector<Histogram<Label, cv::Mat>>& samples) {
    return histogram[0];
}

int main(int argc, char** argv) {
    try {
        std::function<Histogram<Label, cv::Mat>(const std::vector<Histogram<Label, cv::Mat>>& samples)> ensmble_fct = ensemble;
        RTParameter rt_params;
        rt_params.m_maxDepth = 15;
        rt_params.m_minSamples = 3;
        RFParameter<Label, cv::Mat> rf_params(
                true,
                rt_params,
                ensmble_fct, 10);
        PatchParameter patch_params;
        patch_params.patch_height = 30;
        patch_params.patch_width = 30;
        RandomForest<Label, cv::Mat> forest(rf_params, new CenterPixelNodeFactory(patch_params));
        Program program;
        return program.run(argc, argv);
    } catch (...) {
        std::cerr << "Unexpected exception:" << find_exception_msg() << std::endl;
        return EXIT_FAILURE;
    }
}
