#ifndef __DRV_MAILBOX_IFC_H__
#define __DRV_MAILBOX_IFC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define IFC_IN		0x1
#define IFC_OUT 	0x2
#define IFC_BI		0x4

#define IFC_CNT 		(0x1 << 8)
#define IFC_FIX 		(0x2 << 8)
#define IFC_VAR 		(0x4 << 8)

#define IFC_INCNT		(IFC_IN  | IFC_CNT)
#define IFC_INFIX		(IFC_IN  | IFC_FIX)
#define IFC_OUTFIX		(IFC_OUT | IFC_FIX)
#define IFC_BIFIX		(IFC_BI  | IFC_FIX)
#define IFC_INVAR		(IFC_IN  | IFC_VAR)
#define IFC_OUTVAR		(IFC_OUT | IFC_VAR)
#define IFC_BIVAR		(IFC_BI  | IFC_VAR)

#define IFC_MAX_ARG 	 (6)

#define IFC_LAUNCH		  0
#define IFC_RESPONSE	  1

#define IFC_INLINE	static __inline

#define IFC_WAIT_FOREVER			(0xffffffff)

#ifdef _DRV_LLT_
#define IFC_DEBUG_PARAM
#endif

#ifdef IFC_DEBUG_PARAM
#define _ifc_chk_arg(ph, db, sb)	   __ifc_chk_arg(ph, db, sb)
#define _ifc_dbg_arg(ph, db, sb, df)   __ifc_dbg_arg(ph, db, sb, df)
#else
#define _ifc_chk_arg(ph, db, sb)
#define _ifc_dbg_arg(ph, db, sb, df)
#endif

#define IFC_ALIGN					sizeof(unsigned int)
#define ifc_align_size(a, p) 					(((a)+((p)-1)) & ~((p)-1))

#ifdef _DRV_LLT_
#define C_CALL_ARG_R2L
#endif

	struct ifc_param {
		unsigned int type;
		unsigned int size;
	};

	struct ifc_head {
		unsigned int scode;
		unsigned int rcode;
		unsigned int stamp;
		unsigned int retval;
		unsigned int needret;
		unsigned int argv[IFC_MAX_ARG];
		unsigned int data;
		unsigned int data_phy_addr;

#ifdef IFC_DEBUG_PARAM
		struct ifc_param param_tbl[IFC_MAX_ARG];
		signed int param_prob;
#endif
	};

	struct ifc_arg {
		unsigned int addr;
		unsigned int size;
	};

	struct ifc_mb {
		void *mbuf;
		struct ifc_arg argo[IFC_MAX_ARG];
		struct ifc_head head;
		void *waitsem;
		unsigned int total_size;
		unsigned int data_size;
		unsigned long data_vitrual_addr;
	};

#ifdef C_CALL_ARG_R2L
#define _IFC_ADD_ARG0(ary, p)
#define _IFC_ADD_ARG1(ary, d1, t1, a1, s1)						 __ifc_arg_out(ary, d1, (unsigned int)a1, s1, 0)
#define _IFC_ADD_ARG2(ary, d2, t2, a2, s2, d1, t1, a1, s1, ...)  _IFC_ADD_ARG1(ary, d1, t1, a1, s1); __ifc_arg_out(ary, d2, (unsigned int)a2, s2, (unsigned int)a1)
#define _IFC_ADD_ARG3(ary, d3, t3, a3, s3, d2, t2, a2, s2, ...)  _IFC_ADD_ARG2(ary, d2, t2, a2, s2, __VA_ARGS__); __ifc_arg_out(ary, d3, (unsigned int)a3, s3, (unsigned int)a2)
#define _IFC_ADD_ARG4(ary, d4, t4, a4, s4, d3, t3, a3, s3, ...)  _IFC_ADD_ARG3(ary, d3, t3, a3, s3, __VA_ARGS__); __ifc_arg_out(ary, d4, (unsigned int)a4, s4, (unsigned int)a3)
#define _IFC_ADD_ARG5(ary, d5, t5, a5, s5, d4, t4, a4, s4, ...)  _IFC_ADD_ARG4(ary, d4, t4, a4, s4, __VA_ARGS__); __ifc_arg_out(ary, d5, (unsigned int)a5, s5, (unsigned int)a4)
#define _IFC_ADD_ARG6(ary, d6, t6, a6, s6, d5, t5, a5, s5, ...)  _IFC_ADD_ARG5(ary, d5, t5, a5, s5, __VA_ARGS__); __ifc_arg_out(ary, d6, (unsigned int)a6, s6, (unsigned int)a5)

#define _IFC_PUSHDATA0(d, av, p)
#define _IFC_PUSHDATA1(d, av, d1, t1, a1, s1)					  __ifc_push_arg(d, av, d1, (unsigned int)a1, s1, 0)
#define _IFC_PUSHDATA2(d, av, d2, t2, a2, s2, d1, t1, a1, s1)	  _IFC_PUSHDATA1(d, av, d1, t1, a1, s1); __ifc_push_arg(d, av, d2, (unsigned int)a2, s2, (unsigned int)a1)
#define _IFC_PUSHDATA3(d, av, d3, t3, a3, s3, d2, t2, a2, s2, ...) _IFC_PUSHDATA2(d, av, d2, t2, a2, s2, __VA_ARGS__); __ifc_push_arg(d, av, d3, (unsigned int)a3, s3, (unsigned int)a2)
#define _IFC_PUSHDATA4(d, av, d4, t4, a4, s4, d3, t3, a3, s3, ...) _IFC_PUSHDATA3(d, av, d3, t3, a3, s3, __VA_ARGS__); __ifc_push_arg(d, av, d4, (unsigned int)a4, s4, (unsigned int)a3)
#define _IFC_PUSHDATA5(d, av, d5, t5, a5, s5, d4, t4, a4, s4, ...) _IFC_PUSHDATA4(d, av, d4, t4, a4, s4, __VA_ARGS__); __ifc_push_arg(d, av, d5, (unsigned int)a5, s5, (unsigned int)a4)
#define _IFC_PUSHDATA6(d, av, d6, t6, a6, s6, d5, t5, a5, s5, ...) _IFC_PUSHDATA5(d, av, d5, t5, a5, s5, __VA_ARGS__); __ifc_push_arg(d, av, d6, (unsigned int)a6, s6, (unsigned int)a5)

#define _IFC_PCOPY0(in, out, av, ao, p)
#define _IFC_PCOPY1(in, out, av, ao, d1, t1, a1, s1)					  __ifc_copy_arg(in, out, d1, s1, av, ao)
#define _IFC_PCOPY2(in, out, av, ao, d2, t2, a2, s2, d1, t1, a1, s1)	  _IFC_PCOPY1(in, out, av, ao, d1, t1, a1, s1); __ifc_copy_arg(in, out, d2, s2, av, ao)
#define _IFC_PCOPY3(in, out, av, ao, d3, t3, a3, s3, d2, t2, a2, s2, ...) _IFC_PCOPY2(in, out, av, ao, d2, t2, a2, s2, __VA_ARGS__); __ifc_copy_arg(in, out, d3, s3, av, ao)
#define _IFC_PCOPY4(in, out, av, ao, d4, t4, a4, s4, d3, t3, a3, s3, ...) _IFC_PCOPY3(in, out, av, ao, d3, t3, a3, s3, __VA_ARGS__); __ifc_copy_arg(in, out, d4, s4, av, ao)
#define _IFC_PCOPY5(in, out, av, ao, d5, t5, a5, s5, d4, t4, a4, s4, ...) _IFC_PCOPY4(in, out, av, ao, d4, t4, a4, s4, __VA_ARGS__); __ifc_copy_arg(in, out, d5, s5, av, ao)
#define _IFC_PCOPY6(in, out, av, ao, d6, t6, a6, s6, d5, t5, a5, s5, ...) _IFC_PCOPY5(in, out, av, ao, d5, t5, a5, s5, __VA_ARGS__); __ifc_copy_arg(in, out, d6, s6, av, ao)

#define _IFC_OUTSIZE0(av, p)								   0
#define _IFC_OUTSIZE1(av, d1, t1, a1, s1)					   __ifc_out_size(d1, s1, av)
#define _IFC_OUTSIZE2(av, d2, t2, a2, s2, d1, t1, a1, s1)	   _IFC_OUTSIZE1(av, d1, t1, a1, s1) + __ifc_out_size(d2, s2, av)
#define _IFC_OUTSIZE3(av, d3, t3, a3, s3, d2, t2, a2, s2, ...)  _IFC_OUTSIZE2(av, d2, t2, a2, s2, __VA_ARGS__) + __ifc_out_size(d3, s3, av)
#define _IFC_OUTSIZE4(av, d4, t4, a4, s4, d3, t3, a3, s3, ...)  _IFC_OUTSIZE3(av, d3, t3, a3, s3, __VA_ARGS__) + __ifc_out_size(d4, s4, av)
#define _IFC_OUTSIZE5(av, d5, t5, a5, s5, d4, t4, a4, s4, ...)  _IFC_OUTSIZE4(av, d4, t4, a4, s4, __VA_ARGS__) + __ifc_out_size(d5, s5, av)
#define _IFC_OUTSIZE6(av, d6, t6, a6, s6, d5, t5, a5, s5, ...)  _IFC_OUTSIZE5(av, d5, t5, a5, s5, __VA_ARGS__) + __ifc_out_size(d6, s6, av)
#else
#define _IFC_ADD_ARG0(ary, p)
#define _IFC_ADD_ARG1(ary, d1, t1, a1, s1)						 __ifc_arg_out(ary, d1, (unsigned int)a1, s1, 0)
#define _IFC_ADD_ARG2(ary, d2, t2, a2, s2, d1, t1, a1, s1, ...)  __ifc_arg_out(ary, d2, (unsigned int)a2, s2, (unsigned int)a1); _IFC_ADD_ARG1(ary, d1, t1, a1, s1)
#define _IFC_ADD_ARG3(ary, d3, t3, a3, s3, d2, t2, a2, s2, ...)  __ifc_arg_out(ary, d3, (unsigned int)a3, s3, (unsigned int)a2); _IFC_ADD_ARG2(ary, d2, t2, a2, s2, __VA_ARGS__)
#define _IFC_ADD_ARG4(ary, d4, t4, a4, s4, d3, t3, a3, s3, ...)  __ifc_arg_out(ary, d4, (unsigned int)a4, s4, (unsigned int)a3); _IFC_ADD_ARG3(ary, d3, t3, a3, s3, __VA_ARGS__)
#define _IFC_ADD_ARG5(ary, d5, t5, a5, s5, d4, t4, a4, s4, ...)  __ifc_arg_out(ary, d5, (unsigned int)a5, s5, (unsigned int)a4); _IFC_ADD_ARG4(ary, d4, t4, a4, s4, __VA_ARGS__)
#define _IFC_ADD_ARG6(ary, d6, t6, a6, s6, d5, t5, a5, s5, ...)  __ifc_arg_out(ary, d6, (unsigned int)a6, s6, (unsigned int)a5); _IFC_ADD_ARG5(ary, d5, t5, a5, s5, __VA_ARGS__)

#define _IFC_PUSHDATA0(d, av, p)
#define _IFC_PUSHDATA1(d, av, d1, t1, a1, s1)					  __ifc_push_arg(d, av, d1, (unsigned int)a1, s1, 0)
#define _IFC_PUSHDATA2(d, av, d2, t2, a2, s2, d1, t1, a1, s1)	  __ifc_push_arg(d, av, d2, (unsigned int)a2, s2, (unsigned int)a1); _IFC_PUSHDATA1(d, av, d1, t1, a1, s1)
#define _IFC_PUSHDATA3(d, av, d3, t3, a3, s3, d2, t2, a2, s2, ...) __ifc_push_arg(d, av, d3, (unsigned int)a3, s3, (unsigned int)a2); _IFC_PUSHDATA2(d, av, d2, t2, a2, s2, __VA_ARGS__)
#define _IFC_PUSHDATA4(d, av, d4, t4, a4, s4, d3, t3, a3, s3, ...) __ifc_push_arg(d, av, d4, (unsigned int)a4, s4, (unsigned int)a3); _IFC_PUSHDATA3(d, av, d3, t3, a3, s3, __VA_ARGS__)
#define _IFC_PUSHDATA5(d, av, d5, t5, a5, s5, d4, t4, a4, s4, ...) __ifc_push_arg(d, av, d5, (unsigned int)a5, s5, (unsigned int)a4); _IFC_PUSHDATA4(d, av, d4, t4, a4, s4, __VA_ARGS__)
#define _IFC_PUSHDATA6(d, av, d6, t6, a6, s6, d5, t5, a5, s5, ...) __ifc_push_arg(d, av, d6, (unsigned int)a6, s6, (unsigned int)a5); _IFC_PUSHDATA5(d, av, d5, t5, a5, s5, __VA_ARGS__)

#define _IFC_PCOPY0(in, out, av, ao, p)
#define _IFC_PCOPY1(in, out, av, ao, d1, t1, a1, s1)					  __ifc_copy_arg(in, out, d1, s1, av, ao)
#define _IFC_PCOPY2(in, out, av, ao, d2, t2, a2, s2, d1, t1, a1, s1)	  __ifc_copy_arg(in, out, d2, s2, av, ao); _IFC_PCOPY1(in, out, av, ao, d1, t1, a1, s1)
#define _IFC_PCOPY3(in, out, av, ao, d3, t3, a3, s3, d2, t2, a2, s2, ...) __ifc_copy_arg(in, out, d3, s3, av, ao); _IFC_PCOPY2(in, out, av, ao, d2, t2, a2, s2, __VA_ARGS__)
#define _IFC_PCOPY4(in, out, av, ao, d4, t4, a4, s4, d3, t3, a3, s3, ...) __ifc_copy_arg(in, out, d4, s4, av, ao); _IFC_PCOPY3(in, out, av, ao, d3, t3, a3, s3, __VA_ARGS__)
#define _IFC_PCOPY5(in, out, av, ao, d5, t5, a5, s5, d4, t4, a4, s4, ...) __ifc_copy_arg(in, out, d5, s5, av, ao); _IFC_PCOPY4(in, out, av, ao, d4, t4, a4, s4, __VA_ARGS__)
#define _IFC_PCOPY6(in, out, av, ao, d6, t6, a6, s6, d5, t5, a5, s5, ...) __ifc_copy_arg(in, out, d6, s6, av, ao); _IFC_PCOPY5(in, out, av, ao, d5, t5, a5, s5, __VA_ARGS__)

#define _IFC_OUTSIZE0(av, p)								   0
#define _IFC_OUTSIZE1(av, d1, t1, a1, s1)					   __ifc_out_size(d1, s1, av)
#define _IFC_OUTSIZE2(av, d2, t2, a2, s2, d1, t1, a1, s1)	   __ifc_out_size(d2, s2, av) + _IFC_OUTSIZE1(av, d1, t1, a1, s1)
#define _IFC_OUTSIZE3(av, d3, t3, a3, s3, d2, t2, a2, s2, ...)  __ifc_out_size(d3, s3, av) + _IFC_OUTSIZE2(av, d2, t2, a2, s2, __VA_ARGS__)
#define _IFC_OUTSIZE4(av, d4, t4, a4, s4, d3, t3, a3, s3, ...)  __ifc_out_size(d4, s4, av) + _IFC_OUTSIZE3(av, d3, t3, a3, s3, __VA_ARGS__)
#define _IFC_OUTSIZE5(av, d5, t5, a5, s5, d4, t4, a4, s4, ...)  __ifc_out_size(d5, s5, av) + _IFC_OUTSIZE4(av, d4, t4, a4, s4, __VA_ARGS__)
#define _IFC_OUTSIZE6(av, d6, t6, a6, s6, d5, t5, a5, s5, ...)  __ifc_out_size(d6, s6, av) + _IFC_OUTSIZE5(av, d5, t5, a5, s5, __VA_ARGS__)

#endif

#define _IFC_PARAM0(in, out, av, p)
#define _IFC_PARAM1(in, out, av, d1, t1, a1, s1)					  (t1)__ifc_gen_arg(in, out, d1, s1, av)
#define _IFC_PARAM2(in, out, av, d2, t2, a2, s2, d1, t1, a1, s1)	  (t2)__ifc_gen_arg(in, out, d2, s2, av), _IFC_PARAM1(in, out, av, d1, t1, a1, s1)
#define _IFC_PARAM3(in, out, av, d3, t3, a3, s3, d2, t2, a2, s2, ...) (t3)__ifc_gen_arg(in, out, d3, s3, av), _IFC_PARAM2(in, out, av, d2, t2, a2, s2, __VA_ARGS__)
#define _IFC_PARAM4(in, out, av, d4, t4, a4, s4, d3, t3, a3, s3, ...) (t4)__ifc_gen_arg(in, out, d4, s4, av), _IFC_PARAM3(in, out, av, d3, t3, a3, s3, __VA_ARGS__)
#define _IFC_PARAM5(in, out, av, d5, t5, a5, s5, d4, t4, a4, s4, ...) (t5)__ifc_gen_arg(in, out, d5, s5, av), _IFC_PARAM4(in, out, av, d4, t4, a4, s4, __VA_ARGS__)
#define _IFC_PARAM6(in, out, av, d6, t6, a6, s6, d5, t5, a5, s5, ...) (t6)__ifc_gen_arg(in, out, d6, s6, av), _IFC_PARAM5(in, out, av, d5, t5, a5, s5, __VA_ARGS__)

#define _IFC_DECL0(p)		/*void */
#define _IFC_DECL1(d1, t1, a1, s1)		 t1 a1,
#define _IFC_DECL2(d2, t2, a2, s2, ...)  t2 a2, _IFC_DECL1(__VA_ARGS__)
#define _IFC_DECL3(d3, t3, a3, s3, ...)  t3 a3, _IFC_DECL2(__VA_ARGS__)
#define _IFC_DECL4(d4, t4, a4, s4, ...)  t4 a4, _IFC_DECL3(__VA_ARGS__)
#define _IFC_DECL5(d5, t5, a5, s5, ...)  t5 a5, _IFC_DECL4(__VA_ARGS__)
#define _IFC_DECL6(d6, t6, a6, s6, ...)  t6 a6, _IFC_DECL5(__VA_ARGS__)

#define _IFC_INSIZE0(p) 								  0
#define _IFC_INSIZE1(d1, t1, a1, s1)					  __ifc_in_size(d1, s1, 0)
#define _IFC_INSIZE2(d2, t2, a2, s2, d1, t1, a1, s1)	 _IFC_INSIZE1(d1, t1, a1, s1) + __ifc_in_size(d2, s2, (unsigned int)a1)
#define _IFC_INSIZE3(d3, t3, a3, s3, d2, t2, a2, s2, ...) _IFC_INSIZE2(d2, t2, a2, s2, __VA_ARGS__) + __ifc_in_size(d3, s3, (unsigned int)a2)
#define _IFC_INSIZE4(d4, t4, a4, s4, d3, t3, a3, s3, ...) _IFC_INSIZE3(d3, t3, a3, s3, __VA_ARGS__) + __ifc_in_size(d4, s4, (unsigned int)a3)
#define _IFC_INSIZE5(d5, t5, a5, s5, d4, t4, a4, s4, ...) _IFC_INSIZE4(d4, t4, a4, s4, __VA_ARGS__) + __ifc_in_size(d5, s5, (unsigned int)a4)
#define _IFC_INSIZE6(d6, t6, a6, s6, d5, t5, a5, s5, ...) _IFC_INSIZE5(d5, t5, a5, s5, __VA_ARGS__) + __ifc_in_size(d6, s6, (unsigned int)a5)

#define _IFC_DBGPARAM0(p, a)
#define _IFC_DBGPARAM1(ph, d1, t1, a1, s1)					   _ifc_dbg_arg(ph, d1, s1, 0)
#define _IFC_DBGPARAM2(ph, d2, t2, a2, s2, d1, t1, a1, s1)	   _IFC_DBGPARAM1(ph, d1, t1, a1, s1) ; _ifc_dbg_arg(ph, d2, s2, d1)
#define _IFC_DBGPARAM3(ph, d3, t3, a3, s3, d2, t2, a2, s2, ...) _IFC_DBGPARAM2(ph, d2, t2, a2, s2, __VA_ARGS__); _ifc_dbg_arg(ph, d3, s3, d2)
#define _IFC_DBGPARAM4(ph, d4, t4, a4, s4, d3, t3, a3, s3, ...) _IFC_DBGPARAM3(ph, d3, t3, a3, s3, __VA_ARGS__); _ifc_dbg_arg(ph, d4, s4, d3)
#define _IFC_DBGPARAM5(ph, d5, t5, a5, s5, d4, t4, a4, s4, ...) _IFC_DBGPARAM4(ph, d4, t4, a4, s4, __VA_ARGS__); _ifc_dbg_arg(ph, d5, s5, d4)
#define _IFC_DBGPARAM6(ph, d6, t6, a6, s6, d5, t5, a5, s5, ...) _IFC_DBGPARAM5(ph, d5, t5, a5, s5, __VA_ARGS__); _ifc_dbg_arg(ph, d6, s6, d5)

#define _IFC_CHKPARAM0(p, a)
#define _IFC_CHKPARAM1(ph, d1, t1, a1, s1)					   _ifc_chk_arg(ph, d1, s1)
#define _IFC_CHKPARAM2(ph, d2, t2, a2, s2, d1, t1, a1, s1)	   _ifc_chk_arg(ph, d2, s2); _IFC_CHKPARAM1(ph, d1, t1, a1, s1)
#define _IFC_CHKPARAM3(ph, d3, t3, a3, s3, d2, t2, a2, s2, ...) _ifc_chk_arg(ph, d3, s3); _IFC_CHKPARAM2(ph, d2, t2, a2, s2, __VA_ARGS__)
#define _IFC_CHKPARAM4(ph, d4, t4, a4, s4, d3, t3, a3, s3, ...) _ifc_chk_arg(ph, d4, s4); _IFC_CHKPARAM3(ph, d3, t3, a3, s3, __VA_ARGS__)
#define _IFC_CHKPARAM5(ph, d5, t5, a5, s5, d4, t4, a4, s4, ...) _ifc_chk_arg(ph, d5, s5); _IFC_CHKPARAM4(ph, d4, t4, a4, s4, __VA_ARGS__)
#define _IFC_CHKPARAM6(ph, d6, t6, a6, s6, d5, t5, a5, s5, ...) _ifc_chk_arg(ph, d6, s6); _IFC_CHKPARAM5(ph, d5, t5, a5, s5, __VA_ARGS__)

#define _IFC_GEN_CALLx(x, id, name, ...)																							\
int name(_IFC_DECL##x(__VA_ARGS__) unsigned int timeout) 																		\
{																																	\
	struct ifc_mb ifc_b;																											\
	struct ifc_arg *argo = ifc_b.argo;																								\
	unsigned int in_buf;																											\
	unsigned int *argv = ifc_b.head.argv;														\
	unsigned int in_size = (_IFC_INSIZE##x(__VA_ARGS__));																			\
	argo = argo; argv = argv;																											\
	if (x > IFC_MAX_ARG) return 0;																									\
	if (mailbox_ifc_init_buf(&ifc_b, id, IFC_LAUNCH, in_size, timeout)) return 0;													\
	in_buf = ifc_b.head.data;																										\
	_IFC_DBGPARAM##x(&ifc_b.head, __VA_ARGS__);  																	\
	_IFC_PUSHDATA##x(&in_buf, &argv, __VA_ARGS__);										 \
	if (timeout) {				   																				\
		_IFC_ADD_ARG##x(&argo, __VA_ARGS__); 		 \
		return mailbox_ifc_send_wait(&ifc_b, timeout); 												\
	}																																\
	else																															\
		return mailbox_ifc_send_no_wait(&ifc_b);  																 \
}

#define IFC_GEN_EXEC_NAME(name) ife_##name

#define __IFC_GEN_EXECx(x, name, ...)																								 \
int IFC_GEN_EXEC_NAME(name)(struct ifc_head *ifc_h)																				 \
{																																	 \
	struct ifc_mb ifc_b;																											 \
	unsigned int in_b, out_b;																										 \
	unsigned int *argv = ifc_h->argv;																	 \
	unsigned int *argo;																											 \
	unsigned int out_size = (_IFC_OUTSIZE##x(&argv, __VA_ARGS__));																	 \
	if (mailbox_ifc_init_buf(&ifc_b, ifc_h->rcode, IFC_RESPONSE, out_size, 0)) return MAILBOX_ERRO;								 \
	in_b = ifc_h->data; out_b = ifc_b.head.data;  argv = ifc_h->argv;																 \
	_IFC_CHKPARAM##x(ifc_h, __VA_ARGS__);  						 \
	ifc_b.head.retval = (unsigned int)name(_IFC_PARAM##x(&in_b, &out_b, &argv, __VA_ARGS__));										 \
	if (ifc_h->needret) {				 																		 \
		in_b = ifc_h->data; out_b = ifc_b.head.data; ifc_b.head.stamp = ifc_h->stamp; 						  \
		argv = ifc_h->argv;   argo = ifc_b.head.argv; 														 \
		_IFC_PCOPY##x(&in_b, &out_b, &argv, &argo, __VA_ARGS__);   									 \
		return mailbox_ifc_send_no_wait(&ifc_b);											  \
	}																																 \
	else																															 \
		return mailbox_ifc_discard_buf(&ifc_b); 												  \
}

	extern int mailbox_ifc_init_buf(struct ifc_mb *ifc_b,
					unsigned int mailcode,
					unsigned int direct,
					unsigned int inlen,
					unsigned int timeout);

	extern int mailbox_ifc_send_no_wait(struct ifc_mb *ifc_b);

	extern int mailbox_ifc_wait_resp(struct ifc_mb *ifc_b);

	extern int mailbox_ifc_discard_buf(struct ifc_mb *ifc_b);

	extern int mailbox_ifc_send_wait(struct ifc_mb *ifc_b,
					 unsigned int timeout);

	extern int mailbox_ifc_register_exec(unsigned int mailcode,
					     int (*ife_cb) (struct ifc_head *
							    ifc_h)
	    );

	extern void __ifc_arg_out(struct ifc_arg **ary, unsigned int db,
				  unsigned long ab, unsigned int sb,
				  unsigned int af);

	extern void __ifc_push_arg(unsigned long *in, unsigned int **av,
				   unsigned int db, unsigned long ab,
				   unsigned int sb, unsigned int af);

	extern int __ifc_in_size(unsigned int db, unsigned int sb,
				 unsigned int af);

	extern void __ifc_dbg_arg(struct ifc_head *head, unsigned int db,
				  unsigned int sb, unsigned int df);

	extern int __ifc_out_size(unsigned int db, unsigned int sb,
				  unsigned int **pp_af);

	extern long __ifc_gen_arg(unsigned long *in, unsigned long *out,
				  unsigned int db, unsigned int sb,
				  unsigned int **pp_af);

	extern void __ifc_copy_arg(unsigned long *in, unsigned long *out,
				   unsigned int db, unsigned int sb,
				   unsigned int **av, unsigned int **ao);

	extern void __ifc_chk_arg(struct ifc_head *head, unsigned int db,
				  unsigned int sb);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif				/*end of __DRV_MAILBOX_IFC_H__ */
