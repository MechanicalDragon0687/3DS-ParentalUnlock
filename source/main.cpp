
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
			gfxExit();
			fsExit();	
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
	CFGI_ClearParentalControls();

	cout << "Initializing PM services\n";
	if (R_FAILED(pmInit())) {
		cout << "Failed to initialize PM services\n";
		fucked();
	}	
	u32 titlecount=0;
	u64 titles[300];
	u64 title=0;
	AM_GetTitleList(&titlecount, MEDIATYPE_NAND,300,titles);
	for (u32 i=0; i<titlecount; i++) {
		if (titles[i] == 0x0004001000020000 || titles[i] == 0x0004001000021000 || titles[i] == 0x0004001000022000 ||titles[i] == 0x0004001000026000 || titles[i] == 0x0004001000027000 || titles[i] == 0x0004001000028000) {
			title=titles[i];
			break;
		}
	}
	PM_LaunchTitle(MEDIATYPE_NAND,title,0x70);
	pmExit();
	amExit();
	cfguExit();
	gfxExit();

	return 0;
}
