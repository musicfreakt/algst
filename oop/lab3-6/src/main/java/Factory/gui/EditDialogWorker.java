package Factory.gui;

import javax.swing.*;

/**
 * Класс окна Изменения данных
 */
public class EditDialogWorker extends DialogWorker
{
    @Override
    public void progress(WorkerWindow parent)
    {
        setVisible(false);
        int row = parent.dataWorkers.getSelectedRow();
        String[] arr = {parent.dataWorkers.getValueAt(row, 0).toString(), name.getText(), surname.getText(), exp.getText(), (String) specs.getSelectedItem()};
        parent.dataWorkers.setValueAt(name.getText(), row, 1);
        parent.dataWorkers.setValueAt(surname.getText(), row, 2);
        parent.dataWorkers.setValueAt(exp.getText(), row, 3);
        parent.dataWorkers.setValueAt(specs.getSelectedItem(), row, 4);
        parent.editR(arr);
    }

    @Override
    public void init(WorkerWindow parent)
    {
        int row = parent.dataWorkers.getSelectedRow();
        name = new JTextField(parent.dataWorkers.getValueAt(row, 1).toString(), 20);
        surname = new JTextField(parent.dataWorkers.getValueAt(row, 2).toString(), 20);
        exp = new JTextField(parent.dataWorkers.getValueAt(row, 3).toString(), 20);
        specs = new JComboBox(parent.getSpecs());
        checker(0,name);
        checker(1,surname);
        checkerInt(2, exp);
    }

    public EditDialogWorker(JFrame owner, WorkerWindow parent, String title)
    {
        super(owner, parent, title);
    };
}