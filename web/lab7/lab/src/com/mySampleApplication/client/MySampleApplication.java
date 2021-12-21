package com.mySampleApplication.client;

import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.core.client.GWT;
import com.google.gwt.event.dom.client.*;
import com.google.gwt.user.cellview.client.CellTable;
import com.google.gwt.user.cellview.client.HasKeyboardSelectionPolicy;
import com.google.gwt.user.cellview.client.TextColumn;
import com.google.gwt.user.client.ui.*;
import com.google.gwt.user.client.rpc.AsyncCallback;

import java.util.List;

/**
 * Entry point classes define <code>onModuleLoad()</code>
 */
public class MySampleApplication implements EntryPoint {
    private static final String SRV_ERR = "Ошибка сервера! ";
    private static final String SRV_ERR_GET_USER_LIST = "Невозможно получить пользователей.";
    private static final String SRV_ERR_GET_POST_LIST = "Невозможно получить посты.";
    private static final String GET_USER_LIST_BTN = "Получить посты.";
    private static final String CLOSE_BTN = "Закрыть";
    private static final String POSTS_WND_TITLE = "Список постов пользователя ";

    private final MySampleApplicationServiceAsync myService = GWT.create(MySampleApplicationService.class);

    /**
     * This is the entry point method.
     */
    public void onModuleLoad() {
        final ListBox readerListBox = new ListBox();
        final MultiWordSuggestOracle oracle = new MultiWordSuggestOracle();
        final Label errorLabel = new Label();
        final Button sendButton = new Button(GET_USER_LIST_BTN);
//sendButton.addStyleName("sendButton");
        RootPanel.get("readerListBoxContainer").add(readerListBox);
        RootPanel.get("errorLabelContainer").add(errorLabel);
        RootPanel.get("sendButtonContainer").add(sendButton);
        readerListBox.setFocus(true);

        myService.getReaderList(new AsyncCallback<List<String>>() {
            public void onSuccess(List<String> result) {
                oracle.clear();
                oracle.addAll(result);
                for (String r : result) {
                    readerListBox.addItem(r);
                }
            }

            public void onFailure(Throwable caught) {
                errorLabel.setText(SRV_ERR + SRV_ERR_GET_USER_LIST);
            }
        });

        final DialogBox dialogBox = new DialogBox();
        dialogBox.setText(POSTS_WND_TITLE);
        dialogBox.setAnimationEnabled(true);
        final Button closeButton = new Button(CLOSE_BTN);
        closeButton.getElement().setId("closeButton");
        final HTML serverResponseLabel = new HTML();
        VerticalPanel dialogVPanel = new VerticalPanel();
//dialogVPanel.addStyleName("dialogVPanel");

        final CellTable<Post> table = createCellTable();
        dialogVPanel.add(table);

        dialogVPanel.setHorizontalAlignment(VerticalPanel.ALIGN_RIGHT);
        dialogVPanel.add(closeButton);
        dialogBox.setWidget(dialogVPanel);
        closeButton.addClickHandler(event -> {
            dialogBox.hide();
            sendButton.setEnabled(true);
            sendButton.setFocus(true);
        });

        class RPCClickHandler implements ClickHandler, KeyUpHandler {
            public void onClick(ClickEvent event) {
                sendReaderFIOToServer();
            }

            public void onKeyUp(KeyUpEvent event) {
                if (event.getNativeKeyCode() == KeyCodes.KEY_ENTER) {
                    sendReaderFIOToServer();
                }
            }

            private void sendReaderFIOToServer() {
                errorLabel.setText("");
                final String readerFIO = readerListBox.getValue(readerListBox.getSelectedIndex());
                sendButton.setEnabled(false);
                myService.getBookReaderList(readerFIO,
                        new AsyncCallback<List<Post>>() {
                            public void onFailure(Throwable caught) {
                                dialogBox.setText(SRV_ERR);
//serverResponseLabel.addStyleName("serverResponseLabelError");
                                serverResponseLabel.setHTML(SRV_ERR + SRV_ERR_GET_POST_LIST);
                                dialogBox.center();
                                closeButton.setFocus(true);
                            }

                            public void onSuccess(List<Post> result) {
                                dialogBox.setText(POSTS_WND_TITLE + readerFIO);
                                table.setRowCount(result.size(), true);
                                table.setRowData(0, result);
                                dialogBox.center();
                                closeButton.setFocus(true);
                            }
                        });
            }
        }
        RPCClickHandler handler = new RPCClickHandler();
        sendButton.addClickHandler(handler);
    }

    private CellTable<Post> createCellTable() {
        final CellTable<Post> table = new CellTable<Post>();
        table.setKeyboardSelectionPolicy(HasKeyboardSelectionPolicy.KeyboardSelectionPolicy.ENABLED);

        TextColumn<Post> textColumn = new TextColumn<Post>() {
            public String getValue(Post object) {
                return object.getText();
            }
        };
        table.addColumn(textColumn, "Текст");

        TextColumn<Post> dateColumn = new TextColumn<Post>() {
            public String getValue(Post object) {
                return object.getDate().toString();
            }
        };
        table.addColumn(dateColumn, "Дата");

        return table;
    }
}
