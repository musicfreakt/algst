package Factory.gui;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Абстрактный класс Диалогового окна Добавления/Редактирования данных менеджеров
 */
public abstract class DialogSpecialisation extends JDialog {
    protected JTextField name;
    protected Boolean check = false;
    private JButton ok = new JButton("Принять");
    private JButton cancel = new JButton("Закрыть");
    private JLabel nameLab = new JLabel("Название профессии");

    /**
     * Выполнение манипуляций с данными
     *
     * @param parent - Объект класса приложения
     */
    public abstract void progress(SpecialisationWindow parent);

    /**
     * Инициализация
     *
     * @param parent - Объект класса приложения
     */
    public abstract void init(SpecialisationWindow parent);

    /**
     * Основной конструктор
     *
     * @param owner - JFrame приложения
     * @param parent - Объект класса приложения
     * @param title - Title окна
     */
    public DialogSpecialisation(JFrame owner, SpecialisationWindow parent, String title) {
        super(owner, title, true);
        ok.setEnabled(false);
        // Инит кнопок
        init(parent);

        // Создание валидатора полей
        name.getDocument().addDocumentListener(new DocumentListener() {
            @Override
            public void insertUpdate(DocumentEvent e) {
                checker(name);
            }

            @Override
            public void removeUpdate(DocumentEvent e) {
                checker(name);
            }

            @Override
            public void changedUpdate(DocumentEvent e) {

            }
        });

        ok.addActionListener((e) -> progress(parent));
        cancel.addActionListener((e) -> dispose());
        JPanel mainp = new JPanel();

        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(3, 2, 2, 2));

        panel.setSize(300, 100);

        // adds to the GridLayout
        panel.add(nameLab);
        panel.add(name);
        mainp.add(panel);
        add(BorderLayout.CENTER, mainp);
        JPanel but = new JPanel();
        but.add(ok);
        but.add(cancel);
        add(BorderLayout.SOUTH, but);
        setLocation(500, 250);
        setSize(500, 155);
        this.getRootPane().setDefaultButton(ok);
// remove the binding for pressed
        this.getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW)
                .put(KeyStroke.getKeyStroke("ENTER"), "none");
// retarget the binding for released
        this.getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW)
                .put(KeyStroke.getKeyStroke("released ENTER"), "press");
    }

    protected void checker(JTextField field)
    {
        Pattern r = Pattern.compile("^[А-ЯЁ][а-яЁё]{1,10}$");
        Matcher m = r.matcher(field.getText());
        if (m.matches()) {
            field.setBorder(BorderFactory.createLineBorder(Color.GREEN));
            check = true;
        }
        else {
            field.setBorder(BorderFactory.createLineBorder(Color.RED));
            check = false;
        }

        ok.setEnabled(check);
    }
}