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
 * Класс Диалогового окна получения временного периода для отображения данных договоров
 */
public class DialogTimePeriodSelection extends JDialog {

    /** Кнопка принять */
    private JButton ok = new JButton("Принять");

    /** Кнопка отменить */
    private JButton cancel = new JButton("Закрыть");

    /** Дата пикер начала временного отрезка */
    protected JDatePickerImpl dataBegin;

    /** Дата пикер конца временного отрезка */
    protected JDatePickerImpl dataEnd;

    /**
     * Выполнение манипуляций с данными
     *
     * @param parent - Объект класса приложения
     */
    public void progress(ContractWindow parent)
    {
        setVisible(false);
        parent.setDate((Date) dataBegin.getModel().getValue(), (Date) dataEnd.getModel().getValue());
    }
    public void progress(WorkerContractWindow parent)
    {
        setVisible(false);
        parent.setDate((Date) dataBegin.getModel().getValue(), (Date) dataEnd.getModel().getValue());
    }

    /**
     * Инициализация
     */
    public void init()
    {
        UtilDateModel model1 = new UtilDateModel();
        UtilDateModel model2 = new UtilDateModel();
        Properties p = new Properties();
        p.put("text.today", "Today");
        p.put("text.month", "Month");
        p.put("text.year", "Year");
        JDatePanelImpl datePanel1 = new JDatePanelImpl(model1, p);
        JDatePanelImpl datePanel2 = new JDatePanelImpl(model2, p);
        dataBegin = new JDatePickerImpl(datePanel1, new DateLabelFormatter());
        dataEnd = new JDatePickerImpl(datePanel2, new DateLabelFormatter());
    }

    /**
     * Показать интерфейс
     */
    public void display()
    {
        JPanel mainp = new JPanel();
        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(2, 2, 2, 2));

        panel.setSize(300, 100);

        // adds to the GridLayout
        panel.add(new Label("От"));
        panel.add(dataBegin);
        panel.add(new Label("До"));
        panel.add(dataEnd);
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

    /**
     * Основной конструктор
     *
     * @param owner - JFrame приложения
     * @param parent - Объект класса приложения
     * @param title - Title окна
     */
    public DialogTimePeriodSelection(JFrame owner, ContractWindow parent, String title)
    {
        super(owner, title, true);
        // Инит кнопок
        init();
        ok.addActionListener((e) -> progress(parent));
        cancel.addActionListener((e) -> dispose());
        display();
    }
    public DialogTimePeriodSelection(JFrame owner, WorkerContractWindow parent, String title)
    {
        super(owner, title, true);
        // Инит кнопок
        init();
        ok.addActionListener((e) -> progress(parent));
        cancel.addActionListener((e) -> dispose());
        display();
    }

}