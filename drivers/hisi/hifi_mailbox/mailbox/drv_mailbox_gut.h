#ifndef __DRV_MAILBOX_GUT_H__
#define __DRV_MAILBOX_GUT_H__

#include "drv_mailbox_platform.h"
#include "drv_mailbox_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define MAILBOX_GLOBAL_CHANNEL_NUM	(4)

#define MAILBOX_SINGLE_ID_MASK						(0xFF)
#define mailbox_get_channel_id(id)					  ((id) & (~MAILBOX_SINGLE_ID_MASK))

#define mailbox_get_src_id(id)	(unsigned char)(((id) >> MAILBOX_ID_SRC_CPU_OFFSET) & MAILBOX_SINGLE_ID_MASK)

#define mailbox_get_dst_id(id)	(unsigned char)(((id) >> MAILBOX_ID_DST_CPU_OFFSET) & MAILBOX_SINGLE_ID_MASK)

#define mailbox_get_carrier_id(id)	(unsigned char)(((id) >> MAILBOX_ID_CHANNEL_OFFSET) & MAILBOX_SINGLE_ID_MASK)

#define mailbox_get_use_id(id)	(unsigned char)((id) & MAILBOX_SINGLE_ID_MASK)

#define MAILBOX_INIT_MAGIC							(0x87654312)

#define MAILBOX_ALIGN                                sizeof(unsigned int)
#define mailbox_align_size(a, p)                     (((a)+((p)-1)) & ~((p)-1))

#define mailbox_write_reg(Addr, Value)                  (*((volatile unsigned int *)(Addr)) = Value)

#define MAILBOX_CPU_NUM 							(MAILBOX_CPUID_BUTT)

#define MAILBOX_CHANNEL_BUTT(src, dst)	(MAILBOX_CHANNEL_##src##2##dst##_BUTT  & 0xff)	/*& 0xff Ïûpclint */

#define MAILBOX_USER_BUTT(src, dst, channel)    \
    ((unsigned int)MAILBOX_MAILCODE_ITEM_END(src, dst, channel) & 0xff)

#define MAILBOX_CHANNEL_COMPOSE(src, dst, ch)                   \
    /*channel*/                                                 \
    {(unsigned int)MAILBOX_MAILCODE_ITEM_END(src, dst, ch),    \
    /*DataSize*/                                                \
    (unsigned int)MAILBOX_QUEUE_SIZE(src, dst, ch),            \
    /*SingleMax*/                                               \
    (unsigned int)MAILBOX_MAILSIZE_MAX(src, dst, ch),          \
    /*HeadAddr*/                                                \
    (unsigned long)MAILBOX_HEAD_ADDR(src, dst, ch), \
    /*DataAddr*/                                                \
    (unsigned long)MAILBOX_QUEUE_ADDR(src, dst, ch),\
    /*IPC INT NUMBER*/                                          \
    (unsigned int)MAILBOX_IPC_INT_NUM(src, dst, ch)}

#define MAILBOX_USER_QUEUE(mbuf)   (&((struct mb_buff *)(mbuf))->usr_queue)

#define mailbox_queue_left(Rear, Front, Length) \
	(((Rear) > (Front)) ? (((Rear) - (Front)) - sizeof(unsigned int)) : \
	(((Length) - ((Front) - (Rear))) - sizeof(unsigned int)))

	typedef struct mb_queue {
		unsigned long base;
		unsigned int length;
		unsigned long front;
		unsigned long rear;
		unsigned int size;
	} mailbox_queue_stru;

	struct mailbox_id_stru {
		signed char UseId;
		signed char Channel;
		signed char DstCpu;
		signed char SrcCpu;
	};

	struct mb_cb {
		void (*func) (void *mbuf, void *handle, void *data);
		void *handle;
		void *data;
	};

	typedef struct mb_cfg {
		unsigned int butt_id;
		unsigned int data_size;
		unsigned int single_max;
		unsigned long head_addr;
		unsigned long data_addr;
		unsigned int int_src;
	} MAILBOX_CHANNEL_CFG_STRU;

	struct mb_buff {
		unsigned int channel_id;
		unsigned int mailcode;
		unsigned int seq_num;
		void *mutex;
		struct mb *mb;
		struct mb_cb *read_cb;
		struct mb_cfg *config;
#ifdef MAILBOX_OPEN_MNTN
		struct mb_mntn mntn;
#endif
		struct mb_queue mail_queue;
		struct mb_queue usr_queue;
	};

	typedef struct mb_link {
		unsigned int carrier_butt;
		struct mb_buff *channel_buff;
	} MAILBOX_LINK_STRU;

	typedef struct mb {
		unsigned int local_id;
		unsigned int init_flag;
		unsigned int log_prob;
		struct mb_log log_array[MAILBOX_ERRO_ARRAY_NUM];
		struct mb_link send_tbl[MAILBOX_CPUID_BUTT];
		struct mb_link recv_tbl[MAILBOX_CPUID_BUTT];
	} MAILBOX_STRU;

	extern MAILBOX_EXTERN struct mb_cfg g_mailbox_global_cfg_tbl[];

	extern MAILBOX_EXTERN struct mb_buff
	    g_mailbox_channel_handle_pool[MAILBOX_CHANNEL_NUM];

	extern MAILBOX_EXTERN struct mb_cb
	    g_mailbox_user_cb_pool[MAILBOX_USER_NUM];

	extern MAILBOX_EXTERN struct mb g_mailbox_handle;
	MAILBOX_EXTERN struct mb *mailbox_get_mb(void);

	MAILBOX_EXTERN struct mb_buff *mailbox_get_channel_handle(struct mb *mb,
								  unsigned int
								  mailcode);

	MAILBOX_EXTERN int mailbox_register_cb(unsigned int mail_code,
					       void (*cb) (void *mbuf,
							   void *handle,
							   void *data),
					       void *usr_handle,
					       void *usr_data);

	MAILBOX_EXTERN int mailbox_request_buff(unsigned int mailcode,
						void *mbuf);

	MAILBOX_EXTERN int mailbox_sealup_buff(struct mb_buff *mb_buf,
					       unsigned int usr_size);

	MAILBOX_EXTERN int mailbox_flush_buff(struct mb_buff *mbuff);

	MAILBOX_EXTERN int mailbox_send_buff(struct mb_buff *mb_buf);

	MAILBOX_EXTERN int mailbox_release_buff(struct mb_buff *mb_buf);

	MAILBOX_EXTERN int mailbox_write_buff(struct mb_queue *queue,
					      char *data, unsigned int size);

	MAILBOX_EXTERN int mailbox_read_buff(struct mb_queue *queue,
					     char *data, unsigned int size);

	MAILBOX_EXTERN unsigned long mailbox_virt_to_phy(unsigned long
							 virt_addr);

	MAILBOX_EXTERN unsigned long mailbox_phy_to_virt(unsigned long
							 phy_addr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif				/* end of drv_mailbox.h */
