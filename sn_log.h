#ifndef _SN_LOG_HEADER11_
#define _SN_LOG_HEADER11_


#include<iostream>
#include<fstream>
#include<string>
#include<mutex>
#include<chrono>
#include<iomanip>           //put_time
#include<vector>
#include<sstream>
#include<string_view>


#ifdef WIN32
#include<Windows.h>
#include<thread>
#elif defined(__linux__)
#include<pthread.h>
#endif


using sn_string = std::string;

static std::mutex mtx_file_;
static std::mutex mtx_console_;

// command line text color
const std::string cmd_color_reset ="\033[0m";
const std::string cmd_color_cyan = "\033[36m";
const std::string cmd_color_white= "\033[37m";
const std::string cmd_color_yellow = "\033[33m";
const std::string cmd_color_red ="\033[31m";
const std::string cmd_color_purple = "\033[35m";

// define log file name
static std::fstream file_;
static bool log_file_init_ = false;
const sn_string LOG_FILE_PATH = "sn_log.txt";


template<typename T>
void GetString(std::vector<std::string>& str_vec, T&& t){
    std::ostringstream oss;
    oss << t;
    str_vec.push_back(oss.str());
}

template<typename T, typename... Args>
void GetString(std::vector<std::string>& str_vec, T&& t, Args&&... args){
    std::ostringstream oss;
    oss << t;
    str_vec.push_back(oss.str());
    GetString(str_vec, args...);
}

//format string
template<typename T = std::string_view, typename... Args>
std::string sn_format(std::string_view fmt, Args&&... args){
    size_t args_index = 0;

    std::vector<std::string> str_vec;
    GetString(str_vec, args...);

    std::ostringstream oss;
    for(auto it = fmt.begin(); it != fmt.end(); ++it){
        if(*it == '{'){
            if(*(it+1) == '}'){
                if(args_index >= str_vec.size()){
                    printf("Error: Not enough arguments for format string\n");
                    exit(1);
                }
                oss << str_vec[args_index];
                ++args_index;
                ++it;
            }
        } else{
            oss << *it;
        }
    }
    return oss.str();
} 



enum LogLevel{
    SN_NONE,
    SN_DEBUG,
    SN_INFO,
    SN_WARNING,
    SN_ERROR,
    SN_FATAL
};

enum LogMode{
    SN_CONSOLE,
    SN_FILE
};

//程序开始时间
static std::chrono::steady_clock::time_point start_time_ = std::chrono::steady_clock::now();

/*
windows下使用函数控制终端输出颜色，示例：
    ConsoleColor_win(level_);           //设置颜色
    std::cout<<"This is a colorful message"<<std::endl;
    ConsoleColor_win(SN_NONE);          //恢复默认颜色
*/

#ifdef WIN32
static void ConsoleColor_win(LogLevel level_){
        HANDLE hConsole_ = GetStdHandle(STD_OUTPUT_HANDLE);
        switch (level_){
            case SN_DEBUG:
                SetConsoleTextAttribute(hConsole_, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                break;
            case SN_INFO:
                SetConsoleTextAttribute(hConsole_, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                break;
            case SN_WARNING:
                SetConsoleTextAttribute(hConsole_, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                break;
            case SN_ERROR:
                SetConsoleTextAttribute(hConsole_, FOREGROUND_RED | FOREGROUND_INTENSITY);
                break;
            case SN_FATAL:
                SetConsoleTextAttribute(hConsole_, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                break;
            case SN_NONE:
                SetConsoleTextAttribute(hConsole_, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                break;
            default:
                SetConsoleTextAttribute(hConsole_, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                break;
        }
}
#endif  // WIN32 ConsoleColor




static sn_string log_level_to_string(LogLevel level_){
    sn_string level_str_;
    switch(level_){
        case SN_DEBUG:
            level_str_ = "DEBUG";
            break;
        case SN_INFO:
            level_str_ = "INFO";
            break;
        case SN_WARNING:
            level_str_ = "WARNING";
            break;
        case SN_ERROR:
            level_str_ = "ERROR";
            break;
        case SN_FATAL:
            level_str_ = "FATAL";
            break;
        case SN_NONE:
            level_str_ = "NONE";
            break;
        default:
            level_str_ = "UNKNOWN";
            break;
    }
    return level_str_;
}


static sn_string ConsoleColor(LogLevel level_){
    switch(level_){
        case SN_DEBUG:
            return cmd_color_cyan;
            break;
        case SN_INFO:
            return cmd_color_white;
            break;
        case SN_WARNING:
            return cmd_color_yellow;
            break;
        case SN_ERROR:
            return cmd_color_red;
            break;
        case SN_FATAL:
            return cmd_color_purple;
            break;
        case SN_NONE:
            return cmd_color_reset;
            break;
        default:
            return cmd_color_reset;
            break;
    }
}



static void sn_log(sn_string message_, LogLevel level_, LogMode mode_){
    //毫秒计时
    std::chrono::steady_clock::time_point end_time_ = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> run_time_ms_ = end_time_ - start_time_;
    switch (mode_)
    {
    case SN_CONSOLE:
        mtx_console_.lock();
        //使用ANSI转义序列实现文本彩色输出
        std::cout<<ConsoleColor(level_)<<"[ "<<log_level_to_string(level_)<<" ]    "<<message_<<"    [ "<<run_time_ms_.count()<<" ms ]"<<ConsoleColor(SN_NONE)<<std::endl;
        mtx_console_.unlock();
        break;
    case SN_FILE:
        mtx_file_.lock();
        //第一次打开时加入头
        //[2024-07-27 13:28:55]
        //[SN_LOG]  
        if(!log_file_init_){
            file_.open(LOG_FILE_PATH.c_str(), std::ios::out | std::ios::app);
            if(file_.is_open()){
                const std::chrono::time_point<std::chrono::system_clock> now_ = std::chrono::system_clock::now();
                const std::time_t now_t_ = std::chrono::system_clock::to_time_t(now_);
                file_<<"["<<std::put_time(std::localtime(&now_t_), "%Y-%m-%d %H:%M:%S")<<"]"<<std::endl;
                file_<<"[SN_LOG]  "<<std::endl;
                log_file_init_ = true;
                file_.close();
            }
            else{
                std::cout<<"error: unable to initialize log file"<<std::endl;
            }
        }
        //写入日志
        file_.open(LOG_FILE_PATH.c_str(), std::ios::out | std::ios::app);
        if(file_.is_open()){
            file_<<"[ "<<log_level_to_string(level_)<<" ]    "<<message_<<"    [ "<<run_time_ms_.count()<<" ms ]"<<std::endl;
            file_.close();
        }
        else{
            std::cout<<"error: unable to open log file"<<std::endl;
        }
        mtx_file_.unlock();
        break;
    default:
        std::cout<<"error: invalid log mode"<<std::endl;
        break;
    }
}



//console log function
template<typename... Args>
void sn_consolelog_debug(sn_string message_, Args&&... args_){
    sn_log(sn_format(message_, args_...), SN_DEBUG, SN_CONSOLE);
}

static void sn_consolelog_debug(sn_string message_){
    sn_log(message_, SN_DEBUG, SN_CONSOLE);
}

template<typename... Args>
void sn_consolelog_info(sn_string message_, Args&&... args_){
    sn_log(sn_format(message_, args_...), SN_INFO, SN_CONSOLE);
}

static void sn_consolelog_info(sn_string message_){
    sn_log(message_, SN_INFO, SN_CONSOLE);
}

template<typename... Args>
void sn_consolelog_warning(sn_string message_, Args&&... args_){
    sn_log(sn_format(message_, args_...), SN_WARNING, SN_CONSOLE);

}

static void sn_consolelog_warning(sn_string message_){
    sn_log(message_, SN_WARNING, SN_CONSOLE);
}

template<typename... Args>
void sn_consolelog_error(sn_string message_, Args&&... args_){
    sn_log(sn_format(message_, args_...), SN_ERROR, SN_CONSOLE);
}

static void sn_consolelog_error(sn_string message_){
    sn_log(message_, SN_ERROR, SN_CONSOLE);
}

template<typename... Args>
void sn_consolelog_fatal(sn_string message_, Args&&... args_){
    sn_log(sn_format(message_, args_...), SN_FATAL, SN_CONSOLE);
}

static void sn_consolelog_fatal(sn_string message_){
    sn_log(message_, SN_FATAL, SN_CONSOLE);
}

//file log function
template<typename... Args>
void sn_filelog_debug(sn_string message_, Args&&... args_){
    sn_log(sn_format(message_, args_...), SN_DEBUG, SN_FILE);
}

static void sn_filelog_debug(sn_string message_){
    sn_log(message_, SN_DEBUG, SN_FILE);
}

template<typename... Args>
void sn_filelog_info(sn_string message_, Args&&... args_){
    sn_log(sn_format(message_, args_...), SN_INFO, SN_FILE);
}

static void sn_filelog_info(sn_string message_){
    sn_log(message_, SN_INFO, SN_FILE);
}

template<typename... Args>
void sn_filelog_warning(sn_string message_, Args&&... args_){
    sn_log(sn_format(message_, args_...), SN_WARNING, SN_FILE);
}

static void sn_filelog_warning(sn_string message_){
    sn_log(message_, SN_WARNING, SN_FILE);
}

template<typename... Args>
void sn_filelog_error(sn_string message_, Args&&... args_){
    sn_log(sn_format(message_, args_...), SN_ERROR, SN_FILE);
}

static void sn_filelog_error(sn_string message_){
    sn_log(message_, SN_ERROR, SN_FILE);
}

template<typename... Args>
void sn_filelog_fatal(sn_string message_, Args&&... args_){
    sn_log(sn_format(message_, args_...), SN_FATAL, SN_FILE);
}

static void sn_filelog_fatal(sn_string message_){
    sn_log(message_, SN_FATAL, SN_FILE);
}
















#endif  // _SN_LOG_HEADER11_