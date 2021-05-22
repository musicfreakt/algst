package Factory.gui;

import javax.swing.*;

/**
 * Класс окна Изменения данных
 */
public class EditDialogContract extends DialogContract
{
    @Override
    public void progress(ContractWindow parent)
    {
        setVisible(false);
        int row = parent.dataContracts.getSelectedRow();
        String[] arr = {parent.dataContracts.getValueAt(row, 0).toString(),
                description.getText(),
                price.getText(),
                (String) clients.getSelectedItem(),
                (String) managers.getSelectedItem()
        };
        parent.dataContracts.setValueAt(description.getText(), row, 1);
        parent.dataContracts.setValueAt(price.getText(), row, 2);
        parent.dataContracts.setValueAt(clients.getSelectedItem(), row, 3);
        parent.dataContracts.setValueAt(managers.getSelectedItem(), row, 4);

        parent.editR(arr);
    }

    @Override
    public void init(ContractWindow parent)
    {
        int row = parent.dataContracts.getSelectedRow();
        description = new JTextField(parent.dataContracts.getValueAt(row, 1).toString(), 20);
        price = new JTextField(parent.dataContracts.getValueAt(row, 2).toString(), 20);
        checkerFloat(0, price);
    }

    public EditDialogContract(JFrame owner, ContractWindow parent, String title)
    {
        super(owner, parent, title);
    };
}
