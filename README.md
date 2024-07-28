# C++练习 日志输出 很慢

VSCode + MinGW64

compile command:
```cmd
mingw32-make
```

```C++
#include"sn_log.h"


int main(){

    for(int i = 0; i < 10000; ++i){
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

    std::cout<<sn_format("{} {} and {} {} tests completed.",10000, "console logs", 10000, "file logs")<<std::endl;

    return 0;
}
```