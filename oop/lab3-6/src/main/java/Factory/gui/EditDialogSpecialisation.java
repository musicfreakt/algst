package Factory.gui;

import javax.swing.*;

/**
 * Класс окна Изменения данных
 */
public class EditDialogSpecialisation extends DialogSpecialisation
{
    @Override
    public void progress(SpecialisationWindow parent)
    {
        setVisible(false);
        int row = parent.dataSpecialisations.getSelectedRow();
        String[] arr = {parent.dataSpecialisations.getValueAt(row, 0).toString(), name.getText()};
        parent.dataSpecialisations.setValueAt(name.getText(), row, 1);
        parent.editR(arr);
    }

    @Override
    public void init(SpecialisationWindow parent)
    {
        int row = parent.dataSpecialisations.getSelectedRow();
        name = new JTextField(parent.dataSpecialisations.getValueAt(row, 1).toString(), 20);
        checker(name);
    }

    public EditDialogSpecialisation(JFrame owner, SpecialisationWindow parent, String title)
    {
        super(owner, parent, title);
    };
}