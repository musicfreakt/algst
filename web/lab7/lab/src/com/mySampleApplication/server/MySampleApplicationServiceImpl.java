package com.mySampleApplication.server;

import com.google.gwt.user.server.rpc.RemoteServiceServlet;
import com.mySampleApplication.client.Post;
import com.mySampleApplication.client.MySampleApplicationService;

import java.util.*;

public class MySampleApplicationServiceImpl extends RemoteServiceServlet implements MySampleApplicationService {
    // Implementation of sample interface method
    public String getMessage(String msg) {
        return "Client said: ";
    }

    private Map<String, List<Post>> db = null;
    public List<String> getReaderList() {
        if( db == null ){
            initDB();
        }
        String[] tmp = new String[db.keySet().size()];
        db.keySet().toArray(tmp);
        return Arrays.asList(tmp);
    }
    public List<Post> getBookReaderList(String readerFIO) {
        if( db == null ){
            initDB();
        }
        return db.get(readerFIO);
    }

    private void initDB(){
        db = new HashMap<String, List<Post>>();

        List<Post> entries1 = new ArrayList<Post>();
        entries1.add(new Post("Hello World"));
        entries1.add(new Post("dfhjsdkfjhsdkjf"));
        entries1.add(new Post("DF,fdfs"));
        db.put("kivyfreakt", entries1);

        List<Post> entries2 = new ArrayList<Post>();
        entries2.add(new Post("Тест лабы"));
        entries2.add(new Post("Привет"));
        db.put("asscar", entries2);
    }
}