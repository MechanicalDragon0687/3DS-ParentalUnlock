
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <unistd.h>
#include <3ds.h>
#define SECOND(x) (x*1000ULL*1000ULL*1000ULL)
PrintConsole topScreen, bottomScreen;
using namespace std;

void fucked() {
	cout << "\n\nPress [Start] to exit\n\n";	
	while (1) {
		hidScanInput();
		if (hidKeysDown() & KEY_START) { 
			amExit();
			cfguExit();
			gfxExit();
			exit(0);
		}
	}
}

int main(int argc, char* argv[])
{
	gfxInitDefault();
	consoleInit(GFX_TOP, &topScreen);
	consoleInit(GFX_BOTTOM, &bottomScreen);
	consoleSelect(&bottomScreen);
	bool isN3ds = false;
	cout << "Initializing CFG services\n";
	if (R_FAILED(cfguInit())) {
		cout << "Failed to initialize CFG services\n";
		fucked();
	}
	cout << "Initializing AM services\n";
	if (R_FAILED(amInit())) {
		cout << "Failed to initialize APT services\n";
		fucked();
	}
	cout << "Clearing Parental Controls";
//	CFGI_ClearParentalControls();
	u8 MEM00C0000[0xc0] = {0};
	u8 MEMC000001[0x14] = {0};
	u8 MEMC000002[0x200] = {0};
	u8 MEM0100001[0x94] = {0};

	CFG_GetConfigInfoBlk8(0xc0, 0x00C0000, MEM00C0000);
	CFG_GetConfigInfoBlk8(0x14, 0x00C0001, MEM00C0001);
	CFG_GetConfigInfoBlk8(0x200,0x00C0002, MEM00C0002);
	CFG_GetConfigInfoBlk8(0x94, 0x0100001, MEM0101000);
	MEM00C0000[0x0C] = 0;
	MEM00C0000[0x0D] = 0;
	MEM00C0000[0x0E] = 0;
	MEM00C0000[0x0F] = 0;
	CFG_SetConfigInfoBlk8(0xc0, 0x00C0000, MEM00C0000);
	cout << std::hex << MEM0101000 << "\n"
	//CFG_SetConfigInfoBlk8(4, 0xD0000, eulaData);
	fucked("");
	amExit();
	cfguExit();
	gfxExit();

	return 0;
}
