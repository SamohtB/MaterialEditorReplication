#pragma once
#include <string>
#include <unordered_set>

class ShaderTypes
{
public:
	using String = std::string;

	inline static constexpr const char* UNLIT = "UNLIT";
	inline static constexpr const char* LIT = "LIT";

	static bool IsValid(const String& shader)
	{
		static const std::unordered_set<String> valid = { UNLIT, LIT };
		return valid.find(shader) != valid.end();
	}
};

struct ShaderDesc
{
	std::wstring shaderFilePath;
	std::wstring shaderEntry;
	std::wstring shaderTarget;

	ShaderDesc() = default;
	ShaderDesc(std::wstring path, std::wstring entry, std::wstring target) :
		shaderFilePath(path), shaderEntry(entry), shaderTarget(target) {
	}
};
