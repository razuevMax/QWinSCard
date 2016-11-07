#ifndef BASEEXCEPTIONS_H
#define BASEEXCEPTIONS_H
#include <stdexcpt.h>


class BaseExceptions: public std::exception
{
public:
 enum { UNRESOLVED_EXCEPTION=-10 };
 BaseExceptions();
 explicit BaseExceptions(const char * const &);
 explicit BaseExceptions(const QString&);
 BaseExceptions(const BaseExceptions&);
 BaseExceptions&  operator=(const BaseExceptions&);
 virtual  ~BaseExceptions();
 virtual const char* what() const override;
 virtual QString errorString(void) const;
 virtual void setErrorString(const QString&);
 virtual long errorCode(void) const;
 virtual void setErrorCode(long code);
private:
 QString m_errorString;
 long m_errorCode;
};

#endif // BASEEXCEPTIONS_H
