#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__
#include <stdint.h>
#include <string>
#include <memory>
#include <list>
#include <fstream>
#include<sstream>
#include <iostream>
#include <vector>

namespace sylar{


//日志事件
class LogEvent{
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent();
private:
    const char* m_file = nullptr ;//文件名
    int32_t m_line = 0; //行号
    uint32_t m_elapse = 0; //程序启动开始到现在的毫秒数
    uint32_t m_threadId = 0; //线程id
    uint32_t m_fiberId = 0; //协程id
    uint64_t m_time=0; //  日志时间戳
    std::string m_content; //日志内容
};
//日志级别
class LogLevel{//内部枚举，不需要进行显式转换，不安全
public:
    enum Level{
        UNKNOW = 0,
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };
};
// enum class LogLevel{
//     UNKNOW = 0,
//     DEBUG = 1,
//     INFO = 2,
//     WARN = 3,
//     ERROR = 4,
//     FATAL = 5
// };
//日志输出器
class Logger{
public:
    typedef std::shared_ptr<Logger> ptr;
    Logger(const std::string &name = "root");
    void log(LogLevel::Level level,LogEvent::ptr event);
    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);
    void addAppender(LogAppender::ptr appender);//添加日志输出地
    void delAppender(LogAppender::ptr appender);//删除日志输出地
    LogLevel::Level getLevel() const { return m_level;};//获取日志级别
    void setLevel(LogLevel::Level val) { m_level = val;};//设置日志级别
private:
    std::string m_name; //日志器名称
    LogLevel::Level m_level; //日志级别
    std::list<LogAppender::ptr> m_appenders; //日志输出地列表
};

//日志输出地

class LogAppender{
public:
    typedef std::shared_ptr<LogAppender> ptr;
    virtual ~LogAppender(); //实现多态，防止子类释放出现问题
    virtual void log(LogLevel::Level level,LogEvent::ptr event) = 0;
    void setFormatter(LogFormatter::ptr val) { m_formatter = val;};

protected:
    LogLevel::Level m_level;
    LogFormatter::ptr m_formatter;

};

//日志输出格式

class LogFormatter{

public:
    typedef std::shared_ptr<LogFormatter> ptr;
    std::string format(LogEvent::ptr event);//将日志事件格式传入
    LogFormatter(const std::string &pattern);
private:
    class FormatItem{
        public:
            typedef std::shared_ptr<FormatItem> ptr;
            virtual ~FormatItem(){};
            virtual void format(std::ostream& os,LogEvent::ptr event) = 0;

    };
    void init();
private:
    std::string m_pattern;
    std::vector<FormatItem> m_items;
};

class StdoutLogAppender:public LogAppender{//输出到控制台的appender
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    virtual void log(LogLevel::Level level,LogEvent::ptr event) override;
};

class FileLogAppender:public LogAppender{//输出到文件的appender
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    FileLogAppender(const std::string &filename);
    virtual void log(LogLevel::Level level,LogEvent::ptr event) override;
    bool reopen();
private:
    std::ofstream m_filestream;
    std::string m_filename;

};
}
#endif