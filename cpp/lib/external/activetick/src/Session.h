#pragma once
#include <ActiveTickServerAPI/ActiveTickServerAPI.h>
#include <condition_variable>
#include <future>
#include <mutex>
#include <string>

class APISession
{
public:
	APISession();
	virtual ~APISession(void);

public:
	uint64_t GetSessionHandle() const { return m_hSession; }

public:
	bool		Init(const ATGUID& apiUserid, const std::string& serverIpAddress, uint32_t serverPort, const wchar16_t* userid, const wchar16_t* password);
	bool		Destroy();

private:
	static void ATSessionStatusChangeCallback(uint64_t hSession, ATSessionStatusType statusType);
	static void ATLoginResponseCallback(uint64_t hSession, uint64_t hRequest, LPATLOGIN_RESPONSE pResponse);
	static void ATServerTimeUpdateCallback(LPATTIME pServerTime);
	static void ATRequestTimeoutCallback(uint64_t hOrigRequest);

private:
	uint64_t		m_hSession;
	uint64_t		m_hLastRequest;
	wchar16_t		m_userid[50];
	wchar16_t		m_password[50];

private:
	static APISession*	s_pInstance;

private:
	std::promise<int> _promise;
	std::mutex m;
	std::condition_variable cv;
};
