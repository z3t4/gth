

#ifndef _TAGMCPACKET_H_
#define _TAGMCPACKET_H_


struct tagMCPaket_MSC_REPLYLOGIN{
	public:
		enum enumError{
			none=0,
			mismatch_password=1,
			unregister_user=2,
			double_login=3,
			database_error=4,
			block_user=5,
			billing_network_error=6,
			billing_point_not_enough=7,
			billing_error=8,
			not_check_real_name=9,
			over_auth_time= 10, 
			unknown,
		};

		enum enumCertification{
			nick_name=0,
			real_name=1,
			developer=2,
			unknownv2=3,
		};
};

#endif