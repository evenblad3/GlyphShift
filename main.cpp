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
		charMask['A'] = {L'А'};
		charMask['B'] = {L'В', L'Б'};
		charMask['C'] = {L'С'};
		// D
		charMask['E'] = {L'Е', L'Ё'};
		// F
		// G
		charMask['H'] = {L'Н', L'Њ'};
		charMask['I'] = {L'І', L'Ї'};
		// J
		charMask['K'] = {L'К', L'Ќ'};
		// L
		charMask['M'] = {L'М'};
		charMask['N'] = {L'И', L'Й', L'П'};
		charMask['O'] = {L'О', L'Ӧ', L'Ф', L'Ѳ'};
		charMask['P'] = {L'Р'};
		// Q
		charMask['R'] = {L'Я'};
		charMask['S'] = {L'Ѕ'};
		charMask['T'] = {L'Т'};
		// U
		charMask['V'] = {L'Ѵ'};
		charMask['W'] = {L'Ш'};
		charMask['X'] = {L'Х'};
		charMask['Y'] = {L'У'};
		charMask['Z'] = {L'Ζ'};
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
