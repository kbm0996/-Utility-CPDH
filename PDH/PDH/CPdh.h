/*------------------------------------------------------------------------------------------
  PERFORMANCE DATA HELPER Ŭ����

 PDH API�� ���� ī���͸� �̿��Ͽ� OS, ���� ���α׷�, ����, ����̹��� ������ ����͸�


- ����

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
	if (pPdh->AddCounter(df_PDH_ETHERNETRECV_BYTES, nIdx_EthernetRecv) != ERROR_SUCCESS)
		return;
	if (pPdh->AddCounter(df_PDH_ETHERNETSEND_BYTES, nIdx_EthernetSend) != ERROR_SUCCESS)
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

		/// Get Statistics ///
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


		Sleep(1000);
		system("cls");
	}

	pPdh->RemoveCounter(nIdx_CpuUsage);
	pPdh->RemoveCounter(nIdx_MemAvail);
	pPdh->RemoveCounter(nIdx_NpMem);
	pPdh->RemoveCounter(nIdx_EthernetRecv);
	pPdh->RemoveCounter(nIdx_EthernetSend);

	delete pPdh;

-------------------------------------------------------------------------------------------*/


#ifndef __PERFORMANCE_DATA_HELPER_H__
#define __PERFORMANCE_DATA_HELPER_H__
#include <Pdh.h>
#pragma comment(lib, "pdh.lib")
#include <vector>

/*********************************************************************************************
// PDH Browse Counters Define
//
//  PDH �������� �˾Ƴ��� ���� �ش� �ҽ� Ȥ�� `���� �����` �̿�
// https://docs.microsoft.com/ko-kr/windows/desktop/PerfCtrs/browsing-performance-counters
**********************************************************************************************/
/// CPU ///
// * CPU ��뷮�� OS�� ���� ���������� ������� �ʴ� ��쵵 ���� : ���� ���ð��� ���ؼ� ����ϵ��� ��
#define df_PDH_CPUUSAGE_TOTAL L"\\Processor(_Total)\\% Processor Time"	// CPU ��ü ����(%)
#define df_PDH_CPUUSAGE_0 L"\\Processor(0)\\% Processor Time"			// CPU �ھ� ����(%)
#define df_PDH_CPUUSAGE_1 L"\\Processor(1)\\% Processor Time"
#define df_PDH_CPUUSAGE_2 L"\\Processor(2)\\% Processor Time"
#define df_PDH_CPUUSAGE_3 L"\\Processor(3)\\% Processor Time"
//#define df_PDH_CPUUSAGE_USER L"\\Process(NAME)\\% User Time"			// ���μ��� CPU ���� ����(%)
//#define df_PDH_CPUUSAGE_USER L"\\Process(NAME)\\% Processor Time"		// ���μ��� CPU ��ü ����(%)
/// MEMORY ///
#define df_PDH_MEMINUSE_BYTES L"\\Memory\\Committed Bytes" // �̸� �Ҵ�� �޸�(Bytes)
#define df_PDH_MEMAVAIL_BYTES L"\\Memory\\Available Bytes" // ��밡�� �޸�(Bytes)
#define df_PDH_MEMAVAIL_KB L"\\Memory\\Available KBytes" // ��밡�� �޸�(Kilobytes)
#define df_PDH_MEMAVAIL_MB L"\\Memory\\Available MBytes" // ��밡�� �޸�(Megabytes)
#define df_PDH_MEMINUSE_PERCENT L"\\Memory\\% Committed Bytes In Use" // ����� �޸�(Committed Bytes/Commit Limit)(%)
#define df_PDH_MEMLIMIT_BYTES L"\\Memory\\Commit Limit" // �̸� �Ҵ� �Ѱ� �޸�(Bytes)
#define df_PDH_NONPAGEDMEM_BYTES L"\\Memory\\Pool Nonpaged Bytes" // �������� �޸�(Bytes)
/// PROCESS ///
#define df_PDH_PROCESS_COUNT(PROCESS_NAME) L"\\Process("#PROCESS_NAME")\\Thread Count" // ���μ��� ������ ��(n)
#define df_PDH_PROCESS_HANDLE_COUNT(PROCESS_NAME) L"\\Process("#PROCESS_NAME")\\Handle Count" // ���μ��� �ڵ� ��(n)
#define df_PDH_PROCESS_VRMEM_BYTES(PROCESS_NAME) L"\\Process("#PROCESS_NAME")\\Virtual Bytes" // ���μ��� ���� �޸�(Bytes)
// * ���� �޸� = ����� �޸�(Reserved) + ���(boundary, 65KB). * ���� ��� �޸�(commit)���� ����
#define df_PDH_PROCESS_USERMEM_BYTES(PROCESS_NAME) L"\\Process("#PROCESS_NAME")\\Private Bytes" // ���μ��� �����Ҵ� �޸�(Bytes)
// * ���μ��� ���� �޸�/Ŀ�� �޸� ����
#define df_PDH_PROCESS_WORKMEM(PROCESS_NAME) L"\\Process("#PROCESS_NAME")\\Working Set" // ���μ��� �۾� �޸�(Bytes?)
// * ���� ���� �޸𸮿� ���Ǵ� ũ��. ���� �Ҵ� �뷮�� �ƴ� �� ����
#define df_PDH_PROCESS_NPMEM(PROCESS_NAME) L"\\Process("#PROCESS_NAME")\\Pool Nonpaged Bytes" // ���μ��� �������� �޸�(Bytes)
/// ETHERNET ///
#define df_PDH_ETHERNETRECV_BYTES L"\\Network Interface(*)\\Bytes Received/sec" // �̴��� ���ŷ�(Bytes/sec)
#define df_PDH_ETHERNETSEND_BYTES L"\\Network Interface(*)\\Bytes Sent/sec" // �̴��� �۽ŷ�(Bytes/sec)


#define df_MAX_RAW 20

struct stPDHCOUNTER 
{
	stPDHCOUNTER()
	{
		dValue = nNextIdx = nOldestIdx = nRawCount = 0;
	}

	int nIdx;
	
	double dValue;

	HCOUNTER hCounter;	// ī���� �ڵ�

	/// ���� ������ ///
	PDH_RAW_COUNTER RingBuffer[df_MAX_RAW];
	int nNextIdx;		// ���� ���� ���ð� �ε���
	int nOldestIdx;		// ���� ������ ���ð� �ε���
	int nRawCount;		// ����� ���ð� �� ����
};

class CPDH
{
public:
	CPDH();
	virtual ~CPDH();

	BOOL Init();
	void Clean();

	/// COUNTERS ///
	////////////////////////////////////////////////////////////
	// ī���� �߰�
	//  ���� ī���� �߰�
	//
	// Parameters : (WCHAR*) ����(CPdh.h �� define�� ����)
	//				(int&) ������ ī���� �ε���(idx)��, ���н� -1
	// Return :		(LONG) ������ 0, ���н� ������
	////////////////////////////////////////////////////////////
	LONG AddCounter(const WCHAR* pszPdhDefine, int& iOutIdx);

	////////////////////////////////////////////////////////////
	// ī���� ����
	//  Ư�� �ε����� ī���� ����
	//
	// Parameters : (int) ������ ī������ �ε��� ��ȣ
	// Return :		(LONG) ������ 0, ���н� �������̳� -1
	////////////////////////////////////////////////////////////
	LONG RemoveCounter(int nIdx);


	/// DATA ///
	////////////////////////////////////////////////////////////
	// ī���� �ϰ� ����
	//  �ش� Ŭ������ ���� �ȿ� �ִ� ��� ī���� ����
	//
	// Parameters :
	// Return :		(LONG) ������ 0, ���н� ������
	////////////////////////////////////////////////////////////
	LONG CollectQueryData();

	////////////////////////////////////////////////////////////
	// ī���� ���
	//
	// Parameters : (double*) �ּ�
	//				(double*) �ִ�
	//				(double*) ���
	//				(int) ����� ī���� �ε�����
	// Return :		(BOOL) ���� ����
	////////////////////////////////////////////////////////////
	BOOL GetStatistics(double* nMin, double* nMax, double* nMean, int nIdx);

	////////////////////////////////////////////////////////////
	// Ư�� ī���� ��
	//
	// Parameters : (int) ���� ���� ī���� �ε�����
	//				(double*)
	// Return :		(BOOL) ���� ����
	////////////////////////////////////////////////////////////
	BOOL GetCounterValue(int nIdx, double* dValue);

protected:
	/// COUNTERS ///
	////////////////////////////////////////////////////////////
	// ī���� ã��
	//  ���� �� �ش� �ε����� ���� ī���� ã��
	//
	// Parameters : (int) ã�� ī������ �ε���
	// Return :		(stPDHCOUNTER*) ã�� ī������ ������
	////////////////////////////////////////////////////////////
	stPDHCOUNTER* FindPdhCounter(int nIdx);


	/// COUNTER VALUES ///
	////////////////////////////////////////////////////////////
	// Ư�� ī���� ����(double)
	//  ī���Ͱ��� double������ �����ϰ� ���
	//
	// Parameters : (stPDHCOUNTER*) ī���Ͱ��� ������ ����ü ������
	// Return :		(LONG) ������ 0, ���н� ������
	////////////////////////////////////////////////////////////
	LONG UpdateValue(stPDHCOUNTER* pCounter);

	////////////////////////////////////////////////////////////
	// Ư�� ī���� ���� (������)
	//  ī���Ͱ��� �����ϰ� ���
	//
	// Parameters : (stPDHCOUNTER*) ī���Ͱ��� ������ ����ü ������
	// Return :		(LONG) ������ 0, ���н� ������
	////////////////////////////////////////////////////////////
	LONG UpdateRawValue(stPDHCOUNTER* pCounter);

private:
	int		_nIdx;
	HQUERY	_hQuery;
	std::vector<stPDHCOUNTER*> _vPerfData;
	
};

#endif // !__PERFORMANCE_DATA_HELPER_H__
