package Factory.gui;

import javax.swing.*;

/**
 * Класс окна Добавления данных
 */
public class AddDialogSpecialisation extends DialogSpecialisation {

    @Override
    public void progress(SpecialisationWindow parent) {
        setVisible(false);

        parent.addR(name.getText());

        JOptionPane.showMessageDialog(null, "Вы добавили профессию \""+name.getText()+"\"");
    }

    @Override
    public void init(SpecialisationWindow parent)
    {
        name = new JTextField(20);
    }

    public AddDialogSpecialisation(JFrame owner, SpecialisationWindow parent, String title)
    {
        super(owner,parent,title);
    }
}
