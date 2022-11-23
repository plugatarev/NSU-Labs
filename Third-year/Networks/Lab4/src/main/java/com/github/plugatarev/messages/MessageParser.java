package com.github.plugatarev.messages;

import com.github.plugatarev.SnakesProto.GameMessage;
import com.github.plugatarev.SnakesProto.GameMessage.JoinMsg;
import com.github.plugatarev.gamehandler.GameState;
import com.google.protobuf.InvalidProtocolBufferException;
import lombok.experimental.UtilityClass;
import org.apache.commons.lang3.SerializationException;
import org.apache.log4j.Logger;
import com.github.plugatarev.utils.StateUtils;
import com.github.plugatarev.messages.messages.AckMessage;
import com.github.plugatarev.messages.messages.AnnouncementMessage;
import com.github.plugatarev.messages.messages.ErrorMessage;
import com.github.plugatarev.messages.messages.JoinMessage;
import com.github.plugatarev.messages.messages.Message;
import com.github.plugatarev.messages.messages.PingMessage;
import com.github.plugatarev.messages.messages.RoleChangeMessage;
import com.github.plugatarev.messages.messages.StateMessage;
import com.github.plugatarev.messages.messages.SteerMessage;

import java.net.DatagramPacket;
import java.net.InetAddress;
import java.util.Arrays;

@UtilityClass
public final class MessageParser {
    private static final int EMPTY = -1;
    private static final Logger logger = Logger.getLogger(MessageParser.class);

    public static DatagramPacket serializeMessage(Message message, InetAddress dstAddress, int dstPort) {
        byte[] messageBytes = message.getGameMessage().toByteArray();
        return new DatagramPacket(messageBytes, messageBytes.length, dstAddress, dstPort);
    }

    public static Message deserializeMessage(DatagramPacket packet) throws ClassCastException, SerializationException, InvalidProtocolBufferException {
        GameMessage message = GameMessage.parseFrom(Arrays.copyOf(packet.getData(), packet.getLength()));
        validate(message.hasMsgSeq(), "No message sequence");
        if (message.hasAck()) {
            validate(message.hasSenderId(), "No sender id");
            validate(message.hasReceiverId(), "No receiver id");
            return new AckMessage(message.getMsgSeq(), message.getSenderId(), message.getReceiverId());
        }
        else if (message.hasAnnouncement()) {
            return new AnnouncementMessage(message.getAnnouncement().getGamesList(), message.getMsgSeq());
        }
        else if (message.hasError()) {
            validate(message.getError().hasErrorMessage(), "No error message");
            int senderId = message.hasSenderId() ? message.getSenderId() : EMPTY;
            int receivedId = message.hasReceiverId() ? message.getReceiverId() : EMPTY;
            return new ErrorMessage(message.getError().getErrorMessage(), message.getMsgSeq(), senderId, receivedId);
        }
        else if (message.hasJoin()) {
            validate(message.getJoin().hasPlayerName(), "No player name");
            JoinMsg msg = message.getJoin();
            return new JoinMessage(msg.getPlayerType(), msg.getPlayerName(), msg.getGameName(), msg.getRequestedRole(), message.getMsgSeq());
        }
        else if (message.hasPing()) {
            int senderId = message.hasSenderId() ? message.getSenderId() : EMPTY;
            int receivedId = message.hasReceiverId() ? message.getReceiverId() : EMPTY;
            return new PingMessage(message.getMsgSeq(), senderId, receivedId);
        }
        else if (message.hasRoleChange()) {
            validate(message.getRoleChange().hasSenderRole(), "No sender role");
            validate(message.getRoleChange().hasReceiverRole(), "No receiver role");
            validate(message.hasSenderId(), "No sender id");
            validate(message.hasReceiverId(), "No receiver id");

            int senderId = message.hasSenderId() ? message.getSenderId() : EMPTY;
            int receivedId = message.hasReceiverId() ? message.getReceiverId() : EMPTY;
            return new RoleChangeMessage(
                    message.getRoleChange().getSenderRole(),
                    message.getRoleChange().getReceiverRole(),
                    message.getMsgSeq(),
                    senderId,
                    receivedId);
        }
        else if (message.hasState()) {
            validate(message.getState().hasState(), "No state");

            GameState state = StateUtils.getStateFromMessage(message.getState().getState());
            validate(state != null, "Couldn't parse state from message");

            return new StateMessage(
                    state,
                    message.getMsgSeq(),
                    message.hasSenderId() ? message.getSenderId() : EMPTY,
                    message.hasReceiverId() ? message.getReceiverId() : EMPTY
            );
        }
        else if (message.hasSteer()) {
            validate(message.getSteer().hasDirection(), "No direction");
            return new SteerMessage(
                    message.getSteer().getDirection(),
                    message.getMsgSeq(),
                    message.hasSenderId() ? message.getSenderId() : EMPTY,
                    message.hasReceiverId() ? message.getReceiverId() : EMPTY
            );
        }
        else {
            logger.error("Can't deserialize message: No message");
            throw new SerializationException("No message");
        }
    }

    private void validate(boolean value, String errorMessage) {
        if (!value) {
            logger.error("Can't deserialize message: " + errorMessage);
            throw new SerializationException(errorMessage);
        }
    }
}
