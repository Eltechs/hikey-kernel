#include "drv_mailbox_cfg.h"
#include "drv_mailbox_debug.h"
#include "drv_mailbox_gut.h"
#include "drv_mailbox_ifc.h"
#include "drv_mailbox_msg.h"

#undef  _MAILBOX_FILE_
#define _MAILBOX_FILE_   "ifc"
/*extern int BSP_CPU_StateGet(int CpuID);*/
int mailbox_ifc_get_rcode(unsigned int scode)
{
	unsigned int src_id = mailbox_get_src_id(scode);
	unsigned int dst_id = mailbox_get_dst_id(scode);
	/*unsigned long carrier_id   = mailbox_get_carrier_id(scode); */

	return (int)((dst_id << MAILBOX_ID_SRC_CPU_OFFSET)
		     | (src_id << MAILBOX_ID_DST_CPU_OFFSET)
		     /*|(carrier_id << MAILBOX_ID_CHANNEL_OFFSET) */
	    );
}

int mailbox_ifc_buf_pad(struct ifc_mb *ifc_b, unsigned int datalen)
{
	struct mb_queue *que = (MAILBOX_USER_QUEUE(ifc_b->mbuf));

	unsigned int talsize;

	talsize = sizeof(struct ifc_head) + datalen;
	/*ifc_b->head.lmsg = talsize; */

	if ((que->front + sizeof(struct ifc_head)) >= que->base + que->length) {
		/*ifc_b->head.data = que->front + sizeof(struct ifc_head) - que->length; */
		ifc_b->data_vitrual_addr =
		    que->front + sizeof(struct ifc_head) - que->length;
	} else {
		if ((que->front + talsize) >= que->base + que->length) {
			/*ifc_b->head.data = que->base; */
			ifc_b->data_vitrual_addr = que->base;
			talsize += (unsigned int)((que->base + que->length) -
						  (que->front +
						   sizeof(struct ifc_head)));
		} else {
			/*ifc_b->head.data = que->front + sizeof(struct ifc_head); */
			ifc_b->data_vitrual_addr =
			    que->front + sizeof(struct ifc_head);
		}
	}

	if (talsize >
	    (unsigned int)mailbox_queue_left(que->rear, que->front,
					     que->length)) {

		return mailbox_logerro_p1(MAILBOX_FULL, datalen);
	}

	ifc_b->total_size = talsize;
	ifc_b->data_size = datalen;
	return MAILBOX_OK;
}

int mailbox_ifc_send_buf(struct ifc_mb *ifc_b)
{
	int ret_val = 0;
	unsigned long ifc_data = 0;
	struct mb_queue *que = (MAILBOX_USER_QUEUE(ifc_b->mbuf));

	ifc_data = ifc_b->data_vitrual_addr;

	ifc_b->head.data_phy_addr =
	    (unsigned int)mailbox_virt_to_phy(ifc_b->data_vitrual_addr);

	mailbox_write_buff(que, (char *)&ifc_b->head, sizeof(struct ifc_head));

	que->front = ifc_data + ifc_b->data_size;

	if (MAILBOX_OK == mailbox_sealup_buff(ifc_b->mbuf, ifc_b->total_size)) {
		ret_val = mailbox_send_buff(ifc_b->mbuf);
	} else {
		ret_val = (int)MAILBOX_ERRO;
	}

	return ret_val;
}

void mailbox_ifc_waiter(void *mbuf, void *usrhandle, void *data)
{
	struct ifc_mb *ifc_b = (struct ifc_mb *)usrhandle;
	struct mb_queue *queue = MAILBOX_USER_QUEUE(mbuf);
	struct ifc_head head_r;

	int i;

	mailbox_read_buff(queue, (char *)&head_r, sizeof(struct ifc_head));

	if (head_r.stamp == ifc_b->head.stamp) {
		for (i = 0; i < IFC_MAX_ARG; i++) {
			if (ifc_b->argo[i].addr) {
				mailbox_memcpy((void *)(unsigned long)ifc_b->
					       argo[i].addr,
					       (const void *)
					       mailbox_phy_to_virt((unsigned
								    long)
								   (head_r.
								    argv[i])),
					       (ifc_b->argo[i].size));
			}
		}

		ifc_b->head.retval = head_r.retval;
		mailbox_complete(&ifc_b->waitsem);
	}
}

int mailbox_ifc_send_wait(struct ifc_mb *ifc_b, unsigned int timeout)
{
	unsigned int rcode = ifc_b->head.rcode;

	if (MAILBOX_OK != mailbox_register_cb(rcode, mailbox_ifc_waiter, ifc_b,
					      MAILBOX_NULL)) {
		goto wait_exit;
	}

	ifc_b->waitsem = mailbox_init_completion();
	if (MAILBOX_NULL == ifc_b->waitsem) {
		goto wait_exit;
	}
	if (MAILBOX_OK != mailbox_ifc_send_buf(ifc_b)) {
		goto wait_exit;
	}

	if (MAILBOX_OK != mailbox_wait_completion(&ifc_b->waitsem, timeout)) {
		ifc_b->head.retval = MAILBOX_TIME_OUT;

		mailbox_register_cb(rcode, MAILBOX_NULL, MAILBOX_NULL,
				    MAILBOX_NULL);
	}

 wait_exit:
	mailbox_del_completion(&ifc_b->waitsem);
	mailbox_release_buff(ifc_b->mbuf);

	return (int)(ifc_b->head.retval);
}

int mailbox_ifc_send_no_wait(struct ifc_mb *ifc_b)
{
	int ret = mailbox_ifc_send_buf(ifc_b);

	ret |= mailbox_release_buff(ifc_b->mbuf);

	return ret;
}

int mailbox_ifc_discard_buf(struct ifc_mb *ifc_b)
{
	int ret = mailbox_release_buff(ifc_b->mbuf);
	return ret;
}

void mailbox_ifc_executer(void *mbuf, void *handle, void *data)
{
	int (*ife) (struct ifc_head *ifc_h) = handle;
	struct ifc_head head;

	struct mb_queue *queue = MAILBOX_USER_QUEUE(mbuf);
	(void)mailbox_read_buff(queue, (char *)&head, sizeof(struct ifc_head));

	/*head.data = mailbox_phy_to_virt(head.data); */
	if (ife) {
		ife(&head);
	}
}

int mailbox_ifc_register_exec(unsigned int mailcode,
			      int (*ife_cb) (struct ifc_head *ifc_h)
    )
{
	return mailbox_register_cb(mailcode, mailbox_ifc_executer,
				   (void *)ife_cb,
				   (void *)(unsigned long)mailcode);
}

int mailbox_ifc_init_buf(struct ifc_mb *ifc_b,
			 unsigned int mailcode,
			 unsigned int direct,
			 unsigned int inlen, unsigned int timeout)
{
	int ret_val = MAILBOX_OK;
	int try_times = 0;
	unsigned int go_on = MAILBOX_FALSE;

	mailbox_memset(ifc_b, 0x00, sizeof(struct ifc_mb));
	ifc_b->head.scode = mailcode;
	ifc_b->head.retval = MAILBOX_ERRO;

	ret_val = (int)BSP_CPU_StateGet(mailbox_get_dst_id(mailcode));
	if (!ret_val) {
		return (int)MAILBOX_TARGET_NOT_READY;
	}

	if (timeout) {
		ifc_b->head.needret = MAILBOX_TRUE;
	}

	if (IFC_LAUNCH == direct) {
		ifc_b->head.rcode =
		    (unsigned int)mailbox_ifc_get_rcode(mailcode);
		ifc_b->head.stamp = (unsigned int)mailbox_get_timestamp();
	} else if (IFC_RESPONSE == direct) {
		ifc_b->head.rcode = 0;
	}

	do {
		ret_val = mailbox_request_buff(mailcode, (void *)&ifc_b->mbuf);
		if (ifc_b->mbuf) {
			ret_val = mailbox_ifc_buf_pad(ifc_b, inlen);
		}

		if ((int)MAILBOX_FULL == ret_val) {
			go_on =
			    (unsigned int)
			    mailbox_scene_delay(MAILBOX_DELAY_SCENE_IFC_FULL,
						&try_times);
			if (ifc_b->mbuf) {
				mailbox_release_buff(ifc_b->mbuf);
				ifc_b->mbuf = MAILBOX_NULL;
			}
		} else {
			go_on = MAILBOX_FALSE;
		}

	} while (MAILBOX_TRUE == go_on);

	if (MAILBOX_OK != ret_val) {
		if (MAILBOX_NULL != ifc_b->mbuf) {
			mailbox_release_buff(ifc_b->mbuf);
		}
		/*mailbox_show(mailcode,0); */
		/*mailbox_assert(mailcode); */
		return mailbox_logerro_p1(ret_val, mailcode);
	}

	return MAILBOX_OK;
}

void __ifc_arg_out(struct ifc_arg **p_ary, unsigned int db, unsigned long ab,
		   unsigned int sb, unsigned int af)
{
	struct ifc_arg *ary = *p_ary;
	if ((IFC_OUT & db) || (IFC_BI & db)) {
		ary->addr = ab;
		if (IFC_FIX & db) {
			ary->size = sb;
		} else if (IFC_VAR & db) {
			ary->size = af;
		}
	}
	*p_ary = *p_ary + 1;
}

void __ifc_push_arg(unsigned long *in, unsigned int **p_av, unsigned int db,
		    unsigned long ab, unsigned int sb, unsigned int af)
{

	unsigned int *av = *p_av;
	*av = ab;
	if ((IFC_IN & db) || (IFC_BI & db)) {
		if (IFC_CNT & db) {
			*(unsigned int *)(*in) = (unsigned int)ab;
			*in += ifc_align_size(sizeof(unsigned int), IFC_ALIGN);
		} else if (IFC_FIX & db) {
			mailbox_memcpy((void *)*in, (const void *)ab, sb);
			*in += ifc_align_size(sb, IFC_ALIGN);
		} else if (IFC_VAR & db) {
			mailbox_memcpy((void *)*in, (const void *)ab, af);
			*in += ifc_align_size(af, IFC_ALIGN);
		}
	}
	*p_av = *p_av + 1;
}

int __ifc_in_size(unsigned int db, unsigned int sb, unsigned int af)
{
	if ((IFC_IN & db) || (IFC_BI & db)) {
		if (IFC_CNT & db) {
			return ifc_align_size(sizeof(unsigned int), IFC_ALIGN);
		} else if (IFC_FIX & db) {
			return (int)ifc_align_size(sb, IFC_ALIGN);
		} else if (IFC_VAR & db) {
			return (int)ifc_align_size(af, IFC_ALIGN);
		}
	}
	return 0;
}

int __ifc_out_size(unsigned int db, unsigned int sb, unsigned int **pp_a)
{
	unsigned int size = 0;
	if ((IFC_OUT & db) || (IFC_BI & db)) {
		if (IFC_FIX & db) {
			size = ifc_align_size(sb, IFC_ALIGN);
		} else if (IFC_VAR & db) {
			unsigned int v_sz = *(unsigned int *)(*pp_a + 1);
			size = ifc_align_size(v_sz, IFC_ALIGN);
		}
	}
	*pp_a = *pp_a + 1;
	return (int)size;
}

long __ifc_gen_arg(unsigned long *in, unsigned long *out, unsigned int db,
		   unsigned int sb, unsigned int **pp_a)
{
	unsigned long ret_val = 0;
	unsigned int v_sz = 0;
	if (IFC_VAR & db) {
		v_sz = *(unsigned int *)(*pp_a + 1);
	}

	if (IFC_IN & db) {
		if (IFC_CNT & db) {
			ret_val = *(unsigned int *)*in;
			*in = *in + sizeof(unsigned int);
		} else if (IFC_FIX & db) {
			ret_val = *in;
			*in = *in + ifc_align_size(sb, IFC_ALIGN);
		} else if (IFC_VAR & db) {
			ret_val = *in;
			*in = *in + ifc_align_size(v_sz, IFC_ALIGN);
		}
	} else if (IFC_BI & db) {
		if (IFC_FIX & db) {
			ret_val = *in;
			*in = *in + ifc_align_size(sb, IFC_ALIGN);
			*out = *out + ifc_align_size(sb, IFC_ALIGN);
		} else if (IFC_VAR & db) {
			ret_val = *in;
			*in = *in + ifc_align_size(v_sz, IFC_ALIGN);
			*out = *out + ifc_align_size(v_sz, IFC_ALIGN);
		}
	} else if (IFC_OUT & db) {
		ret_val = *out;
		if (IFC_FIX & db) {
			*out = *out + ifc_align_size(sb, IFC_ALIGN);
		} else if (IFC_VAR & db) {
			*out = *out + ifc_align_size(v_sz, IFC_ALIGN);
		}
	}
	*pp_a = *pp_a + 1;
	return (long)ret_val;
}

void __ifc_copy_arg(unsigned long *in, unsigned long *out, unsigned int db,
		    unsigned int sb, unsigned int **p_av, unsigned int **p_ao)
{
	unsigned int cpsize = 0;
	unsigned int *av = *p_av;
	unsigned int *ao = *p_ao;

	*ao = 0;
	if (IFC_FIX & db) {
		cpsize = ifc_align_size(sb, IFC_ALIGN);
	} else if (IFC_VAR & db) {
		unsigned int v_sz = *(av + 1);
		cpsize = ifc_align_size(v_sz, IFC_ALIGN);
	} else if (IFC_CNT & db) {
		cpsize = sizeof(unsigned int);
	}

	if (IFC_BI & db) {
		mailbox_memcpy((void *)(*out), (const void *)(*in),
			       (unsigned int)cpsize);
		*ao = (unsigned int)mailbox_virt_to_phy(*out);
		*out += cpsize;
		*in += cpsize;
	} else if (IFC_IN & db) {
		*in += cpsize;
	} else if (IFC_OUT & db) {
		*ao = (unsigned int)mailbox_virt_to_phy(*out);
		*out += cpsize;
	}

	*p_av = *p_av + 1;
	*p_ao = *p_ao + 1;
}
