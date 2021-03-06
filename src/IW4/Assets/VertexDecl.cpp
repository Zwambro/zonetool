// ======================= ZoneTool =======================
// zonetool, a fastfile linker for various
// Call of Duty titles. 
//
// Project: https://github.com/ZoneTool/zonetool
// Author: RektInator (https://github.com/RektInator)
// License: GNU GPL v3.0
// ========================================================
#include "stdafx.hpp"

namespace ZoneTool
{
	namespace IW4
	{
		IVertexDecl::IVertexDecl()
		{
		}

		IVertexDecl::~IVertexDecl()
		{
		}

		VertexDecl* IVertexDecl::parse(const std::string& name, std::shared_ptr<ZoneMemory>& mem, bool preferLocal)
		{
			auto path = "techsets\\" + name + ".vertexdecl";

			AssetReader read(mem);
			if (!read.Open(path, preferLocal))
			{
				return nullptr;
			}

			ZONETOOL_INFO("Parsing vertexdecl \"%s\"...", name.data());

			auto asset = read.Array<VertexDecl>();
			asset->name = read.String();
			read.Close();

			if (asset->streamCount > 13)
			{
				ZONETOOL_FATAL("Vertexdecl %s has more than 13 streams.", &name[0]);
			}

			// 
			for (int i = 0; i < asset->streamCount; i++)
			{
				if ((asset->streams[i].source > 9 || asset->streams[i].source < 0) ||
					(asset->streams[i].dest > 32 || asset->streams[i].dest < 0))
				{
					ZONETOOL_ERROR("%i %i", asset->streams[i].source, asset->streams[i].dest);
					ZONETOOL_FATAL("Stream %u for vertex %s is invalid!", i, name);
				}
			}

			return asset;
		}

		void IVertexDecl::init(void* asset, std::shared_ptr<ZoneMemory>& mem)
		{
			this->m_asset = reinterpret_cast<VertexDecl*>(asset);
			this->m_name = this->m_asset->name + "_IW5"s;
		}

		std::unordered_map<std::uint32_t, std::uint32_t> mappedStreams;

		void IVertexDecl::init(const std::string& name, std::shared_ptr<ZoneMemory>& mem)
		{
			this->m_name = name;
			this->m_asset = this->parse(name, mem);

			if (!this->m_asset)
			{
				ZONETOOL_ERROR("VertexDecl %s not found.", &name[0]);
				this->m_asset = DB_FindXAssetHeader(this->type(), this->name().data()).vertexdecl;
			}

			FILE* vertexDecls = fopen("vertexdecls.txt", "a");
			fprintf(vertexDecls, "%s has %i streams.\n", &name[0], this->m_asset->streamCount);
			fclose(vertexDecls);
		}

		void IVertexDecl::prepare(std::shared_ptr<ZoneBuffer>& buf, std::shared_ptr<ZoneMemory>& mem)
		{
		}

		void IVertexDecl::load_depending(IZone* zone)
		{
		}

		std::string IVertexDecl::name()
		{
			return this->m_name;
		}

		std::int32_t IVertexDecl::type()
		{
			return vertexdecl;
		}

		void IVertexDecl::write(IZone* zone, std::shared_ptr<ZoneBuffer>& buf)
		{
			sizeof VertexDecl;

			auto data = this->m_asset;
			auto dest = buf->write(data);

			buf->push_stream(3);
			START_LOG_STREAM;

			dest->name = buf->write_str(this->name());

			ZONETOOL_INFO("vertexdecl %s has %u streams.", data->name, data->streamCount);
			for (int i = 0; i < data->streamCount; i++)
			{
				ZONETOOL_INFO("stream %u: maps 0x%02X to 0x%02X", i, data->streams[i].source, data->streams[i].dest);
			}

			END_LOG_STREAM;
			buf->pop_stream();
		}

		void IVertexDecl::dump(VertexDecl* asset)
		{
			AssetDumper write;
			if (!write.Open("techsets\\"s + asset->name + ".vertexdecl"s))
			{
				return;
			}

			write.Array(asset, 1);
			write.String(asset->name);
			write.Close();
		}
	}
}
