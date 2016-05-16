#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H


#include <exception>

class msg_exception : public std::exception
{
private:
    std::string err_msg;

public:
    msg_exception(const std::string& msg) : err_msg(msg)
    {}

    ~msg_exception() throw() {}

    const char *what() const throw()
    {
        return this->err_msg.c_str();
    }
};

class bin_error_exception : public msg_exception
{
public:
    bin_error_exception(const std::string& msg) : msg_exception(msg)
    {}
};

class lib_error_exception : public msg_exception
{
public:
    lib_error_exception(const std::string& msg) : msg_exception(msg)
    {}
};
#endif // EXCEPTIONS_H
