#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <tchar.h>

//This function prints an error message based on the function supplied.
void ErrorMes(LPTSTR lpszFunction)
{
	//Retrieve the system error message for the last-error code.

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	//Display the error message. Exit the process.

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	wprintf(L"%s failed with error %d: %s",
		lpszFunction, dw, lpMsgBuf);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);

}

//Display info for file in specified cluster.
BOOL FindWithCluster(TCHAR* volume, LONGLONG cluster) {

	HANDLE hDevice = CreateFile(volume,      //The volume that should be opened.
		GENERIC_READ,                       //Access type (Read).
		FILE_SHARE_READ | FILE_SHARE_WRITE,  //Share mode type.
		NULL,                                //Security attributes.
		OPEN_EXISTING,                       //Disposal parameters.
		FILE_FLAG_BACKUP_SEMANTICS,          //Other file attributes.
		NULL);

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		ErrorMes(L"CreateFile");
		return FALSE;
	}

	//Initialize variables for input data.

	LOOKUP_STREAM_FROM_CLUSTER_INPUT input = { 0 };
	input.NumberOfClusters = 1;
	input.Cluster[0].QuadPart = cluster;

	//Initialize the output buffer.

	BYTE output[5000];
	DWORD dwRes = 0;
	LOOKUP_STREAM_FROM_CLUSTER_OUTPUT result = { 0 };

	//Use IOCTL for getting buffer output

	BOOL bufferOutput = DeviceIoControl((HANDLE)hDevice,   // The handle to use. 
		FSCTL_LOOKUP_STREAM_FROM_CLUSTER, // IOCTL function code, in this case being the lookup stream.
		(LPVOID)&input,        // Buffer for input data. 
		(DWORD)        sizeof(input),     // Input buffer size. 
		(LPVOID)output,       // Buffer for output data. 
		(DWORD)5000,    // Output buffer size. 
		(LPDWORD)&dwRes,   // No. of bytes to be returned. 
		NULL);    // OVERLAPPED struct

	//If IOCTL failed...
	if (bufferOutput == FALSE) {
		ErrorMes(L"DeviceIoControl");
		return FALSE;
	}

	//...otherwise, copy buffer into the struct of LOOKUP_STREAM_FROM_CLUSTER_OUTPUT
	memcpy(&result, output, sizeof(LOOKUP_STREAM_FROM_CLUSTER_OUTPUT));

	if (result.NumberOfMatches == 0) {
		wprintf(L"The file was not found.\n");
		return FALSE;
	}

	wprintf(L"File info:\n");

	//Go to the first struct match.
	BYTE* p = (BYTE*)output + result.Offset;
	LOOKUP_STREAM_FROM_CLUSTER_ENTRY* pentry = (LOOKUP_STREAM_FROM_CLUSTER_ENTRY*)p;

	//Output flag info.
	wprintf(L"Flags: 0x%x ", (UINT)pentry->Flags);

	if ((pentry->Flags & LOOKUP_STREAM_FROM_CLUSTER_ENTRY_FLAG_PAGE_FILE) > 0) wprintf(L"(Pagefile)");
	else if ((pentry->Flags & LOOKUP_STREAM_FROM_CLUSTER_ENTRY_FLAG_FS_SYSTEM_FILE) > 0)  wprintf(L"(Internal filesystem file)");
	else if ((pentry->Flags & LOOKUP_STREAM_FROM_CLUSTER_ENTRY_FLAG_TXF_SYSTEM_FILE) > 0) wprintf(L"(Internal TXF file)");
	else wprintf(L"(Normal file)");

	wprintf(L"\nFileName: %s\n", pentry->FileName);
	return TRUE;
}

int _tmain(int argc, _TCHAR* argv[])
{
	LONGLONG inp = 0;
	wprintf(L"Make sure this program is run as admin! \n");
	wprintf(L"Enter cluster index: \n");
	scanf("%llu", &inp);

	FindWithCluster(L"\\\\.\\C:", inp);

	system("PAUSE");
	return 0;
}