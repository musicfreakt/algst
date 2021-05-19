package Factory.gui;

import javax.swing.*;

/**
 * Класс окна Добавления данных
 */
public class AddDialogClient extends DialogClient {

    @Override
    public void progress(ClientWindow parent) {
        setVisible(false);
        String[] arr = {name.getText(),surname.getText(),company.getText()};

        parent.addR(arr);

        JOptionPane.showMessageDialog(null, "Вы добавили клиента \""+arr[0]+" "+arr[1]+"\"");
    }

    @Override
    public void init(ClientWindow parent)
    {
        name = new JTextField(20);
        surname = new JTextField(20);
        company = new JTextField(20);
    }

    public AddDialogClient(JFrame owner, ClientWindow parent, String title)
    {
        super(owner,parent,title);
    }
}
