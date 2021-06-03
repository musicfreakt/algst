package Factory.gui;

import javax.swing.*;

/**
 * Класс окна добавления данных профессии
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

    /** Конструктор окна добавления данных профессии
     *
     * @param owner - JFrame приложения
     * @param parent - объект класса приложения
     * @param title - название окна
     */
    public AddDialogSpecialisation(JFrame owner, SpecialisationWindow parent, String title)
    {
        super(owner,parent,title);
    }
}
