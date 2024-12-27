# Simple practice of header-only logging library

## MinGW64 compiler on Windows<br>

compile command:
```cmd
mingw32-make
```
cpp file for testing:
```C++
#include"sn_log.h"

int main(){

    int log_num = 1000;

    for(int i = 0; i < log_num; ++i){
        sn::log::console_debug("Debug message: {} {}", i+1, "debug");
        sn::log::file_debug("Debug message: {} {}", i+1, "debug");
        sn::log::console_info("Info message: {} {}", i+1, "info");
        sn::log::file_info("Info message: {} {}", i+1, "info");
        sn::log::console_warning("Warn message: {} {}", i+1, "warning");
        sn::log::file_warning("Warn message: {} {}", i+1, "warning");
        sn::log::console_error("Error message: {} {}", i+1, "error");
        sn::log::file_error("Error message: {} {}", i+1, "error");
        sn::log::console_fatal("Fatal message: {} {}", i+1, "fatal");
        sn::log::file_fatal("Fatal message: {} {}", i+1, "fatal");
    }
    sn::log::console_debug("Console log test completed.");
    sn::log::file_debug("File log test completed.");
    sn::log::log(sn::format("Test {} completed.", log_num), sn::log::LogLevel::SN_DEBUG, sn::log::LogMode::SN_CONSOLE);

    return 0;
}
```
