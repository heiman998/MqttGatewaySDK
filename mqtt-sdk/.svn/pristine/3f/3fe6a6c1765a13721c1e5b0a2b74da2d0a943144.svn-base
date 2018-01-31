
#ifndef _QCA_SNIFFER_H_
#define _QCA_SNIFFER_H_

/** 
 * @brief: sniffer mode 
 * SCAN_CHANNEL_MODE：sniffer前扫描信道，只探测扫描到的AP所在信道,有点是能加快锁定信道的速度
 * ALL_CHANNEL_MODE：扫描所有信道（1~13） 
*/
typedef enum  
{
	SCAN_CHANNEL_MODE = 1,			  
	ALL_CHANNEL_MODE				 
}tChannelMode_Type;

/** 
 * @brief: sniffer mode 
 * SCAN_CHANNEL_MODE：sniffer前扫描信道，只探测扫描到的AP所在信道,有点是能加快锁定信道的速度
 * ALL_CHANNEL_MODE：扫描所有信道（1~13） 
*/
typedef enum  
{
	SNIFFER = 1,			  
	AIRKISS				 
}tSuccessMode_Type;


/** 
 * @brief: 模式使能控制
*/
typedef enum  
{
	DIS_ALL	= 0,	// 关闭sniffer和airkiss模式
	EN_ALL	= 1,	// 同时开启sniffer和airkiss模式
	EN_SNIFFER,		// 仅开启sniffer模式
	EN_AIRKISS		// 仅开启airkiss模式
}tEnable_Type;

typedef enum
{
	AIRKISS_V1 = 0,
	AIRKISS_V2,
}tAirkiss_mode;

/** 
 * @brief:	sniffer成功后执行的回调函数指针
 * *ssid：		ssid
 * *password：	password
 * response：	响应码
 * tmode:		成功模式	SNIFFER：response = 0(内部专用), 回复find包；
 *									response !=0，调用udp_sniffer_respond(6001, 10, &response, 1);
 *							AIRKISS：调用udp_sniffer_respond(10000, 10, &response, 1);
*/
typedef void (*sniffer_cb)(char *ssid, char *password, unsigned char response, tSuccessMode_Type tmode);


/**
***************************************************************************
*@brief		：	获取sniffer lib版本信息
*@param[in]	：	void
*@return	：	none
*@warning	：
*@see		：
***************************************************************************
*/
char * qca_sniffer_get_version(void);

/**
***************************************************************************
*@brief		：	启动sniffer
*@param[in]	：	mode：			sniffer扫描模式
*				sniffer_func：	sniffer成功后的回调函
*@return	：	none
*@see		：	
***************************************************************************
*/
void qca_sniffer_start(tChannelMode_Type mode, sniffer_cb sniffer_func);

/**
***************************************************************************
*@brief		：	set sniffer/airkiss enable/disable status
*@param[in]	：	tEnMode		使能/禁止 sniffer模式或airkiss模式
*@return	：	none
*@warning	：
*@see		：	本函数用于使能（或禁止）sniffer功能, 随时有效
*				默认tEnMode = EN_ALL
***************************************************************************
*/
void qca_set_sniffer_en(tEnable_Type tEnMode);

/**
***************************************************************************
*@brief		：	其它线程判断是否需要挂起线程
*@param[in]	：	0	不挂起
*				1	挂起
*@return	：	none
*@see		：	sniffer关键时刻其他线程最好挂起，以免影响sniffer效果
*				已知必须挂起的任务是扫描AP,因为扫描AP需要几秒且会导致sniffer
*				不能正常接收数据, 扫描AP可以调用函数“qca_sniffer_check_ap()”
***************************************************************************
*/
int qca_get_thread_suspend(void);

/**
***************************************************************************
*@brief		：	判断AP是否存在
*@param[in]	：	*ssid
*@return	：	0	找到
*				-1  未找到
*@see		：	当启动station模式时连接AP超时（或出错）进入smart link模式后
*				可通过此接口函数检测AP是否存在，并考虑是否进入station模式
***************************************************************************
*/
int qca_sniffer_check_ap(char *ssid);

/**
***************************************************************************
*@brief		：	设置Airkiss秘钥
*@param[in]	：	key		最长可以为128bit，若传入的key不足128bit，则默认用0填充
*@return	：	0		成功
*				-1		失败（通常是参数错误）
*@see		：	
***************************************************************************
*/
int qca_airkiss_set_key(unsigned char* key, unsigned int length);

/**
***************************************************************************
*@brief		：	设置Sniffer秘钥
*@param[in]	：	key		最长可以为128bit，若传入的key不足128bit，则默认用0填充
*@return	：	0		成功
*				-1		失败（通常是参数错误）
*@see		：
***************************************************************************
*/
int qca_sniffer_set_key(const unsigned char* key, unsigned int length);

/**
***************************************************************************
*@brief		：	回复sniffer/airkiss响应
*@param[in]	：	port	端口号（Airkiss：10000）
*				count	回复次数
*				*datas	数据起始地址
*				length	数据长度
*@return	：	0		成功
*				-1		失败
*@see		：	
***************************************************************************
*/
int udp_sniffer_response(int port, int count, char *data, int length);

void qca_airkiss(int mode, char *deviceType, char *deviceID, int respond);

#endif

