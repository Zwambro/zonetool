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

namespace ZoneTool
{
	namespace IW5
	{
		ScriptFile* IScriptFile::parse(const std::string& name, std::shared_ptr<ZoneMemory>& mem)
		{
			if (FileSystem::FileExists(name + ".cgsc") && FileSystem::FileExists(name + ".cgsc.stack"))
			{
				auto scriptfile = mem->Alloc<ScriptFile>();
				scriptfile->name = mem->StrDup(name);

				ZONETOOL_INFO("Parsing scriptfile \"%s\"...", name.data());

				// parse bytecode
				auto file = FileSystem::FileOpen(name + ".cgsc", "rb"s);
				if (file)
				{
					ZONETOOL_INFO("Parsing scriptfile bytecode for script \"%s\"...", name.data());

					auto size = FileSystem::FileSize(file);
					auto data = FileSystem::ReadBytes(file, size);

					scriptfile->bytecode = mem->Alloc<char>(size);
					scriptfile->bytecodeLen = size;
					memcpy(scriptfile->bytecode, &data[0], size);

					FileSystem::FileClose(file);
				}

				// parse stack
				file = FileSystem::FileOpen(name + ".cgsc.stack", "rb"s);
				if (file)
				{
					ZONETOOL_INFO("Parsing scriptfile heap for script \"%s\"...", name.data());

					auto size = FileSystem::FileSize(file);
					auto data = FileSystem::ReadBytes(file, size);

					ZoneBuffer buf(data);
					auto compressed = buf.compress_zlib();

					scriptfile->len = size;
					scriptfile->compressedLen = compressed.size();
					scriptfile->buffer = mem->Alloc<char>(size);
					memcpy(
						const_cast<char*>(scriptfile->buffer),
						compressed.data(),
						size);

					FileSystem::FileClose(file);
				}

				return scriptfile;
			}

			return nullptr;
		}

		void IScriptFile::init(const std::string& name, std::shared_ptr<ZoneMemory>& mem)
		{
			this->m_name = name;
			this->m_asset = this->parse(name, mem);

			if (!this->m_asset)
			{
				this->m_asset = DB_FindXAssetHeader(this->type(), this->m_name.data(), 1).scriptfile;
			}
		}

		void IScriptFile::prepare(std::shared_ptr<ZoneBuffer>& buf, std::shared_ptr<ZoneMemory>& mem)
		{
		}

		void IScriptFile::load_depending(IZone* zone)
		{
		}

		std::string IScriptFile::name()
		{
			return this->m_name;
		}

		std::int32_t IScriptFile::type()
		{
			return scriptfile;
		}

		void IScriptFile::write(IZone* zone, std::shared_ptr<ZoneBuffer>& buf)
		{
			auto data = this->m_asset;
			auto dest = buf->write<ScriptFile>(data);

			buf->push_stream(3);
			START_LOG_STREAM;

			dest->name = buf->write_str(this->name());

			buf->push_stream(8);

			if (data->buffer)
			{
				auto destbuffer = buf->write(data->buffer, data->compressedLen);
				// encrypt_data(const_cast<char*>(destbuffer), data->compressedLen);
				ZoneBuffer::ClearPointer(&dest->buffer);
			}

			if (data->bytecode)
			{
				auto destbytecode = buf->write(data->bytecode, data->bytecodeLen);
				// encrypt_data(destbytecode, data->bytecodeLen);
				ZoneBuffer::ClearPointer(&dest->bytecode);
			}

			buf->pop_stream();

			END_LOG_STREAM;
			buf->pop_stream();

			// encrypt_data(dest, sizeof ScriptFile);
		}

		void IScriptFile::dump(ScriptFile* asset)
		{
			/*auto fp = FileSystem::FileOpen(asset->name, "wb");

			if (fp)
			{
				if (asset->compressedLen)
				{
					std::vector < std::uint8_t > uncompressed;
					uncompressed.resize(asset->len);

					auto status = uncompress(uncompressed.data(), (uLongf*)&asset->len, (Bytef*)asset->buffer, asset->compressedLen);

					fwrite(uncompressed.data(), uncompressed.size(), 1, fp);
				}
			}

			FileSystem::FileClose(fp);*/
		}

		IScriptFile::IScriptFile()
		{
		}

		IScriptFile::~IScriptFile()
		{
		}
	}
}
