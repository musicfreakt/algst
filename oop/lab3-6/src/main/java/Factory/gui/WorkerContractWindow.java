package Factory.gui;

import Factory.model.*;
import Factory.service.*;
import Factory.exceptions.*;
import Factory.util.ReportUtil;

import org.w3c.dom.*;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import javax.swing.*;
import javax.swing.table.*;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.io.*;
import java.util.List;
import java.util.Date;
import java.util.logging.*;

/** Класс приложения, визуализирующий экранную форму с договорами, которые выполняет рабочий */
public class WorkerContractWindow
{
    /** Стандартный конструктор */
    WorkerContractWindow(int workerId)
    {
        this.workerId = workerId;
        show();
    }

    /** Идентификатор рабочего, для которого открыто окно */
    private int workerId;

    /** Окно приложения */
    private JFrame window;

    /** Модель таблицы */
    private DefaultTableModel model;

    /** Добавить */
    private JButton add;

    /** Удалить */
    private JButton delete;

    /** Печать */
    private JButton print;

    /** Показать описание */
    private JButton description;

    /** Панель инструментов */
    private JToolBar toolBar;

    /** Таблица */
    protected JTable dataContracts;

    /** Поле поискового запроса */
    private JTextField textSearch;

    /** Поиск */
    private JButton search;

    /** Показать просроченные договоры */
    private JButton outdated;

    /** Показать договоры за указанный период */
    private JButton time;

    /** Сбросить */
    private JButton drop;

    /** Скролл */
    private JScrollPane scroll;

    /** Сервис рабочих */
    private EmployeeService workerService = new EmployeeService();

    /** Сервис контрактов */
    private ContractService contractService = new ContractService();

    /** Поток 2 отвечает за формирование отчет */
    Thread t2 = new Thread();

    /** Логгер класса */
    private static final Logger log = Logger.getLogger(WorkerContractWindow.class.getName());

    /** Диалог добавления данных */
    private AddDialogWorkerContract addDialog;

    /** Диалог изменения временного промежутка */
    private DialogTimePeriodSelection dialogTimePeriodSelection;

    /** Начало временного отрезка для сортировки */
    private Date dateBegin;

    /** Конец временного отрезка для сортировки */
    private Date dateEnd;

    public void show()
    {
        log.info("Открытие окна WorkerContractWindow");
        window = new JFrame("Список договоров завода");
        window.setSize(1000,500);
        window.setLocation(310,130);
        window.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        // Создание кнопок и прикрепление иконок
        log.info("Добавление кнопок к окну WorkerContractWindow");
        add = new JButton("Добавить");
        delete = new JButton("Удалить");
        print = new JButton("Печать");
        description = new JButton("Описание");

        // Настройка подсказок
        add.setToolTipText("Добавить контракт");
        delete.setToolTipText("Удалить контракт");
        print.setToolTipText("Распечатать контракты");
        description.setToolTipText("Показать описание контракта");
        // Добавление кнопок на панель инструментов
        toolBar = new JToolBar("Панель инструментов");
        toolBar.add(add);
        toolBar.add(delete);
        toolBar.add(print);
        toolBar.add(description);
        // Размещение панели инструментов
        window.setLayout(new BorderLayout());
        window.add(toolBar,BorderLayout.NORTH);

        // Создание таблицы с данными
        String[] columns = {"ID", "Описание", "Цена", "Клиент", "Менеджер", "Дата подписания", "Дата окончания работ", "Состояние"};
        log.info("Добавление таблицы с данными к окну ContractWindow");
        List<Contract> contractsList = (workerService.findById(workerId)).getContracts();
        String [][] data = new String[contractsList.size()][5];
        for (int i = 0; i < contractsList.size(); i++)
        {
            data[i] = contractsList.get(i).toTableFormat();
        }

        // Настройка таблицы
        model = new DefaultTableModel(data, columns)
        {
            public boolean isCellEditable(int rowIndex, int columnIndex)
            {
                return false;
            }
        };
        this.dataContracts = new JTable(model);
        RowSorter<TableModel> sort = new TableRowSorter<TableModel>(model);
        dataContracts.setRowSorter(sort);
        dataContracts.setFont(new Font(Font.SERIF,Font.BOLD,14));
        dataContracts.setIntercellSpacing(new Dimension(0,1));
        dataContracts.setRowHeight(dataContracts.getRowHeight()+10);
        dataContracts.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);

        dataContracts.setDefaultRenderer(dataContracts.getColumnClass(1), new DefaultTableCellRenderer(){
            public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
                super.setHorizontalAlignment(SwingConstants.CENTER);
                super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
                return this;
            }

        });

        // Если не выделена строка, то прячем кнопки
        dataContracts.getSelectionModel().addListSelectionListener((e) -> {
            boolean check = !dataContracts.getSelectionModel().isSelectionEmpty();
            delete.setVisible(check);
            description.setVisible(check);
            print.setVisible(check);
        });

        scroll = new JScrollPane(this.dataContracts);

        // Размещение таблицы с данными
        window.add(scroll,BorderLayout.CENTER);

        // Подготовка компонентов поиска
        textSearch = new JTextField();
        textSearch.setColumns(20);
        search = new JButton("Поиск");
        outdated = new JButton("Просроченные");
        time = new JButton("Период");
        drop = new JButton("Сброс");
        outdated.setToolTipText("Показать просроченные договоры");
        time.setToolTipText("Показать договоры за определенный промежуток времени");
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
        searchPanel.add(outdated);
        searchPanel.add(time);
        searchPanel.add(drop);

        // Размещение панели поиска внизу окна
        window.add(searchPanel,BorderLayout.SOUTH);

        add.addActionListener((e) -> {
            log.info("Старт Add listener");
            addDialog = new AddDialogWorkerContract(window, WorkerContractWindow.this, "Добавление записи");
            addDialog.setVisible(true);
        });

        add.setMnemonic(KeyEvent.VK_A);
        delete.addActionListener((e) -> {
            log.info("Старт Delete listener");
            if (dataContracts.getRowCount() > 0) {
                if (dataContracts.getSelectedRow() != -1) {
                    try
                    {
                        Contract c = contractService.findById(Integer.parseInt(dataContracts.getValueAt(dataContracts.getSelectedRow(), 0).toString()));
                        (workerService.findById(workerId)).removeContract(c);
                        model.removeRow(dataContracts.convertRowIndexToModel(dataContracts.getSelectedRow()));
                        contractService.update(c);
                        JOptionPane.showMessageDialog(window, "Вы удалили строку");
                        log.info("Была удалена строка данных");
                    }
                    catch (Exception ex)
                    {
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

        description.addActionListener((e) -> {
            log.info("Старт Description listener");
            if (dataContracts.getRowCount() > 0) {
                if (dataContracts.getSelectedRow() != -1) {
                    try
                    {
                        JOptionPane.showMessageDialog(window, dataContracts.getValueAt(dataContracts.getSelectedRow(), 1).toString());
                    }
                    catch (Exception ex)
                    {
                        JOptionPane.showMessageDialog(null, "Ошибка");
                        log.log(Level.SEVERE, "Исключение: ", ex);
                    }
                } else {
                    JOptionPane.showMessageDialog(null, "Вы не выбрали договор");
                    log.log(Level.WARNING, "Исключение: не выбрана строка");
                }
            } else {
                JOptionPane.showMessageDialog(null, "В данном окне нет записей");
                log.log(Level.WARNING, "Исключение: нет записей");
            }
        });

        description.setMnemonic(KeyEvent.VK_D);

        print.addActionListener((e)->{
            log.info("Старт Print listener");
            t2 = new Thread(() -> {
                try {
                    checkList();
                    makeXml();
                    ReportUtil.print("dataContracts.xml", "window/dataContracts", "contracts.jrxml", "reportContracts.pdf");
                    JOptionPane.showMessageDialog(null, "2 поток закончил работу. Отчет создан");
                } catch (Exception ex) {
                    JOptionPane.showMessageDialog(null, "Ошибка: " + ex.toString());
                    log.log(Level.SEVERE, "Исключение: ", ex);
                }
            });
            t2.start();
        });

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
                dataContracts.setRowSorter(sorter);
            }
        });

        outdated.addActionListener((e) -> {
            log.info("Старт outdated listener");
            if (dataContracts.getRowCount() > 0)
            {
                try
                {
                    List<Contract> contractList = contractService.findOutdated(workerId);
                    String [][] d = new String[contractList.size()][5];
                    for (int i = 0; i < contractList.size(); i++)
                    {
                        d[i] = contractList.get(i).toTableFormat();
                    }

                    model.setDataVector(d, columns);
                    model.fireTableDataChanged();
                }
                catch (Exception ex)
                {
                    JOptionPane.showMessageDialog(null, "Ошибка");
                    log.log(Level.SEVERE, "Исключение: ", ex);
                }
            } else {
                JOptionPane.showMessageDialog(null, "В данном окне нет записей");
                log.log(Level.WARNING, "Исключение: нет записей");
            }
        });

        outdated.setMnemonic(KeyEvent.VK_D);

        time.addActionListener((e) -> {
            log.info("Старт time listener");
            if (dataContracts.getRowCount() > 0)
            {
                try
                {
                    dialogTimePeriodSelection = new DialogTimePeriodSelection(window, WorkerContractWindow.this, "Установка периода времени");
                    dialogTimePeriodSelection.setVisible(true);

                    if (dateBegin != null && dateEnd != null)
                    {
                        if (dateBegin.getTime() > dateEnd.getTime())
                            throw new UnacceptableTimePeriod();

                        List<Contract> contractList = contractService.findTimePeriod(dateBegin, dateEnd, workerId);
                        String[][] d = new String[contractList.size()][5];
                        for (int i = 0; i < contractList.size(); i++) {
                            d[i] = contractList.get(i).toTableFormat();
                        }

                        model.setDataVector(d, columns);
                        model.fireTableDataChanged();
                    }
                }
                catch (Exception ex)
                {
                    JOptionPane.showMessageDialog(null, "Ошибка:" + ex.toString());
                    log.log(Level.SEVERE, "Исключение: ", ex);
                }
            } else {
                JOptionPane.showMessageDialog(null, "В данном окне нет записей");
                log.log(Level.WARNING, "Исключение: нет записей");
            }
        });

        time.setMnemonic(KeyEvent.VK_D);

        drop.addActionListener((e) -> {
            log.info("Старт drop listener");
            if (dataContracts.getRowCount() > 0)
            {
                try
                {
                    List<Contract> contractList = (workerService.findById(workerId)).getContracts();
                    String [][] d = new String[contractList.size()][5];
                    for (int i = 0; i < contractList.size(); i++)
                    {
                        d[i] = contractList.get(i).toTableFormat();
                    }

                    model.setDataVector(d, columns);
                    model.fireTableDataChanged();
                }
                catch (Exception ex)
                {
                    JOptionPane.showMessageDialog(null, "Ошибка");
                    log.log(Level.SEVERE, "Исключение: ", ex);
                }
            } else {
                JOptionPane.showMessageDialog(null, "В данном окне нет записей");
                log.log(Level.WARNING, "Исключение: нет записей");
            }
        });

        drop.setMnemonic(KeyEvent.VK_D);

        window.setVisible(true);
    }

    /**
     * Метод проверки списка на отсутсвие записей
     * @throws EmptyFileException моё исключение
     */
    private void checkList() throws EmptyFileException
    {
        if(model.getRowCount() == 0)
            throw new EmptyFileException();
    }

    /** Метод загрузки данных в XML файл */
    public void makeXml() {
        try {
            // Создание парсера документа
            DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
            // Создание пустого документа
            Document doc = builder.newDocument();
            // Создание корневого элемента window и добавление его в документ
            Node window = doc.createElement("window");
            doc.appendChild(window);
            // Создание дочерних элементов dataEmploy и присвоение значений атрибутам
            for (int i = 0; i < model.getRowCount(); i++) {
                Element dataManager = doc.createElement("dataContracts");
                window.appendChild(dataManager);
                dataManager.setAttribute("description", (String) model.getValueAt(i, 1));
                dataManager.setAttribute("price", (String) model.getValueAt(i, 2));
                dataManager.setAttribute("client", (String) model.getValueAt(i, 3));
                dataManager.setAttribute("manager", (String) model.getValueAt(i, 4));
                dataManager.setAttribute("dateBegin", (String) model.getValueAt(i, 5));
                dataManager.setAttribute("dateEnd", (String) model.getValueAt(i, 6));
                dataManager.setAttribute("isEnd", (String) model.getValueAt(i, 7));
            }
            try {
                // Создание преобразователя документа
                Transformer trans = TransformerFactory.newInstance().newTransformer();
                // Создание файла с именем dataEmploy.xml для записи документа
                java.io.FileWriter fw = new FileWriter("dataClients.xml");
                // Запись документа в файл
                trans.transform(new DOMSource(doc), new StreamResult(fw));
            } catch (TransformerConfigurationException e) {
                e.printStackTrace();
            } catch (TransformerException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }


        } catch (ParserConfigurationException e) {
            e.printStackTrace();
        }
    }

    /**
     * Вспомогательный метод добавления данных в таблицу
     * @param ID - данные, полученные от пользователя
     */
    public void addR(int ID)
    {
        Contract c = contractService.findById(ID);
        (workerService.findById(workerId)).addContract(c);
        model.addRow(c.toTableFormat());
        contractService.update(c);
    }

    /**
     * Вспомогательный метод получения даты для поиска
     * @param begin - начало временного отрезка
     * @param end - конец временного отрезка
     */
    public void setDate(Date begin, Date end)
    {
        dateBegin = begin;
        dateEnd = end;
    }


}
