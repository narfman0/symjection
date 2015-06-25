// symjection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"


int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}


void attach(int pid){
	HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, 0, (DWORD)pid);
	char path[160];
	if(GetModuleFileNameEx((DWORD)pid, 0, (TCHAR*)path, 160)){
		void* remoteImports = getImportsBase(path);


	}
}

void* getImportsBase(const char* path, const char* lib){
	FILE bin = fopen(path, "r");

	struct IMAGE_DOS_HEADER  dos_header;
	fread((void*)dos_header, sizeof(IMAGE_DOS_HEADER), 1, bin);
	fseek(bin, (dos_header.obj.e_lfanew + 4), SEEK_SET);

	struct IMAGE_FILE_HEADER pe_header;
	fread((void*)pe_header, sizeof(IMAGE_FILE_HEADER), 1, bin);

	struct IMAGE_OPTIONAL_HEADER opt_header;
	fread((void*)opt_header, sizeof(IMAGE_OPTIONAL_HEADER), 1, bin);

	IMAGE_SECTION_HEADER* sect_headers = new IMAGE_SECTION_HEADER[pe_header.NumberOfSections];
	fread((void*)sect_headers, sizeof(IMAGE_SECTION_HEADER), pe_header.NumberOfSections, bin);

	for(int i = 0; i < pe_header.NumberOfSections; i++){
		if(0 == strcmp(".idata", sect_headers[i].Name)){
			bool nextImportExists = TRUE;
			fseek(bin, sect_headers[i].VirtualAddress, SEEK_SET);
			while(nextImportExists){
				struct IMAGE_IMPORT_DESCRIPTOR import;
				fread((void*)import, sizeof(IMAGE_IMPORT_DESCRIPTOR), 1, bin);
				if(!import.Name){
					nextImportExists = FALSE;
				}
				if(0 == strcmp(import.Name, lib)){
					 return (void*)((import.FirstThunk - sect_headers[i].VirtualAddress) + opt_header.ImageBase);
				}
			}
		}
	}
	return NULL;
}


