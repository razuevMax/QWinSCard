//! \file baseexceptions.h
//! \brief Header file for BaseExceptions class.
//! \details File is contains class BaseExceptions must implement a std::exception in Qt-style class
#ifndef BASEEXCEPTIONS_H
#define BASEEXCEPTIONS_H
#include <stdexcpt.h>

//! \brief This class must implement a std::exception in Qt-style class
class BaseExceptions: public std::exception
{
public:
 enum { UNRESOLVED_EXCEPTION = -10 };//!< Enumeration of unresolved exception
 //! \fn BaseExceptions::BaseExceptions() noexcept
 //! \brief Default constructor.
 BaseExceptions() noexcept;
 //! \fn BaseExceptions::BaseExceptions(const BaseExceptions&) noexcept
 //! \brief copy constructor.
 //! \param[in] _That The other BaseExceptions object.
 BaseExceptions(const BaseExceptions&) noexcept;
 //! \fn BaseExceptions::BaseExceptions( const char * const &) noexcept
 //! \brief c-string constructor.
 //! \param[in] _What c-string exception message
 explicit BaseExceptions(const char * const &) noexcept;
 //! \fn BaseExceptions::BaseExceptions( const QString&) noexcept
 //! \brief Qt-string constructor.
 //! \param[in] _What Qt-string exception message
 explicit BaseExceptions(const QString&) noexcept;
 //! \fn BaseExceptions::BaseExceptions(long code) noexcept
 //! \brief error code constructor.
 //! \param[in] code long error code
 explicit BaseExceptions(long code) noexcept;
 //! \fn BaseExceptions& BaseExceptions::operator=( const BaseExceptions&)
 //! \brief Assignment operator.
 //! \param[in] _That The other BaseExceptions object.
 //! \return A shallow copy of this object.
 BaseExceptions&  operator=(const BaseExceptions&);
 //! \fn BaseExceptions::~BaseExceptions() noexcept
 //! \brief Destructor.
 virtual  ~BaseExceptions() noexcept;
 //! \fn const char* BaseExceptions::what() const noexcept
 //! \brief override std::exception function what. Get c-string exception message
 //! \return "Unknown exception" if it fails, else a pointer to a c-string exception message
 const char* what() const noexcept;
 //! \fn QString errorString(void) const
 //! \brief Get Qt-string exception message
 //! \return "Unknown exception" if it fails, else a Qt-string exception message
 QString errorString(void) const;
 //! \fn void BaseExceptions::setErrorString( const QString&)
 //! \brief Sets exception message Qt-string
 //! \param[in] errStr The exception message Qt-string
 void setErrorString(const QString&);
 //! \fn long BaseExceptions::errorCode( void ) const
 //! \brief Get error code
 //! \return A long error code
 long errorCode(void) const;
 //! \fn void BaseExceptions::setErrorCode( long code )
 //! \brief Sets error code.
 //! \param[in] code The long error code
 void setErrorCode(long code);
private:
 QString m_errorString;//!<The exception message Qt-string
 long m_errorCode;//!<The long error code
};

#endif // BASEEXCEPTIONS_H
