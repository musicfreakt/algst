package Factory.gui;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Абстрактный класс Диалогового окна Добавления/Редактирования данных сотрудников
 */
public abstract class DialogWorker extends JDialog {
    protected JTextField name;
    protected JTextField surname;
    protected JTextField exp;
    protected JComboBox specs;
    protected Boolean[] check = {false,false,false};
    private JButton ok = new JButton("Принять");
    private JButton cancel = new JButton("Закрыть");
    private JLabel nameLab = new JLabel("Имя");
    private JLabel surnameLab = new JLabel("Фамилия");
    private JLabel expLab = new JLabel("Опыт работы");
    private JLabel specialisationLab = new JLabel("Специализация");

    /**
     * Выполнение манипуляций с данными
     *
     * @param parent - Объект класса приложения
     */
    public abstract void progress(WorkerWindow parent);

    /**
     * Инициализация
     *
     * @param parent - Объект класса приложения
     */
    public abstract void init(WorkerWindow parent);

    /**
     * Основной конструктор
     *
     * @param owner - JFrame приложения
     * @param parent - Объект класса приложения
     * @param title - Title окна
     */
    public DialogWorker(JFrame owner, WorkerWindow parent, String title) {
        super(owner, title, true);
        ok.setEnabled(false);
        // Инит кнопок
        init(parent);

        // Создание валидатора полей
        name.getDocument().addDocumentListener(new DocumentListener() {
            @Override
            public void insertUpdate(DocumentEvent e) {
                checker(0, name);
            }

            @Override
            public void removeUpdate(DocumentEvent e) {
                checker(0, name);
            }

            @Override
            public void changedUpdate(DocumentEvent e) {

            }
        });

        surname.getDocument().addDocumentListener(new DocumentListener() {
            @Override
            public void insertUpdate(DocumentEvent e) {
                checker(1,surname);
            }

            @Override
            public void removeUpdate(DocumentEvent e) {
                checker(1,surname);
            }

            @Override
            public void changedUpdate(DocumentEvent e) {

            }
        });

        exp.getDocument().addDocumentListener(new DocumentListener() {
            @Override
            public void insertUpdate(DocumentEvent e) {
                checker(2,exp);
            }

            @Override
            public void removeUpdate(DocumentEvent e) {
                checker(2,exp);
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
        panel.add(surnameLab);
        panel.add(surname);
        panel.add(expLab);
        panel.add(exp);
        panel.add(specialisationLab);
        panel.add(specs);
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

    protected void checker(int i, JTextField field){
        Pattern r = Pattern.compile("^[А-ЯЁ][а-яЁё]{1,10}$");
        Matcher m = r.matcher(field.getText());
        if (m.matches()) {
            field.setBorder(BorderFactory.createLineBorder(Color.GREEN));
            check[i] = true;
        }
        else {
            field.setBorder(BorderFactory.createLineBorder(Color.RED));
            check[i] = false;
        }
        if(check[0] && check[1] && check[2])
            ok.setEnabled(true);
        else
            ok.setEnabled(false);
    }
}