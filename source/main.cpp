
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
void wait() {
	//cout << "\n\nPress [Start] to exit\n\n";	
	while (1) {
		hidScanInput();
		if (hidKeysDown() & KEY_START) { 
			return;
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
	u8 ParentalSettings[0xc0] = {0};
	u8 ParentalCOPPACS[0x14] = {0};
	u8 ParentalEmail[0x200] = {0};
	u8 ParentalConfig[0x94] = {0};

	CFG_GetConfigInfoBlk8(0xc0, 0x00C0000, ParentalSettings);
	CFG_GetConfigInfoBlk8(0x14, 0x00C0001, ParentalCOPPACS);
	CFG_GetConfigInfoBlk8(0x200,0x00C0002, ParentalEmail);
	CFG_GetConfigInfoBlk8(0x94, 0x0100001, ParentalConfig);
	//ParentalSettings[0x00] = 0;
	//ParentalSettings[0x01] = 0;
	//ParentalSettings[0x02] = 0;
	//ParentalSettings[0x03] = 0;
//	ParentalSettings[0x03] = 0;
	std::fill(ParentalSettings,ParentalSettings+0x10,0);
	std::fill(ParentalConfig+0x0d,ParentalConfig+0x20,0);
	std::fill(ParentalEmail,ParentalEmail+0x200,0);
	std::fill(ParentalCOPPACS,ParentalCOPPACS+0x14,0);
	CFG_SetConfigInfoBlk8(0xc0, 0x00C0000, ParentalSettings);
	CFG_SetConfigInfoBlk8(0x14, 0x00C0001, ParentalCOPPACS);
	CFG_SetConfigInfoBlk8(0x200, 0x00C0002, ParentalEmail);
	CFG_SetConfigInfoBlk8(0x94, 0x00C0000, ParentalConfig);
	CFG_UpdateConfigSavegame(); 
	//CFG_SetConfigInfoBlk8(4, 0xD0000, eulaData);
	fucked();
	amExit();
	cfguExit();
	gfxExit();

	return 0;
}
