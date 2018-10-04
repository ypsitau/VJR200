// license:BSD-3-Clause
// copyright-holders:FIND
#ifndef __APPSETTING_H__
#define __APPSETTING_H__

class AppSetting
{
public:
	AppSetting();
	bool Init();
	~AppSetting();
	void WriteIni();
protected:
	TCHAR path[MAX_PATH];
};

#endif