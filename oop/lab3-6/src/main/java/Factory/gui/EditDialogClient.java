package Factory.gui;

import javax.swing.*;

/**
 * Класс окна изменения данных клиентов
 */
public class EditDialogClient extends DialogClient
{
    @Override
    public void progress(ClientWindow parent)
    {
        setVisible(false);
        int row = parent.dataClients.getSelectedRow();
        String[] arr = {parent.dataClients.getValueAt(row, 0).toString(), name.getText(), surname.getText(), company.getText()};
        parent.dataClients.setValueAt(name.getText(), row, 1);
        parent.dataClients.setValueAt(surname.getText(), row, 2);
        parent.dataClients.setValueAt(company.getText(), row, 3);
        parent.editR(arr);
    }

    @Override
    public void init(ClientWindow parent)
    {
        int row = parent.dataClients.getSelectedRow();
        name = new JTextField(parent.dataClients.getValueAt(row, 1).toString(), 20);
        surname = new JTextField(parent.dataClients.getValueAt(row, 2).toString(), 20);
        company = new JTextField(parent.dataClients.getValueAt(row, 3).toString(), 20);
        checker(0,name);
        checker(1,surname);
        checker(2,company);
    }

    /** Конструктор окна изменения данных клиента
     *
     * @param owner - JFrame приложения
     * @param parent - объект класса приложения
     * @param title - название окна
     */
    public EditDialogClient(JFrame owner, ClientWindow parent, String title)
    {
        super(owner, parent, title);
    };
}
