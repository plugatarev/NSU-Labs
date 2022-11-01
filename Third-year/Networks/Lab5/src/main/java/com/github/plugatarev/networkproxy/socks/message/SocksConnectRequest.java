package com.github.plugatarev.networkproxy.messages;

import lombok.Getter;
import lombok.Setter;

public final class SocksConnectRequest {
    @Getter @Setter private byte version;
    @Getter private byte[] authenticationMethods;

    public void setNumOfMethods(byte numOfMethods) {
        authenticationMethods = new byte[numOfMethods];
    }
}
