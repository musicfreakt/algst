package com.mySampleApplication.client;

import com.google.gwt.user.client.rpc.AsyncCallback;

import java.util.List;

public interface MySampleApplicationServiceAsync {
    void getMessage(String msg, AsyncCallback<String> async);

    void getReaderList(AsyncCallback<List<String>> callback);
    void getBookReaderList(String readerFIO, AsyncCallback<List<Post>> callback);
}