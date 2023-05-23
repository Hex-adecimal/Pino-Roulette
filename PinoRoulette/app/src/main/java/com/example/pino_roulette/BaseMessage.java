package com.example.pino_roulette;

public class BaseMessage {
    String message;
    String sender;
    String createdAt;

    public BaseMessage() {

    }
    public String getSender() { return sender; }
    public String getMessage() { return message; }
    public String getCreatedAt() { return createdAt; }

    BaseMessage(String message, String sender, String createdAt) {
        this.message = message;
        this.sender = sender;
        this.createdAt = createdAt;
    }
}
