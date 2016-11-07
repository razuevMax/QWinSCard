#ifndef NATIVESCARD_H
#define NATIVESCARD_H

#include "APDUResponse.h"
#include "APDUCommand.h"
#include "WinSCardErr.h"
#include <QtCore/qmap.h>
#include <QtCore/quuid.h>

class CardEvent;

namespace Smartcards
{
  /// <summary>
  /// SCOPE context
  /// </summary>
  enum SCOPE : DWORD
  {
   /// <summary>
   /// The context is a user context, and any database operations are performed within the
   /// domain of the user.
   /// </summary>
   User,

   /// <summary>
   /// The context is that of the current terminal, and any database operations are performed
   /// within the domain of that terminal.  (The calling application must have appropriate
   /// access permissions for any database actions.)
   /// </summary>
   Terminal,

   /// <summary>
   /// The context is the system context, and any database operations are performed within the
   /// domain of the system.  (The calling application must have appropriate access
   /// permissions for any database actions.)
   /// </summary>
   System
  };
  /// <summary>
  /// SHARE mode enumeration
  /// </summary>
  enum SHARE : DWORD
  {
   /// <summary>
   /// This application is not willing to share this card with other applications.
   /// </summary>
   Exclusive = 1,

   /// <summary>
   /// This application is willing to share this card with other applications.
   /// </summary>
   Shared,

   /// <summary>
   /// This application demands direct control of the reader, so it is not available to other applications.
   /// </summary>
   Direct
  };


  /// <summary>
  /// PROTOCOL enumeration
  /// </summary>
  enum	PROTOCOL : DWORD
  {
   /// <summary>
   /// There is no active protocol.
   /// </summary>
   Undefined = 0x00000000,

   /// <summary>
   /// T=0 is the active protocol.
   /// </summary>
   T0 = 0x00000001,

   /// <summary>
   /// T=1 is the active protocol.
   /// </summary>
   T1 = 0x00000002,

   /// <summary>
   /// T=1 or T=0 can be the active protocol
   /// </summary>
   T0orT1 = T0 | T1
  };


  /// <summary>
  /// DISCONNECT action enumeration
  /// </summary>
  enum DISCONNECT : DWORD
  {
   /// <summary>
   /// Don't do anything special on close
   /// </summary>
   Leave,

   /// <summary>
   /// Reset the card on close
   /// </summary>
   Reset,

   /// <summary>
   /// Power down the card on close
   /// </summary>
   Unpower,

   /// <summary>
   /// Eject(!) the card on close
   /// </summary>
   Eject
  };
/// <summary>
/// Values for AttrId of SCardGetAttrib
/// </summary>
class SCARD_ATTR_VALUE
{
 static const DWORD SCARD_CLASS_COMMUNICATIONS_CONST = 2;
 static const DWORD SCARD_CLASS_PROTOCOL_CONST = 3;
 static const DWORD SCARD_CLASS_MECHANICAL_CONST = 6;
 static const DWORD SCARD_CLASS_VENDOR_DEFINED_CONST = 7;
 static const DWORD SCARD_CLASS_IFD_PROTOCOL_CONST = 8;
 static const DWORD SCARD_CLASS_ICC_STATE_CONST = 9;
 static const DWORD SCARD_CLASS_SYSTEM_CONST = 0x7fff;
public:
	static const DWORD CHANNEL_ID=SCARD_CLASS_COMMUNICATIONS_CONST<<16|0x0110;
	static const DWORD CHARACTERISTICS=SCARD_CLASS_MECHANICAL_CONST<<16|0x0150;
	static const DWORD CURRENT_PROTOCOL_TYPE=SCARD_CLASS_IFD_PROTOCOL_CONST<<16|0x0201;
	static const DWORD DEVICE_UNIT=SCARD_CLASS_SYSTEM_CONST<<16|0x0001;
	static const DWORD DEVICE_FRIENDLY_NAME=SCARD_CLASS_SYSTEM_CONST<<16|0x0003;
	static const DWORD DEVICE_SYSTEM_NAME=SCARD_CLASS_SYSTEM_CONST<<16|0x0004;
	static const DWORD ICC_PRESENCE=SCARD_CLASS_ICC_STATE_CONST<<16|0x0300;
	static const DWORD ICC_INTERFACE_STATUS=SCARD_CLASS_ICC_STATE_CONST<<16|0x0301;
	static const DWORD ATR_STRING=SCARD_CLASS_ICC_STATE_CONST<<16|0x0303;
	static const DWORD ICC_TYPE_PER_ATR=SCARD_CLASS_ICC_STATE_CONST<<16|0x0304;
	static const DWORD PROTOCOL_TYPES=SCARD_CLASS_PROTOCOL_CONST<<16|0x0120;
	static const DWORD VENDOR_NAME=SCARD_CLASS_VENDOR_DEFINED_CONST<<16|0x0100;
	static const DWORD VENDOR_IFD_TYPE=SCARD_CLASS_VENDOR_DEFINED_CONST<<16|0x0101;
	static const DWORD VENDOR_IFD_VERSION=SCARD_CLASS_VENDOR_DEFINED_CONST<<16|0x0102;
	static const DWORD VENDOR_IFD_SERIAL_NO=SCARD_CLASS_VENDOR_DEFINED_CONST<<16|0x0103;
};

/// <summary>
/// CARD_STATE enumeration, used by the PC/SC function SCardGetStatusChanged
/// </summary>
enum CARD_STATUS_STATE :DWORD
{
 Unaware = 0x00000000,
 Ignore = 0x00000001,
 Changed = 0x00000002,
 Unknown = 0x00000004,
 Unvailable = 0x00000008,
 Empty = 0x00000010,
 Present = 0x00000020,
 ATRMatch = 0x00000040,
 ExclusiveStatus = 0x00000080,
 InUse = 0x00000100,
 Mute = 0x00000200,
 Unpowered = 0x00000400
};

enum CARD_STATE :DWORD
{
 Absent = SCARD_ABSENT,
 PresentState = SCARD_PRESENT,
 Swallowed = SCARD_SWALLOWED,
 Powered = SCARD_POWERED,
 Negotable = SCARD_NEGOTIABLE,
 Specific = SCARD_SPECIFIC
};

class WinSCard;

class ReadersStates
{
public:
 friend class WinSCard;
 ReadersStates(void);
 ReadersStates(const std::initializer_list<std::pair<QString, DWORD>>& ilist);
 ReadersStates(PSCARD_READERSTATEW pReaderStates, DWORD cbReaderStates);
 ReadersStates(const QMap<QString, SCARD_READERSTATEW>& readerStates);
 ReadersStates(const QVector<SCARD_READERSTATEW>& readerStates);
 ~ReadersStates(void);
 void assign(const QVector<SCARD_READERSTATEW>& readerStates);
 void addNewReader(void);
 void removeNewReader(void);
 void add(const QString& readerName, DWORD currentState, LPVOID userData = nullptr, DWORD eventState = 0);
 void add(const QString& readerName, DWORD currentState, const QByteArray& ATR, LPVOID userData = nullptr, DWORD eventState = 0);
 bool remove(const QString& readerName);
 DWORD currentState(const QString& readerName);
 DWORD eventState(const QString& readerName);
 QByteArray ATR(const QString& readerName);
 LPVOID userData(const QString& readerName);
 void setCurrentState(const QString& readerName, DWORD state);
 void setEventState(const QString& readerName, DWORD state);
 void setATR(const QString& readerName, const QByteArray& ATR);
 void setUserData(const QString& readerName, LPVOID userData);
 bool readerPresent(const QString& readerName) const;
private:
 void addDefault(const QString& readerName);
 static wchar_t* toWCharArray(const QString& str);
 QMap<QString, SCARD_READERSTATEW> mReaderStates;
};

class WinSCard
{
public:
 static const DWORD ATR_LENGTH = SCARD_ATR_LENGTH;
 static const DWORD AUTOALLOCATE = SCARD_AUTOALLOCATE;
 static const DWORD RESPONSE_MAX_LENGTH=0x0200;
 static const DWORD CCID_READER_PRESENT_STATE = CARD_STATUS_STATE::Unknown | CARD_STATUS_STATE::Ignore | CARD_STATUS_STATE::Changed;
 static const DWORD CCID_READER_EMPTY_STATE = CARD_STATUS_STATE::Unvailable | CARD_STATUS_STATE::Ignore | CARD_STATUS_STATE::Changed;
 static const QString ALL_READERS;
 static const QString DEFAULT_READERS;
 static const QString LOCAL_READERS;
 static const QString SYSTEM_READERS;
 WinSCard(void);
	virtual ~WinSCard(void);
 //class data functions
 SCARDCONTEXT context(void) const { return m_hContext; }
 SCARDHANDLE handle(void) const { return m_hCard; }
 void throwErrors(bool isThrowingErrors) { m_bThrowingErrors=isThrowingErrors; }
 bool isThrowingErrors(void) const { return m_bThrowingErrors; }
 long lastError(void) const { return m_nLastError; }
 bool isError(void) const { return m_nLastError!= SUCCESS; }
 //Smart Card Database Query Functions
	/// <summary>
	/// Wraps the PCSC function
	/// LONG SCardListReaders(SCARDCONTEXT hContext, 
	///		LPCTSTR mszGroups, 
	///		LPTSTR mszReaders, 
	///		LPDWORD pcchReaders 
	///	);
	/// </summary>
	/// <returns>A string array of the readers</returns>
 virtual QStringList ListReaders(void);
 virtual QStringList ListReaderGroups(void);
 virtual QUuid GetProviderId(const QString& cardName);
 virtual QStringList ListCards(const QByteArray& cardAtr, const QVector<QUuid>& guidInterfaces);
 virtual QStringList ListCards(const QVector<QUuid>& guidInterfaces);
 virtual QStringList ListCards(const QByteArray& cardAtr);
 virtual QStringList ListCards(void);
 virtual QVector<QUuid> ListInterfaces(const QString& cardName);
 //Smart Card Database Management Functions
 virtual void AddReaderToGroup(const QString& ReaderName, const QString& GroupName);
 virtual void ForgetCardType(const QString& cardName);
 virtual void ForgetReader(const QString& ReaderName);
 virtual void ForgetReaderGroup(const QString& GroupName);
 virtual void IntroduceReader(const QString& ReaderName, const QString& DeviceName);
 virtual void IntroduceReaderGroup(const QString& GroupName);
 virtual void RemoveReaderFromGroup(const QString& ReaderName, const QString& GroupName);
 virtual void IntroduceCardType(const QString& cardName, const QVector<QUuid>& guidInterfaces, const QByteArray& cardAtr, const QByteArray& cardAtrMask, QUuid *pguidPrimaryProvider = nullptr);
 virtual void IntroduceCardType(const QString& cardName, const QVector<QUuid>& guidInterfaces, const QByteArray& cardAtr, QUuid *pguidPrimaryProvider = nullptr);
 virtual void IntroduceCardType(const QString& cardName, const QByteArray& cardAtr, const QByteArray& cardAtrMask, QUuid *pguidPrimaryProvider = nullptr);
 virtual void IntroduceCardType(const QString& cardName, const QByteArray& cardAtr, QUuid *pguidPrimaryProvider = nullptr);
 //Resource Manager Context Functions
 /// <summary>
 /// Wraps the PCSC function 
 /// LONG SCardEstablishContext(
 ///		IN DWORD dwScope,
 ///		IN LPCVOID pvReserved1,
 ///		IN LPCVOID pvReserved2,
 ///		OUT LPSCARDCONTEXT phContext
 ///	);
 /// </summary>
 /// <param name="Scope"></param>
 /// 
 virtual void EstablishContext(SCOPE Scope);
 virtual bool isContextEstablished(void) const { return mContextEstablished; }
 virtual bool isContextValid(void);
 /// <summary>
 /// Wraps the PCSC function
 /// LONG SCardReleaseContext(
 ///		IN SCARDCONTEXT hContext
 ///	);
 /// </summary>
 virtual void ReleaseContext(void);
 //Smart Card and Reader Access Functions
 /// <summary>
 ///  Wraps the PCSC function
 ///  LONG SCardConnect(
 ///		IN SCARDCONTEXT hContext,
 ///		IN LPCTSTR szReader,
 ///		IN DWORD dwShareMode,
 ///		IN DWORD dwPreferredProtocols,
 ///		OUT LPSCARDHANDLE phCard,
 ///		OUT LPDWORD pdwActiveProtocol
 ///	);
 /// </summary>
 /// <param name="Reader"></param>
 /// <param name="ShareMode"></param>
 /// <param name="PreferredProtocols"></param>
 virtual long Connect(const QString& Reader, SHARE ShareMode, PROTOCOL PreferredProtocols);
 virtual PROTOCOL Reconnect(SHARE ShareMode, PROTOCOL PreferredProtocols, DISCONNECT Initialization);
 /// <summary>
 /// Wraps the PCSC function
 ///	LONG SCardDisconnect(
 ///		IN SCARDHANDLE hCard,
 ///		IN DWORD dwDisposition
 ///	);
 /// </summary>
 /// <param name="Disposition"></param>
 virtual	void Disconnect(DISCONNECT Disposition);
 virtual bool isConnected(void) const { return mCardConnected; }
 /// <summary>
 /// Wraps the PCSC function
 /// LONG SCardTransmit(
 ///		SCARDHANDLE hCard,
 ///		LPCSCARD_I0_REQUEST pioSendPci,
 ///		LPCBYTE pbSendBuffer,
 ///		DWORD cbSendLength,
 ///		LPSCARD_IO_REQUEST pioRecvPci,
 ///		LPBYTE pbRecvBuffer,
 ///		LPDWORD pcbRecvLength
 ///	);
 /// </summary>
 /// <param name="ApduCmd">APDUCommand object with the APDU to send to the card</param>
 /// <returns>An APDUResponse object with the response from the card</returns>
 virtual APDUResponse Transmit(APDUCommand ApduCmd);
 virtual QByteArray GetCardStatus(DWORD& state, DWORD& protocol);
 /// <summary>
 /// Wraps the PSCS function
 /// LONG SCardBeginTransaction(
 ///     SCARDHANDLE hCard
 //  );
 /// </summary>
 /// 
 virtual void BeginTransaction(void);
 /// <summary>
 /// Wraps the PCSC function
 /// LONG SCardEndTransaction(
 ///     SCARDHANDLE hCard,
 ///     DWORD dwDisposition
 /// );
 /// </summary>
 /// <param name="Disposition">A value from DISCONNECT enum</param>
 virtual void EndTransaction(DISCONNECT Disposition);
 //Direct Card Access Functions
 /// <summary>
 /// Gets the attributes of the card
 /// </summary>
 /// <param name="AttribId">Identifier for the Attribute to get</param>
 /// <returns>Attribute content</returns>
 virtual QByteArray GetAttribute(DWORD AttribId);
 virtual void SetAttribute(DWORD AttribId, const QByteArray& attr);
 virtual bool CardControl(DWORD controlCode, const QByteArray& inData, QByteArray& outData);
 virtual bool CardControl(DWORD controlCode, const QByteArray& inData);
 virtual DWORD CardControl(DWORD controlCode, LPVOID pInData, DWORD cInData, LPVOID pOutData, DWORD cOutData);
 //Smart Card Tracking Functions
 virtual void GetStatusChange(DWORD dwTimeout, ReadersStates& readerStates);
 virtual void LocateCards(const QStringList& cards, ReadersStates& readerStates);
 virtual void LocateCardsByATR(const QVector<QPair<QByteArray, QByteArray>>& atrMask, ReadersStates& readerStates);
 virtual void CardCancel(void);
private:
 SCARDCONTEXT	m_hContext{ 0 };
 SCARDHANDLE m_hCard{ 0 };
 DWORD	m_nProtocol{ T0 };
 bool mContextEstablished{ false };
 bool mCardConnected{ false };
 long	m_nLastError{ SUCCESS };
 APDUCommand mLastCommand{ 0,0,0,0 };
 APDUResponse mLastResponse{ 0,0 };
 bool m_bThrowingErrors{ true };
};

}
#endif