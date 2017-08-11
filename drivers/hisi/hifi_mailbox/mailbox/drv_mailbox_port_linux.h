#ifndef _DRV_MAILBOX_PORT_LINUX_H_
#define _DRV_MAILBOX_PORT_LINUX_H_

#include <linux/kernel.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if 0
#ifndef _DRV_LLT_
#include <mach/hardware.h>

#define MEM_CORE_SHARE_PHY2VIRT(phy)		 (((unsigned int)phy) - IPC_SHARE_MEM_ADDR + IPC_SHARE_MEM_VIRT_ADDR)
#define MEM_CORE_SHARE_VIRT2PHY(virt)		 (((unsigned int)virt) - IPC_SHARE_MEM_VIRT_ADDR + IPC_SHARE_MEM_ADDR)

#else

#define MEM_CORE_SHARE_PHY2VIRT(phy)		 (phy)
#define MEM_CORE_SHARE_VIRT2PHY(virt)		 (virt)
#endif
#endif

#define MEM_CORE_SHARE_PHY2VIRT(phy)		 (phy)
#define MEM_CORE_SHARE_VIRT2PHY(virt)		 (virt)

#define MAILBOX_LOCAL_CPUID 						MAILBOX_CPUID_ACPU

#define MAILBOX_CHANNEL_NUM \
(MAILBOX_CHANNEL_BUTT(ACPU, HIFI)	 \
+ MAILBOX_CHANNEL_BUTT(HIFI, ACPU)	 \
)

#define MAILBOX_USER_NUM	\
(MAILBOX_USER_BUTT(HIFI, ACPU, MSG) \
)

#ifdef _DRV_LLT_
#define mailbox_out(p)								(printk p)
#else
#define mailbox_out(p)								(printk p)
#endif

#define _MAILBOX_LINE_								__LINE__

#define _MAILBOX_FILE_								(void *)(0) /*__FILE__*/

#define MAILBOX_LOG_LEVEL							MAILBOX_LOG_ERROR

#ifndef MAILBOX_OPEN_MNTN
#define MAILBOX_OPEN_MNTN
#endif

#define MAILBOX_RECORD_USEID_NUM					(64)

#define RT	"\n"

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif	/*_DRV_MAILBOX_LINUX_H_*/
