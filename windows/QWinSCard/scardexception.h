#ifndef SCARDEXCEPTION_H
#define SCARDEXCEPTION_H

#include "baseexceptions.h"

class SCardException : public BaseExceptions
{
	public:
		explicit SCardException(long id) noexcept;
  virtual ~SCardException() noexcept;
  static QString getMessage(long code); 
private:
 static QMap<long,QLatin1String> errorMessages;
};

#endif //SCARDEXCEPTION_H