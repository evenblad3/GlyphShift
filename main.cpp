#include <cwchar>
#include <clocale>
#include <string>
#include <unordered_map>
#include <vector>
#include <random>
#include <ctime>
#include <fstream>
#include <locale>
#include <codecvt>

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
		charMask['B'] = {L'В', L'Б'};
		charMask['C'] = {L'С'};
		charMask['D'] = {L'Đ', L'Ď'};
		charMask['E'] = {
			L'Е', L'Ε', L'Ё', L'È', L'Ẻ', L'Ẽ',L'É',
			L'Ẹ', L'Ê', L'Ề', L'Ể',L'Ễ', L'Ế', L'Ệ'
		};
		// F
		// G
		charMask['H'] = {L'Н', L'Η'};
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
		// Q
		charMask['R'] = {L'Я'};
		charMask['S'] = {L'Ѕ'};
		charMask['T'] = {L'Т', L'Τ'};
		charMask['U'] = {
			L'Ù', L'Ủ', L'Ũ', L'Ú', L'Ụ', L'Ư', L'Ừ', L'Ử', L'Ữ', L'Ứ', L'Ự' 
		};
		charMask['V'] = {L'Ѵ'};
		charMask['W'] = {L'Ш'};
		charMask['X'] = {L'Х'};
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
};


int main() {
	setlocale(LC_ALL, "");
	std::wprintf(L"Hello Wide ass characters...\n");
	std::wstring myinput = L"Wide ABC :)";
	GlyphShift obfuscator;
	std::wstring obfuscated = obfuscator.obfuscate(myinput);
	std::wprintf(L"Original   %ls\n", myinput.c_str());
	std::wprintf(L"Obfuscated %ls\n", obfuscated.c_str());
	std::wofstream outFile("bro.txt");
	if (!outFile) {
		std::printf("File was not written!\n");
		return -1;
	}
	outFile.imbue(std::locale(outFile.getloc(), new std::codecvt_utf8<wchar_t>()));
    outFile << obfuscated;
    outFile.close();
	std::printf("File written!\n");

	return 0;
}
