/*
 * Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

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

	void displayMessage(const char *str);
	bool messagePool(void);
	bool messageAck(uint8_t idx) const {return msgAckIdx == idx ? msgAck : false;}
	uint8_t messageIndex(void) {return msgIdx++;}
	void sendMessage(uint8_t idx, const char *str);
	void sendMessageAck(uint8_t idx);

private:
	struct request_t {
		uint8_t req;
		request_t *next;
	} *reqs;

	struct message_t {
		uint8_t idx;
		char str[BUFFER_SIZE - 1];
		message_t *next;
	} *msgs;

	void pushRequest(pkgRequest_t *req);
	request_t *popRequest(void);
	void removeRequests(uint8_t req);

	void pushMessage(pkgMessage_t *msg);
	message_t *popMessage(void);

	uint16_t reqCnt, msgCnt;
	uint8_t reqAck, reqType;
	uint8_t msgIdx, msgAckIdx;
	bool msgAck;
};

#endif
