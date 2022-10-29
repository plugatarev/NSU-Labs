package com.github.plugatarev.networkproxy.socks;

import lombok.Setter;

public final class SocksConnectResponse {
    private static final byte VERSION = 0x05;

    @Setter private byte method = 0x00;

    public byte[] toByteArray() {
        return new byte[] { VERSION, this.method };
    }
}