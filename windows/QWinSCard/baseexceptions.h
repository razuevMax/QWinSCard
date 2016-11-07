#ifndef BASEEXCEPTIONS_H
#define BASEEXCEPTIONS_H
#include <stdexcpt.h>


class BaseExceptions: public std::exception
{
public:
 enum { UNRESOLVED_EXCEPTION=-10 };
 BaseExceptions() noexcept;
 BaseExceptions(const BaseExceptions&) noexcept;
 explicit BaseExceptions(const char * const &) noexcept;
 explicit BaseExceptions(const QString&) noexcept;
 explicit BaseExceptions(long code) noexcept;
 BaseExceptions&  operator=(const BaseExceptions&);
 virtual  ~BaseExceptions() noexcept;
 const char* what() const noexcept;
 QString errorString(void) const;
 void setErrorString(const QString&);
 long errorCode(void) const;
 void setErrorCode(long code);
private:
 QString m_errorString;
 long m_errorCode;
};

#endif // BASEEXCEPTIONS_H
