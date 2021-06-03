package Factory.gui;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import org.jdatepicker.impl.*;
import java.awt.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Абстрактный класс диалогового окна Добавления/Редактирования данных менеджеров
 */
public abstract class DialogContract extends JDialog
{
    protected JTextField description;
    protected JTextField price;
    protected JComboBox clients;
    protected JComboBox managers;
    protected JDatePickerImpl dataBegin;
    protected JDatePickerImpl dataEnd;

    protected Boolean[] check = {false};
    private JButton ok = new JButton("Принять");
    private JButton cancel = new JButton("Закрыть");

    /**
     * Выполнение манипуляций с данными
     *
     * @param parent - Объект класса приложения
     */
    public abstract void progress(ContractWindow parent);

    /**
     * Инициализация
     *
     * @param parent - Объект класса приложения
     */
    public abstract void init(ContractWindow parent);

    /**
     * Основной конструктор
     *
     * @param owner - JFrame приложения
     * @param parent - Объект класса приложения
     * @param title - Title окна
     */
    public DialogContract(JFrame owner, ContractWindow parent, String title)
    {
        super(owner, title, true);

        JLabel descriptionLab = new JLabel("Описание");
        JLabel priceLab = new JLabel("Цена");
        JLabel clientLab = new JLabel("Клиент");
        JLabel managerLab = new JLabel("Менеджер");
        JLabel beginLab = new JLabel("Дата подписания договора");
        JLabel endLab = new JLabel("Дата окончания работ");

        ok.setEnabled(false);
        // Инит кнопок
        init(parent);


        price.getDocument().addDocumentListener(new DocumentListener() {
            @Override
            public void insertUpdate(DocumentEvent e) {
                checkerFloat(0,price);
            }

            @Override
            public void removeUpdate(DocumentEvent e) {
                checkerFloat(0,price);
            }

            @Override
            public void changedUpdate(DocumentEvent e) {

            }
        });

        ok.addActionListener((e) -> progress(parent));
        cancel.addActionListener((e) -> dispose());
        JPanel mainp = new JPanel();

        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(6, 2, 2, 2));

        panel.setSize(300, 300);

        // adds to the GridLayout
        panel.add(descriptionLab);
        panel.add(description);
        panel.add(priceLab);
        panel.add(price);
        panel.add(clientLab);
        panel.add(clients);
        panel.add(managerLab);
        panel.add(managers);
        panel.add(beginLab);
        panel.add(dataBegin);
        panel.add(endLab);
        panel.add(dataEnd);

        mainp.add(panel);
        add(BorderLayout.CENTER, mainp);
        JPanel but = new JPanel();
        but.add(ok);
        but.add(cancel);
        add(BorderLayout.SOUTH, but);
        setLocation(500, 250);
        setSize(500, 250);
        this.getRootPane().setDefaultButton(ok);
// remove the binding for pressed
        this.getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW)
                .put(KeyStroke.getKeyStroke("ENTER"), "none");
// retarget the binding for released
        this.getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW)
                .put(KeyStroke.getKeyStroke("released ENTER"), "press");
    }

    /**
     * Проверка поля на корректность введенных данных
     *
     * @param i - номер поля
     * @param field - проверяемое поле
     */
    protected void checkerFloat(int i, JTextField field)
    {
        Pattern r = Pattern.compile("^[+-]?(([1-9][0-9]*)|(0))([.,][0-9]+)?$");
        Matcher m = r.matcher(field.getText());
        if (m.matches()) {
            field.setBorder(BorderFactory.createLineBorder(Color.GREEN));
            check[i] = true;
        }
        else {
            field.setBorder(BorderFactory.createLineBorder(Color.RED));
            check[i] = false;
        }

        ok.setEnabled(check[0]);
    }


}