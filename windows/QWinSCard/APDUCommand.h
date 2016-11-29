//! \file APDUCommand.h
//! \brief Header file for APDUCommand class.
#ifndef APDUCOMMAND_H
#define APDUCOMMAND_H
#include <winscard.h>
#include <QtCore/QByteArray>

//! \namespace Smartcards
//! \brief Namespace of smartcard classes
namespace Smartcards
{
//! \class APDUCommand
//! \brief This class represents the APDU command
class APDUCommand
{
public:
	static const int	APDU_MIN_LENGTH = 4;//!<Minimum size in bytes of an APDU command
private:
 BYTE m_bClass{ 0 };//!< Class(CLA) byte of APDU command
 BYTE m_bIns{ 0 };//!< Instruction(INS) byte of APDU command
 BYTE m_bP1{ 0 };//!< First param(P1) byte of APDU command
 BYTE m_bP2{ 0 };//!< Second param(P2) byte of APDU command
 BYTE m_nLe{ 0 };//!< Number of data expected in APDU command
 QByteArray m_vData;//!< Byte array contains data inserting in APDU command
public:
	/*!
	 \brief Constructor
	 \param[in] bCla Class(CLA) byte of APDU command
	 \param[in] bIns Instruction(INS) byte of APDU command
	 \param[in] bP1 First param(P1) byte of APDU command. Default value is 0.
	 \param[in] bP2 Second param(P2) byte of APDU command. Default value is 0.
	 \param[in] vData Data to send to the card if any, empty if no data to send.  Default is empty.
	 \param[in] nLe Number of data expected, 0 if none. Default value is 0.
 */
 APDUCommand(BYTE bCla, BYTE bIns, BYTE bP1 = 0, BYTE bP2 = 0, const QByteArray& vData = QByteArray(), BYTE nLe = 0);
 /*!
 \brief Constructor with initilizer list
 \param[in] bCla Class(CLA) byte of APDU command
 \param[in] bIns Instruction(INS) byte of APDU command
 \param[in] bP1 First param(P1) byte of APDU command
 \param[in] bP2 Second param(P2) byte of APDU command
 \param[in] ilist Initializer list of bytes contains data to send to the card.
 \param[in] nLe Number of data expected, 0 if none. Default value is 0.
 */
 APDUCommand(BYTE bCla, BYTE bIns, BYTE bP1, BYTE bP2, std::initializer_list<char> ilist, BYTE nLe = 0);
 /*!
 \brief Destructor
 */
	virtual ~APDUCommand(void);
 /*!
 \brief Set a APDU command
 \param[in] bCla Class(CLA) byte of APDU command
 \param[in] bIns Instruction(INS) byte of APDU command
 \param[in] bP1 First param(P1) byte of APDU command. Default value is 0.
 \param[in] bP2 Second param(P2) byte of APDU command. Default value is 0.
 \param[in] vData Data to send to the card if any, empty if no data to send.  Default is empty.
 \param[in] nLe Number of data expected, 0 if none. Default value is 0.
 */
 void setCommand(BYTE bCla, BYTE bIns, BYTE bP1=0, BYTE bP2=0, const QByteArray& vData = QByteArray(), BYTE nLe = 0);
 /*!
 \brief Convert this object into a string representation.
 \return Formatted string with APDU command in hex
 */
	QString toString(void) const;
 /*!
 \brief Class(CLA) byte set property
 \param[in] cls Class(CLA) byte
 */
 void setClass(BYTE cls) { m_bClass=cls; }
 /*!
 \brief Instruction(INS) byte set property
 \param[in] ins Instruction(INS) byte
 */
 void setIns(BYTE ins) { m_bIns = ins; }
 /*!
 \brief First param(P1) byte set property
 \param[in] p1 First param(P1) byte
 */
 void setP1(BYTE p1) { m_bP1=p1; }
 /*!
 \brief Second param(P2) byte set property
 \param[in] p2 Second param(P2) byte
 */
 void setP2(BYTE p2) { m_bP2=p2; }
 /*!
 \brief Number of data set property
 \param[in] le Number of data
 */
 void setLe(BYTE le) { m_nLe = le; }
 /*!
 \brief Data to send to the card set property
 \param[in] data Data to send to the card
 */
 void setData(const QByteArray& data) { m_vData = data; }
 /*!
 \brief Class(CLA) byte get property
 \return Class(CLA) byte
 */
 BYTE getClass(void) const { return m_bClass; }
 /*!
 \brief Instruction(INS) byte get property
 \return Instruction(INS) byte
 */
 BYTE getIns(void) const { return m_bIns; }
 /*!
 \brief First param(P1) byte get property
 \return First param(P1) byte
 */
 BYTE getP1(void) const { return m_bP1; }
 /*!
 \brief Second param(P2) byte get property
 \return Second param(P2) byte
 */
 BYTE getP2(void) const { return m_bP2; }
 /*!
 \brief Number of data byte get property
 \return Number of data byte
 */
 BYTE getLe(void) const { return m_nLe; }
 /*!
 \brief Data to send to the card get property
 \return Data to send to the card
 */
 QByteArray getData(void) const { return m_vData; }
 /*!
 \brief Get size of data in the byte array
 \return size_t cast of data size
 */
 size_t getDataSize(void) const { return static_cast<size_t>(m_vData.size()); }
 /*!
 \brief Check empty data
 \return true if data empty, otherwise false
 */
 bool emptyData(void) const { return m_vData.isEmpty(); }
 /*!
 \brief Create byte array contains the APDU command for transmit to the card
 \return Byte array contains the APDU command
 */
 QByteArray createCommandVector(void) const;
};

}
#endif