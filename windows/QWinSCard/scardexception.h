//! \file SCardException.h
//! \brief Header file for SCardException class.
//! \details File is contains class SCardException must implement a card exception based on WinSCard errors
#ifndef SCARDEXCEPTION_H
#define SCARDEXCEPTION_H

#include "baseexceptions.h"
//! \brief This class must implement a card exception based on WinSCard errors
class SCardException : public BaseExceptions
{
	public:
  //! \fn SCardException::SCardException(long code) noexcept
  //! \brief error code constructor.
  //! \param[in] code long error code
		explicit SCardException(long code) noexcept;
  //! \fn SCardException::~SCardException() noexcept
  //! \brief Destructor.
  virtual ~SCardException() noexcept;
  //! \fn QString getMessage(long code)
  //! \brief static function returns Qt-String message from winscard long error code
  //! \param[in] code long error code    
  //! \return Qt-string error message from errorMessages map or message from winAPI FormatMessage function
  static QString getMessage(long code); 
private:
 static QMap<long,QLatin1String> errorMessages;//!< map contains error messages to winscard codes
};

#endif //SCARDEXCEPTION_H