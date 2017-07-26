/*
 * hifi msg define.
 *
 * Copyright (c) 2013 Hisilicon Technologies CO., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _AUDIO_HIFI_H
#define _AUDIO_HIFI_H

#include <linux/types.h>

typedef enum {
	HIFI_CHN_SYNC_CMD = 0,
	HIFI_CHN_READNOTICE_CMD,
	HIFI_CHN_INVAILD_CMD
} HIFI_CHN_CMD_TYPE;

typedef struct HIFI_CHN_CMD_STRUCT {
	HIFI_CHN_CMD_TYPE cmd_type;
	unsigned int sn;
} HIFI_CHN_CMD;

struct misc_io_async_param {
	unsigned int para_in_l;
	unsigned int para_in_h;
	unsigned int para_size_in;
};

/* misc_io_sync_cmd */
struct misc_io_sync_param {
	unsigned int para_in_l;
	unsigned int para_in_h;
	unsigned int para_size_in;

	unsigned int para_out_l;
	unsigned int para_out_h;
	unsigned int para_size_out;
};

/* misc_io_senddata_cmd */
struct misc_io_senddata_async_param {
	unsigned int para_in_l;
	unsigned int para_in_h;
	unsigned int para_size_in;

	unsigned int data_src_l;
	unsigned int data_src_h;
	unsigned int data_src_size;
};

struct misc_io_senddata_sync_param {
	unsigned int para_in_l;
	unsigned int para_in_h;
	unsigned int para_size_in;

	unsigned int src_l;
	unsigned int src_h;
	unsigned int src_size;

	unsigned int dst_l;
	unsigned int dst_h;
	unsigned int dst_size;

	unsigned int para_out_l;
	unsigned int para_out_h;
	unsigned int para_size_out;
};

struct misc_io_get_phys_param {
	unsigned int flag;
	unsigned int phys_addr_l;
	unsigned int phys_addr_h;
};

struct misc_io_dump_buf_param {
	unsigned int user_buf_l;
	unsigned int user_buf_h;
	unsigned int clear;	/*clear current log buf */
	unsigned int buf_size;
};

/*
  *voice proxy interface
  */
/*****************************************************************************
  2 macro define
*****************************************************************************/
#define PROXY_VOICE_CODEC_MAX_DATA_LEN (32)	/* 16 bit */
#define PROXY_VOICE_RTP_MAX_DATA_LEN (256)	/* 16 bit */
/*****************************************************************************
  5 msg define
*****************************************************************************/

/* the MsgID define between PROXY and Voice */
enum voice_proxy_voice_msg_id {
	ID_VOICE_PROXY_RCTP_OM_INFO_NTF = 0xDDEC,
	ID_PROXY_VOICE_RCTP_OM_INFO_CNF = 0xDDED,
	ID_VOICE_PROXY_AJB_OM_INFO_NTF = 0xDDEE,
	ID_PROXY_VOICE_AJB_OM_INFO_CNF = 0xDDEF,
	ID_PROXY_VOICE_LTE_RX_NTF = 0xDDF0,
	ID_VOICE_PROXY_LTE_RX_CNF = 0xDDF1,
	ID_VOICE_PROXY_LTE_RX_NTF = 0xDDF2,
	ID_PROXY_VOICE_LTE_RX_CNF = 0xDDF3,
	ID_VOICE_PROXY_LTE_TX_NTF = 0xDDF4,
	ID_PROXY_VOICE_LTE_TX_CNF = 0xDDF5,
	ID_PROXY_VOICE_LTE_TX_NTF = 0xDDF6,
	ID_VOICE_PROXY_LTE_TX_CNF = 0xDDF7,

	ID_PROXY_VOICE_WIFI_RX_NTF = 0xDDF8,
	ID_VOICE_PROXY_WIFI_RX_CNF = 0xDDF9,
	ID_VOICE_PROXY_WIFI_TX_NTF = 0xDDFA,
	ID_PROXY_VOICE_WIFI_TX_CNF = 0xDDFB,
	ID_PROXY_VOICE_STATUS_IND = 0xDDFC,
	ID_PROXY_VOICE_ENCRYPT_KEY_BEGIN = 0xDDFD,
	ID_PROXY_VOICE_ENCRYPT_KEY_END = 0xDDFE,
	ID_PROXY_VOICE_DATA_MSGID_BUT
};

/*
 * 6 STRUCT define
 */

/*
 * describe: the struct of the Rx request between PROXY and hifi_voice by lte
 */
struct voice_proxy_lte_rx_notify {
	uint16_t msg_id;
	uint16_t sn;
	uint32_t ts;
	uint16_t codec_type;
	uint16_t dtx_enable;
	uint16_t rate_mode;
	uint16_t error_flag;
	uint16_t frame_type;
	uint16_t quality_idx;
	uint16_t data[PROXY_VOICE_CODEC_MAX_DATA_LEN];
	uint32_t ssrc;
	uint32_t reserved;
};

/*
 * describe: the struct of the Rx request between PROXY and hifi_voice by wifi
 * the struct is as the same to IMSA_VOICE_RTP_RX_DATA_IND_STRU
 */
struct voice_proxy_wifi_rx_notify {
	uint16_t msg_id;
	uint16_t reserved;
	uint32_t channel_id;
	uint32_t port_type;	/* Port Type: 0 RTP; 1 RTCP */
	uint16_t data_len;
	uint16_t data_seq;
	uint8_t frag_seq;
	uint8_t frag_max;
	uint16_t reserved2;
	uint8_t data[PROXY_VOICE_RTP_MAX_DATA_LEN];
};

/*
 * describe: the struct of the confirm between PROXY and hifi_voice
 */
struct voice_proxy_confirm {
	uint16_t msg_id;
	uint16_t reserved;
	uint32_t result;
};

/*
 * describe: the struct of the Tx request between Voice-Proxy and hifi_voice by lte
 */
struct voice_proxy_lte_tx_notify {
	uint16_t msg_id;
	uint16_t reserved;
	uint16_t amr_type;
	uint16_t frame_type;
	uint16_t data[PROXY_VOICE_CODEC_MAX_DATA_LEN];
};

/*
 * describe: the struct of the Tx request between Voice-Proxy and hifi_voice by wifi
 */
struct voice_proxy_wifi_tx_notify {
	uint16_t msg_id;
	uint16_t reserved;
	uint32_t channel_id;
	uint32_t port_type;
	uint16_t data_len;
	uint16_t data_seq;
	uint8_t frag_seq;
	uint8_t frag_max;
	uint16_t reserved2;
	uint8_t data[PROXY_VOICE_RTP_MAX_DATA_LEN];
};

struct voice_proxy_status {
	uint16_t msg_id;
	uint16_t reserved;
	uint32_t status;
	uint32_t socket_cfg;
};

struct voice_proxy_voice_encrypt_key_end {
	uint16_t msg_id;
	uint16_t reserved;
	bool encrypt_negotiation_result;
	bool reserved2[3];
};
struct misc_io_pcm_buf_param {
	uint64_t buf;
	uint32_t buf_size;
};


/* XAF STRUCTURES */

/* ...command/response message */
typedef struct xf_proxy_msg {
	/* ...session ID */
	uint32_t                 id;

	/* ...proxy API command/reponse code */
	uint32_t                 opcode;

	/* ...length of attached buffer */
	uint32_t                 length;

	/* ...shared logical address of message buffer */
	uint64_t                 address;

}	__attribute__((__packed__)) xf_proxy_msg_t;

/*
  *end
  */

#define HIFI_MISC_IOCTL_ASYNCMSG		_IOWR('A', 0x70, struct misc_io_async_param)
#define HIFI_MISC_IOCTL_SYNCMSG 		_IOW('A', 0x71, struct misc_io_sync_param)
#define HIFI_MISC_IOCTL_SENDDATA_SYNC	_IOW('A', 0x72, struct misc_io_senddata_sync_param)
#define HIFI_MISC_IOCTL_GET_PHYS		_IOWR('A', 0x73, struct misc_io_get_phys_param)
#define HIFI_MISC_IOCTL_TEST			_IOWR('A', 0x74, struct misc_io_senddata_sync_param)
#define HIFI_MISC_IOCTL_WRITE_PARAMS	_IOWR('A', 0x75, struct misc_io_sync_param)
#define HIFI_MISC_IOCTL_DUMP_HIFI		_IOWR('A', 0x76, struct misc_io_dump_buf_param)
#define HIFI_MISC_IOCTL_DUMP_CODEC		_IOWR('A', 0x77, struct misc_io_dump_buf_param)
#define HIFI_MISC_IOCTL_WAKEUP_THREAD	_IOW('A',  0x78, unsigned int)
#define HIFI_MISC_IOCTL_DISPLAY_MSG		_IOWR('A', 0x79, struct misc_io_dump_buf_param)
#define HIFI_MISC_IOCTL_WAKEUP_PCM_READ_THREAD _IOW('A',  0x7a, unsigned int)
#define HIFI_MISC_IOCTL_PCM_GAIN _IOW('A',  0x7b, struct misc_io_pcm_buf_param)
#define HIFI_MISC_IOCTL_XAF_IPC_MSG_SEND _IOW('A',  0x7c, xf_proxy_msg_t)
#define HIFI_MISC_IOCTL_XAF_IPC_MSG_RECV _IOR('A', 0x7d, xf_proxy_msg_t)

#ifdef CLT_VOICE
#define CLT_HIFI_MISC_IOCTL_SEND_VOICE _IOWR('A', 0x90, struct misc_io_async_param)
#endif

#define HIFI_MISC_IOCTL_GET_VOICE_BSD_PARAM	_IOWR('A',  0x7c, unsigned int)
#define INT_TO_ADDR(low, high) (void *) (unsigned long)((unsigned long long)(low) | ((unsigned long long)(high)<<32))
#define GET_LOW32(x) (unsigned int)(((unsigned long long)(unsigned long)(x))&0xffffffffULL)
#define GET_HIG32(x) (unsigned int)((((unsigned long long)(unsigned long)(x))>>32)&0xffffffffULL)

#endif				/* _AUDIO_HIFI_H */
