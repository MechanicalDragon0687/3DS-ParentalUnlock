
#include <iostream>
#include <string>
#include <unistd.h>
#include <3ds.h>

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

	cout << "Allocating memory\n";
	u8 ParentalSettings[0xc0] = {0};
	u8 ParentalCOPPACS[0x14] = {0};
	u8 ParentalEmail[0x200] = {0};
	u8 ParentalConfig[0x94] = {0};

	cout << "Reading current CONFIG data\n";
	CFG_GetConfigInfoBlk8(0xc0, 0x00C0000, ParentalSettings);
	CFG_GetConfigInfoBlk8(0x14, 0x00C0001, ParentalCOPPACS);
	CFG_GetConfigInfoBlk8(0x200,0x00C0002, ParentalEmail);
	CFG_GetConfigInfoBlk8(0x94, 0x0100001, ParentalConfig);

	cout << "Clearing Parental Controls\n";
	std::fill(ParentalSettings,ParentalSettings+0x10,0);
	std::fill(ParentalConfig+0x0d,ParentalConfig+0x20,0);
	std::fill(ParentalEmail,ParentalEmail+0x200,0);
	std::fill(ParentalCOPPACS,ParentalCOPPACS+0x14,0);

	cout << "Updating empty Parental Control settings\n";
	if (R_FAILED(CFG_SetConfigInfoBlk8(0xc0, 0x00C0000, ParentalSettings))) {
		cout << "Failed to reset Parental Control settings\n";
	}

	cout << "Updating empty COPPA settings\n";
	if (R_FAILED(CFG_SetConfigInfoBlk8(0x14, 0x00C0001, ParentalCOPPACS))) {
		cout << "Failed to reset Child Protection settings\n";
	}

	cout << "Updating empty Parent Email settings\n";
	if (R_FAILED(CFG_SetConfigInfoBlk8(0x200, 0x00C0002, ParentalEmail))) {
		cout << "Failed to reset Parent email\n";
	}

	cout << "Updating empty Parent secret question and pin\n";
	if (R_FAILED(CFG_SetConfigInfoBlk8(0x94, 0x0100001, ParentalConfig))) {
		cout << "Failed to set Parent secret question and pin settings\n";
	}

	cout << "Saving cleared settings\n";
	if (R_FAILED(CFG_UpdateConfigSavegame())) {
		cout << "Failed to update all settings\n";
	}
 	cout << "\nDone!\n";
	fucked();
	
	// should never get here
	amExit();
	cfguExit();
	gfxExit();

	return 0;
}
