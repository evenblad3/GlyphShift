#include <cwchar>
#include <clocale>
#include <string>
#include <unordered_map>
#include <vector>
#include <random>
#include <ctime>
#include <Windows.h>

class GlyphShift {
private:
	std::unordered_map<wchar_t, std::vector<wchar_t>> charMask;
	std::mt19937 rand;

public:
	GlyphShift() : rand(std::time(nullptr)) {
		initMap();
	}

	void initMap() {
		/*
		=== NUMBERS ===
		*/
		// 1
		// 2
		charMask['3'] = {L'Ʒ', L'ʒ', L'Ȝ', L'ȝ', L'Ҙ', L'ҙ'};
		// 4

		/*
		=== AlPHABETS ===
		*/
		charMask['A'] = {
			L'А', L'Α', L'Á', L'Ä', L'À', L'Ả', L'Ã', L'Á', L'Ạ', L'Ă', L'Ằ',
			L'Ẳ', L'Ẵ', L'Ắ', L'Ặ', L'Â', L'Ầ', L'Ẩ', L'Ẫ', L'Ấ', L'Ậ', L'Ą'
		};
		charMask['a'] = {L'а', L'а', L'а', L'ă', L'å', L'ä', L'ā', L'ą'};
		charMask['B'] = {L'В', L'Б'};
		charMask['C'] = {L'С'};
		charMask['c'] = {L'с', L'ç', L'ć'};
		charMask['D'] = {L'Đ', L'Ď'};
		charMask['E'] = {
			L'Е', L'Ε', L'Ё', L'È', L'Ẻ', L'Ẽ',L'É',
			L'Ẹ', L'Ê', L'Ề', L'Ể',L'Ễ', L'Ế', L'Ệ'
		};
		// F
		// G
		charMask['H'] = {L'Н', L'Η', L'Ӈ'};
		charMask['I'] = {L'І', L'Ї'};
		charMask['J'] = {L'Ј'};
		charMask['K'] = {L'К', L'Ќ'};
		charMask['L'] = {L'Ł'};
		charMask['M'] = {L'М'};
		charMask['N'] = {L'И', L'Й', L'П'};
		charMask['O'] = {
			L'О', L'Ӧ', L'Ο', L'Ѳ', L'Ò', L'Ỏ', L'Õ', L'Ó', L'Ọ', L'Ô',
			L'Ồ', L'Ổ', L'Ỗ', L'Ố', L'Ộ', L'Ơ', L'Ờ', L'Ở', L'Ỡ', L'Ớ', L'Ợ' 
		};
		charMask['P'] = {L'Р', L'Ρ'};
		charMask['p'] = {L'р'};
		charMask['Q'] = {L'Ԛ'};
		charMask['R'] = {L'Я'};
		charMask['S'] = {L'Ѕ'};
		charMask['T'] = {L'Т', L'Τ'};
		charMask['U'] = {
			L'Ù', L'Ủ', L'Ũ', L'Ú', L'Ụ', L'Ư', L'Ừ', L'Ử', L'Ữ', L'Ứ', L'Ự' 
		};
		charMask['V'] = {L'Ѵ'};
		charMask['W'] = {L'Ԝ', L'Ш'};
		charMask['X'] = {L'Х', L'Ҳ'};
		charMask['Y'] = {L'Υ'};
		charMask['Z'] = {L'Ζ', L'Ζ', L'Ż', L'Ž'};
	}

	std::wstring obfuscate(const std::wstring &input) {
		std::wstring result;

		for (wchar_t character : input) {
			auto it = charMask.find(character);
			if (it != charMask.end() && !it->second.empty()) {
				std::uniform_int_distribution<int> dist(0, it->second.size() - 1);
				result += it->second[dist(rand)];
			} else {
				result += character;
			}
		}
		return result;
	}

	void copyToClipboard(const std::wstring &ws) {
		if (!OpenClipboard(nullptr)) {
			std::printf("Unable to access clipboard.\n");
			return;
		}

		struct clipboardDistructor {
			~clipboardDistructor() {
				CloseClipboard();
			}
		} cd;

		EmptyClipboard();

		HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, (ws.size() + 1) * sizeof(wchar_t));
		if (hg == nullptr) {
			std::printf("Memory allocation failed.\n");
			CloseClipboard();
			return;
		}

		wchar_t *lpstrCopy = static_cast<wchar_t*>(GlobalLock(hg));
		if (lpstrCopy == nullptr) {
			std::printf("Unable to lock global memory failed.\n");
			GlobalFree(hg);
			CloseClipboard();
			return;
		}

		std::wmemcpy(lpstrCopy, ws.c_str(), ws.size() + 1);
		GlobalUnlock(hg);
		SetClipboardData(CF_UNICODETEXT, hg);
		CloseClipboard();
	}
};


int main() {
	setlocale(LC_ALL, "");
	std::wprintf(L"=== GlyphShift ===\n");
	wchar_t buffer[CHAR_MAX];
	std::printf("Enter text to obfuscate: ");
	fgetws(buffer, CHAR_MAX, stdin);
	buffer[wcscspn(buffer, L"\n")] = '\0';

	GlyphShift obfuscator;
	std::wstring obfuscated = obfuscator.obfuscate(buffer);

	std::wprintf(L"Original:   %ls\n", buffer);
	std::wprintf(L"Obfuscated: %ls\n", obfuscated.c_str());

	obfuscator.copyToClipboard(obfuscated);

	return 0;
}
