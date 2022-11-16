package com.github.plugatarev.messages;

import com.github.plugatarev.datatransfer.NetNode;
import com.github.plugatarev.messages.messages.RoleChangeMessage;
import com.github.plugatarev.messages.messages.ErrorMessage;
import com.github.plugatarev.messages.messages.JoinMessage;
import com.github.plugatarev.messages.messages.PingMessage;
import com.github.plugatarev.messages.messages.StateMessage;
import com.github.plugatarev.messages.messages.SteerMessage;

public interface MessageHandler {
    void handle(NetNode sender, SteerMessage message);
    void handle(NetNode sender, JoinMessage message);
    void handle(NetNode sender, PingMessage message);
    void handle(NetNode sender, StateMessage message);
    void handle(NetNode sender, ErrorMessage message);
    void handle(NetNode sender, RoleChangeMessage message);
}
