package Factory.gui;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Класс Диалогового окна добавления контрактов рабочему
 */
public class AddDialogWorkerContract extends JDialog
{
    protected JTextField name;
    protected boolean check;
    private JButton ok;
    private JButton cancel;

    /**
     * Выполнение манипуляций с данными
     *
     * @param parent - Объект класса приложения
     */
    public void progress(WorkerContractWindow parent)
    {
        setVisible(false);
        try
        {
            parent.addR(Integer.parseInt(name.getText()));
            JOptionPane.showMessageDialog(null, "Вы договор для выполения рабонику");
        }
        catch (Exception ex)
        {
            JOptionPane.showMessageDialog(null, "Ошибка" + ex.toString());

        }
    };

    /**
     * Инициализация
     *
     * @param parent - Объект класса приложения
     */
    public void init(WorkerContractWindow parent)
    {
        ok = new JButton("Принять");
        cancel = new JButton("Закрыть");
        name = new JTextField(20);
    };

    /**
     * Основной конструктор
     *
     * @param owner - JFrame приложения
     * @param parent - Объект класса приложения
     * @param title - Title окна
     */
    public AddDialogWorkerContract(JFrame owner, WorkerContractWindow parent, String title)
    {
        super(owner, title, true);
        // Инит кнопок
        init(parent);
        check = false;
        ok.setEnabled(false);


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
        panel.add(new Label("ID контракта"));
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
        this.getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW)
                .put(KeyStroke.getKeyStroke("ENTER"), "none");
        this.getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW)
                .put(KeyStroke.getKeyStroke("released ENTER"), "press");
    }

    /**
     * Проверка поля на корректность введенных данных
     * @param field - проверяемое поле
     */
    protected void checker(JTextField field)
    {
        Pattern r = Pattern.compile("^[+-]?(([1-9][0-9]*)|(0))$");
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