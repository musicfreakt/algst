package com.mySampleApplication.client;

import java.io.Serializable;
import java.util.Date;

public class Post implements Serializable {
    private static final long serialVersionUID = 1L;
    private String text;
    private Date datetime;

    public Post() {
    }

    public Post(String text) {
        this.text = text;
        this.datetime = new Date();
    }

    public String getText() {
        return text;
    }

    public Date getDate() {
        return datetime;
    }

}