#pragma once
#include "Include/FTPluginCore.h"
#include "Include/FTPluginQuoteInterface.h"
#include "Include/FTPluginTradeInterface.h"
#include "Protocol/ProtoDataStruct_Trade.h"
#include "TimerWnd.h"
#include "MsgHandler.h"
#include "JsonCpp/json.h"
#include "DelayOrderIDCvt_HK.h" 

class CPluginHKTradeServer;

class CPluginSetOrderStatus_HK : public CTimerWndInterface,
	public CMsgHandlerEventInterface, public IOrderIDCvtNotify_HK
{
public:
	CPluginSetOrderStatus_HK();
	virtual ~CPluginSetOrderStatus_HK();
	
	void Init(CPluginHKTradeServer* pTradeServer, ITrade_HK*  pTradeOp);
	void Uninit();	
	void SetTradeReqData(int nCmdID, const Json::Value &jsnVal, SOCKET sock);
	void NotifyOnSetOrderStatus(Trade_Env enEnv, UINT nCookie, Trade_SvrResult enSvrRet, UINT64 nOrderID, UINT16 nErrCode);

protected:
	//CTimerWndInterface 
	virtual void OnTimeEvent(UINT nEventID);

	//CMsgHandlerEventInterface
	virtual void OnMsgEvent(int nEvent,WPARAM wParam,LPARAM lParam);

protected:
	virtual void OnCvtOrderID_Local2Svr(int nResult, Trade_Env eEnv, INT64 nLocalID, INT64 nServerID); 

protected:
	//tomodify 1
	typedef SetOrderStatus_Req	TradeReqType;
	typedef SetOrderStatus_Ack	TradeAckType;

	struct	StockDataReq
	{
		SOCKET	sock;
		DWORD	dwReqTick;
		DWORD	dwLocalCookie;
		TradeReqType req;

		bool   bWaitDelaySvrID; //�ȴ�svr����ID
	};
	
	typedef std::vector<StockDataReq*>		VT_REQ_TRADE_DATA;	
	

protected:	
	void HandleTimeoutReq();
	void HandleTradeAck(TradeAckType *pAck, SOCKET	sock);
	void SetTimerHandleTimeout(bool bStartOrStop);
	void ClearAllReqAckData();
	
private:
	bool	IsReqDataExist(StockDataReq* pReq); 
	void	DoRemoveReqData(StockDataReq* pReq);
	void	DoTryProcessTradeOpt(StockDataReq* pReq);

protected:
	CPluginHKTradeServer	*m_pTradeServer;
	ITrade_HK				*m_pTradeOp;	
	BOOL					m_bStartTimerHandleTimeout;
	
	CTimerMsgWndEx		m_TimerWnd;
	CMsgHandler			m_MsgHandler;

	VT_REQ_TRADE_DATA	m_vtReqData;

	CDelayOrderIDCvt_HK  m_stOrderIDCvt; 

};