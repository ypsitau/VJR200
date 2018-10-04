// license:BSD-3-Clause
// copyright-holders:FIND
#ifndef __OPTIONDIALOG_H__
#define __OPTIONDIALOG_H__
#define ID_TAB1 0
#define ID_TAB2 1
#define ID_TAB3 2
#define ID_TAB4 3
#define ID_TAB5 4

BOOL CALLBACK DlgOptionProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Tab1Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Tab2Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Tab3Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Tab4Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Tab5Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

#endif