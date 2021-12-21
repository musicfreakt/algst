package com.mySampleApplication.client;

import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

import java.util.List;

@RemoteServiceRelativePath("MySampleApplicationService")
public interface MySampleApplicationService extends RemoteService {
        // Sample interface method of remote interface
        String getMessage(String msg);

        List<String> getReaderList();
        List<Post> getBookReaderList(String readerFIO);

        /**
         * Utility/Convenience class.
         * Use MySampleApplicationService.App.getInstance() to access static instance of MySampleApplicationServiceAsync
         /
         /
         public static class App {
         private static MySampleApplicationServiceAsync ourInstance = GWT.create(MySampleApplicationService.class);

         public static synchronized MySampleApplicationServiceAsync getInstance() {
         return ourInstance;
         }
         }*/
}