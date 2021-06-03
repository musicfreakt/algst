package Factory.gui;

import javax.swing.*;

/**
 * Класс окна изменения данных менеджера
 */
public class EditDialogManager extends DialogManager
{
    @Override
    public void progress(ManagerWindow parent)
    {
        setVisible(false);
        int row = parent.dataManagers.getSelectedRow();
        String[] arr = {parent.dataManagers.getValueAt(row, 0).toString(), name.getText(), surname.getText()};
        parent.dataManagers.setValueAt(name.getText(), row, 1);
        parent.dataManagers.setValueAt(surname.getText(), row, 2);
        parent.editR(arr);
    }

    @Override
    public void init(ManagerWindow parent)
    {
        int row = parent.dataManagers.getSelectedRow();
        name = new JTextField(parent.dataManagers.getValueAt(row, 1).toString(), 20);
        surname = new JTextField(parent.dataManagers.getValueAt(row, 2).toString(), 20);
        checker(0,name);
        checker(1,surname);
    }

    /** Конструктор окна изменения данных менеджера
     *
     * @param owner - JFrame приложения
     * @param parent - объект класса приложения
     * @param title - название окна
     */
    public EditDialogManager(JFrame owner, ManagerWindow parent, String title)
    {
        super(owner, parent, title);
    };
}