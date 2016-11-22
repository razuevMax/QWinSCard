//! \file winscarderr.h
//! \brief Header file contains enumerations of winscard error codes.
#ifndef WINSCARDERR_H
#define WINSCARDERR_H

#include <winerror.h>
#include <winscard.h>

//! \namespace Smartcards
//! \brief Namespace of smartcard classes
namespace Smartcards
{
 //! An enumeration of the source of the error. New facilities are occasionally added by Microsoft.
 enum FACILITY_CODES:DWORD
 {
  SYSTEM_FACILITY=FACILITY_NULL,//!< Default system facility
  SCARD_FACILITY=FACILITY_SCARD//!< Smart card facility
 };

 //! An enumeration of the winscard error codes.
 enum ERRCODES : long
 {
 SUCCESS=SCARD_S_SUCCESS,//!< Success or no error.
 INTERNAL_ERROR=SCARD_F_INTERNAL_ERROR,//!< Internal error.(0x80100001)
 CANCELLED=SCARD_E_CANCELLED,//!< The action was canceled by an SCardCancel request.(0x80100002)
 INVALID_HANDLE=SCARD_E_INVALID_HANDLE,//!< The supplied handle was not valid.(0x80100003)
 INVALID_PARAMETER=SCARD_E_INVALID_PARAMETER,//!< One or more of the supplied parameters could not be properly interpreted.(0x80100004)
 INVALID_TARGET=SCARD_E_INVALID_TARGET,//!< Registry startup information is missing or not valid.(0x80100005)
 NO_MEMORY=SCARD_E_NO_MEMORY,//!< Not enough memory available to complete this command.(0x80100006)
 WAITED_TOO_LONG=SCARD_F_WAITED_TOO_LONG,//!< An internal consistency timer has expired.(0x80100007)
 INSUFFICIENT_BUFFER=SCARD_E_INSUFFICIENT_BUFFER,//!< The data buffer for returned data is too small for the returned data.(0x80100008)
 UNKNOWN_READER=SCARD_E_UNKNOWN_READER,//!< The specified reader name is not recognized.(0x80100009)
 TIMEOUT=SCARD_E_TIMEOUT,//!< The user-specified time-out value has expired.(0x8010000A)
 SHARING_VIOLATION=SCARD_E_SHARING_VIOLATION,//!< The smart card cannot be accessed because of other outstanding connections.(0x8010000B)
 NO_SMARTCARD=SCARD_E_NO_SMARTCARD,//!< The operation requires a smart card, but no smart card is currently in the device.(0x8010000C)
 UNKNOWN_CARD=SCARD_E_UNKNOWN_CARD,//!< The specified smart card name is not recognized.(0x8010000D)
 CANT_DISPOSE=SCARD_E_CANT_DISPOSE,//!< The system could not dispose of the media in the requested manner.(0x8010000E)
 PROTO_MISMATCH=SCARD_E_PROTO_MISMATCH,//!< The requested protocols are incompatible with the protocol currently in use with the card.(0x8010000F)
 NOT_READY=SCARD_E_NOT_READY,//!< The reader or card is not ready to accept commands.(0x80100010)
 INVALID_VALUE=SCARD_E_INVALID_VALUE,//!< One or more of the supplied parameter values could not be properly interpreted.(0x80100011)
 SYSTEM_CANCELLED=SCARD_E_SYSTEM_CANCELLED,//!< The action was canceled by the system, presumably to log off or shut down.(0x80100012)
 COMM_ERROR=SCARD_F_COMM_ERROR,//!< An internal communications error has been detected.(0x80100013)
 UNKNOWN_ERROR=SCARD_F_UNKNOWN_ERROR,//!< An internal error has been detected, but the source is unknown.(0x80100014)
 INVALID_ATR=SCARD_E_INVALID_ATR,//!< An ATR string obtained from the registry is not a valid ATR string.(0x80100015)
 NOT_TRANSACTED=SCARD_E_NOT_TRANSACTED,//!< An attempt was made to end a nonexistent transaction.(0x80100016)
 READER_UNAVAILABLE=SCARD_E_READER_UNAVAILABLE,//!< The specified reader is not currently available for use.(0x80100017)
 SHUTDOWN=SCARD_P_SHUTDOWN,//!< The operation has been aborted to allow the server application to exit.(0x80100018)
 PCI_TOO_SMALL=SCARD_E_PCI_TOO_SMALL,//!< The PCI receive buffer was too small.(0x80100019)
 READER_UNSUPPORTED=SCARD_E_READER_UNSUPPORTED,//!< The reader driver does not meet minimal requirements for support.(0x8010001A)
 DUPLICATE_READER=SCARD_E_DUPLICATE_READER,//!< The reader driver did not produce a unique reader name.(0x8010001B)
 CARD_UNSUPPORTED=SCARD_E_CARD_UNSUPPORTED,//!< The smart card does not meet minimal requirements for support.(0x8010001C)
 NO_SERVICE=SCARD_E_NO_SERVICE,//!< The Smart Card Resource Manager is not running.(0x8010001D)
 CARD_SERVICE_STOPPED=SCARD_E_SERVICE_STOPPED,//!< The Smart Card Resource Manager has shut down.(0x8010001E)
 UNEXPECTED=SCARD_E_UNEXPECTED,//!< An unexpected card error has occurred.(0x8010001F)
 ICC_INSTALLATION=SCARD_E_ICC_INSTALLATION,//!< No primary provider can be found for the smart card.(0x80100020)
 ICC_CREATEORDER=SCARD_E_ICC_CREATEORDER,//!< The requested order of object creation is not supported.(0x80100021)
 UNSUPPORTED_FEATURE=SCARD_E_UNSUPPORTED_FEATURE,//!< This smart card does not support the requested feature.(0x80100022)
 DIR_NOT_FOUND=SCARD_E_DIR_NOT_FOUND,//!< The specified directory does not exist in the smart card.(0x80100023)
 FILE_NOT_FOUND=SCARD_E_FILE_NOT_FOUND,//!< The specified file does not exist in the smart card.(0x80100024)
 NO_DIR=SCARD_E_NO_DIR,//!< The supplied path does not represent a smart card directory.(0x80100025)
 NO_FILE=SCARD_E_NO_FILE,//!< The supplied path does not represent a smart card file.(0x80100026)
 NO_ACCESS=SCARD_E_NO_ACCESS,//!< Access is denied to the file.(0x80100027)
 WRITE_TOO_MANY=SCARD_E_WRITE_TOO_MANY,//!< An attempt was made to write more data than would fit in the target object.(0x80100028)
 BAD_SEEK=SCARD_E_BAD_SEEK,//!< An error occurred in setting the smart card file object pointer.(0x80100029)
 INVALID_CHV=SCARD_E_INVALID_CHV,//!< The supplied PIN is incorrect.(0x8010002A)
 UNKNOWN_RES_MNG =SCARD_E_UNKNOWN_RES_MNG,//!< An unrecognized error code was returned.(0x8010002B)
 NO_SUCH_CERTIFICATE =SCARD_E_NO_SUCH_CERTIFICATE,//!< The requested certificate does not exist.(0x8010002C)
 CERTIFICATE_UNAVAILABLE =SCARD_E_CERTIFICATE_UNAVAILABLE,//!< The requested certificate could not be obtained.(0x8010002D)
 NO_READERS_AVAILABLE =SCARD_E_NO_READERS_AVAILABLE,//!< Cannot find a smart card reader.(0x8010002E)
 COMM_DATA_LOST =SCARD_E_COMM_DATA_LOST,//!< A communications error with the smart card has been detected.(0x8010002F)
 NO_KEY_CONTAINER =SCARD_E_NO_KEY_CONTAINER,//!< The requested key container does not exist on the smart card.(0x80100030)
 SERVER_TOO_BUSY =SCARD_E_SERVER_TOO_BUSY,//!< The Smart Card Resource Manager is too busy to complete this operation.(0x80100031)
 UNSUPPORTED_CARD =SCARD_W_UNSUPPORTED_CARD,//!< The reader cannot communicate with the card, due to ATR string configuration conflicts.(0x80100065)
 UNRESPONSIVE_CARD =SCARD_W_UNRESPONSIVE_CARD,//!< The smart card is not responding to a reset.(0x80100066)
 UNPOWERED_CARD =SCARD_W_UNPOWERED_CARD,//!< Power has been removed from the smart card, so that further communication is not possible.(0x80100067)
 RESET_CARD =SCARD_W_RESET_CARD,//!< The smart card was reset.(0x80100068)
 REMOVED_CARD =SCARD_W_REMOVED_CARD,//!< The smart card has been removed, so further communication is not possible.(0x80100069)
 SECURITY_VIOLATION =SCARD_W_SECURITY_VIOLATION,//!< Access was denied because of a security violation.(0x8010006A)
 WRONG_CHV =SCARD_W_WRONG_CHV,//!< The card cannot be accessed because the wrong PIN was presented.(0x8010006B)
 CHV_BLOCKED =SCARD_W_CHV_BLOCKED,//!< The card cannot be accessed because the maximum number of PIN entry attempts has been reached.(0x8010006C)
 CARD_EOF =SCARD_W_EOF,//!< The end of the smart card file has been reached.(0x8010006D)
 CANCELLED_BY_USER =SCARD_W_CANCELLED_BY_USER,//!< The action was canceled by the user.(0x8010006E)
 CARD_NOT_AUTHENTICATED =SCARD_W_CARD_NOT_AUTHENTICATED,//!< No PIN was presented to the smart card.(0x8010006F)
 CACHE_ITEM_NOT_FOUND =SCARD_W_CACHE_ITEM_NOT_FOUND,//!< The requested item could not be found in the cache.(0x80100070)
 CACHE_ITEM_STALE =SCARD_W_CACHE_ITEM_STALE,//!< The requested cache item is too old and was deleted from the cache.(0x80100071)
#if _MSC_VER>1700
 PIN_CACHE_EXPIRED =SCARD_E_PIN_CACHE_EXPIRED,//!< The smart card PIN cache has expired.(0x80100032)
 NO_PIN_CACHE =SCARD_E_NO_PIN_CACHE,//!< The smart card PIN cannot be cached.(0x80100033)
 READ_ONLY_CARD =SCARD_E_READ_ONLY_CARD,//!< The smart card is read only and cannot be written to.(0x80100034)
 CACHE_ITEM_TOO_BIG =SCARD_W_CACHE_ITEM_TOO_BIG,//!< The new cache item exceeds the maximum per-item size defined for the cache.(0x80100072)
#endif
 // Standard codes                                               
 SYSTEM_INVALID_HANDLE = ERROR_INVALID_HANDLE,//!< System error. Invalid handle.(0x6)                                     
 SYSTEM_NOT_SUPPORTED = ERROR_NOT_SUPPORTED//!< System error. The request is not supported.(0x80070032)
};

}//namespace Smartcards

#endif
