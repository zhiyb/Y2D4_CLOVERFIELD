/*
 *  Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#ifndef STATUS_H
#define STATUS_H

#include <communication.h>

class status_t
{
public:
	void init(void);

	void checkIlMatto2(void);
	void checkRemote(void);
	package_t *pool(package_t *pkg);

	struct {
		bool IlMatto2;
		bool IlMatto2Updated;
		bool remote;
		bool remoteUpdated;
	} exist;

private:
	bool pingChk;
	void ping(uint8_t cmd = COM_PING);
	void pingCheck(void);
	void pingRemoteCheck(void);
};

#endif
