#include "CPdh.h"

#define TTEST(NAME) L"\\Process("#NAME")\\Thread Count"

void main()
{
	CPDH* pPdh = new CPDH();

	/// Add Counter ///
	int nIdx_CpuUsage = -1;
	int nIdx_MemAvail = -1;
	int nIdx_NpMem = -1;
	int nIdx_EthernetRecv = -1;
	int nIdx_EthernetSend = -1;
	if (pPdh->AddCounter(df_PDH_CPUUSAGE_TOTAL, nIdx_CpuUsage) != ERROR_SUCCESS)
		return;
	if (pPdh->AddCounter(df_PDH_MEMAVAIL_MB, nIdx_MemAvail) != ERROR_SUCCESS)
		return;
	if (pPdh->AddCounter(df_PDH_NONPAGEDMEM_BYTES, nIdx_NpMem) != ERROR_SUCCESS)
		return;
	//if (pPdh->AddCounter(df_PDH_ETHERNETRECV_BYTES, nIdx_EthernetRecv) != ERROR_SUCCESS)
	//	return;
	//if (pPdh->AddCounter(df_PDH_ETHERNETSEND_BYTES, nIdx_EthernetSend) != ERROR_SUCCESS)
	//	return;
	if (pPdh->AddCounter(L"\\Network Interface(Intel[R] Dual Band Wireless-AC 3168)\\Bytes Received/sec", nIdx_EthernetRecv) != ERROR_SUCCESS) 
		return;
	if (pPdh->AddCounter(L"\\Network Interface(Intel[R] Dual Band Wireless-AC 3168)\\Bytes Sent/sec", nIdx_EthernetSend) != ERROR_SUCCESS)
		return;
	

	/// Performance Monitoring ///
	double dCpu, dMem, dNpmem, dERecv, dESend;
	double dMin, dMax, dMean;
	while (1)
	{
		/// Collect Data ///
		if (pPdh->CollectQueryData())
			continue;

		/// Update Counters ///
		if (!pPdh->GetCounterValue(nIdx_CpuUsage, &dCpu)) dCpu = 0;
		if (!pPdh->GetCounterValue(nIdx_MemAvail, &dMem)) dMem = 0;
		if (!pPdh->GetCounterValue(nIdx_NpMem, &dNpmem)) dNpmem = 0;
		if (!pPdh->GetCounterValue(nIdx_EthernetRecv, &dERecv)) dERecv = 0;
		if (!pPdh->GetCounterValue(nIdx_EthernetSend, &dESend)) dESend = 0;

		/*---------------------------------------------------------------------*/
		/// Get Statistics ///
		wprintf(L"===========================================\n");
		wprintf(L" Monitoring Server\n");
		wprintf(L"===========================================\n");

		wprintf(L" - HW CPU Usage 	: %.1f %%", dCpu);
		if (pPdh->GetStatistics(&dMin, &dMax, &dMean, nIdx_CpuUsage))
			wprintf(L" (Min %.1f / Max %.1f / Mean %.1f)", dMin, dMax, dMean);
		wprintf(L"\n");

		wprintf(L" - HW Available Memory	: %.0f MB\n", dMem);

		wprintf(L" - HW NonPaged Memory	: %.3f MB\n", dNpmem / 1024 / 1024);

		wprintf(L" - Ethernet RecvBytes	: %.3f B/sec", dERecv);
		if (pPdh->GetStatistics(&dMin, &dMax, &dMean, nIdx_EthernetRecv))
			wprintf(L" (Min %.1f / Max %.1f / Mean %.1f)", dMin, dMax, dMean);
		wprintf(L"\n");

		wprintf(L" - Ethernet SendBytes	: %.3f B/sec", dESend);
		if (pPdh->GetStatistics(&dMin, &dMax, &dMean, nIdx_EthernetSend))
			wprintf(L" (Min %.1f / Max %.1f / Mean %.1f)", dMin, dMax, dMean);
		wprintf(L"\n");
		/*---------------------------------------------------------------------*/


		Sleep(1000);
		system("cls");
	}

	pPdh->RemoveCounter(nIdx_CpuUsage);
	pPdh->RemoveCounter(nIdx_MemAvail);
	pPdh->RemoveCounter(nIdx_NpMem);
	pPdh->RemoveCounter(nIdx_EthernetRecv);
	pPdh->RemoveCounter(nIdx_EthernetSend);

	delete pPdh;
}