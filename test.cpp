#include"sn_log.h"


int main(){

    int log_num = 10000;

    for(int i = 0; i < log_num; ++i){
        sn::consolelog_debug("Debug message: {} {}", i+1, "debug");
        sn::filelog_debug("Debug message: {} {}", i+1, "debug");
        sn::consolelog_info("Info message: {} {}", i+1, "info");
        sn::filelog_info("Info message: {} {}", i+1, "info");
        sn::consolelog_warning("Warn message: {} {}", i+1, "warning");
        sn::filelog_warning("Warn message: {} {}", i+1, "warning");
        sn::consolelog_error("Error message: {} {}", i+1, "error");
        sn::filelog_error("Error message: {} {}", i+1, "error");
        sn::consolelog_fatal("Fatal message: {} {}", i+1, "fatal");
        sn::filelog_fatal("Fatal message: {} {}", i+1, "fatal");
    }

    sn::consolelog_debug("Console log test completed.");
    sn::filelog_debug("File log test completed.");

    std::cout<<sn::format("{} {} and {} {} tests completed.", log_num, "console logs", log_num, "file logs")<<std::endl;

    return 0;
}