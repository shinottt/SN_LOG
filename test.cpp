
#include"a.h"
#include"b.h"
#include"sn_log.h"


int main(){

    for(int i = 0; i < 1; ++i){
        sn_consolelog_debug("Debug message: {} {}", i+1, "debug");
        sn_filelog_debug("Debug message: {} {}", i+1, "debug");
        sn_consolelog_info("Info message: {} {}", i+1, "info");
        sn_filelog_info("Info message: {} {}", i+1, "info");
        sn_consolelog_warning("Warn message: {} {}", i+1, "warning");
        sn_filelog_warning("Warn message: {} {}", i+1, "warning");
        sn_consolelog_error("Error message: {} {}", i+1, "error");
        sn_filelog_error("Error message: {} {}", i+1, "error");
        sn_consolelog_fatal("Fatal message: {} {}", i+1, "fatal");
        sn_filelog_fatal("Fatal message: {} {}", i+1, "fatal");
    }

    sn_consolelog_debug("Console log test completed.");
    sn_filelog_debug("File log test completed.");

    std::cout<<sn_format("{} {} and {} {} tests completed.", 1, "console logs", 1, "file logs")<<std::endl;

    return 0;
}