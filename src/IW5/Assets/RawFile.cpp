// ======================= ZoneTool =======================
// zonetool, a fastfile linker for various
// Call of Duty titles. 
//
// Project: https://github.com/ZoneTool/zonetool
// Author: RektInator (https://github.com/RektInator)
// License: GNU GPL v3.0
// ========================================================
#include "stdafx.hpp"
#include <zlib/zlib.h>

#define ZONETOOL_BRANDING		"Compiled using ZoneTool by RektInator, localhost & X3RX35."

namespace ZoneTool
{
	namespace IW5
	{
		RawFile* IRawFile::parse(const std::string& name, std::shared_ptr<ZoneMemory>& mem)
		{
			if (FileSystem::FileExists(name))
			{
				auto rawfile = mem->Alloc<RawFile>();
				rawfile->name = mem->StrDup(name);

				auto file = FileSystem::FileOpen(name, "rb"s);
				if (file)
				{
					auto size = FileSystem::FileSize(file);
					auto data = FileSystem::ReadBytes(file, size);

					ZoneBuffer buf(data);
					auto compressed = buf.compress_zlib();

					rawfile->len = size;
					rawfile->compressedLen = compressed.size();
					rawfile->buffer = mem->Alloc<char>(size);
					memcpy(
						const_cast<char*>(rawfile->buffer),
						compressed.data(),
						size);

					FileSystem::FileClose(file);
				}
				else
				{
					rawfile->buffer = ZONETOOL_BRANDING;
					rawfile->compressedLen = strlen(ZONETOOL_BRANDING);
					rawfile->len = strlen(ZONETOOL_BRANDING);
				}

				return rawfile;
			}

			return nullptr;
		}

		void IRawFile::init(const std::string& name, std::shared_ptr<ZoneMemory>& mem)
		{
			this->m_name = name;
			this->m_asset = parse(name, mem);

			if (!this->m_asset)
			{
				this->m_asset = DB_FindXAssetHeader(this->type(), this->m_name.data(), 1).rawfile;
			}
		}

		void IRawFile::prepare(std::shared_ptr<ZoneBuffer>& buf, std::shared_ptr<ZoneMemory>& mem)
		{
		}

		void IRawFile::load_depending(IZone* zone)
		{
		}

		std::string IRawFile::name()
		{
			return this->m_name;
		}

		std::int32_t IRawFile::type()
		{
			return rawfile;
		}

		void IRawFile::write(IZone* zone, std::shared_ptr<ZoneBuffer>& buf)
		{
			const auto data = this->m_asset;
			auto dest = buf->write<RawFile>(data);

			buf->push_stream(3);
			START_LOG_STREAM;

			dest->name = buf->write_str(this->name());

			if (data->buffer)
			{
				buf->align(0);
				buf->write(data->buffer, data->compressedLen);
				ZoneBuffer::ClearPointer(&dest->buffer);
			}

			END_LOG_STREAM;
			buf->pop_stream();
		}

		void IRawFile::dump(RawFile* asset)
		{
			auto fp = FileSystem::FileOpen(asset->name, "wb");

			if (fp)
			{
				if (asset->compressedLen)
				{
					std::vector<std::uint8_t> uncompressed;
					uncompressed.resize(asset->len);

					auto status = uncompress(uncompressed.data(), (uLongf*)&asset->len, (Bytef*)asset->buffer,
					                         asset->compressedLen);

					fwrite(uncompressed.data(), uncompressed.size(), 1, fp);
				}
			}

			FileSystem::FileClose(fp);
		}

		IRawFile::IRawFile()
		{
		}

		IRawFile::~IRawFile()
		{
		}
	}
}
