#include "ntdll.h"
#include "Helper.h"

namespace Helper
{


	std::vector<char> loadBinaryFile(const std::string& path)
	{
		std::vector<char> result;
		std::ifstream in{ path, std::ios::binary };
		if (!in)
			return result;
		in.seekg(0, std::ios_base::end);
		result.resize(static_cast<std::size_t>(in.tellg()));
		in.seekg(0, std::ios_base::beg);
		in.read(result.data(), result.size());
		return result;
	}

	bool decodeVFONT(std::vector<char>& buffer)
	{
		constexpr std::string_view tag = "VFONT1";
		unsigned char magic = 0xA7;

		if (buffer.size() <= tag.length())
			return false;

		const auto tagIndex = buffer.size() - tag.length();
		if (std::memcmp(tag.data(), &buffer[tagIndex], tag.length()))
			return false;

		unsigned char saltBytes = buffer[tagIndex - 1];
		const auto saltIndex = tagIndex - saltBytes;
		--saltBytes;

		for (std::size_t i = 0; i < saltBytes; ++i)
			magic ^= (buffer[saltIndex + i] + 0xA7) % 0x100;

		for (std::size_t i = 0; i < saltIndex; ++i) {
			unsigned char xored = buffer[i] ^ magic;
			magic = (buffer[i] + 0xA7) % 0x100;
			buffer[i] = xored;
		}

		buffer.resize(saltIndex);
		return true;
	}

	ImWchar* getFontGlyphRanges()
	{
		static ImVector<ImWchar> ranges;
		if (ranges.empty()) {
			ImFontGlyphRangesBuilder builder;
			constexpr ImWchar baseRanges[]{
				0x0100, 0x024F, // Latin Extended-A + Latin Extended-B
				0x0300, 0x03FF, // Combining Diacritical Marks + Greek/Coptic
				0x0600, 0x06FF, // Arabic
				0x0E00, 0x0E7F, // Thai
				0
			};
			builder.AddRanges(baseRanges);
			builder.AddRanges(ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
			builder.AddRanges(ImGui::GetIO().Fonts->GetGlyphRangesChineseSimplifiedCommon());
			builder.AddText("\u9F8D\u738B\u2122");
			builder.BuildRanges(&ranges);
		}
		return ranges.Data;
	}

	ImFont* addFontFromVFONT(const std::string& path, float size, const ImWchar* glyphRanges, bool merge)
	{
		auto file = loadBinaryFile(path);
		if (!decodeVFONT(file))
			return nullptr;

		ImFontConfig cfg;
		cfg.FontData = file.data();
		cfg.FontDataSize = file.size();
		cfg.FontDataOwnedByAtlas = false;
		cfg.MergeMode = merge;
		cfg.GlyphRanges = glyphRanges;
		cfg.SizePixels = size;

		return ImGui::GetIO().Fonts->AddFont(&cfg);
	}

	ImFont* addFontFromttf(const std::string& path, float size, const ImWchar* glyphRanges, bool merge)
	{

		ImFontConfig cfg;
		cfg.MergeMode = merge;
		cfg.GlyphRanges = glyphRanges;
		cfg.SizePixels = size;

		return ImGui::GetIO().Fonts->AddFontFromFileTTF(path.c_str(), size, &cfg, glyphRanges);
	}

	std::tuple<UINT64, UINT64> GetModuleByName(const wchar_t* name) {
		UINT64 reAddr = 0;
		UINT64 reLen = 0;
		auto peb = NtCurrentPeb();

		PLIST_ENTRY ListHeader;
		PLIST_ENTRY CurrentListEntry;
		PLDR_DATA_TABLE_ENTRY LdrDataTable;

		PPEB_LDR_DATA ldr = peb->Ldr;
		ListHeader = &ldr->InMemoryOrderModuleList;

		CurrentListEntry = ListHeader->Flink;
		while (CurrentListEntry != ListHeader)
		{
			LdrDataTable = CONTAINING_RECORD(CurrentListEntry, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

			if (!_wcsicmp(name, (const wchar_t*)LdrDataTable->BaseDllName.Buffer))
			{
				reAddr = (UINT64)LdrDataTable->DllBase;
				reLen = (UINT64)LdrDataTable->SizeOfImage;
				break;
			}
			CurrentListEntry = CurrentListEntry->Flink;
		}

		return std::make_tuple(reAddr, reLen);
	}


};
