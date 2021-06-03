package Factory.gui;

import javax.swing.*;

/**
 * Класс окна добавления данных менеджера
 */
public class AddDialogManager extends DialogManager {

    @Override
    public void progress(ManagerWindow parent) {
        setVisible(false);
        String[] arr = {name.getText(),surname.getText()};

        parent.addR(arr);

        JOptionPane.showMessageDialog(null, "Вы добавили сотрудника \""+arr[0]+" "+arr[1]+"\"");
    }

    @Override
    public void init(ManagerWindow parent)
    {
        name = new JTextField(20);
        surname= new JTextField(20);
    }

    /** Конструктор окна добавления данных менеджера
     *
     * @param owner - JFrame приложения
     * @param parent - объект класса приложения
     * @param title - название окна
     */
    public AddDialogManager(JFrame owner, ManagerWindow parent, String title)
    {
        super(owner,parent,title);
    }
}
