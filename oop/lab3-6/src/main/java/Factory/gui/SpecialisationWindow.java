package Factory.gui;

import Factory.model.*;
import Factory.service.*;

import javax.swing.*;
import javax.swing.table.*;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.util.List;
import java.util.logging.*;

public class SpecialisationWindow
{
    SpecialisationWindow()
    {
        show();
    }

    /** Окно приложения */
    private JFrame window;

    /** Модель таблицы */
    private DefaultTableModel model;

    /** Добавить */
    private JButton add;

    /** Удалить */
    private JButton delete;

    /** Изменить */
    private JButton edit;

    /** Печать */
    private JButton print;

//    /** Работники */
//    private JButton employees;

    /** Панель инструментов */
    private JToolBar toolBar;

    /** Таблица */
    protected JTable dataSpecialisations;

    /** Поле поискового запроса */
    private JTextField textSearch;

    /** Поиск */
    private JButton search;

    /** Скролл */
    private JScrollPane scroll;

    /** Сервис Менеджера */
    private SpecialisationService specialisationService = new SpecialisationService();

    /** Поток 1 отвечает за редактирование данных */
    Thread t1 = new Thread();
    /** Поток 2 отвечает за формирование отчет */
    Thread t2 = new Thread();

    /** Логгер класса */
    private static final Logger log = Logger.getLogger(SpecialisationWindow.class.getName());

    private AddDialogSpecialisation addDialogSpecialisation;
    private EditDialogSpecialisation editDialogSpecialisation;

    public void show()
    {
        log.info("Открытие окна SpecialisationWindow");
        window = new JFrame("Список менеджеров завода");
        window.setSize(1000,500);
        window.setLocation(310,130);
        window.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        // Создание кнопок и прикрепление иконок
        log.info("Добавление кнопок к окну SpecialisationWindow");
        add = new JButton("Добавить");
        delete = new JButton("Удалить");
        edit = new JButton("Редактировать");
        print = new JButton("Печать");

        // Настройка подсказок
        add.setToolTipText("Добавить информацию о рабочих");
        delete.setToolTipText("Удалить информацию о рабочих");
        edit.setToolTipText("Изменить информацию о рабочих");
        print.setToolTipText("Распечатать информацию о рабочих");
        // Добавление кнопок на панель инструментов
        toolBar = new JToolBar("Панель инструментов");
        toolBar.add(add);
        toolBar.add(delete);
        toolBar.add(edit);
        toolBar.add(print);
        // Размещение панели инструментов
        window.setLayout(new BorderLayout());
        window.add(toolBar,BorderLayout.NORTH);
        // Создание таблицы с данными
        log.info("Добавление таблицы с данными к окну SpecialisationWindow");
        String[] columns = {"ID", "Название должности"};

        List<Specialisation> specialisationsList = specialisationService.findAll();
        String [][] data = new String[specialisationsList.size()][5];
        for (int i = 0; i < specialisationsList.size(); i++)
        {
            data[i] = specialisationsList.get(i).toTableFormat();
        }

        // Настройка таблицы
        model = new DefaultTableModel(data, columns)
        {
            public boolean isCellEditable(int rowIndex, int columnIndex)
            {
                return false;
            }
        };
        this.dataSpecialisations = new JTable(model);
        dataSpecialisations.setFont(new Font(Font.SERIF,Font.BOLD,14));
        dataSpecialisations.setIntercellSpacing(new Dimension(0,1));
        dataSpecialisations.setRowHeight(dataSpecialisations.getRowHeight()+10);
        dataSpecialisations.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);

        dataSpecialisations.setDefaultRenderer(dataSpecialisations.getColumnClass(1), new DefaultTableCellRenderer(){
            public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
                super.setHorizontalAlignment(SwingConstants.CENTER);
                super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
                return this;
            }

        });

        scroll = new JScrollPane(this.dataSpecialisations);

        // Размещение таблицы с данными
        window.add(scroll,BorderLayout.CENTER);

        textSearch = new JTextField();
        textSearch.setColumns(20);
        search = new JButton("Поиск");
        window.getRootPane().setDefaultButton(search);
// remove the binding for pressed
        window.getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW)
                .put(KeyStroke.getKeyStroke("ENTER"), "none");
// retarget the binding for released
        window.getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW)
                .put(KeyStroke.getKeyStroke("released ENTER"), "press");
        // Добавление компонентов на панель
        JPanel searchPanel = new JPanel();
        searchPanel.add(textSearch);
        searchPanel.add(search);

        // Размещение панели поиска внизу окна
        window.add(searchPanel,BorderLayout.SOUTH);

        // Если не выделена строка, то прячем кнопки
        dataSpecialisations.getSelectionModel().addListSelectionListener((e) -> {
            boolean check = !dataSpecialisations.getSelectionModel().isSelectionEmpty();
            edit.setVisible(check);
            delete.setVisible(check);
        });

        add.addActionListener((e) -> {
            log.info("Старт Add listener");
            addDialogSpecialisation = new AddDialogSpecialisation(window, SpecialisationWindow.this, "Добавление записи");
            addDialogSpecialisation.setVisible(true);
        });

        add.setMnemonic(KeyEvent.VK_A);
        delete.addActionListener((e) -> {
            log.info("Старт Delete listener");
            if (dataSpecialisations.getRowCount() > 0) {
                if (dataSpecialisations.getSelectedRow() != -1) {
                    try {
                        specialisationService.delete(Integer.parseInt(dataSpecialisations.getValueAt(dataSpecialisations.getSelectedRow(), 0).toString()));
                        model.removeRow(dataSpecialisations.convertRowIndexToModel(dataSpecialisations.getSelectedRow()));
                        JOptionPane.showMessageDialog(window, "Вы удалили строку");
                        log.info("Была удалена строка данных");
                    } catch (Exception ex) {
                        JOptionPane.showMessageDialog(null, "Ошибка");
                        log.log(Level.SEVERE, "Исключение: ", ex);
                    }
                } else {
                    JOptionPane.showMessageDialog(null, "Вы не выбрали строку для удаления");
                    log.log(Level.WARNING, "Исключение: не выбрана строка для удаление");
                }
            } else {
                JOptionPane.showMessageDialog(null, "В данном окне нет записей. Нечего удалять");
                log.log(Level.WARNING, "Исключение: нет записей. нечего удалять");
            }
        });

        delete.setMnemonic(KeyEvent.VK_D);

        edit.addActionListener((e)-> {
            log.info("Старт Edit listener");
            if (model.getRowCount() != 0) {
                if (dataSpecialisations.getSelectedRow() != -1) {
                    t1 = new Thread(() -> {
                        JOptionPane.showMessageDialog(null,"1 поток запущен");
                        editDialogSpecialisation = new EditDialogSpecialisation(window, SpecialisationWindow.this, "Редактирование");
                        editDialogSpecialisation.setVisible(true);
                    });
                    t1.start();
                } else {
                    JOptionPane.showMessageDialog(null, "Не выбрана строка. Нечего редактировать");
                    log.log(Level.WARNING, "Исключение: не выбрана строка для редактирования");
                }
            } else {
                JOptionPane.showMessageDialog(null, "В данном окне нет записей. Нечего редактировать");
                log.log(Level.WARNING, "Исключение: нет записей. нечего редактировать");
            }
        });
        edit.setMnemonic(KeyEvent.VK_E);

        search.addActionListener((e) -> {
            if (model.getRowCount() != 0) {
                if (!textSearch.getText().isEmpty())
                    log.info("Запуск нового поиска по ключевому слову: " + textSearch.getText());
                else
                    log.info("Сброс ключевого слова поиска");
                TableRowSorter<TableModel> sorter = new TableRowSorter<TableModel>(((DefaultTableModel) model));
                sorter.setStringConverter(new TableStringConverter() {
                    @Override
                    public String toString(TableModel model, int row, int column) {
                        return model.getValueAt(row, column).toString().toLowerCase();
                    }
                });
                sorter.setRowFilter(RowFilter.regexFilter("(?i)" + textSearch.getText().toLowerCase()));
                dataSpecialisations.setRowSorter(sorter);
            }
        });

        window.setVisible(true);
    }

    public void addR(String name)
    {
        Specialisation newS = new Specialisation(name);
        specialisationService.persist(newS);
        model.addRow(newS.toTableFormat());
    }

    public void editR(String[] arr)
    {
        Specialisation M = specialisationService.findById(Integer.parseInt(arr[0]));
        M.setName(arr[1]);
        specialisationService.update(M);
    }

}
