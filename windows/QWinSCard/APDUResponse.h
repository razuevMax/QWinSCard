/*!
\file APDUResponse.h
\brief Header file for APDUResponse class
 File is contains class APDUResponse represents the APDU response sent by the card
*/
#ifndef APDURESPONSE_H
#define APDURESPONSE_H
#include <winscard.h>
#include <QByteArray>

//! \namespace Smartcards
//! \brief Namespace of smartcard classes
namespace Smartcards
{
//! \class APDUResponse
//! \brief This class represents the APDU response sent by the card
class APDUResponse
{
 QByteArray	m_vData;//!< Byte array of data from the card
 BYTE	m_bSw1{ 0 };//!< First status byte
 BYTE	m_bSw2{ 0 };//!< Second status byte
public:
 //! \brief Status bytes length
 static const int SW_LENGTH = 2;
 //! \brief Constructor from the byte data sent back by the card
 //! \param[in] vData Byte array of data from the card
 APDUResponse(const QByteArray& vData= QByteArray());
 //! \fn APDUResponse::APDUResponse( const APDUResponse& response )
 //! \brief Copy constructor.
 //! \param[in] response The response.
 APDUResponse(const APDUResponse& response);
 //! \fn APDUResponse::APDUResponse(BYTE sw1, BYTE sw2)
 //! \brief Constructor from the two status bytes
 //! \param[in] sw1 First status byte
 //! \param[in] sw2 Second status byte
 APDUResponse(BYTE sw1, BYTE sw2);
 //! \fn APDUResponse& APDUResponse::operator=(const APDUResponse& other)
 //! \brief Assignment operator.
 //! \param[in] other The other response object.
 //! \return A shallow copy of this object.
 APDUResponse& operator=(const APDUResponse& other);
 //! \fn APDUResponse::~APDUResponse(void)
 //! \brief Destructor.
 ~APDUResponse(void);
 //! \fn QByteArray APDUResponse::getData( void )
 //! \brief Response data get property.
 //! \return byte array of data sent by the card minus the 2 status bytes (SW1, SW2). Is empty if no data were sent by the card.
 QByteArray getData(void) const { return m_vData; }
 //! \fn void APDUResponse::setData( const QByteArray& vData )
 //! \brief Response data set property.
 //! \param[in] vData Byte array of data from the card
 void setData(const QByteArray& vData);
 //! \fn BYTE getSW1(void) const
 //! \brief SW1 byte get property
 //! \return first status byte
 BYTE getSW1(void) const { return m_bSw1; }
 //! \fn BYTE getSW2(void) const
 //! \brief SW2 byte get property
 //! \return second status byte
 BYTE getSW2(void) const { return m_bSw2; }
 //! \fn BYTE getSW2(void) const
 //! \brief Status get property
 //! \return status word
 WORD getStatus(void) const { return static_cast<WORD>((static_cast<short>(m_bSw1) << 8) + static_cast<short>(m_bSw2)); }
 //! \fn QString APDUResponse::toString( void ) const
 //! \brief Convert this object into a string representation.
 //! \return A QString that represents this object.
 QString toString(void) const;
};

}
Q_DECLARE_METATYPE(Smartcards::APDUResponse)

#endif