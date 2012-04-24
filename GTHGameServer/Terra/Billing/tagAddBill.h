



#ifndef _TAGADDBILL_H_
#define _TAGADDBILL_H_


struct tagAddBillPacket
{
	public:		

		enum enumResult_billing_authorization{
			success=0,
			access_deny_db=2,
			network_error=3,
			not_exist_authorization=10,
			shortage_point=11,
			not_exist_session=12,
			already_connected=17,
			illegal_user=18,
			unknown_packet=20,
			error_com=25,
			error_timeout_response_billserver=26,
			connect_fail_billingDB=27,
			unknown_game_no=30,
			error_billing_account_service_module=31,
			not_exist_authorization_this_game=33,
			error_busy=35,
			unknown=100,
			com_error_when_ip_check=200,
			not_enougf_service_provider=201,
			com_error_when_authorization_key_check=210,
			not_found_authorization_key=211,
		};
		

		enum{
			maxbytes_serverGUIDstring=32,
			maxbytes_session = 32,
			maxbytes_user_id = 40,
			maxbytes_user_ip = 24,
			maxbytes_user_status=3,

			packet_type_game_start			  = 0x00000001,
			packet_type_game_end              = 0x00000002,
			packet_type_server_alive		  = 0x00000003,
			packet_type_user_alert			  = 0x00000005,
			packet_type_user_sync			  = 0x00000006,
			packet_type_server_conn			  = 0x0000000c,
			packet_type_server_reset		  = 0x00000004,
			packet_type_billing_authorization = 0x00000008,

		};

	public:
		int		Packet_Type ;			
		int		Packet_Result ;			
		char	Game_Server[32] ;		
		char	Session[maxbytes_session];	
		char	User_CC[4] ;			
		char	User_No[20] ;			
		char	User_ID[maxbytes_user_id] ;			
		char	User_IP[maxbytes_user_ip] ;			
		char	User_Gender ;			
		char	User_Status[maxbytes_user_status] ;			
		char	User_PayType[4] ;		
		int		User_Age ;				
		int		Game_No ;				
		char	Bill_PayType[2] ;		
		char	Bill_Method[2] ;			
		char	Bill_Expire[12] ;		
		int		Bill_Remain ;			
};


struct tagAddBillPacket_User_Alert{
	enum enumResult{
		remain_time=1,
		now_log_off=0,
		refund=-1,
		double_login=-2,
		start_reserved_bill=-3,
		force_exit=-4,
	};
};



struct tagAddBillPacket_Game_Start{
		enum enumResult{
			success=0,
			access_deny_db=2,
			network_error=3,
			not_exist_authorization=10,
			shortage_point=11,
			not_exist_session=12,
			already_connected=17,
			illegal_user=18,
			unknown_packet=20,
			error_com=25,
			error_timeout_response_billserver=26,
			connect_fail_billingDB=27,
			unknown_game_no=30,
			error_billing_account_service_module=31,
			not_exist_authorization_this_game=33,
			error_busy=35,
			unknown=100,
			com_error_when_ip_check=200,
			not_enougf_service_provider=201,
			com_error_when_authorization_key_check=210,
			not_found_authorization_key=211,
		};
};



#endif

