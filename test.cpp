

#include"sn_log.h"


int main(){

    for(int i=0;i<10000;i++){
        sn_consolelog_debug("debug message");
        sn_consolelog_info("info message");
        sn_consolelog_warning("warning message");
        sn_consolelog_error("error message");
        sn_consolelog_fatal("fatal message");
    }

    for(int i=0;i<10000;i++){
        sn_filelog_debug("debug message");
        sn_filelog_info("info message");
        sn_filelog_warning("warning message");
        sn_filelog_error("error message");
        sn_filelog_fatal("fatal message");
    }

    return 0;
}