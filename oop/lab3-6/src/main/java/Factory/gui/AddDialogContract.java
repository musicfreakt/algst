package Factory.gui;

import javax.swing.*;

/**
 * Класс окна Добавления данных
 */
public class AddDialogContract extends DialogContract {

    @Override
    public void progress(ContractWindow parent) {
        setVisible(false);
        String[] arr = {description.getText(),
                        price.getText(),
                        (String) clients.getSelectedItem(),
                        (String) managers.getSelectedItem()};
        parent.addR(arr);

        JOptionPane.showMessageDialog(null, "Вы добавили новый договор");
    }

    @Override
    public void init(ContractWindow parent)
    {
        description = new JTextField(20);
        price = new JTextField(20);
        clients = new JComboBox(parent.getClients());
        managers = new JComboBox(parent.getManagers());
    }

    public AddDialogContract(JFrame owner, ContractWindow parent, String title)
    {
        super(owner,parent,title);
    }
}


