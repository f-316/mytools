#pragma once

#ifdef UNICODE
	#define PIPE_NAME_TOSVR L"\\\\.\\pipe\\to_svr"
	#define PIPE_NAME_TOCLT L"\\\\.\\pipe\\to_clt"

	#define SERVER_SEND_EVT L"pipe_svr_send_evt"
	#define SERVER_RECV_EVT L"pipe_svr_recv_evt"
	#define CLIENT_SEND_EVT L"pipe_clt_send_evt"
	#define CLIENT_RECV_EVT L"pipe_clt_recv_evt"
#else
	#define PIPE_NAME_TOSVR "\\\\.\\pipe\\to_svr"
	#define PIPE_NAME_TOCLT "\\\\.\\pipe\\to_clt"

	#define SERVER_SEND_EVT "pipe_svr_send_evt"
	#define SERVER_RECV_EVT "pipe_svr_recv_evt"
	#define CLIENT_SEND_EVT "pipe_clt_send_evt"
	#define CLIENT_RECV_EVT "pipe_clt_recv_evt"
#endif
