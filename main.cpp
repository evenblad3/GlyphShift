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
		charMask['A'] = {L'А', L'А', L'А', L'А', L'А', L'А'};
		charMask['B'] = {L'В', L'Б', L'В', L'Б'};
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
