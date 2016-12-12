//! \file NativeSCard.h
//! \brief Header file for WinSCard class.
//! \details File is contains source of class WinSCard must implement a winscard api.
#include <winscard.h>
#include <QtCore>
#include "NativeSCard.h"
#include "scardexception.h"

using namespace Smartcards;

const QString WinSCard::ALL_READERS = QString::fromWCharArray(SCARD_ALL_READERS);
const QString WinSCard::DEFAULT_READERS = QString::fromWCharArray(SCARD_DEFAULT_READERS);
const QString WinSCard::LOCAL_READERS = QString::fromWCharArray(SCARD_LOCAL_READERS);
const QString WinSCard::SYSTEM_READERS = QString::fromWCharArray(SCARD_SYSTEM_READERS);

ReadersStates::ReadersStates()
{
}

ReadersStates::ReadersStates(const std::initializer_list<std::pair<QString, DWORD>>& ilist)
{
 for (auto& item : ilist)
  add(item.first, item.second);
}

ReadersStates::ReadersStates(PSCARD_READERSTATEW pReaderStates, DWORD cbReaderStates)
{
 for (DWORD idx = 0; idx<cbReaderStates; idx++)
  mReaderStates.insert(QString::fromWCharArray(pReaderStates[idx].szReader),pReaderStates[idx]);
}

ReadersStates::ReadersStates(const QMap<QString, SCARD_READERSTATEW>& readerStates)
 :mReaderStates(readerStates)
{
}

ReadersStates::ReadersStates(const QVector<SCARD_READERSTATEW>& readerStates)
{
 for (auto item : readerStates)
  mReaderStates.insert(QString::fromWCharArray(item.szReader), item);
}

ReadersStates::~ReadersStates()
{
 //Release memory before delete map
 for (auto& item : mReaderStates)
  delete[] item.szReader;
}

void ReadersStates::assign(const QVector<SCARD_READERSTATEW>& readerStates)
{
 mReaderStates.clear();
 for (auto item : readerStates)
  mReaderStates.insert(QString::fromWCharArray(item.szReader), item);
}

void ReadersStates::addNewReader()
{
 addDefault("\\\\?PnP?\\Notification");
}

void ReadersStates::removeNewReader()
{
 remove("\\\\?PnP?\\Notification");
}

DWORD ReadersStates::currentState(const QString& readerName)
{
 if(!mReaderStates.contains(readerName))
  addDefault(readerName);
 return mReaderStates.value(readerName).dwCurrentState;
}

DWORD ReadersStates::eventState(const QString& readerName)
{
 if(!mReaderStates.contains(readerName))
  addDefault(readerName);
 return mReaderStates.value(readerName).dwEventState;
}

QByteArray ReadersStates::ATR(const QString& readerName)
{
 if(!mReaderStates.contains(readerName))
  addDefault(readerName);
 LPCBYTE pAtr = mReaderStates.value(readerName).rgbAtr;
 DWORD cbAtr = mReaderStates.value(readerName).cbAtr; 
 return QByteArray::fromRawData(reinterpret_cast<const char*>(pAtr),static_cast<int>(cbAtr));
}

LPVOID ReadersStates::userData(const QString& readerName)
{
 if (!mReaderStates.contains(readerName))
  addDefault(readerName);
 return mReaderStates.value(readerName).pvUserData;
}

void ReadersStates::setCurrentState(const QString& readerName, DWORD state)
{
 if (!mReaderStates.contains(readerName))
  addDefault(readerName);
 mReaderStates[readerName].dwCurrentState=state;
}

void ReadersStates::setEventState(const QString& readerName, DWORD state)
{
 if (!mReaderStates.contains(readerName))
  addDefault(readerName);
 mReaderStates[readerName].dwEventState = state;
}

void ReadersStates::setATR(const QString& readerName, const QByteArray& ATR)
{
 if (!mReaderStates.contains(readerName))
  addDefault(readerName);
 mReaderStates[readerName].cbAtr = static_cast<DWORD>(ATR.size());
 qCopy(ATR.begin(), ATR.end(), mReaderStates[readerName].rgbAtr);
}

void ReadersStates::setUserData(const QString& readerName, LPVOID userData)
{
 if (!mReaderStates.contains(readerName))
  addDefault(readerName);
 mReaderStates[readerName].pvUserData = userData;
}

bool ReadersStates::readerPresent(const QString& readerName) const
{
 return mReaderStates.contains(readerName);
}

void ReadersStates::addDefault(const QString& readerName)
{
 SCARD_READERSTATEW stateData;
 stateData.pvUserData = nullptr;
 stateData.dwCurrentState = 0;
 stateData.dwEventState = 0;
 stateData.szReader = toWCharArray(readerName);
 stateData.cbAtr = 0;
 mReaderStates.insert(readerName,stateData);
}

wchar_t* ReadersStates::toWCharArray(const QString& str)
{
 int size = str.size();
 wchar_t *wszReader = new wchar_t[static_cast<size_t>(size) + 1];
 size=str.toWCharArray(wszReader);
 wszReader[size] = 0;
 return wszReader;
}

void ReadersStates::add(const QString& readerName, DWORD currentState, LPVOID userData/*=nullptr*/, DWORD eventState/* = 0*/)
{
 add(readerName, currentState, QByteArray(), userData, eventState);
}

void ReadersStates::add(const QString& readerName, DWORD currentState, const QByteArray& ATR, LPVOID userData/*=nullptr*/, DWORD eventState/* = 0*/)
{
 SCARD_READERSTATEW stateData;
 stateData.pvUserData = userData;
 stateData.dwCurrentState = currentState;
 stateData.dwEventState = eventState;
 stateData.szReader = toWCharArray(readerName);
 stateData.cbAtr = static_cast<DWORD>(ATR.size());
 qCopy(ATR.begin(), ATR.end(), stateData.rgbAtr);
 mReaderStates.insert(readerName,stateData);
}

bool ReadersStates::remove(const QString& readerName)
{
 return mReaderStates.remove(readerName)>0;
}

WinSCard::WinSCard(void)
{
}

WinSCard::~WinSCard(void)
{
 m_bThrowingErrors=false;
 if(mContextEstablished)
  ReleaseContext();
}

QByteArray WinSCard::GetCardStatus(DWORD& state, DWORD& protocol)
{
 QByteArray atr(32,0);
 if (!mCardConnected)
 {
  m_nLastError = READER_UNAVAILABLE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return atr;
 }
 DWORD	pchReaders = AUTOALLOCATE;
 DWORD cByte = 32;
 wchar_t *szListReaders = nullptr;
 m_nLastError = SCardStatusW(m_hCard, szListReaders, &pchReaders, &state, &protocol, reinterpret_cast<LPBYTE>(atr.data()), &cByte);
 if (SUCCESS == m_nLastError)
 {
  atr.resize(static_cast<int>(cByte));
  m_nLastError = SCardFreeMemory(m_hContext, szListReaders);
 }
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 return atr;
}

QStringList WinSCard::ListReaders(void)
{
 QStringList readers;
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return readers;
 }
 QVector<wchar_t> readersV;
 DWORD	pchReaders = 0;
 wchar_t *pReader;
 m_nLastError=SCardListReadersW(m_hContext, nullptr, nullptr,&pchReaders);
 if(SUCCESS==m_nLastError)
 {
  readersV.resize(static_cast<int>(pchReaders));
  m_nLastError=SCardListReadersW(m_hContext, nullptr, readersV.data(),&pchReaders);
  if(SUCCESS==m_nLastError)
   {
    pReader=readersV.data();
    while('\0'!=*pReader)
     {
      readers.append(QString::fromWCharArray(pReader));
      // Advance to the next value.
      pReader = pReader + wcslen(pReader) + 1;
     }
   }
 }
 if(SUCCESS!=m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 return readers;
}

QStringList WinSCard::ListReaderGroups(void)
{
 QStringList result;
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return result;
 }
 QVector<wchar_t> readerGroupsV;
 DWORD	pchreaderGroups = 0;
 wchar_t *preaderGroup;
 m_nLastError = SCardListReaderGroupsW(m_hContext, nullptr, &pchreaderGroups);
 if (SUCCESS == m_nLastError)
 {
  readerGroupsV.resize(static_cast<int>(pchreaderGroups));
  m_nLastError = SCardListReaderGroupsW(m_hContext, readerGroupsV.data(), &pchreaderGroups);
  if (SUCCESS == m_nLastError)
  {
   preaderGroup = readerGroupsV.data();
   while ('\0' != *preaderGroup)
   {
    result.append(QString::fromWCharArray(preaderGroup));
    preaderGroup = preaderGroup + wcslen(preaderGroup) + 1;
   }
  }
 }
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 return result;
}

QUuid WinSCard::GetProviderId(const QString& cardName)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return QUuid();
 }
 GUID guidData = { 0 };
 m_nLastError = SCardGetProviderIdW(m_hContext, reinterpret_cast<LPCWSTR>(cardName.unicode()), &guidData);
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 return QUuid(guidData);
}

QStringList WinSCard::ListCards(const QByteArray& cardAtr, const QVector<QUuid>& guidInterfaces)
{
 QStringList result;
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return result;
 }
 QVector<GUID> guidData;
 for (auto& item : guidInterfaces)
  guidData.append(GUID(item));
 QVector<wchar_t> cardsV;
 DWORD	pchCards = 0;
 wchar_t *pCardName;
 m_nLastError = SCardListCardsW(m_hContext, (cardAtr.size()<ATR_LENGTH) ? nullptr : reinterpret_cast<LPCBYTE>(cardAtr.data()), guidData.isEmpty() ? nullptr : guidData.data(), static_cast<DWORD>(guidData.size()), nullptr, &pchCards);
 if (SUCCESS == m_nLastError)
 {
  cardsV.resize(static_cast<int>(pchCards));
  m_nLastError = SCardListCardsW(m_hContext, (cardAtr.size()<ATR_LENGTH) ? nullptr : reinterpret_cast<LPCBYTE>(cardAtr.data()), guidData.isEmpty() ? nullptr : guidData.data(), static_cast<DWORD>(guidData.size()), cardsV.data(), &pchCards);
  if (SUCCESS == m_nLastError)
  {
   pCardName = cardsV.data();
   while ('\0' != *pCardName)
   {
    result.append(QString::fromWCharArray(pCardName));
    pCardName = pCardName + wcslen(pCardName) + 1;
   }
  }
 }
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 return result;
}

QStringList WinSCard::ListCards(const QVector<QUuid>& guidInterfaces)
{
 if (guidInterfaces.isEmpty())
 {
  m_nLastError = INVALID_PARAMETER;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return QStringList();
 }
 return ListCards(QByteArray(), guidInterfaces);
}

QStringList WinSCard::ListCards(const QByteArray& cardAtr)
{
 if (cardAtr.size()<ATR_LENGTH)
 {
  m_nLastError = INVALID_ATR;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return QStringList();
 }
 return ListCards(cardAtr, QVector<QUuid>());
}

QStringList WinSCard::ListCards(void)
{
 return ListCards(QByteArray(), QVector<QUuid>());
}

QVector<QUuid> WinSCard::ListInterfaces(const QString& cardName)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return QVector<QUuid>();
 }
 QVector<GUID> guidData;
 DWORD pcQUuidInterfaces = 0;
 m_nLastError = SCardListInterfacesW(m_hContext, reinterpret_cast<LPCWSTR>(cardName.unicode()), nullptr, &pcQUuidInterfaces);
 if (SUCCESS == m_nLastError)
 {
  guidData.resize(static_cast<int>(pcQUuidInterfaces));
  m_nLastError = SCardListInterfacesW(m_hContext, reinterpret_cast<LPCWSTR>(cardName.unicode()), guidData.data(), &pcQUuidInterfaces);
 }
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 QVector<QUuid> result;
 for (auto& item : guidData)
  result.append(QUuid(item));
 return result;
}

void WinSCard::AddReaderToGroup(const QString& ReaderName, const QString& GroupName)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 m_nLastError = SCardAddReaderToGroupW(m_hContext, reinterpret_cast<LPCWSTR>(ReaderName.unicode()), reinterpret_cast<LPCWSTR>(GroupName.unicode()));
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

void WinSCard::ForgetCardType(const QString& cardName)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 m_nLastError = SCardForgetCardTypeW(m_hContext, reinterpret_cast<LPCWSTR>(cardName.unicode()));
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

void WinSCard::ForgetReader(const QString& ReaderName)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 m_nLastError = SCardForgetReaderW(m_hContext, reinterpret_cast<LPCWSTR>(ReaderName.unicode()));
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

void WinSCard::ForgetReaderGroup(const QString& GroupName)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 m_nLastError = SCardForgetReaderGroupW(m_hContext, reinterpret_cast<LPCWSTR>(GroupName.unicode()));
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

void WinSCard::IntroduceReader(const QString& ReaderName, const QString& DeviceName)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 m_nLastError = SCardIntroduceReaderW(m_hContext, reinterpret_cast<LPCWSTR>(ReaderName.unicode()), reinterpret_cast<LPCWSTR>(DeviceName.unicode()));
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

void WinSCard::IntroduceReaderGroup(const QString& GroupName)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 m_nLastError = SCardIntroduceReaderGroupW(m_hContext, reinterpret_cast<LPCWSTR>(GroupName.unicode()));
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

void WinSCard::RemoveReaderFromGroup(const QString& ReaderName, const QString& GroupName)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 m_nLastError = SCardRemoveReaderFromGroupW(m_hContext, reinterpret_cast<LPCWSTR>(ReaderName.unicode()), reinterpret_cast<LPCWSTR>(GroupName.unicode()));
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

void WinSCard::IntroduceCardType(const QString& cardName, const QVector<QUuid>& guidInterfaces, const QByteArray& cardAtr, const QByteArray& cardAtrMask, QUuid *pguidPrimaryProvider)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 GUID guidPrimaryProviderData{ 0 };
 if(pguidPrimaryProvider!=nullptr)
  guidPrimaryProviderData =GUID(*pguidPrimaryProvider);
 QVector<GUID> guidInterfacesData;
 for (auto& item : guidInterfaces)
  guidInterfacesData.append(GUID(item));
 m_nLastError = SCardIntroduceCardTypeW(m_hContext, reinterpret_cast<LPCWSTR>(cardName.unicode()), &guidPrimaryProviderData, guidInterfacesData.isEmpty() ? nullptr : guidInterfacesData.data(), static_cast<DWORD>(guidInterfacesData.size()), reinterpret_cast<LPCBYTE>(cardAtr.data()), cardAtrMask.isEmpty() ? nullptr : reinterpret_cast<LPCBYTE>(cardAtrMask.data()), static_cast<DWORD>(cardAtr.size()));
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

void WinSCard::IntroduceCardType(const QString& cardName, const QVector<QUuid>& guidInterfaces, const QByteArray& cardAtr, QUuid *pguidPrimaryProvider)
{
 IntroduceCardType(cardName, guidInterfaces, cardAtr, QByteArray(), pguidPrimaryProvider);
}

void WinSCard::IntroduceCardType(const QString& cardName, const QByteArray& cardAtr, const QByteArray& cardAtrMask, QUuid *pguidPrimaryProvider)
{
 IntroduceCardType(cardName, QVector<QUuid>(), cardAtr, cardAtrMask, pguidPrimaryProvider);
}

void WinSCard::IntroduceCardType(const QString& cardName, const QByteArray& cardAtr, QUuid *pguidPrimaryProvider)
{
 IntroduceCardType(cardName, QVector<QUuid>(), cardAtr, QByteArray(), pguidPrimaryProvider);
}

void WinSCard::EstablishContext(SCOPE Scope)
{
 if (mContextEstablished)
  return;
 m_nLastError = SCardEstablishContext(Scope, nullptr, nullptr, &m_hContext);
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 mContextEstablished = true;
}

bool WinSCard::isContextValid()
{
 if (!mContextEstablished)
  return false;
 m_nLastError = SCardIsValidContext(m_hContext);
 if (SYSTEM_INVALID_HANDLE == m_nLastError)
  return false;
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 return true;
}

void WinSCard::ReleaseContext(void)
{
 if (!mContextEstablished)
  return;
 m_nLastError = SCardReleaseContext(m_hContext);
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 mContextEstablished = false;
 mCardConnected = false;
}

long WinSCard::Connect(const QString& Reader, SHARE ShareMode, PROTOCOL PreferredProtocols)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return NO_SERVICE;
 }
 if (mCardConnected)
  Disconnect(Leave);
 m_nLastError=SCardConnectW(m_hContext, reinterpret_cast<LPCWSTR>(Reader.unicode()),ShareMode,PreferredProtocols,&m_hCard,&m_nProtocol);
 if(RESET_CARD==m_nLastError)
  {
   m_nLastError=SCardReconnect(m_hCard,ShareMode,PreferredProtocols,Smartcards::Leave,&m_nProtocol);
  }
 if (SUCCESS != m_nLastError)
 {
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return m_nLastError;
 }
 mCardConnected = true;
 return SUCCESS;
}

PROTOCOL WinSCard::Reconnect(SHARE ShareMode, PROTOCOL PreferredProtocols, DISCONNECT Initialization)
{
 if (!mCardConnected || !mContextEstablished)
 {
  m_nLastError = READER_UNAVAILABLE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return Undefined;
 }
 DWORD retProtocol;
 m_nLastError = SCardReconnect(m_hCard, ShareMode, PreferredProtocols, Initialization, &retProtocol);
 if (SUCCESS != m_nLastError)
 {
  mCardConnected = false;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
 }
 return static_cast<PROTOCOL>(retProtocol);
}

void WinSCard::Disconnect(DISCONNECT Disposition)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 if (!mCardConnected)
  return;
 m_nLastError = SCardDisconnect(m_hCard, Disposition);
 if (SUCCESS != m_nLastError)
  throw SCardException(m_nLastError);
 mCardConnected = false;
}

APDUResponse WinSCard::Transmit(APDUCommand ApduCmd)
{
 if (!mCardConnected)
 {
  m_nLastError = READER_UNAVAILABLE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return APDUResponse(0x90, 0x00);
 }
 SCARD_IO_REQUEST	ioRequest;
 ioRequest.dwProtocol=m_nProtocol;
 ioRequest.cbPciLength=8;
 QByteArray commandData, retData;
 DWORD	RecvLength=RESPONSE_MAX_LENGTH;
 QByteArray responseData(RESPONSE_MAX_LENGTH,0);
 mLastCommand = ApduCmd;
 commandData =ApduCmd.createCommandVector();
 m_nLastError=SCardTransmit(m_hCard,&ioRequest,reinterpret_cast<LPCBYTE>(commandData.constData()), static_cast<DWORD>(commandData.size()), nullptr, reinterpret_cast<LPBYTE>(responseData.data()), &RecvLength);
 if(RESET_CARD==m_nLastError)
  {
   m_nProtocol=Reconnect(Shared, T0orT1, Leave);
   if(SUCCESS!=m_nLastError && m_bThrowingErrors)
    throw SCardException(m_nLastError);
   RecvLength=RESPONSE_MAX_LENGTH;
   responseData.clear();
   m_nLastError= SCardTransmit(m_hCard, &ioRequest, reinterpret_cast<LPCBYTE>(commandData.constData()), static_cast<DWORD>(commandData.size()), nullptr, reinterpret_cast<LPBYTE>(responseData.data()), &RecvLength);
  }
 if(SUCCESS!=m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 responseData.resize(static_cast<int>(RecvLength));
 APDUResponse response(responseData);
 return response;
}

void WinSCard::BeginTransaction(void)
{
 if (!mCardConnected)
 {
  m_nLastError = READER_UNAVAILABLE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 m_nLastError = SCardBeginTransaction(m_hCard);
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

void WinSCard::EndTransaction(DISCONNECT Disposition)
{
 if (!mCardConnected)
 {
  m_nLastError = READER_UNAVAILABLE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 m_nLastError = SCardEndTransaction(m_hCard, Disposition);
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

void WinSCard::GetStatusChange(DWORD dwTimeout, ReadersStates& readerStates)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
 }
 auto readerStatesData = readerStates.mReaderStates.values().toVector();
 m_nLastError = SCardGetStatusChangeW(m_hContext, dwTimeout, readerStatesData.data(), static_cast<DWORD>(readerStatesData.size()));
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 readerStates.assign(readerStatesData);
}

void WinSCard::LocateCards(const QStringList& cards, ReadersStates& readerStates)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 QVector<wchar_t> mszCards;
 for (auto cardName: cards)
 {
  std::wstring cardNameData = cardName.toStdWString();
  qCopy(cardNameData.begin(), cardNameData.end(), mszCards.end());
  mszCards.push_back(0);
 }
 mszCards.push_back(0);
 auto readerStatesData = readerStates.mReaderStates.values().toVector();
 m_nLastError = SCardLocateCardsW(m_hContext, mszCards.data(), readerStatesData.data(), static_cast<DWORD>(readerStatesData.size()));
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 readerStates.assign(readerStatesData);
}

void WinSCard::LocateCardsByATR(const QVector<QPair<QByteArray, QByteArray>>& atrMask, ReadersStates& readerStates)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 QVector<_SCARD_ATRMASK> atrMaskVector;
 for (auto& item : atrMask)
 {
  _SCARD_ATRMASK atrMaskData;
  atrMaskData.cbAtr = static_cast<DWORD>(item.first.size());
  qCopy(item.first.begin(), item.first.end(), atrMaskData.rgbAtr);
  qCopy(item.second.begin(), item.second.end(), atrMaskData.rgbMask);
  atrMaskVector.push_back(atrMaskData);
 }
 auto readerStatesData = readerStates.mReaderStates.values().toVector();
 m_nLastError = SCardLocateCardsByATRW(m_hContext, atrMaskVector.data(), static_cast<DWORD>(atrMaskVector.size()), readerStatesData.data(), static_cast<DWORD>(readerStatesData.size()));
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 readerStates.assign(readerStatesData);
}

void WinSCard::CardCancel()
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 m_nLastError = SCardCancel(m_hContext);
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

QByteArray WinSCard::GetAttribute(DWORD AttribId)
{
 QByteArray attr;
 DWORD attrLen = 0;
 if (!mCardConnected)
 {
  m_nLastError = READER_UNAVAILABLE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return attr;
 }
 m_nLastError=SCardGetAttrib(m_hCard,AttribId,nullptr,&attrLen);
 if(SUCCESS==m_nLastError && 0!=attrLen)
  {
   attr.resize(static_cast<int>(attrLen));
   m_nLastError = SCardGetAttrib(m_hCard,AttribId,reinterpret_cast<LPBYTE>(attr.data()),&attrLen);
  }
 if(SUCCESS!=m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 return attr;
}

void WinSCard::SetAttribute(DWORD AttribId, const QByteArray& attr)
{
 if (!mCardConnected)
 {
  m_nLastError = READER_UNAVAILABLE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 m_nLastError = SCardSetAttrib(m_hCard, AttribId, reinterpret_cast<const BYTE*>(attr.data()), static_cast<DWORD>(attr.size()));
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

bool WinSCard::CardControl(DWORD controlCode, const QByteArray& inData, QByteArray& outData)
{
 if (!mCardConnected)
 {
  m_nLastError = READER_UNAVAILABLE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return false;
 }
 DWORD retBytesCount = static_cast<DWORD>(outData.size());
 m_nLastError = SCardControl(m_hCard, controlCode, inData.data(), static_cast<DWORD>(inData.size()), outData.data(), static_cast<DWORD>(outData.size()), &retBytesCount);
 if (SUCCESS != m_nLastError)
 {
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return false;
 }
 outData.resize(static_cast<int>(retBytesCount));
 return true;
}

bool WinSCard::CardControl(DWORD controlCode, const QByteArray& inData)
{
 QByteArray outData;
 return CardControl(controlCode, inData, outData);
}

DWORD WinSCard::CardControl(DWORD controlCode, LPVOID pInData, DWORD cInData, LPVOID pOutData, DWORD cOutData)
{
 if (!mCardConnected)
 {
  m_nLastError = READER_UNAVAILABLE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return 0;
 }
 DWORD retBytesCount = cOutData;
 m_nLastError = SCardControl(m_hCard, controlCode, pInData, cInData, pOutData, cOutData, &retBytesCount);
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 return retBytesCount;
}
