// license:BSD-3-Clause
// copyright-holders:FIND
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// class Address
// アドレス空間を管理するクラス
//
// 機能 : RAM内容を所持する、ROMファイルの読み込み、リード・ライト
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifdef _WIN32
#include <shlobj.h>
#endif
#include "JRSystem.h"
#include "CjrFormat.h"
#include "Address.h"
#include "VJR200.h"

using namespace std;

extern JRSystem sys;

CEREAL_CLASS_VERSION(Address, CEREAL_VER);

Address::Address()
{
}

Address::~Address()
{
    delete[] memory;
    delete[] attribute;
    if (buff)
        delete[] buff;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 初期化、メモリ内容の設定
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool Address::Init()
{
    memory = new uint8_t[MEM_SIZE];
    if (memory == nullptr)
        return false;

    attribute = new DevType[MEM_SIZE];
    if (attribute == nullptr)
        return false;

    memset(memory, 0, 0xffff);
	
	switch (g_ramInitPattern) {
	case 0:
		for (int i = 0; i < 0x8000; i += 4) {
			if ((i & 0x100) == 0) {
				memory[i] = 0xff;
				memory[i + 1] = 0xff;
			}
			else {
				memory[i + 2] = 0xff;
				memory[i + 3] = 0xff;
			}
		}
		break;
	case 1:
		for (int i = 0; i < 0x8000; i += 2) {
			if ((i & 0x80) == 0) {
				memory[i + 1] = 0xff;
			}
			else {
				memory[i] = 0xff;
			}
		}
		break;
	}


    for (int i = 0xc001; i < 0xc100; i += 2) {
        memory[i] = 0xff;
    }
    for (int i = 0xc401; i < 0xc500; i += 2) {
        memory[i] = 0xff;
    }

    for (int i = 0; i < 0x8000; ++i) {
        attribute[i] = DevType::Ram;
    }
    if (g_bRamExpand1) { // RAM拡張1
        for (int i = 0x8000; i < 0xa000; ++i) {
            attribute[i] = DevType::Ram;
        }
    }
    else {
        for (int i = 0x8000; i < 0xa000; ++i) {
            attribute[i] = DevType::None;
        }
    }
    if (g_bRamExpand2) { // RAM拡張2
        for (int i = 0xa000; i < 0xc000; ++i) {
            attribute[i] = DevType::Ram;
        }
    }
    else {
        for (int i = 0xa000; i < 0xc000; ++i) {
            attribute[i] = DevType::Rom;
        }
    }
    for (int i = 0xc000; i < 0xc800; ++i) {
        attribute[i] = DevType::Ram;
    }
    for (int i = 0xc800; i < 0xc9ff; ++i) {
        attribute[i] = DevType::Mn1271;
    }
    for (int i = 0xca00; i < 0xcc00; ++i) {
        attribute[i] = DevType::Crtc;
    }
    for (int i = 0xcc00; i < 0xd000; ++i) {
        attribute[i] = DevType::None;
    }
    for (int i = 0xd000; i < 0xd800; ++i) {
        attribute[i] = DevType::Ram;
    }
    for (int i = 0xd800; i < 0xe000; ++i) {
        attribute[i] = DevType::None;
    }
    for (int i = 0xe000; i < 0x10000; ++i) {
        attribute[i] = DevType::Rom;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 指定されたアドレスを1byte読む
//
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t Address::ReadByte(uint16_t address)
{
    DevType d = attribute[address];
    switch (d)
    {
        case DevType::None:
            return 0;
            break;
        case DevType::Rom:
            return memory[address];
            break;
        case DevType::Ram:
            if (address >= 0 && address <= 0xbfff) ++g_dramWait;
            return memory[address];
            break;
        case DevType::Mn1271:
        {
            int i = address - 0xc800;
            while (i >= 32)
                i -= 32;
            return sys.pMn1271->Read(i);
            break;
        }
            break;
        case DevType:: Crtc:
        {
            int i = address - 0xca00;
            return sys.pCrtc->Read(i);
            break;
        }
            break;
        default:
            assert(false);
            return 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 指定されたアドレスを1byte読む（デバッグ表示用）
//
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t Address::ReadByteForDebug(uint16_t address)
{
    DevType d = attribute[address];
    switch (d)
    {
        case DevType::None:
            return 0;
            break;
        case DevType::Rom:
            return memory[address];
            break;
        case DevType::Ram:
            if (address >= 0 && address <= 0xbfff) ++g_dramWait;
            return memory[address];
            break;
        case DevType::Mn1271:
        {
            int i = address - 0xc800;
            while (i >= 32)
                i -= 32;
            return sys.pMn1271->ReadForDebug(i);
            break;
        }
            break;
        case DevType::Crtc:
        {
            int i = address - 0xca00;
            return sys.pCrtc->Read(i);
            break;
        }
            break;
        default:
            assert(false);
            return 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 指定されたアドレスに1byte書き込む
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void Address::WriteByte(uint16_t address, uint8_t b)
{
    DevType d = attribute[address];
    switch (d)
    {
        case DevType::None:
            break;
        case DevType::Rom:
            break;
        case DevType::Ram:
            if (address >= 0 && address <= 0xbfff) ++g_dramWait;
            memory[address] = b;
            break;
        case DevType::Mn1271:
        {
            int i = address - 0xc800;
            while (i >= 32)
                i -= 32;
            sys.pMn1271->Write(i, b);
            break;
        }
        case DevType::Crtc:
        {
            int i = address - 0xca00;
            sys.pCrtc->Write(i, b);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ROMファイルの内容をアドレス空間に書き込む
//
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL Address::LoadRomFile()
{
    FILE *fp = nullptr;
    fp = _tfopen(g_pRomFile, _T("rb"));
    if (fp == nullptr) {
        _tcscpy(g_pRomFile, _T(""));
        return FALSE;
    }

    if (buff == nullptr) {
        if (!(buff = new uint8_t[MEM_SIZE]))
            return FALSE;
    }

    fread(buff, sizeof(uint8_t), MEM_SIZE, fp);
    fclose(fp);

    if (!g_bRamExpand2)
        memcpy(&memory[0xa000], buff, 8192);
    memcpy(&memory[0xe000], &buff[8192], 8192);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// メモリ内容をファイルとして書き出し
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
void Address::SaveDumpFile()
{
	HANDLE hFile;
	DWORD dwNumberOfBytesWritten;
	TCHAR myDocumentsFolder[MAX_PATH];

	if (buff == nullptr) {
		if (!(buff = new uint8_t[MEM_SIZE]))
			return;
	}

	SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, NULL, myDocumentsFolder);
	_tcscat(myDocumentsFolder, _T("\\dump.bin"));
	for (int i = 0; i < MEM_SIZE; ++i)
		buff[i] = ReadByteForDebug(i);

	hFile = CreateFile(myDocumentsFolder,
		GENERIC_WRITE,
		0,
		0,
		CREATE_ALWAYS,
		0,
		0
	);
	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBox(g_hMainWnd, g_strTable[(int)Msg::Failed_to_open_the_file], NULL, 0);
		return;
	}

	WriteFile(hFile,
		buff,
		MEM_SIZE,
		&dwNumberOfBytesWritten,
		NULL);

	CloseHandle(hFile);
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 「高速ロード」を実行
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool Address::CjrQuickLoad(const TCHAR * filename)
{
    BOOL ret = true;
    FILE *fp = nullptr;

    if (buff == nullptr) {
        if (!(buff = new uint8_t[MEM_SIZE]))
            return false;
    }

    if (CheckFileFormat(filename) != 1)
        return false;

    fp = _tfopen(filename, _T("rb"));
    if (fp == nullptr) {
        _tcscpy(g_pRomFile, _T(""));
        return false;
    }
    int size = fread(buff, sizeof(uint8_t), MEM_SIZE, fp);
    fclose(fp);

    CjrFormat cjr;
    cjr.SetCjrData(buff, size);
    uint8_t* bin = cjr.GetBinDataArray();

    if (cjr.isBasic()) {
        memcpy(&memory[0x801], bin, cjr.GetBinSize());
        uint16_t a = (uint16_t)(0x801 + cjr.GetBinSize()) - 1;
        memory[0x71] = (uint8_t)(a >> 8);
        memory[0x72] = (uint8_t)a;
    }
    else {
        int a = cjr.GetStartAddress();
        memcpy(&memory[a], bin, cjr.GetBinSize());
    }
    return true;

}
