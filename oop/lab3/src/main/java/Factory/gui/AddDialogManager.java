package Factory.gui;

import javax.swing.*;

/**
 * Класс окна Добавления данных
 */
public class AddDialogManager extends DialogManager {

    @Override
    public void progress(ManagerWindow parent) {
        setVisible(false);
        String[] arr = {name.getText(),surname.getText()};

        // ДОБАВИТЬ НОВОГО ДУРАЧКА
        parent.addR(arr);

        JOptionPane.showMessageDialog(null, "Вы добавили сотрудника \""+arr[0]+" "+arr[1]+"\"");
    }

    @Override
    public void init(ManagerWindow parent)
    {
        name = new JTextField(20);
        surname= new JTextField(20);
    }

    public AddDialogManager(JFrame owner, ManagerWindow parent, String title)
    {
        super(owner,parent,title);
    }
}
