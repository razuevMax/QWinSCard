#include "stdafx.h"
#include "baseexceptions.h"

BaseExceptions::BaseExceptions() noexcept
 :m_errorCode(UNRESOLVED_EXCEPTION)
{

}

BaseExceptions::BaseExceptions( const char * const & _What) noexcept
 :m_errorString(_What), m_errorCode(UNRESOLVED_EXCEPTION)
{
}

BaseExceptions::BaseExceptions( const BaseExceptions& _That) noexcept
{
 *this = _That;
}

BaseExceptions::BaseExceptions( const QString& _What) noexcept
 :m_errorString(_What), m_errorCode(UNRESOLVED_EXCEPTION)
{

}

BaseExceptions::~BaseExceptions()
{

}

BaseExceptions& BaseExceptions::operator=( const BaseExceptions& _That)
{
 m_errorString=_That.m_errorString;
 m_errorCode=_That.m_errorCode;
 return *this;
}

const char* BaseExceptions::what() const noexcept
{
 return m_errorString.isEmpty() ? "Unknown exception" : m_errorString.toLocal8Bit().constData();
}

QString BaseExceptions::errorString( void ) const
{
 return m_errorString.isEmpty() ? "Unknown exception" : m_errorString;
}

void BaseExceptions::setErrorString( const QString& errStr)
{
 m_errorString=errStr;
}

long BaseExceptions::errorCode( void ) const
{
 return m_errorCode;
}

void BaseExceptions::setErrorCode( long code )
{
 m_errorCode=code;
}
