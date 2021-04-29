#pragma once
#include "Session.h"
#include <ActiveTickServerRequestor.h>
#include <string>

#include "DateManager.h"

class Requestor : public ActiveTickServerRequestor
{
public:
	Requestor(const APISession& session, DateManager* m_dateManager);
	virtual ~Requestor(void);

private:
	// ActiveTickServerRequestor's virtual overrides
	virtual void		OnATBarHistoryDbResponse(uint64_t origRequest, ATBarHistoryResponseType responseType, LPATBARHISTORY_RESPONSE pResponse);
	virtual void		OnATTickHistoryDbResponse(uint64_t origRequest, ATTickHistoryResponseType responseType, LPATTICKHISTORY_RESPONSE pResponse);

	virtual void		OnATMarketMoversDbResponse(uint64_t origRequest, ATMarketMoversDbResponseType responseType, LPATMARKET_MOVERSDB_RESPONSE pResponse);
	virtual void		OnATQuoteDbResponse(uint64_t origRequest, ATQuoteDbResponseType responseType, LPATQUOTEDB_RESPONSE pResponse, uint32_t responseCount);

	virtual void		OnATMarketMoversStreamResponse(uint64_t origRequest, ATStreamResponseType responseType, LPATMARKET_MOVERS_STREAM_RESPONSE pResponse);
	virtual void		OnATQuoteStreamResponse(uint64_t origRequest, ATStreamResponseType responseType, LPATQUOTESTREAM_RESPONSE pResponse, uint32_t responseCount);

	virtual void		OnATSectorListResponse(uint64_t origRequest, LPATSECTORLIST_RECORD pRecords, uint32_t recordsCount);
	virtual void		OnATConstituentListResponse(uint64_t origRequest, LPATSYMBOL pSymbols, uint32_t symbolsCount);

	virtual void		OnATRequestTimeout(uint64_t origRequest);
private:
	DateManager* m_dateManager;
};
