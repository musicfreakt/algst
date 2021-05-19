package Factory.gui;

import javax.swing.*;

/**
 * Класс окна Добавления данных
 */
public class AddDialogWorker extends DialogWorker
{
    @Override
    public void progress(WorkerWindow parent) {
        setVisible(false);
        String[] arr = {name.getText(), surname.getText(), exp.getText(), (String) specs.getSelectedItem()};

        parent.addR(arr);

        JOptionPane.showMessageDialog(null, "Вы добавили сотрудника \""+arr[0]+" "+arr[1]+"\"");
    }

    @Override
    public void init(WorkerWindow parent)
    {
        name = new JTextField(20);
        surname = new JTextField(20);
        exp = new JTextField(20);
        specs = new JComboBox(parent.getSpecs());
    }

    public AddDialogWorker(JFrame owner, WorkerWindow parent, String title)
    {
        super(owner, parent,title);
    }
}
