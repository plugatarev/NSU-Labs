package com.github.plugatarev.networkproxy.socks;

import lombok.Getter;
import lombok.Setter;

import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;

public final class SocksRequest {
    @Getter @Setter private byte version;
    @Getter @Setter private byte commandCode;
    @Getter @Setter private String domainName;
    @Getter @Setter private byte parseError = 0x00;
    @Getter private final byte[] ip4Address = new byte[4];
    @Getter @Setter private byte addressType;
    @Getter @Setter private short destinationPort;

    public InetSocketAddress getAddress() throws UnknownHostException {
        return new InetSocketAddress(InetAddress.getByAddress(this.ip4Address), this.destinationPort);
    }
}
