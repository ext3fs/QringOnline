static	void	P_proc_AW_LOGIN_RESULT(const int dbagent_fd)
{
	struct	body_AW_LOGIN_RESULT	*body	= (struct body_AW_LOGIN_RESULT *)(P_recv_q->data);


	if( D_GetClient_sec_active(dbagent_fd) == 0 )
		return;

	//check body_size 
	if( P_recv_q->data_size != sizeof(struct body_AW_LOGIN_RESULT) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_LOGIN_RESULT(%d) bad body_size(%d)\n",
					dbagent_fd, sizeof(struct body_AW_LOGIN_RESULT), P_recv_q->data_size);
		P_remove_client(dbagent_fd);
		return;
	}

	//check fd
	if( (P_recv_q->fd_or_seq < 1) || (P_recv_q->fd_or_seq > MAX_FD) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_LOGIN_RESULT bad fd(%d)\n", dbagent_fd, P_recv_q->fd_or_seq);
		P_remove_client(dbagent_fd);
		return;
	}

	//check user_id
	if( (body->user_id[0] == 0) || (body->user_id[USER_ID_LEN] != 0) )
	{
		body->user_id[USER_ID_LEN]	= 0;
		U_Printf("[PROC]Error dbagent_fd(%d) AW_LOGIN_RESULT bad user_id(%s)\n", dbagent_fd, body->user_id);
		P_remove_client(dbagent_fd);
		return;
	}

	//check session
	if( (P_recv_q->sec_active != D_GetClient_sec_active(P_recv_q->fd_or_seq))
				|| (P_recv_q->session != D_GetClient_session(P_recv_q->fd_or_seq)) )
	{
		U_Printf("[PROC]Discord dbagent_fd(%d) AW_LOGIN_RESULT fd_or_seq(%d) sec_active(%d) session(%d) user_id(%s)\n",
								dbagent_fd, P_recv_q->fd_or_seq, P_recv_q->session, body->user_id);
		//send GA_LOGOUT
		U_Printf("[PROC]send_queue_in UNICAST(%d) WA_LOGOUT user_id(%s)\n", P_recv_q->fd_or_seq, body->user_id);
		P_send_size	= MK_GetPacket_WA_LOGOUT(P_send_buf, P_recv_q->fd_or_seq, P_recv_q->sec_active, P_recv_q->session, body->user_id);
		U_Send_queue_in(SEND_UNICAST, dbagent_fd, 0, 0, 0, P_send_size, P_send_buf);
		return;		
	}
	
	//check already login
	if( D_GetClient_user_id(P_recv_q->fd_or_seq) )
	{
		U_Printf("[PROC]Error dbagent_fd(%d) AW_LOGIN_RESULT already login user_id(%s)(%s)\n",
							dbagent_fd, D_GetClient_user_id(P_recv_q->fd_or_seq), body->user_id);
		P_remove_client(dbagent_fd);
		return;
	}

	//send GC_LOGIN_RESULT
	U_Printf("[PROC]send_queue_in UNICAST(%d) GC_LOGIN_RESULT result_code(%d) user_id(%s)\n",
								P_recv_q->fd_or_seq, body->result_code, body->user_id);
	P_send_size	= MK_GetPacket_GC_LOGIN_RESULT(P_send_buf, body->result_code);
	U_Send_queue_in(SEND_UNICAST, P_recv_q->fd_or_seq, 0, 0, 0, P_send_size, P_send_buf);


	if( body->result_code )
	{
		//remove client	
		P_remove_client(P_recv_q->fd_or_seq);	
	}
	else
	{
		//set G_Login_number
		++G_Login_number;

		//set user_id
		D_SetClient_user_id(P_recv_q->fd_or_seq, body->user_id);

		U_Printf("[PROC]Login success fd(%d) user_id(%s) user_ip(%s) G_Login_number(%d) G_Connection_number(%d)\n",
				P_recv_q->fd_or_seq, body->user_id, D_GetClient_ip(P_recv_q->fd_or_seq), G_Login_number, G_Connection_number);
	}
}

