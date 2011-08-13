#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#include "djdevioctls.h"
#include "commondefs.h"

int m_nBulkHandle;
DJ_DEVICE_STATE g_DEVICE_INFO[1];

#define BULK_IOCTL_TIMEOUT 	1000	// Retry time-out is set to 1 second (1000 ms)
#define BULK_IOCTL_RETRY_DELAY	50	// Delay between retry is set to 50 ms.
int BulkIoctl(int nRequest, void *pParam) {
	int nRetCode, nRetry = BULK_IOCTL_TIMEOUT / BULK_IOCTL_RETRY_DELAY;
	bool bPrintError = true;

	if (m_nBulkHandle < 0) {
		printf("Invalid hdjbulk handle with nRequest=%d.\n", nRequest);
		return -1;
	}

	while (nRetry-- > 0) {
		nRetCode = ioctl(m_nBulkHandle, nRequest, pParam);
		if (nRetCode >= 0) {
			return nRetCode;
		}
		usleep(BULK_IOCTL_RETRY_DELAY * 1000);
	}

	printf("BULK - IOCTL fail with code %d (nRequest=0x%.8X, errno=%d)\n",
			nRetCode, nRequest, errno);
	return nRetCode;
}

int SingleBulkIoctl(int nBulkHandle, int nRequest, void *pParam) {
	int nRetCode;

	m_nBulkHandle = nBulkHandle;
	nRetCode = BulkIoctl(nRequest, pParam);
	m_nBulkHandle = -1;
	return nRetCode;
}


bool SetRecordInput(RECORD_INPUT eRecordInput, DJ_DEVICE_STATE *pState) {
	unsigned long ulValue;
	unsigned short usValue;

	if (pState->ulProductCode == DJCONSOLE_PRODUCT_CODE) {
		switch (eRecordInput) {
			case LINE_IN:
				ulValue = DJC_AUDIOCFG_ANALOG_IN_LINE;
				break;
			case MIC:
				ulValue = 0;
				break;
			case DIGITAL_COAXIAL:
				ulValue = DJC_AUDIOCFG_DIGITAL_IN|DJC_AUDIOCFG_DIGITAL_IN_COAXIAL;
				break;
			case DIGITAL_OPTICAL:
				ulValue = DJC_AUDIOCFG_DIGITAL_IN;
				break;
			default:
				break;
		}
		ulValue = (ulValue << 16) | (DJC_AUDIOCFG_DIGITAL_IN |
				DJC_AUDIOCFG_ANALOG_IN_LINE |
				DJC_AUDIOCFG_DIGITAL_IN_COAXIAL);
		if (SingleBulkIoctl(pState->nBULK_FHandle,
					DJ_IOCTL_SET_DJCONSOLE_CONFIG, &ulValue) < 0) {
			return false;
		}

		pState->djConsole.eRecordingInput = eRecordInput;
	} else {
		printf("Unsupported device\n");
		return false;
	}

	printf("bulk-SetRecordInput (eRecordInput=%d) with success\n", eRecordInput);
	return true;
}

bool Open(int nDeviceIndex, DJ_DEVICE_STATE *pState) {
	char szBulkFilename[40];
	struct stat stat_buf;

	if (pState == NULL) {
		return false;
	}

	pState->bBULK_Init = false;
	pState->ulBULK_Version = 0x00000000;
	pState->ulBULK_ReadSize = 0;
	pState->ulBULK_OutSize = 0;
	pState->nBULK_FileIndex = -1;

	sprintf(szBulkFilename, "/dev/hdjbulk%d", nDeviceIndex);
	if (stat(szBulkFilename, &stat_buf) < 0)
		return true;

	pState->nBULK_FHandle = open(szBulkFilename, O_RDWR, 0);
	if (pState->nBULK_FHandle < 0)
		return true;

	m_nBulkHandle = pState->nBULK_FHandle;
	BulkIoctl(DJ_IOCTL_GET_PRODUCT_CODE, &pState->ulProductCode);
	BulkIoctl(DJ_IOCTL_GET_LOCATION_ID, pState->szLocationID);
	BulkIoctl(DJ_IOCTL_GET_VERSION, &pState->ulBULK_Version);
	BulkIoctl(DJ_IOCTL_GET_CONTROL_DATA_INPUT_PACKET_SIZE,
			&pState->ulBULK_ReadSize);
	BulkIoctl(DJ_IOCTL_GET_CONTROL_DATA_OUTPUT_PACKET_SIZE,
			&pState->ulBULK_OutSize);
	BulkIoctl(DJ_IOCTL_ACQUIRE_NETLINK_UNIT, &pState->nNetlinkUnit);
	m_nBulkHandle = -1;


	if (pState->ulProductCode == DJCONSOLE_PRODUCT_UNKNOWN ||
		pState->ulProductCode>DJCONTROLSTEEL_PRODUCT_CODE ||
		pState->szLocationID[0] == '\0') {
		close(pState->nBULK_FHandle);
		printf("BULK - Open (failed)\n");
		return false;
	}

	pState->bBULK_Init = true;
	pState->nBULK_FileIndex = nDeviceIndex;
	printf("BULK - Open (%s)\n", pState->szLocationID);
	return true;

}

bool Close(DJ_DEVICE_STATE *pState) {
	if (pState->bBULK_Init) {
		printf("BULK - Close (%s)\n", pState->szLocationID);
		close(pState->nBULK_FHandle);
		pState->bBULK_Init = false;
		pState->ulProductCode = DJCONSOLE_PRODUCT_UNKNOWN;
		pState->ulBULK_Version = 0x00000000;
		pState->nBULK_FileIndex = -1;
	}

	return true;
}

int main() {
	memset((void *)&g_DEVICE_INFO[0], 0, sizeof(DJ_DEVICE_STATE));
	g_DEVICE_INFO[0].nBULK_FHandle = -1;

	m_nBulkHandle = -1;
	Open(0, (DJ_DEVICE_STATE *) &g_DEVICE_INFO[0]);
	if (g_DEVICE_INFO[0].bBULK_Init) {
		SetRecordInput(LINE_IN, (DJ_DEVICE_STATE *) &g_DEVICE_INFO[0]);
	}
}
