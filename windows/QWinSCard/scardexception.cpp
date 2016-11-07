#include <QtCore/qmap.h>
#include <QtCore/QObject>

#include "scardexception.h"
#include "winscarderr.h"

QMap<long, QLatin1String> SCardException::errorMessages(
{ { Smartcards::SUCCESS,QLatin1String("No error was encountered.") },
  { Smartcards::INTERNAL_ERROR,QLatin1String("An internal consistency check failed.")},
  { Smartcards::INVALID_HANDLE,QLatin1String("The supplied handle was not valid.") },
  { Smartcards::INVALID_PARAMETER,QLatin1String("One or more of the supplied parameters could not be properly interpreted.") },
  { Smartcards::CANCELLED,QLatin1String("The action was canceled by an SCardCancel request.") },
  { Smartcards::INVALID_TARGET,QLatin1String("Registry startup information is missing or not valid.") },
  { Smartcards::NO_MEMORY,QLatin1String("Not enough memory available to complete this command.") },
  { Smartcards::WAITED_TOO_LONG,QLatin1String("An internal consistency timer has expired.") },
  { Smartcards::INSUFFICIENT_BUFFER,QLatin1String("The data buffer for returned data is too small for the returned data.") },
  { Smartcards::UNKNOWN_READER,QLatin1String("The specified reader name is not recognized.") },
  { Smartcards::TIMEOUT,QLatin1String("The user-specified time-out value has expired.") },
  { Smartcards::SHARING_VIOLATION,QLatin1String("The smart card cannot be accessed because of other outstanding connections.") },
  { Smartcards::NO_SMARTCARD,QLatin1String("The operation requires a smart card, but no smart card is currently in the device.") },
  { Smartcards::UNKNOWN_CARD,QLatin1String("The specified smart card name is not recognized.") },
  { Smartcards::CANT_DISPOSE,QLatin1String("The system could not dispose of the media in the requested manner.") },
  { Smartcards::PROTO_MISMATCH,QLatin1String("The requested protocols are incompatible with the protocol currently in use with the card.") },
  { Smartcards::NOT_READY,QLatin1String("The reader or card is not ready to accept commands.") },
  { Smartcards::INVALID_VALUE,QLatin1String("One or more of the supplied parameter values could not be properly interpreted.") },
  { Smartcards::SYSTEM_CANCELLED,QLatin1String("The action was canceled by the system, presumably to log off or shut down.") },
  { Smartcards::COMM_ERROR,QLatin1String("An internal communications error has been detected.") },
  { Smartcards::UNKNOWN_ERROR,QLatin1String("An internal error has been detected, but the source is unknown.") },
  { Smartcards::INVALID_ATR,QLatin1String("An ATR string obtained from the registry is not a valid ATR string.") },
  { Smartcards::NOT_TRANSACTED,QLatin1String("An attempt was made to end a nonexistent transaction.") },
  { Smartcards::READER_UNAVAILABLE,QLatin1String("The specified reader is not currently available for use.") },
  { Smartcards::SHUTDOWN,QLatin1String("The operation has been aborted to allow the server application to exit.") },
  { Smartcards::PCI_TOO_SMALL,QLatin1String("The PCI receive buffer was too small.") },
  { Smartcards::READER_UNSUPPORTED,QLatin1String("The reader driver does not meet minimal requirements for support.") },
  { Smartcards::DUPLICATE_READER,QLatin1String("The reader driver did not produce a unique reader name.") },
  { Smartcards::CARD_UNSUPPORTED,QLatin1String("The smart card does not meet minimal requirements for support.") },
  { Smartcards::NO_SERVICE,QLatin1String("The Smart Card Resource Manager is not running.") },
  { Smartcards::CARD_SERVICE_STOPPED,QLatin1String("The Smart Card Resource Manager has shut down.") },
  { Smartcards::UNEXPECTED,QLatin1String("An unexpected card error has occurred.") },
  { Smartcards::ICC_INSTALLATION,QLatin1String("No primary provider can be found for the smart card.") },
  { Smartcards::ICC_CREATEORDER,QLatin1String("The requested order of object creation is not supported.") },
  { Smartcards::UNSUPPORTED_FEATURE,QLatin1String("This smart card does not support the requested feature.") },
  { Smartcards::DIR_NOT_FOUND,QLatin1String("The specified directory does not exist in the smart card.") },
  { Smartcards::FILE_NOT_FOUND,QLatin1String("The specified file does not exist in the smart card.") },
  { Smartcards::NO_DIR,QLatin1String("The supplied path does not represent a smart card directory.") },
  { Smartcards::NO_FILE,QLatin1String("The supplied path does not represent a smart card file.") },
  { Smartcards::NO_ACCESS,QLatin1String("Access is denied to the file.") },
  { Smartcards::WRITE_TOO_MANY,QLatin1String("An attempt was made to write more data than would fit in the target object.") },
  { Smartcards::BAD_SEEK,QLatin1String("An error occurred in setting the smart card file object pointer.") },
  { Smartcards::INVALID_CHV,QLatin1String("The supplied PIN is incorrect.") },
  { Smartcards::UNKNOWN_RES_MNG,QLatin1String("An unrecognized error code was returned.") },
  { Smartcards::NO_SUCH_CERTIFICATE,QLatin1String("The requested certificate does not exist.") },
  { Smartcards::CERTIFICATE_UNAVAILABLE,QLatin1String("The requested certificate could not be obtained.") },
  { Smartcards::NO_READERS_AVAILABLE,QLatin1String("Cannot find a smart card reader.") },
  { Smartcards::COMM_DATA_LOST,QLatin1String("A communications error with the smart card has been detected.") },
  { Smartcards::NO_KEY_CONTAINER,QLatin1String("The requested key container does not exist on the smart card.") },
  { Smartcards::SERVER_TOO_BUSY,QLatin1String("The Smart Card Resource Manager is too busy to complete this operation.") },
  { Smartcards::UNSUPPORTED_CARD,QLatin1String("The reader cannot communicate with the card, due to ATR string configuration conflicts.") },
  { Smartcards::UNRESPONSIVE_CARD,QLatin1String("The smart card is not responding to a reset.") },
  { Smartcards::UNPOWERED_CARD,QLatin1String("Power has been removed from the smart card, so that further communication is not possible.") },
  { Smartcards::RESET_CARD,QLatin1String("The smart card was reset.") },
  { Smartcards::REMOVED_CARD,QLatin1String("The smart card has been removed, so further communication is not possible.") },
  { Smartcards::SECURITY_VIOLATION,QLatin1String("Access was denied because of a security violation.") },
  { Smartcards::WRONG_CHV,QLatin1String("The card cannot be accessed because the wrong PIN was presented.") },
  { Smartcards::CHV_BLOCKED,QLatin1String("The card cannot be accessed because the maximum number of PIN entry attempts has been reached.") },
  { Smartcards::CARD_EOF,QLatin1String("The end of the smart card file has been reached.") },
  { Smartcards::CANCELLED_BY_USER,QLatin1String("The action was canceled by the user.") },
  { Smartcards::CARD_NOT_AUTHENTICATED,QLatin1String("No PIN was presented to the smart card.") },
  { Smartcards::CACHE_ITEM_NOT_FOUND,QLatin1String("The requested item could not be found in the cache.") },
  { Smartcards::CACHE_ITEM_STALE,QLatin1String("The requested cache item is too old and was deleted from the cache.") },
  #if _MSC_VER>1700
   { Smartcards::PIN_CACHE_EXPIRED,QLatin1String("The smart card PIN cache has expired.") },
   { Smartcards::NO_PIN_CACHE,QLatin1String("The smart card PIN cannot be cached.") },
   { Smartcards::READ_ONLY_CARD,QLatin1String("The smart card is read only and cannot be written to.") },
   { Smartcards::CACHE_ITEM_TOO_BIG,QLatin1String("The new cache item exceeds the maximum per-item size defined for the cache.") }
  #endif
}
);

SCardException::SCardException(long code) noexcept
	: BaseExceptions(QObject::tr("WinSCard Error:%1:%2").arg(getMessage(code)).arg(code))
{
 BaseExceptions::setErrorCode(code);
}

SCardException::~SCardException() noexcept
{

}

QString SCardException::getMessage(long code)
{
 if (errorMessages.contains(code))
  return errorMessages.value(code);
 wchar_t *err;
 DWORD len = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr, static_cast<DWORD>(code), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&err), 0, nullptr);
 if (len == 0)
  return QString(QStringLiteral("Unexpected error."));
 QString ret = QString::fromWCharArray(err, static_cast<int>(len));
 LocalFree(err);
 return ret;
}
