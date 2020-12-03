#pragma once
#include <unordered_map>
#include "../Includes/ExecuteDefs.h"
#include "../Share/StdUtils.hpp"

USING_NS_OTP;

class WtTWapExeUnit : public ExecuteUnit
{
public:
	WtTWapExeUnit();
	virtual ~WtTWapExeUnit();

private:
	void	do_calc();
	void	fire_at_once(double qty);

public:
	/*
	*	所属执行器工厂名称
	*/
	virtual const char* getFactName() override;

	/*
	*	执行单元名称
	*/
	virtual const char* getName() override;

	/*
	*	初始化执行单元
	*	ctx		执行单元运行环境
	*	code	管理的合约代码
	*/
	virtual void init(ExecuteContext* ctx, const char* stdCode, WTSVariant* cfg) override;

	/*
	*	订单回报
	*	localid	本地单号
	*	code	合约代码
	*	isBuy	买or卖
	*	leftover	剩余数量
	*	price	委托价格
	*	isCanceled	是否已撤销
	*/
	virtual void on_order(uint32_t localid, const char* stdCode, bool isBuy, double leftover, double price, bool isCanceled) override;

	/*
	*	tick数据回调
	*	newTick	最新的tick数据
	*/
	virtual void on_tick(WTSTickData* newTick) override;

	/*
	*	成交回报
	*	code	合约代码
	*	isBuy	买or卖
	*	vol		成交数量，这里没有正负，通过isBuy确定买入还是卖出
	*	price	成交价格
	*/
	virtual void on_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double price) override;

	/*
	*	下单结果回报
	*/
	virtual void on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message) override;

	/*
	*	设置新的目标仓位
	*	code	合约代码
	*	newVol	新的目标仓位
	*/
	virtual void set_position(const char* stdCode, double newVol) override;

	/*
	*	交易通道就绪回调
	*/
	virtual void on_channel_ready() override;

	/*
	*	交易通道丢失回调
	*/
	virtual void on_channel_lost() override;


private:
	WTSTickData* _last_tick;	//上一笔行情
	double		_target_pos;	//目标仓位
	bool		_channel_ready;


	WTSCommodityInfo*	_comm_info;

	//////////////////////////////////////////////////////////////////////////
	//执行参数
	typedef std::unordered_map<uint32_t, uint64_t> Orders;
	Orders			_orders;
	StdRecurMutex	_mtx_ords;
	uint32_t		_cancel_cnt;

	//////////////////////////////////////////////////////////////////////////
	//参数
	uint32_t		_total_secs;	//执行总时间，单位s
	uint32_t		_total_times;	//总执行次数
	uint32_t		_tail_secs;		//执行尾部时间
	uint32_t		_ord_sticky;	//挂单时限，单位s
	uint32_t		_price_mode;	//价格模式：0-最新价，1-最优价，2-对手价
	uint32_t		_price_offset;		//挂单价格偏移，相对于几乎价格偏移，买+卖-

	//////////////////////////////////////////////////////////////////////////
	//临时变量
	double			_this_target;	//本轮目标仓位
	uint32_t		_fire_span;		//发单间隔
	uint32_t		_fired_times;	//已执行次数
	uint64_t		_last_fire_time;
};

