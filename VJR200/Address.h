// license:BSD-3-Clause
// copyright-holders:FIND
#ifndef __ADDRESS_H__
#define __ADDRESS_H__
//#ifdef _ANDROID
//#include "stdafx.h"
//#endif
#include <cstdint>
#include <cereal/cereal.hpp>

enum class DevType { None, Rom, Ram, Mn1271, Crtc };

class Address
{
public:
	Address();
	~Address();

	bool Init();
	uint8_t ReadByte(uint16_t address);
	uint8_t ReadByteForDebug(uint16_t address);
	void WriteByte(uint16_t address, uint8_t b);

#ifndef _ADNROID
	void SaveDumpFile();
#endif
	bool CjrQuickLoad(const TCHAR* filename);
	BOOL LoadRomFile();
	template <class Archive>
	void serialize(Archive & ar, std::uint32_t const version);

protected:
	static const int MEM_SIZE = 65536;
	DevType* attribute;
	uint8_t* memory;
	uint8_t* buff = nullptr;

};

template<class Archive>
inline void Address::serialize(Archive & ar, std::uint32_t const version)
{
	ar(cereal::binary_data(memory, sizeof(uint8_t) * 65536));
	ar(cereal::binary_data(attribute, sizeof(uint8_t) * 65536));
}

#endif