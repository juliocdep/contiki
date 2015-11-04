/*
 * radio.c
 *
 *  Created on: 06/10/2015
 *      Author: Julio
 */

#include "contiki.h"
#include "contiki-net.h"
#include "net/netstack.h"

#include "dev/radio.h"
#include "radio.h"
#include "board.h"

#define USE_BAND_915

#if defined( USE_BAND_433 )

#define RF_FREQUENCY                                434000000 // Hz

#elif defined( USE_BAND_470 )

#define RF_FREQUENCY                                470000000 // Hz

#elif defined( USE_BAND_868 )

#define RF_FREQUENCY                                868000000 // Hz

#elif defined( USE_BAND_915 )

#define RF_FREQUENCY                                915000000 // Hz

#else
    #error "Please define a frequency band in the compiler options."
#endif

#define TX_OUTPUT_POWER                             20        // dBm

// MODEM LORA:
#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       7         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         5         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false

// MODEM FSK:
#define FSK_FDEV                                    25e3      // Hz
#define FSK_DATARATE                                50e3      // bps
#define FSK_BANDWIDTH                               50e3      // Hz
#define FSK_AFC_BANDWIDTH                           83.333e3  // Hz
#define FSK_PREAMBLE_LENGTH                         5         // Same for Tx and Rx
#define FSK_FIX_LENGTH_PAYLOAD_ON                   false


typedef enum
{
	RADIO_OK = 0,
	RADIO_ERROR = 1,
}Status_t;

#define RX_TIMEOUT_VALUE                            1000000 // usec
#define BUFFER_SIZE                                 127 // Define the payload size here

uint16_t TX_BufferSize = BUFFER_SIZE;
uint8_t TX_Buffer[BUFFER_SIZE];

uint16_t RX_BufferSize = BUFFER_SIZE;
uint8_t RX_Buffer[BUFFER_SIZE];

static uint8_t isRadioOn = 0;
static uint8_t isPacketRecived = 0;

int8_t RssiValue = 0;
int8_t SnrValue = 0;


/*!
 * Radio events function pointer
 */
static RadioEvents_t RadioEvents;

/*!
 * \brief Function to be executed on Radio Tx Done event
 */
void OnTxDone( void );

/*!
 * \brief Function to be executed on Radio Rx Done event
 */
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );

/*!
 * \brief Function executed on Radio Tx Timeout event
 */
void OnTxTimeout( void );

/*!
 * \brief Function executed on Radio Rx Timeout event
 */
void OnRxTimeout( void );

/*!
 * \brief Function executed on Radio Rx Error event
 */
void OnRxError( void );

void InitLoRa(void);
void InitFSK(void);

static int init(void);
static int prepare(const void *payload, unsigned short payload_len);
static int transmit(unsigned short transmit_len);
static int send(const void *payload, unsigned short payload_len);
static int read(void *buf, unsigned short buf_len);
static int channel_clear(void);
static int receiving_packet(void);
static int pending_packet(void);
static int on(void);
static int off(void);
static radio_result_t get_value(radio_param_t param, radio_value_t *value);
static radio_result_t set_value(radio_param_t param, radio_value_t value);
static radio_result_t get_object(radio_param_t param, void *dest, size_t size);
static radio_result_t set_object(radio_param_t param, const void *src, size_t size);

/*---------------------------------------------------------------------------*/
PROCESS(semtech_radio_process, "Semtech radio driver");
/*---------------------------------------------------------------------------*/

static int init(void)
{
    // Radio initialization
    RadioEvents.TxDone = OnTxDone;
    RadioEvents.RxDone = OnRxDone;
    RadioEvents.TxTimeout = OnTxTimeout;
    RadioEvents.RxTimeout = OnRxTimeout;
    RadioEvents.RxError = OnRxError;

    Radio.Init(&RadioEvents);

    Radio.SetChannel(RF_FREQUENCY);

    InitLoRa();

    Radio.Sleep();

    process_start(&semtech_radio_process, NULL);

    return 0;
}

static int prepare(const void *payload, unsigned short payload_len)
{
	if (payload_len > BUFFER_SIZE)
		payload_len = BUFFER_SIZE;

	TX_BufferSize = payload_len;

	memcpy(TX_Buffer, payload, TX_BufferSize);

	return 0;
}

static int transmit(unsigned short transmit_len)
{
	if (!isRadioOn)
		return RADIO_TX_ERR;

	if (transmit_len > TX_BufferSize)
		transmit_len = TX_BufferSize;

	Radio.Send(TX_Buffer, transmit_len);

	return RADIO_TX_OK;
}

static int send(const void *payload, unsigned short payload_len)
{
	prepare(payload, payload_len);
	return transmit(payload_len);
}

static int read(void *buf, unsigned short buf_len)
{
	if (!isRadioOn && !isPacketRecived)
		return 0;

	if (buf_len > RX_BufferSize)
		buf_len = RX_BufferSize;

	memcpy(buf, RX_Buffer, buf_len);

	isPacketRecived = 0;

	return buf_len;
}

static int channel_clear(void)
{
	return 1;
}

static int receiving_packet(void)
{
	return 0;
}

static int pending_packet(void)
{
	return isPacketRecived;
}

static int on(void)
{
	if (isRadioOn)
		return 0;

	Radio.Rx(0);

	isRadioOn = 1;

	return 0;
}

static int off(void)
{
	if (!isRadioOn)
		return 0;

	Radio.Sleep();

	isRadioOn = 0;

	return 0;
}

static radio_result_t get_value(radio_param_t param, radio_value_t *value)
{
	if(!value)
		return RADIO_RESULT_INVALID_VALUE;

	return RADIO_RESULT_NOT_SUPPORTED;
}

static radio_result_t set_value(radio_param_t param, radio_value_t value)
{
	return RADIO_RESULT_NOT_SUPPORTED;
}

static radio_result_t get_object(radio_param_t param, void *dest, size_t size)
{
	return RADIO_RESULT_NOT_SUPPORTED;
}

static radio_result_t set_object(radio_param_t param, const void *src, size_t size)
{
	return RADIO_RESULT_NOT_SUPPORTED;
}

void InitLoRa(void)
{
	Radio.Standby();

	Radio.SetModem(MODEM_LORA);

	Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
	                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
	                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
	                                   true, 0, 0, LORA_IQ_INVERSION_ON, 3000000 );

	Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
								   LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
								   LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
								   0, true, 0, 0, LORA_IQ_INVERSION_ON, true );
}

void InitFSK(void)
{
	Radio.Standby();

	Radio.SetModem(MODEM_FSK);

	Radio.SetTxConfig( MODEM_FSK, TX_OUTPUT_POWER, FSK_FDEV, 0,
	                                  FSK_DATARATE, 0,
	                                  FSK_PREAMBLE_LENGTH, FSK_FIX_LENGTH_PAYLOAD_ON,
	                                  true, 0, 0, 0, 3000000 );

	Radio.SetRxConfig( MODEM_FSK, FSK_BANDWIDTH, FSK_DATARATE,
								  0, FSK_AFC_BANDWIDTH, FSK_PREAMBLE_LENGTH,
								  0, FSK_FIX_LENGTH_PAYLOAD_ON, 0, true,
								  0, 0,false, true );
}

void OnTxDone( void )
{
	LedToggle(LED_TX);
	Radio.Rx(0);
}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    RX_BufferSize = size;
    memcpy(RX_Buffer, payload, RX_BufferSize);
    RssiValue = rssi;
    SnrValue = snr;

    isPacketRecived = 1;

    LedToggle(LED_RX);
    Radio.Rx(0);

    process_poll(&semtech_radio_process);
}

void OnTxTimeout( void )
{
	Radio.Rx(0);
}

void OnRxTimeout( void )
{
	Radio.Rx(0);
}

void OnRxError( void )
{
	Radio.Rx(0);
}

const struct radio_driver semtech_radio_driver = {
	.init = init,
	.prepare = prepare,
	.transmit = transmit,
	.send = send,
	.read = read,
	.channel_clear = channel_clear,
	.receiving_packet = receiving_packet,
	.pending_packet = pending_packet,
	.on = on,
	.off = off,
	.get_value = get_value,
	.set_value = set_value,
	.get_object = get_object,
	.set_object = set_object,
};

PROCESS_THREAD(semtech_radio_process, ev, data)
{
	int len;
	PROCESS_BEGIN();

	while (1)
	{
		PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);

		packetbuf_clear();
		len = read(packetbuf_dataptr(), PACKETBUF_SIZE);

		if (len > 0)
		{
			packetbuf_set_datalen(len);

			NETSTACK_RDC.input();
		}
	}

	PROCESS_END();
}
