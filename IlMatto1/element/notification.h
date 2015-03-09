#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <communication.h>
#include <rtouch.h>
#include "package.h"

class notification_t
{
public:
	void init(void);
	package_t *pool(package_t *pkg);
	bool show(void);
	void displayRequest(uint8_t req);
	uint8_t requestPool(void);
	uint8_t requestAck(uint8_t req) const {return req == reqType ? reqAck : PKG_REQUEST_INVALID;}

	void sendRequest(uint8_t req);
	void sendRequestAck(uint8_t req, uint8_t ack);

private:
	struct request_t {
		uint8_t req;
		request_t *next;
	} *reqs;

	void pushRequest(pkgRequest_t *req);
	request_t *popRequest(void);
	void removeRequests(uint8_t req);

	uint16_t reqSize;
	uint8_t reqAck, reqType;
};

#endif
