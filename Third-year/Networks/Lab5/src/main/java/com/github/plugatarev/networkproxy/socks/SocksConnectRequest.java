package com.github.plugatarev.networkproxy.socks;

import lombok.Getter;
import lombok.Setter;

public final class SocksConnectRequest {
    @Getter @Setter private byte version;
    @Getter private byte[] methods;

    public void setNumOfMethods(byte numOfMethods) {
        this.methods = new byte[numOfMethods];
    }
}
