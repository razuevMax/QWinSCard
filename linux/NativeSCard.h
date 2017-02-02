//! \file NativeSCard.h
//! \brief Header file for WinSCard class and auxiliary classes.
//! \details File is contains class WinSCard must implement a winscard api.
#ifndef NATIVESCARD_H
#define NATIVESCARD_H

#include "APDUResponse.h"
#include "APDUCommand.h"
#include "WinSCardErr.h"
#include "pcsc-lite/pcsclite.h"
#include "pcsc-lite/winscard.h"

//! \namespace Smartcards
//! \brief Namespace of smartcard classes
namespace Smartcards
{
 //forward declare
 class CardEvent;
 //! SCOPE context
 enum SCOPE : DWORD
 {
  User,//!< The context is a user context, and any database operations are performed within the domain of the user.
  Terminal,//!< The context is that of the current terminal, and any database operations are performed within the domain of that terminal.  (The calling application must have appropriate access permissions for any database actions.)
  System//!< The context is the system context, and any database operations are performed within the domain of the system.  (The calling application must have appropriate access permissions for any database actions.)
 };
 //! SHARE mode enumeration
 enum SHARE : DWORD
 {
  Exclusive = 1,//!< This application is not willing to share this card with other applications.
  Shared,//!< This application is willing to share this card with other applications.
  Direct//!< This application demands direct control of the reader, so it is not available to other applications.
 };

 //! PROTOCOL enumeration
 enum	PROTOCOL : DWORD
 {
  Undefined = 0x00000000,//!< There is no active protocol.
  T0 = 0x00000001,//!< T=0 is the active protocol.
  T1 = 0x00000002,//!< T=1 is the active protocol.
  T0orT1 = T0 | T1//!< T=1 or T=0 can be the active protocol
 };

 //! DISCONNECT action enumeration
 enum DISCONNECT : DWORD
 {
  Leave,//!< Don't do anything special on close
  Reset,//!< Reset the card on close
  Unpower,//!< Power down the card on close
  Eject//!< Eject the card on close
 };

 //! Values for AttrId of SCardGetAttrib
 class SCARD_ATTR_VALUE
 {
  static const DWORD SCARD_CLASS_VENDOR_INFO_CONST = 1;//!< Vendor information definitions
  static const DWORD SCARD_CLASS_POWER_MGMT_CONST = 4;//!< Power Management definitions
  static const DWORD SCARD_CLASS_SECURITY_CONST = 5;//!< Security Assurance definitions
  static const DWORD SCARD_CLASS_PERF_CONST = 0x7ffe;//!< performace counters
  static const DWORD SCARD_CLASS_COMMUNICATIONS_CONST = 2;//!< Communications attribute
  static const DWORD SCARD_CLASS_PROTOCOL_CONST = 3;//!< Communication definitions
  static const DWORD SCARD_CLASS_MECHANICAL_CONST = 6;//!< Mechanical characteristic definitions
  static const DWORD SCARD_CLASS_VENDOR_DEFINED_CONST = 7;//!<  Vendor specific definitions
  static const DWORD SCARD_CLASS_IFD_PROTOCOL_CONST = 8;//!< Interface Device Protocol options
  static const DWORD SCARD_CLASS_ICC_STATE_CONST = 9;//!< ICC State specific definitions
  static const DWORD SCARD_CLASS_SYSTEM_CONST = 0x7fff;//!< System-specific definitions
 public:
  static const DWORD VENDOR_NAME = SCARD_CLASS_VENDOR_INFO_CONST << 16 | 0x0100;//!< Vendor name
  static const DWORD VENDOR_IFD_TYPE = SCARD_CLASS_VENDOR_INFO_CONST << 16 | 0x0101;//!< Vendor-supplied interface device type (model designation of reader).
  static const DWORD VENDOR_IFD_VERSION = SCARD_CLASS_VENDOR_INFO_CONST << 16 | 0x0102;//!< Vendor-supplied interface device version (in the form 0xMMmmbbbb where MM = major version, mm = minor version, and bbbb = build number).
  static const DWORD VENDOR_IFD_SERIAL_NO = SCARD_CLASS_VENDOR_INFO_CONST << 16 | 0x0103;//!< Vendor-supplied interface device serial number.
  static const DWORD CHANNEL_ID = SCARD_CLASS_COMMUNICATIONS_CONST << 16 | 0x0110;
  /*!< /brief Channel identifier
  Encoded as 0xDDDDCCCC, where DDDD = data channel type and CCCC = channel number:
  The following encodings are defined for DDDD:
  0x01 serial I/O; CCCC is a port number.
  0x02 parallel I/O; CCCC is a port number.
  0x04 PS/2 keyboard port; CCCC is zero.
  0x08 SCSI; CCCC is SCSI ID number.
  0x10 IDE; CCCC is device number.
  0x20 USB; CCCC is device number.
  0xFy vendor-defined interface with y in the range zero through 15; CCCC is vendor defined.
  */
  static const DWORD PROTOCOL_TYPES = SCARD_CLASS_PROTOCOL_CONST << 16 | 0x0120;
  /*!<  /brief Protocol types
  Encoded as 0x0rrrpppp where rrr is RFU and should be 0x000.
  pppp encodes the supported protocol types.
  A '1' in a given bit position indicates support for the associated ISO protocol,
  so if bits zero and one are set, both T=0 and T=1 protocols are supported.
  */
  static const DWORD DEFAULT_CLK = SCARD_CLASS_PROTOCOL_CONST << 16 | 0x0121;//!< Default clock rate, in kHz
  static const DWORD MAX_CLK = SCARD_CLASS_PROTOCOL_CONST << 16 | 0x0122;//!< Maxinum clock rate, in kHz
  static const DWORD DEFAULT_DATA_RATE = SCARD_CLASS_PROTOCOL_CONST << 16 | 0x0123;//!< Default data rate, in bps
  static const DWORD MAX_DATA_RATE = SCARD_CLASS_PROTOCOL_CONST << 16 | 0x0124;//!< Maximum data rate, in bps
  static const DWORD MAX_IFSD = SCARD_CLASS_PROTOCOL_CONST << 16 | 0x0125;//!< Maximum bytes for information file size device.
  static const DWORD POWER_MGMT_SUPPORT = SCARD_CLASS_POWER_MGMT_CONST << 16 | 0x0131;//!< Zero if device does not support power down while smart card is inserted. Nonzero otherwise.
  static const DWORD USER_TO_CARD_AUTH_DEVICE = SCARD_CLASS_SECURITY_CONST << 16 | 0x0140;//!< User to card authetication device
  static const DWORD USER_AUTH_INPUT_DEVICE = SCARD_CLASS_SECURITY_CONST << 16 | 0x0142;//!< User authetication input device
  static const DWORD CHARACTERISTICS = SCARD_CLASS_MECHANICAL_CONST << 16 | 0x0150;
  /*!< /brief Indicating which mechanical characteristics are supported.
  If zero, no special characteristics are supported. Note that multiple bits can be set:
  0x00000001 Card swallowing mechanism
  0x00000002 Card ejection mechanism
  0x00000004 Card capture mechanism
  All other values are reserved for future use (RFU).
  */
  static const DWORD CURRENT_PROTOCOL_TYPE = SCARD_CLASS_IFD_PROTOCOL_CONST << 16 | 0x0201;
  /*!< /brief Current protocol type
  Encoded as 0x0rrrpppp where rrr is RFU and should be 0x000.
  pppp encodes the current protocol type.
  Whichever bit has been set indicates which ISO protocol is currently in use.
  (For example, if bit zero is set, T=0 protocol is in effect.)
  */
  static const DWORD CURRENT_CLK = SCARD_CLASS_IFD_PROTOCOL_CONST << 16 | 0x0202;//!< Current clock rate, in kHz
  static const DWORD CURRENT_F = SCARD_CLASS_IFD_PROTOCOL_CONST << 16 | 0x0203;//!< Current clock conversion factor
  static const DWORD CURRENT_D = SCARD_CLASS_IFD_PROTOCOL_CONST << 16 | 0x0204;//!< Current bit rate conversion factor
  static const DWORD CURRENT_N = SCARD_CLASS_IFD_PROTOCOL_CONST << 16 | 0x0205;//!< Current guard time
  static const DWORD CURRENT_W = SCARD_CLASS_IFD_PROTOCOL_CONST << 16 | 0x0206;//!< Current work waiting time
  static const DWORD CURRENT_IFSC = SCARD_CLASS_IFD_PROTOCOL_CONST << 16 | 0x0207;//!< Current byte size for information field size card
  static const DWORD CURRENT_IFSD = SCARD_CLASS_IFD_PROTOCOL_CONST << 16 | 0x0208;//!< Current Current byte size for information field size device
  static const DWORD CURRENT_BWT = SCARD_CLASS_IFD_PROTOCOL_CONST << 16 | 0x0209;//!< Current block waiting time
  static const DWORD CURRENT_CWT = SCARD_CLASS_IFD_PROTOCOL_CONST << 16 | 0x020a;//!< Current character waiting time
  static const DWORD CURRENT_EBC_ENCODING = SCARD_CLASS_IFD_PROTOCOL_CONST << 16 | 0x020b;
  /*!< /brief Current error block control encoding.
  0 = longitudinal redundancy check (LRC)
  1 = cyclical redundancy check (CRC)
  */
  static const DWORD EXTENDED_BWT = SCARD_CLASS_IFD_PROTOCOL_CONST << 16 | 0x020c;//!< Extended BWT
  static const DWORD ICC_PRESENCE = SCARD_CLASS_ICC_STATE_CONST << 16 | 0x0300;
  /*!< /brief Single byte indicating smart card presence.
  0 = not present
  1 = card present but not swallowed (applies only if reader supports smart card swallowing)
  2 = card present (and swallowed if reader supports smart card swallowing)
  4 = card confiscated.
  */
  static const DWORD ICC_INTERFACE_STATUS = SCARD_CLASS_ICC_STATE_CONST << 16 | 0x0301;//!< Single byte. Zero if smart card electrical contact is not active; nonzero if contact is active.
  static const DWORD CURRENT_IO_STATE = SCARD_CLASS_ICC_STATE_CONST << 16 | 0x0302;//!< Current IO state
  static const DWORD ATR_STRING = SCARD_CLASS_ICC_STATE_CONST << 16 | 0x0303;//!< ATR string
  static const DWORD ICC_TYPE_PER_ATR = SCARD_CLASS_ICC_STATE_CONST << 16 | 0x0304;
  /*!< /brief Single byte indicating smart card type.
  0 = unknown type
  1 = 7816 Asynchronous
  2 = 7816 Synchronous
  Other values RFU.
  */
  static const DWORD ESC_RESET = SCARD_CLASS_VENDOR_DEFINED_CONST << 16 | 0xA000;//!< ESC reset
  static const DWORD ESC_CANCEL = SCARD_CLASS_VENDOR_DEFINED_CONST << 16 | 0xA003;//!< ESC cancel
  static const DWORD ESC_AUTHREQUEST = SCARD_CLASS_VENDOR_DEFINED_CONST << 16 | 0xA005;//!< ESC authetication reques
  static const DWORD MAXINPUT = SCARD_CLASS_VENDOR_DEFINED_CONST << 16 | 0xA007;//!< Maximum input
  static const DWORD DEVICE_UNIT = SCARD_CLASS_SYSTEM_CONST << 16 | 0x0001;
  /*!< /brief Instance of this vendor's reader attached to the computer.
  The first instance will be device unit 0, the next will be unit 1 (if it is the same brand of reader) and so on.
  Two different brands of readers will both have zero for this value.
  */
  static const DWORD DEVICE_IN_USE = SCARD_CLASS_SYSTEM_CONST << 16 | 0x0002;//!< Reserved for future use
  static const DWORD DEVICE_FRIENDLY_NAME_ASCII = SCARD_CLASS_SYSTEM_CONST << 16 | 0x0003;//!< Device friendly name in ASCII
  static const DWORD DEVICE_SYSTEM_NAME_A_ASCII = SCARD_CLASS_SYSTEM_CONST << 16 | 0x0004;//!< Device sysyem name in ASCII
  static const DWORD DEVICE_FRIENDLY_NAME = SCARD_CLASS_SYSTEM_CONST << 16 | 0x0005;//!< Device friendly name in Unicode
  static const DWORD DEVICE_SYSTEM_NAME = SCARD_CLASS_SYSTEM_CONST << 16 | 0x0006;//!< Device sysyem name in Unicode
  static const DWORD SUPRESS_T1_IFS_REQUEST = SCARD_CLASS_SYSTEM_CONST << 16 | 0x0007;//!< Suppress sending of T=1 IFSD packet from the reader to the card. (Can be used if the currently inserted card does not support an IFSD request.)
  static const DWORD PERF_NUM_TRANSMISSIONS = SCARD_CLASS_PERF_CONST << 16 | 0x0001;//!< Perfomance number transmissions
  static const DWORD PERF_BYTES_TRANSMITTED = SCARD_CLASS_PERF_CONST << 16 | 0x0002;//!< Perfomance bytes transmitted
  static const DWORD PERF_TRANSMISSION_TIME = SCARD_CLASS_PERF_CONST << 16 | 0x0003;//!< Perfomance transmission time
 };

 //! Card status enumeration, used by the class ReaderStates
 enum CARD_STATUS_STATE :DWORD
 {
  Unaware = 0x00000000,//!< The application is unaware of the current state, and would like to know. The use of this value results in an immediate return from state transition monitoring services. This is represented by all bits set to zero.
  Ignore = 0x00000001,//!< The application is not interested in this reader, and it should not be considered during monitoring operations. If this bit value is set, all other bits are ignored.
  Changed = 0x00000002,//!< State is changed
  Unknown = 0x00000004,//!< State is unknow
  Unvailable = 0x00000008,//!< The application expects that this reader is not available for use. If this bit is set, then all the following bits are ignored.
  Empty = 0x00000010,//!< The application expects that there is no card in the reader. If this bit is set, all the following bits are ignored.
  Present = 0x00000020,//!< The application expects that there is a card in the reader.
  ATRMatch = 0x00000040,//!< The application expects that there is a card in the reader with an ATR that matches one of the target cards. If this bit is set, 'Present' is assumed. This bit has no meaning to SCardGetStatusChange beyond 'Present'.
  ExclusiveStatus = 0x00000080,//!< he application expects that the card in the reader is allocated for exclusive use by another application. If this bit is set, 'Present' is assumed.
  InUse = 0x00000100,//!< The application expects that the card in the reader is in use by one or more other applications, but may be connected to in shared mode. If this bit is set, 'Present' is assumed.
  Mute = 0x00000200,//!< The application expects that there is an unresponsive card in the reader.
  Unpowered = 0x00000400//!< Card is unpowered
 };

 //! Card state enumeration, used by the GetCardStatus function
 enum CARD_STATE :DWORD
 {
  Absent = SCARD_ABSENT,//!< There is no card in the reader.
  PresentState = SCARD_PRESENT,//!< There is a card in the reader, but it has not been moved into position for use.
  Swallowed = SCARD_SWALLOWED,//!< There is a card in the reader in position for use. The card is not powered.
  Powered = SCARD_POWERED,//!< Power is being provided to the card, but the reader driver is unaware of the mode of the card.
  Negotable = SCARD_NEGOTIABLE,//!< The card has been reset and is awaiting PTS negotiation.
  Specific = SCARD_SPECIFIC//!< The card has been reset and specific communication protocols have been established.
 };

 // forward declare
 class WinSCard;

 //! Interface class for Smart Card Tracking Functions. Contains readers states.
 class ReadersStates
 {
 public:
  friend class WinSCard;
  //! \fn ReadersStates::ReadersStates()
  //! \brief Default constructor.
  ReadersStates(void);
  //! \fn ReadersStates::ReadersStates(const std::initializer_list<std::pair<QString, DWORD>>& ilist)
  //! \brief Constructor from initializer list.
  //! \param[in] ilist The list of pair {"ReaderName", ReaderState}.
  ReadersStates(const std::initializer_list<std::pair<QString, DWORD>>& ilist);
  //! \fn ReadersStates::ReadersStates(PSCARD_READERSTATEW pReaderStates, DWORD cbReaderStates)
  //! \brief Constructor from array of Winscard structures SCARD_READERSTATE.
  //! \param[in] pReaderStates  Pointer to array of states of the readers.
  //! \param[in] cbReaderStates Size array of states of the readers.
  ReadersStates(SCARD_READERSTATE* pReaderStates, DWORD cbReaderStates);
  //! \fn ReadersStates::ReadersStates(const QMap<QString, SCARD_READERSTATEW>& readerStates)
  //! \brief Constructor from QMap contains pairs {"ReaderName", SCARD_READERSTATE}.
  //! \param[in] readerStates Map of states of the readers by reader name.
  ReadersStates(const QMap<QString, SCARD_READERSTATE>& readerStates);
  //! \fn ReadersStates::ReadersStates(const QVector<SCARD_READERSTATEW>& readerStates)
  //! \brief Constructor from QVector contains Winscard structures SCARD_READERSTATE.
  //! \param[in] readerStates Vector of Winscard structures SCARD_READERSTATE.
  ReadersStates(const QVector<SCARD_READERSTATE>& readerStates);
  //! \fn ReadersStates::~ReadersStates()
  //! \brief Destructor.
  ~ReadersStates(void);
  //! \fn void ReadersStates::assign(const QVector<SCARD_READERSTATEW>& readerStates)
  //! \brief Assigns the given reader states.
  //! \param[in] readerStates Vector of Winscard structures SCARD_READERSTATE.
  void assign(const QVector<SCARD_READERSTATE>& readerStates);
  //! \fn void ReadersStates::addNewReader()
  //! \brief Adds default winscard reader name for search new reader.
  void addNewReader(void);
  //! \fn void ReadersStates::removeNewReader()
  //! \brief Removes default winscard reader name for new reader.
  void removeNewReader(void);
  //! \fn void ReadersStates::add(const QString& readerName, DWORD currentState, LPVOID userData, DWORD eventState)
  //! \brief Adds reader state in map. ATR not present.
  //! \param[in] readerName   Name of the reader.
  //! \param[in] currentState The current state.
  //! \param[in] userData     User defined data.
  //! \param[in] eventState   The event state.
  void add(const QString& readerName, DWORD currentState, LPVOID userData = nullptr, DWORD eventState = 0);
  //! \fn void ReadersStates::add(const QString& readerName, DWORD currentState, const QByteArray& ATR, LPVOID userData, DWORD eventState)
  //! \brief Adds reader state in map. ATR present.
  //! \param[in] readerName   Name of the reader.
  //! \param[in] currentState The current state.
  //! \param[in] ATR          The ATR of card.
  //! \param[in] userData     User defined data.
  //! \param[in] eventState   The event state.
  void add(const QString& readerName, DWORD currentState, const QByteArray& ATR, LPVOID userData = nullptr, DWORD eventState = 0);
  //! \fn bool ReadersStates::remove(const QString& readerName)
  //! \brief Removes the given reader state.
  //! \param[in] readerName The reader name to remove.
  //! \return True if reader realy removed, false if reader not present in map.
  bool remove(const QString& readerName);
  //! \fn DWORD ReadersStates::currentState(const QString& readerName)
  //! \brief Current state.
  //! \param[in] readerName Name of the reader.
  //! \return a current state of reader.
  DWORD currentState(const QString& readerName);
  //! \fn DWORD ReadersStates::eventState(const QString& readerName)
  //! \brief Event state.
  //! \param[in] readerName Name of the reader.
  //! \return a event state of reader.
  DWORD eventState(const QString& readerName);
  //! \fn QByteArray ReadersStates::ATR(const QString& readerName)
  //! \brief Atrs the given reader name.
  //! \param[in] readerName Name of the reader.
  //! \return A QByteArray contains ATR of card in reader.
  QByteArray ATR(const QString& readerName);
  //! \fn LPVOID ReadersStates::userData(const QString& readerName)
  //! \brief User data.
  //! \param[in] readerName Name of the reader.
  //! \return a pointer to user defined data associate by reader.
  LPVOID userData(const QString& readerName);
  //! \fn void ReadersStates::setCurrentState(const QString& readerName, DWORD state)
  //! \brief Sets current state.
  //! \param[in] readerName Name of the reader.
  //! \param[in] state      The current state.
  void setCurrentState(const QString& readerName, DWORD state);
  //! \fn void ReadersStates::setEventState(const QString& readerName, DWORD state)
  //! \brief Sets event state.
  //! \param[in] readerName Name of the reader.
  //! \param[in] state      The event state.
  void setEventState(const QString& readerName, DWORD state);
  //! \fn void ReadersStates::setATR(const QString& readerName, const QByteArray& ATR)
  //! \brief Sets ATR of card.
  //! \param[in] readerName Name of the reader.
  //! \param[in] ATR        The QByteArray contains ATR of card.
  void setATR(const QString& readerName, const QByteArray& ATR);
  //! \fn void ReadersStates::setUserData(const QString& readerName, LPVOID userData)
  //! \brief Sets user data.
  //! \param[in] readerName Name of the reader.
  //! \param[in] userData   User defined data pointer.
  void setUserData(const QString& readerName, LPVOID userData);
  //! \fn bool ReadersStates::readerPresent(const QString& readerName) const
  //! \brief Check present Reader in readerStates
  //! \param[in] readerName Name of the reader.
  //! \return True if it succeeds, false if it fails.
  bool readerPresent(const QString& readerName) const;
 private:
  //! \fn void ReadersStates::addDefault(const QString& readerName)
  //! \brief Adds a default reader state in inner map.
  //! \param[in] readerName Name of the reader.
  void addDefault(const QString& readerName);
  //! \fn wchar_t* ReadersStates::toWCharArray(const QString& str)
  //! \brief Converts a Qt-string str to a wchar_t array.
  //! \warning wchar_t array allocates inner function. Deallocate responsibility lies caller.
  //! \param[in] str The Qt-string.
  //! \return Str as a wchar_t*.
  static wchar_t* toWCharArray(const QString& str);
  QMap<QString, SCARD_READERSTATE> mReaderStates;//!< QMap contains reader states by reader name
 };

 //! This class must implement a winscard api.
 class WinSCard
 {
 public:
  static const DWORD ATR_LENGTH = SCARD_ATR_LENGTH;//!< Maximim ATR length
  static const DWORD AUTOALLOCATE = SCARD_AUTOALLOCATE;//!< Constant indicating auto allocate memory in winscard functions
  static const DWORD RESPONSE_MAX_LENGTH = 0x0200;//!< Maximum length of card response in bytes.
  static const DWORD CCID_READER_PRESENT_STATE = CARD_STATUS_STATE::Unknown | CARD_STATUS_STATE::Ignore | CARD_STATUS_STATE::Changed;//!< Reader present state
  static const DWORD CCID_READER_EMPTY_STATE = CARD_STATUS_STATE::Unvailable | CARD_STATUS_STATE::Ignore | CARD_STATUS_STATE::Changed;//!< Reader empty state
  //! \fn WinSCard::WinSCard(void)
  //! \brief Default constructor. 
  WinSCard(void);
  //! \fn WinSCard::WinSCard(SCARDCONTEXT context, SCARDHANDLE handle)
    //! \brief Constructor with external handles.
    WinSCard(SCARDCONTEXT context, SCARDHANDLE handle = 0);
    //! \fn WinSCard::~WinSCard(void)
    //! \brief Destructor.
    virtual ~WinSCard(void);
    //! \fn SCARDCONTEXT context(void) const
    //! \brief Get current resource manager context.
    //! \return A current resource manager context.
    SCARDCONTEXT context(void) const { return m_hContext; }
    //! \fn  SCARDHANDLE handle(void) const
    //! \brief Get current card handle.
    //! \return A current card handle.
    SCARDHANDLE handle(void) const { return m_hCard; }
    //! \fn void setContext(SCARDCONTEXT context)
    //! \brief Set new resource manager context. Release current context.
    //! \param[in] context A new resource manager context.
    void setContext(SCARDCONTEXT context);
    //! \fn  void setHandle(SCARDHANDLE handle)
    //! \brief Set new card handle. Disconnect current handle.
    //! \param[in] handle A new card handle.
    void setHandle(SCARDHANDLE handle);
  //! \fn throwErrors(bool isThrowingErrors)
  //! \brief Set the flag of throwing exception on errors.
  //! \param[in] isThrowingErrors If true - throwing exceptions on errros, else stores errors in lastError.
  void throwErrors(bool isThrowingErrors) { m_bThrowingErrors = isThrowingErrors; }
  //! \fn  bool isThrowingErrors(void) const
  //! \brief Get flag of throwing exception on errors.
  //! \return If true - throwing exceptions on errros, else stores errors in lastError.
  bool isThrowingErrors(void) const { return m_bThrowingErrors; }
  //! \fn  long lastError(void) const
  //! \brief Get last error code.
  //! \return Last error code. SUCCESS if no error.
  long lastError(void) const { return m_nLastError; }
  //! \fn  bool isError(void) const
  //! \brief Check is last error present
  //! \return true if error present, else false.
  bool isError(void) const { return m_nLastError != SUCCESS; }
  //! \defgroup scdb_funcs Smart Card Database Query Functions
  //! @{
  //! \fn QStringList WinSCard::ListReaders(void)
  //! \brief Get list readers.
  //! \exception SCardException Thrown when a Card error condition occurs and throwErrors is set true.
  //! \return A list of readers names.
  virtual QStringList ListReaders(void);
  //! \fn QStringList WinSCard::ListReaderGroups(void)
  //! \brief Get list reader groups.
  //! \exception SCardException Thrown when a Card error condition occurs and throwErrors is set true.
  //! \return A list of reader groups.
  //! @}
  virtual QStringList ListReaderGroups(void);
  //! \defgroup scresmng_funcs Resource Manager Context Functions
  //! @{
  //! \fn void WinSCard::EstablishContext(SCOPE Scope)
  //! \brief Establishes the resource manager context (the scope) within which database operations are performed.
  //! \exception SCardException Thrown when a Card error condition occurs and throwErrors is set true.
  //! \param[in] Scope Scope of the resource manager context.
  virtual void EstablishContext(SCOPE Scope);
  //! \fn bool isContextEstablished(void) const
  //! \brief Check is resource manager context is established.
  //! \return True if it established, false if it not.
  virtual bool isContextEstablished(void) const { return mContextEstablished; }
  //! \fn bool WinSCard::isContextValid()
  //! \brief Query if resource manager context is valid.
  //! \exception SCardException Thrown when a Card error condition occurs and throwErrors is set true.
  //! \return True if the context is valid, false if not.
  virtual bool isContextValid(void);
  //! \fn void WinSCard::ReleaseContext(void)
  //! \brief Closes an established resource manager context, freeing any resources allocated under that context, including SCARDHANDLE objects and memory allocated using the AUTOALLOCATE length designator.
  //! \exception SCardException Thrown when a Card error condition occurs and throwErrors is set true.
  //! @}
  virtual void ReleaseContext(void);
  //! \defgroup scredacs_funcs Smart Card and Reader Access Functions
  //! @{
  //! \fn long WinSCard::Connect(const QString& Reader, SHARE ShareMode, PROTOCOL PreferredProtocols)
  //! \brief Establishes a connection between the calling application and a smart card contained by a specific reader. If no card exists in the specified reader, an exception is throwed else return SUCCESS
  //! \exception SCardException Thrown when a Card error condition occurs and throwErrors is set true.
  //! \param[in] Reader             The reader name.
  //! \param[in] ShareMode          The share mode.
  //! \param[in] PreferredProtocols The preferred protocols.
  //! \return A SUCCESS if successful. NO_SERVICE if context not valid.
  virtual long Connect(const QString& Reader, SHARE ShareMode, PROTOCOL PreferredProtocols);
  //! \fn PROTOCOL WinSCard::Reconnect(SHARE ShareMode, PROTOCOL PreferredProtocols, DISCONNECT Initialization)
  //! \brief reestablishes an existing connection between the calling application and a smart card. This function moves a card handle from direct access to general access, or acknowledges and clears an error condition that is preventing further access to the card.
  //! \exception SCardException Thrown when a Card error condition occurs and throwErrors is set true.
  //! \param[in] ShareMode          Flag that indicates whether other applications may form connections to this card.
  //! \param[in] PreferredProtocols Bitmask of acceptable protocols for this connection.
  //! \param[in] Initialization     Type of initialization that should be performed on the card.
  //! \return Flag that indicates the established active protocol.
  virtual PROTOCOL Reconnect(SHARE ShareMode, PROTOCOL PreferredProtocols, DISCONNECT Initialization);
  //! \fn void WinSCard::Disconnect(DISCONNECT Disposition)
  //! \brief Terminates a connection previously opened between the calling application and a smart card in the target reader.
  //! \warning It requires that the context has been established
  //! \exception SCardException Thrown when a Card error condition occurs and throwErrors is set true.
  //! \param[in] Disposition Action to take on the card in the connected reader on close.
  virtual	void Disconnect(DISCONNECT Disposition);
  //! \fn bool isConnected(void) const
  //! \brief Check is card connected.
  //! \return True if it connected, false if it not.
  virtual bool isConnected(void) const { return mCardConnected; }
  //! \fn APDUResponse WinSCard::Transmit(APDUCommand ApduCmd)
  //! \brief Sends a service request to the smart card and expects to receive data back from the card.
  //! \exception SCardException Thrown when a Card error condition occurs and throwErrors is set true.
  //! \param[in] ApduCmd The apdu command.
  //! \return An APDUResponse.
  virtual APDUResponse Transmit(APDUCommand ApduCmd);
  //! \fn QByteArray WinSCard::GetCardStatus(DWORD& state, DWORD& protocol)
  //! \brief Gets card status(card state, card protocol and card ATR).
  //! \warning It requires that the card has been connected
  //! \exception SCardException Thrown when a Card error condition occurs and throwErrors is set true.
  //! \param[in,out] state    The card state.
  //! \param[in,out] protocol The card protocol.
  //! \return The card ATR.
  virtual QByteArray GetCardStatus(DWORD& state, DWORD& protocol);
  //! \fn void WinSCard::BeginTransaction(void)
  //! \brief Begins a transaction. The function waits for the completion of all other transactions before it begins. After the transaction starts, all other applications are blocked from accessing the smart card while the transaction is in progress.
  //! \exception SCardException Thrown when a Card error condition occurs and throwErrors is set true. 
  virtual void BeginTransaction(void);
  //! \fn void WinSCard::EndTransaction(DISCONNECT Disposition)
  //! \brief Completes a previously declared transaction, allowing other applications to resume interactions with the card.
  //! \exception SCardException Thrown when a Card error condition occurs and throwErrors is set true.
  //! \param[in] Disposition Action to take on the card in the connected reader on close.
  //! @}
  virtual void EndTransaction(DISCONNECT Disposition);
  //! \defgroup scdacs_funcs Direct Card Access Functions
  //! @{
  //! \fn QByteArray WinSCard::GetAttribute(DWORD AttribId)
  //! \brief Retrieves the current reader attributes for the given handle. It does not affect the state of the reader, driver, or card.
  //! \exception SCardException Thrown when a Card error condition occurs and throwErrors is set true.
  //! \param[in] AttribId Identifier for the attribute.
  //! \return The byte array of attribute data.
  virtual QByteArray GetAttribute(DWORD AttribId);
  //! \fn void WinSCard::SetAttribute(DWORD AttribId, const QByteArray& attr)
  //! \brief Sets the given reader attribute for the given handle. It does not affect the state of the reader, reader driver, or smart card. Not all attributes are supported by all readers (nor can they be set at all times) as many of the attributes are under direct control of the transport protocol.
  //! \exception SCardException Thrown when a Card error condition occurs and throwErrors is set true.
  //! \param[in] AttribId Identifier for the attribute.
  //! \param[in] attr     The byte array of attribute data.
  virtual void SetAttribute(DWORD AttribId, const QByteArray& attr);
  //! \fn bool WinSCard::CardControl(DWORD controlCode, const QByteArray& inData, QByteArray& outData)
  //! \brief Gives you direct control of the reader. You can call it any time after a successful call to Connect and before a successful call to Disconnect. The effect on the state of the reader depends on the control code.
  //! \exception SCardException Thrown when a Card error condition occurs and throwErrors is set true.
  //! \param[in]          controlCode Control code for the operation. This value identifies the specific operation to be performed.
  //! \param[in]          inData      Byte array that contains the data required to perform the operation. Not empty.
  //! \param[in,out] outData     Byte array that receives the operation's output data.
  //! \return True if it succeeds, false if it fails.
  virtual bool CardControl(DWORD controlCode, const QByteArray& inData, QByteArray& outData);
  //! \fn bool WinSCard::CardControl(DWORD controlCode, const QByteArray& inData)
  //! \brief Gives you direct control of the reader. You can call it any time after a successful call to Connect and before a successful call to Disconnect. The effect on the state of the reader depends on the control code. Only inData version.
  //! \exception SCardException Thrown when a Card error condition occurs and throwErrors is set true.
  //! \param[in]          controlCode Control code for the operation. This value identifies the specific operation to be performed.
  //! \param[in]          inData      Byte array that contains the data required to perform the operation. Not empty.
  //! \return True if it succeeds, false if it fails.
  virtual bool CardControl(DWORD controlCode, const QByteArray& inData);
  //! \fn DWORD WinSCard::CardControl(DWORD controlCode, LPVOID pInData, DWORD cInData, LPVOID pOutData, DWORD cOutData)
  //! \brief  Direct pointer version. Gives you direct control of the reader. You can call it any time after a successful call to Connect and before a successful call to Disconnect. The effect on the state of the reader depends on the control code.
  //! \exception SCardException Thrown when a Card error condition occurs and throwErrors is set true.
  //! \param[in] controlCode Control code for the operation. This value identifies the specific operation to be performed.
  //! \param[in] pInData     Pointer to a buffer that contains the data required to perform the operation. This parameter can be NULL if the controlCode parameter specifies an operation that does not require input data.
  //! \param[in] cInData     Size, in bytes, of the buffer pointed to by pInData.
  //! \param[in] pOutData    Pointer to a buffer that receives the operation's output data. This parameter can be NULL if the controlCode parameter specifies an operation that does not produce output data.
  //! \param[in] cOutData    Size, in bytes, of the buffer pointed to by pOutData.
  //! \return A DWORD that receives the size, in bytes, of the data stored into the buffer pointed to by pOutData.
  //! @}
  virtual DWORD CardControl(DWORD controlCode, LPVOID pInData, DWORD cInData, LPVOID pOutData, DWORD cOutData);
  //! \defgroup sctrack_funcs Smart Card Tracking Functions
  //! @{
  //! \fn void WinSCard::GetStatusChange(DWORD dwTimeout, ReadersStates& readerStates)
  //! \brief Blocks execution until the current availability of the cards in a specific set of readers changes.
  //! \details The caller supplies a list of readers to be monitored by an ReadersStates and the maximum amount of time (in milliseconds) that it is willing to wait for an action to occur on one of the listed readers. 
  //! \details Note that GetStatusChange uses the user-supplied value in the CurrentState members of the ReaderStates as the definition of the current state of the readers.
  //! \details The function returns when there is a change in availability, having filled in the EventState members of ReaderStates appropriately.
  //! \exception SCardException Thrown when a Card error condition occurs and throwErrors is set true.
  //! \param[in]          dwTimeout    The maximum amount of time, in milliseconds, to wait for an action. A value of zero causes the function to return immediately. A value of INFINITE causes this function never to time out.
  //! \param[in,out] readerStates Class contains states of the readers.
  //! @}
  virtual void GetStatusChange(DWORD dwTimeout, ReadersStates& readerStates);
private:
 SCARDCONTEXT	m_hContext{ 0 };//!< Stores a handle that identifies the resource manager context.
 SCARDHANDLE m_hCard{ 0 };//!< Stores a handle that identifies the connection to the smart card in the designated reader.
 DWORD	m_nProtocol{ T0 };//!< Stores a protocol of connected card
 bool mContextEstablished{ false };//!< Flag of control establish resource manager context.
 bool mCardConnected{ false };//!< Flag of control connecting card.
 long	m_nLastError{ SUCCESS };//!< Stores last error code
 APDUCommand mLastCommand{ 0,0,0,0 };//!< Stores last transmit command
 APDUResponse mLastResponse{ 0,0 };//!< Stores last response from transit command
 bool m_bThrowingErrors{ true };//!< Flag of control throwing exceptions
 bool m_bReleaseContext{ true };
};

}
#endif
