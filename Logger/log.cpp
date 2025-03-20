#include <log.h>

namespace sylar{
Logger::Logger(const std::string &name)
    :m_name(name){

};

void Logger::log(LogLevel::Level level,LogEvent::ptr event){
    if(level >= m_level){
        for(auto &i : m_appenders){
            i->log(level,event);
        }
    }
}
void Logger::debug(LogEvent::ptr event){
    log(LogLevel::DEBUG,event);//调用log函数
};

void Logger::info(LogEvent::ptr event){
    log(LogLevel::INFO,event);
}

void Logger::warn(LogEvent::ptr event){
    log(LogLevel::WARN,event);
}

void Logger::error(LogEvent::ptr event){
    log(LogLevel::ERROR,event);
};

void Logger::fatal(LogEvent::ptr event){
    log(LogLevel::FATAL,event);
};

void Logger::addAppender(LogAppender::ptr appender){
    m_appenders.push_back(appender);
}

void Logger::delAppender(LogAppender::ptr appender)
{
    for (auto it = m_appenders.begin(); it != m_appenders.end();++it){
        if(*it == appender){//使用*才能访问迭代器中的元素，不使用的话，it是一个迭代器，不是元素
            m_appenders.erase(it);
        }
    }
}
}
