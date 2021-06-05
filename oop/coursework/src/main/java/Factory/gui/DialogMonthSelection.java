package Factory.gui;

import Factory.util.DateLabelFormatter;
import org.jdatepicker.impl.JDatePanelImpl;
import org.jdatepicker.impl.JDatePickerImpl;
import org.jdatepicker.impl.UtilDateModel;

import javax.swing.*;
import java.awt.*;
import java.util.Date;
import java.util.Properties;

/**
 * Класс диалогового окна получения месяца для отчета по заводу
 */
public class DialogMonthSelection extends JDialog {

    /** Кнопка принять */
    private JButton ok = new JButton("Принять");

    /** Кнопка отменить */
    private JButton cancel = new JButton("Закрыть");

    /** Дата пикер месяца */
    protected JDatePickerImpl month;

    /**
     * Выполнение манипуляций с данными
     *
     * @param parent - Объект класса приложения
     */
    public void progress(ContractWindow parent)
    {
        setVisible(false);
        parent.setMonth((Date) month.getModel().getValue());
    }

    /**
     * Инициализация
     */
    public void init()
    {
        UtilDateModel model = new UtilDateModel();
        Properties p = new Properties();
        p.put("text.today", "Today");
        p.put("text.month", "Month");
        p.put("text.year", "Year");
        JDatePanelImpl datePanel = new JDatePanelImpl(model, p);
        month = new JDatePickerImpl(datePanel, new DateLabelFormatter());
    }

    /**
     * Основной конструктор
     *
     * @param owner - JFrame приложения
     * @param parent - Объект класса приложения
     * @param title - Title окна
     */
    public DialogMonthSelection(JFrame owner, ContractWindow parent, String title)
    {
        super(owner, title, true);
        // Инит кнопок
        init();
        ok.addActionListener((e) -> progress(parent));
        cancel.addActionListener((e) -> dispose());

        JPanel mainp = new JPanel();
        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(1, 2, 2, 2));

        panel.setSize(300, 100);

        // adds to the GridLayout
        panel.add(new Label("Выберите месяц (день не важен)"));
        panel.add(month);
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
}