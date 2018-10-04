// license:BSD-3-Clause
// copyright-holders:FIND
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// class AppSetting
// INIファイルの読み出し、書き込みを担当
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <Shlobj.h>
#include <Shlwapi.h>
#include "VJR200.h"
#include "AppSetting.h"

using namespace std;

#pragma comment(lib, "Shlwapi.lib")

AppSetting::AppSetting()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 初期化　INIファイル読み込み
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool AppSetting::Init()
{
	SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, NULL, path);
		wcscat(path, L"\\FIND_JR");
	if (!PathFileExists(path))
		CreateDirectory(path, 0);
	wcscat(path, L"\\VJR200");
	if (!PathFileExists(path))
		CreateDirectory(path, 0);
	wcscat(path, L"\\vjr200.ini");

	if (!PathFileExists(path))
	{
		HANDLE hFile;
		hFile = CreateFile(path,
			GENERIC_WRITE,
			0,
			0,
			CREATE_ALWAYS,
			0,
			0
		);

		if (hFile == INVALID_HANDLE_VALUE) {
			return false;
		}
		else {
			CloseHandle(hFile);
		}

	}
	else { // iniが存在した場合は読み込み
		TCHAR buff[MAX_PATH] = {};

		g_windowPos.left = GetPrivateProfileInt(_T("Window"), _T("PosX"), 100, path);
		g_windowPos.top = GetPrivateProfileInt(_T("Window"), _T("PosY"), 100, path);
		g_viewScale = GetPrivateProfileInt(_T("Window"), _T("viewScale"), 2, path);
		g_bSquarePixel = GetPrivateProfileInt(_T("Window"), _T("SquarePixel"), 0, path) == 1 ? true : false;
		g_bSmoothing = GetPrivateProfileInt(_T("Window"), _T("Smoothing"), 0, path) == 1 ? true : false;
		g_bPrinterLed = GetPrivateProfileInt(_T("Window"), _T("PrinterLed"), 1, path) == 1 ? true : false;
		g_bStatusBar = GetPrivateProfileInt(_T("Window"), _T("StatusBar"), 1, path) == 1 ? true : false;

		GetPrivateProfileString(_T("Path"), _T("RomFile"), _T(""), buff, MAX_PATH, path);
		_tcscpy(g_pRomFile, buff);
		GetPrivateProfileString(_T("Path"), _T("FontFile"), _T(""), buff, MAX_PATH, path);
		_tcscpy(g_pFontFile, buff);

		TCHAR c[5], tmpBuf[MAX_PATH];
		tmpBuf[0] = '\0';
		for (unsigned int i = 0; i < RECENT_FILES_NUM; ++i) {
			_tcscat(tmpBuf,_T("RFilesforCJRSet"));
			_itot(i, c, 10);
			_tcscat(tmpBuf, c);
			GetPrivateProfileString(_T("Path"), tmpBuf, _T(""), buff, MAX_PATH, path);
			g_rFilesforCJRSet.push_back(buff);
			tmpBuf[0] = '\0';
		}

		for (unsigned int i = 0; i < RECENT_FILES_NUM; ++i) {
			_tcscat(tmpBuf, _T("RFilesforQLoad"));
			_itot(i, c, 10);
			_tcscat(tmpBuf, c);
			GetPrivateProfileString(_T("Path"), tmpBuf, _T(""), buff, MAX_PATH, path);
			g_rFilesforQLoad.push_back(buff);
			tmpBuf[0] = '\0';
		}

		g_bFpsCpu = GetPrivateProfileInt(_T("Options"), _T("fpsCpu"), 0, path) == 1 ? true : false;
		g_bRamExpand1 = GetPrivateProfileInt(_T("Options"), _T("ramExpand1"), 0, path) == 1 ? true : false;
		g_bRamExpand2 = GetPrivateProfileInt(_T("Options"), _T("ramExpand2"), 0, path) == 1 ? true : false;
		g_ramInitPattern = GetPrivateProfileInt(_T("Options"), _T("ramInitPattern"), 0, path);
		g_refRate = GetPrivateProfileInt(_T("Options"), _T("refRate"), 0, path);
		g_bRefRateAuto = GetPrivateProfileInt(_T("Options"), _T("refRateAuto"), 0, path) == 1 ? true : false;
		g_soundVolume = GetPrivateProfileInt(_T("Options"), _T("soundVolume"), 40, path);
		g_bOverClockLoad = GetPrivateProfileInt(_T("Options"), _T("overClockLoad"), 0, path) == 1 ? true : false;
		g_bCmtAddBlank = GetPrivateProfileInt(_T("Options"), _T("cmtAddBlank"), 1, path) == 1 ? true : false;
		g_quickTypeS = GetPrivateProfileInt(_T("Options"), _T("quickType"), 30, path);
		g_language = GetPrivateProfileInt(_T("Options"), _T("language"), 0, path);
		g_bRomajiKana = GetPrivateProfileInt(_T("Options"), _T("romajiKana"), 0, path) == 1 ? true : false;

		g_stereo1 = GetPrivateProfileInt(_T("Sound"), _T("stereo1"), 5, path);
		g_stereo2 = GetPrivateProfileInt(_T("Sound"), _T("stereo2"), 5, path);
		g_stereo3 = GetPrivateProfileInt(_T("Sound"), _T("stereo3"), 5, path);

		g_prOutput = GetPrivateProfileInt(_T("Printer"), _T("output"), 0, path);
		g_prMaker = GetPrivateProfileInt(_T("Printer"), _T("maker"), 0, path);
		g_prAutoFeed = GetPrivateProfileInt(_T("Printer"), _T("autoFeed"), 1, path);

		g_b2buttons = GetPrivateProfileInt(_T("Joypad"), _T("2buttons"), 0, path) == 1 ? true : false;
		g_Joypad1pA = GetPrivateProfileInt(_T("Joypad"), _T("1pAbutton"), 0, path);
		g_Joypad1pB = GetPrivateProfileInt(_T("Joypad"), _T("1pBbutton"), 0, path);
		g_Joypad2pA = GetPrivateProfileInt(_T("Joypad"), _T("2pAbutton"), 0, path);
		g_Joypad2pB = GetPrivateProfileInt(_T("Joypad"), _T("2pBbutton"), 0, path);
		g_forcedJoypadA = GetPrivateProfileInt(_T("Joypad"), _T("ForcedJoystickA"), 32, path);
		g_forcedJoypadB = GetPrivateProfileInt(_T("Joypad"), _T("ForcedJoystickB"), 32, path);
		g_sBufferWriteInterval = GetPrivateProfileInt(_T("Sound"), _T("sBufferWriteInterval"), 60, path);
	}
	return true;
}

AppSetting::~AppSetting()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// INIファイル書き込み
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void AppSetting::WriteIni()
{
	TCHAR buff[MAX_PATH];

	WritePrivateProfileString(_T("Window"), _T("SquarePixcel"), NULL, path); // スペルミスなので削除
	WritePrivateProfileString(_T("Options"), _T("g_quickTypeV"), NULL, path); // スペルミスなので削除

	wsprintf(buff, _T("%d"), g_windowPos.left);
	WritePrivateProfileString(_T("Window"), _T("PosX"), buff, path);
	wsprintf(buff, _T("%d"), g_windowPos.top);
	WritePrivateProfileString(_T("Window"), _T("PosY"), buff, path);
	wsprintf(buff, _T("%d"), g_viewScale);
	WritePrivateProfileString(_T("Window"), _T("viewScale"), buff, path);
	wsprintf(buff, _T("%d"), g_bSquarePixel);
	WritePrivateProfileString(_T("Window"), _T("SquarePixel"), buff, path);
	wsprintf(buff, _T("%d"), g_bSmoothing ? 1 : 0);
	WritePrivateProfileString(_T("Window"), _T("Smoothing"), buff, path);
	wsprintf(buff, _T("%d"), g_bPrinterLed ? 1 : 0);
	WritePrivateProfileString(_T("Window"), _T("PrinterLed"), buff, path);
	wsprintf(buff, _T("%d"), g_bStatusBar ? 1 : 0);
	WritePrivateProfileString(_T("Window"), _T("StatusBar"), buff, path);

	WritePrivateProfileString(_T("Path"), _T("RomFile"), g_pRomFile, path);
	WritePrivateProfileString(_T("Path"), _T("FontFile"), g_pFontFile, path);

	TCHAR tmpBuf[MAX_PATH];
	tmpBuf[0] = '\0';
	if (g_rFilesforCJRSet.size() >= RECENT_FILES_NUM + 1)
		while (g_rFilesforCJRSet.size() > RECENT_FILES_NUM) g_rFilesforCJRSet.pop_back();
	for (unsigned int i = 0; i < g_rFilesforCJRSet.size(); ++i) {
		wsprintf(tmpBuf, _T("%s%d"), _T("RFilesforCJRSet"), i);
		WritePrivateProfileString(_T("Path"), tmpBuf, g_rFilesforCJRSet[i].data(), path);
		tmpBuf[0] = '\0';
	}

	if (g_rFilesforQLoad.size() >= RECENT_FILES_NUM + 1)
		while (g_rFilesforQLoad.size() > RECENT_FILES_NUM) g_rFilesforQLoad.pop_back();
	for (unsigned int i = 0; i < g_rFilesforQLoad.size(); ++i) {
		wsprintf(tmpBuf, _T("%s%d"), _T("RFilesforQLoad"), i);
		WritePrivateProfileString(_T("Path"), tmpBuf, g_rFilesforQLoad[i].data(), path);
		tmpBuf[0] = '\0';
	}

	wsprintf(buff, _T("%d"), g_bFpsCpu ? 1 : 0);
	WritePrivateProfileString(_T("Options"), _T("fpsCpu"), buff, path);
	wsprintf(buff, _T("%d"), g_bRamExpand1 ? 1 : 0);
	WritePrivateProfileString(_T("Options"), _T("ramExpand1"), buff, path);
	wsprintf(buff, _T("%d"), g_bRamExpand2 ? 1 : 0);
	WritePrivateProfileString(_T("Options"), _T("ramExpand2"), buff, path);
	wsprintf(buff, _T("%d"), g_ramInitPattern);
	WritePrivateProfileString(_T("Options"), _T("ramInitPattern"), buff, path);
	wsprintf(buff, _T("%d"), g_refRate);
	WritePrivateProfileString(_T("Options"), _T("refRate"), buff, path);
	wsprintf(buff, _T("%d"), g_bRefRateAuto ? 1 : 0);
	WritePrivateProfileString(_T("Options"), _T("refRateAuto"), buff, path);
	wsprintf(buff, _T("%d"), g_soundVolume);
	WritePrivateProfileString(_T("Options"), _T("soundVolume"), buff, path);
	wsprintf(buff, _T("%d"), g_bOverClockLoad ? 1 : 0);
	WritePrivateProfileString(_T("Options"), _T("overClockLoad"), buff, path);
	wsprintf(buff, _T("%d"), g_bCmtAddBlank ? 1 : 0);
	WritePrivateProfileString(_T("Options"), _T("cmtAddBlank"), buff, path);
	wsprintf(buff, _T("%d"), g_quickTypeS);
	WritePrivateProfileString(_T("Options"), _T("quickType"), buff, path);
	wsprintf(buff, _T("%d"), g_language);
	WritePrivateProfileString(_T("Options"), _T("language"), buff, path);
	wsprintf(buff, _T("%d"), g_bRomajiKana ? 1 : 0);
	WritePrivateProfileString(_T("Options"), _T("romajiKana"), buff, path);

	wsprintf(buff, _T("%d"), g_stereo1);
	WritePrivateProfileString(_T("Sound"), _T("stereo1"), buff, path);
	wsprintf(buff, _T("%d"), g_stereo2);
	WritePrivateProfileString(_T("Sound"), _T("stereo2"), buff, path);
	wsprintf(buff, _T("%d"), g_stereo3);
	WritePrivateProfileString(_T("Sound"), _T("stereo3"), buff, path);

	wsprintf(buff, _T("%d"), g_prOutput);
	WritePrivateProfileString(_T("Printer"), _T("output"), buff, path);
	wsprintf(buff, _T("%d"), g_prMaker);
	WritePrivateProfileString(_T("Printer"), _T("maker"), buff, path);
	wsprintf(buff, _T("%d"), g_prAutoFeed);
	WritePrivateProfileString(_T("Printer"), _T("autoFeed"), buff, path);

	wsprintf(buff, _T("%d"), g_b2buttons ? 1 : 0);
	WritePrivateProfileString(_T("Joypad"), _T("2buttons"), buff, path);
	wsprintf(buff, _T("%d"), g_Joypad1pA);
	WritePrivateProfileString(_T("Joypad"), _T("1pAbutton"), buff, path);
	wsprintf(buff, _T("%d"), g_Joypad1pB);
	WritePrivateProfileString(_T("Joypad"), _T("1pBbutton"), buff, path);
	wsprintf(buff, _T("%d"), g_Joypad2pA);
	WritePrivateProfileString(_T("Joypad"), _T("2pAbutton"), buff, path);
	wsprintf(buff, _T("%d"), g_Joypad2pB);
	WritePrivateProfileString(_T("Joypad"), _T("2pBbutton"), buff, path);
	wsprintf(buff, _T("%d"), g_forcedJoypadA);
	WritePrivateProfileString(_T("Joypad"), _T("ForcedJoystickA"), buff, path);
	wsprintf(buff, _T("%d"), g_forcedJoypadB);
	WritePrivateProfileString(_T("Joypad"), _T("ForcedJoystickB"), buff, path);
	wsprintf(buff, _T("%d"), g_sBufferWriteInterval);
	WritePrivateProfileString(_T("Sound"), _T("sBufferWriteInterval"), buff, path);


	WritePrivateProfileString(NULL, NULL, NULL, path);
}