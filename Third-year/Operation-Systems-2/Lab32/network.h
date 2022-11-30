#ifndef LAB32_NETWORK_H
#define LAB32_NETWORK_H

#include "header.h"
#include "connection.h"

int getProxySocket(int port);
int getServerSocket(char* url);

void wrongMethod(connection* conn);
void cannotResolve(connection* conn);

#endif
