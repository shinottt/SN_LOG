# C++练习 日志输出 很慢

VSCode + MinGW64

```C++
#include"sn_log.h"

int main(){
    //控制台输出
    sn_consolelog_debug("This is a debug message");
    sn_consolelog_info("This is an info message");
    sn_consolelog_warning("This is a warning message");
    sn_consolelog_error("This is an error message");
    sn_consolelog_fatal("This is a fatal message");

    //文件输出
    sn_filelog_debug("This is a debug message", "log.txt");
    sn_filelog_info("This is an info message", "log.txt");
    sn_filelog_warning("This is a warning message", "log.txt");
    sn_filelog_error("This is an error message", "log.txt");
    sn_filelog_fatal("This is a fatal message", "log.txt");

    return 0;
}
```