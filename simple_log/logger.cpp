#include "logger.h"
#include <cstdlib>
#include <ctime>
 //初始化静态成员变量
Logger *Logger::m_SingleInstance = NULL;
std::mutex Logger::m_Mutex;
std::ofstream Logger::m_error_log_file;
std::ofstream Logger::m_info_log_file;
std::ofstream Logger::m_warn_log_file;


Logger* Logger::GetInstance()
{
    //  这里使用了两个 if判断语句的技术称为双检锁；好处是，只有判断指针为空的时候才加锁，
    //  避免每次调用 GetInstance的方法都加锁，锁的开销毕竟还是有点大的。
    if (m_SingleInstance == NULL) 
    {
        std::unique_lock<std::mutex> lock(m_Mutex); // 加锁
        if (m_SingleInstance == NULL)
        {
            m_SingleInstance = new  Logger();
        }
    }

    return m_SingleInstance;
}

void Logger::deleteInstance()
{
    std::unique_lock<std::mutex> lock(m_Mutex); // 加锁
    if (m_SingleInstance)
    {
        delete m_SingleInstance;
        m_SingleInstance = NULL;
    }
}
 
void initLogger(const std::string&info_log_filename,
                const std::string&warn_log_filename,
               const std::string&error_log_filename){
   Logger::m_info_log_file.open(info_log_filename.c_str());
   Logger::m_warn_log_file.open(warn_log_filename.c_str());
   Logger::m_error_log_file.open(error_log_filename.c_str());
}

std::ostream& Logger::getStream(log_rank_t log_rank){
   switch (log_rank)
   {
   case INFO:
      m_info_log_file.is_open() ?m_info_log_file : std::cout;
      break;
   case WARNING:
      m_warn_log_file.is_open()? m_warn_log_file : std::cerr;
      break;
   case ERROR:
      m_error_log_file.is_open()? m_error_log_file : std::cerr;
      break;
   default:
      m_log_rank = log_rank;
      break;
   }
}
 
std::ostream& Logger::start(log_rank_t log_rank,
                            const int32_t line,
                            const std::string&function) {
   time_t tm;
   time(&tm);
   char time_string[128];
   ctime_r(&tm, time_string);
   return getStream(log_rank) << time_string
                               << "function (" << function << ")"
                               << "line " << line << "\t"
                               <<std::flush;
}
 
Logger::~Logger(){
   getStream(m_log_rank) << std::endl << std::flush;
   
   if (FATAL == m_log_rank) {
       m_info_log_file.close();
       m_info_log_file.close();
       m_info_log_file.close();
       abort();
    }
}