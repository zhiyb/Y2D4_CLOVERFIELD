/*
 * Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#include "notification.h"
#include "common.h"

#define TEXT_ZOOM	2
#define TEXT_X		8
#define TEXT_Y		(tft.height() / 4)

#define BUTTON_X	0
#define BUTTON_Y	(tft.height() * 3 / 4)
#define BUTTON_W	(tft.width() / 2)
#define BUTTON_H	(tft.height() - BUTTON_Y)
#define BUTTON_A_CLR	Green
#define BUTTON_R_CLR	Red

#define BUTTON_T_X(l)	((BUTTON_W - FONT_WIDTH * TEXT_ZOOM * l) / 2)
#define BUTTON_T_Y	((BUTTON_H - FONT_HEIGHT * TEXT_ZOOM) / 2)

namespace name
{
	const char PROGMEM sketch[] = "Shared sketch";
	const char PROGMEM audio[] = "Audio";
}

const char *requestName[PKG_REQUEST_COUNT] = {name::sketch, name::audio};

void notification_t::init(void)
{
	reqSize = 0;
}

void notification_t::pushRequest(pkgRequest_t *req)
{
	request_t **ptr = &reqs;
	while (*ptr) {
		if ((*ptr)->req == req->s.req)
			return;
		ptr = &(*ptr)->next;
	}
	request_t *p = new request_t;
	p->req = req->s.req;
	p->next = 0;
	*ptr = p;
	reqSize++;
}

notification_t::request_t *notification_t::popRequest(void)
{
	if (!reqs)
		return 0;
	request_t *req = reqs;
	reqs = reqs->next;
	reqSize--;
	return req;
}

void notification_t::removeRequests(uint8_t req)
{
	request_t **ptr = &reqs;
	while (*ptr) {
		if ((*ptr)->req == req) {
			request_t *p = *ptr;
			*ptr = p->next;
			delete p;
			reqSize--;
		} else
			ptr = &(*ptr)->next;
	}
}

package_t *notification_t::pool(package_t *pkg)
{
	reqAck = PKG_REQUEST_INVALID;
	if (!pkg || (pkg->command != COM_W_RECV && pkg->command != COM_W_SEND))
		return pkg;
	if (pkg->data[0] == PKG_TYPE_REQUEST) {
		pkgRequest_t *req = (pkgRequest_t *)pkg->data;
		uint8_t r = req->s.req;
		pushRequest(req);
		uart0_done(pkg);
		notification.sendRequestAck(r, PKG_REQUEST_RECEIVED);
		return 0;
	} else if (pkg->data[0] == PKG_TYPE_REQUEST_ACK) {
		pkgRequestAck_t *ack = (pkgRequestAck_t *)pkg->data;
		if (ack->s.ack == PKG_REQUEST_CLOSED)
			removeRequests(ack->s.req);
		reqType = ack->s.req;
		reqAck = ack->s.ack;
		uart0_done(pkg);
		return 0;
	}
	return pkg;
}

bool notification_t::show(void)
{
	request_t *req = popRequest();
	if (!req)
		return false;
	pool::request(req->req);
	delete req;
	status.request.refresh = true;
	return true;
}

void notification_t::sendRequest(uint8_t req)
{
	package_t *pkg;
	while (!(pkg = uart0_txPackage()));
	pkg->command = COM_W_SEND;
	pkg->length = 2;
	pkgRequest_t *request = (pkgRequest_t *)pkg->data;
	request->s.type = PKG_TYPE_REQUEST;
	request->s.req = req;
	pkg->valid++;
	uart0_send();
}

void notification_t::sendRequestAck(uint8_t req, uint8_t ack)
{
	package_t *pkg;
	while (!(pkg = uart0_txPackage()));
	pkg->command = COM_W_SEND;
	pkg->length = 3;
	pkgRequestAck_t *request = (pkgRequestAck_t *)pkg->data;
	request->s.type = PKG_TYPE_REQUEST_ACK;
	request->s.req = req;
	request->s.ack = ack;
	pkg->valid++;
	uart0_send();
}

void notification_t::displayRequest(uint8_t req)
{
	tft.setBackground(Black);
	tft.setForeground(0x667F);
	tft.clean();
	tft.setXY(TEXT_X, TEXT_Y);
	tft.setZoom(TEXT_ZOOM);
	tft.putString(PSTR("Opponent request:\n"), true);
	tft.setX(TEXT_X);
	tft.putString(requestName[req], true);

	tft.setForeground(Black);
	tft.setBackground(BUTTON_A_CLR);
	tft.rectangle(BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H, BUTTON_A_CLR);
	tft.setXY(BUTTON_X + BUTTON_T_X(6), BUTTON_Y + BUTTON_T_Y);
	tft.putString(PSTR("Accept"), true);
	tft.setBackground(BUTTON_R_CLR);
	tft.rectangle(BUTTON_X + BUTTON_W, BUTTON_Y, BUTTON_W, BUTTON_H, BUTTON_R_CLR);
	tft.setXY(BUTTON_X + BUTTON_W + BUTTON_T_X(6), BUTTON_Y + BUTTON_T_Y);
	tft.putString(PSTR("Reject"), true);
}

uint8_t notification_t::requestPool(void)
{
	if (!touch.pressed())
		return PKG_REQUEST_RECEIVED;
	const rTouch::coord_t pos = touch.position();
	if (pos.y < (int16_t)BUTTON_Y || pos.x < (int16_t)BUTTON_X)
		return PKG_REQUEST_RECEIVED;
	return pos.x - BUTTON_X > (int16_t)BUTTON_W ? PKG_REQUEST_REJECT : PKG_REQUEST_ACCEPT;
}
