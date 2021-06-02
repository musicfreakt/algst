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

/** Класс приложения, визуализирующий экранную форму с договорами */
public class ContractWindow
{
    /** Стандартный конструктор */
    ContractWindow()
    {
        this.clientID = -1;
        show();
    }

    /** Стандартный конструктор */
    ContractWindow(int id)
    {
        this.clientID = id;
        show();
    }

    private int clientID;

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

    /** Завершить */
    private JButton end;

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

    /** Сервис Контрактов */
    private ContractService contractService = new ContractService();

    /** Сервис Менеджеров */
    private ManagerService managerService = new ManagerService();

    /** Сервис Клиентов */
    private ClientService clientService = new ClientService();

    /** Поток 1 отвечает за редактирование данных */
    Thread t1 = new Thread();

    /** Поток 2 отвечает за формирование отчет */
    Thread t2 = new Thread();

    /** Логгер класса */
    private static final Logger log = Logger.getLogger(ContractWindow.class.getName());

    /** Диалог добавления данных */
    private AddDialogContract addDialogContract;

    /** Диалог измения данных */
    private EditDialogContract editDialogContract;

    /** Диалог изменения временного промежутка */
    private DialogTimePeriodSelection dialogTimePeriodSelection;

    /** Начало временного отрезка для сортировки */
    private Date dateBegin;

    /** Конец временного отрезка для сортировки */
    private Date dateEnd;

    public void show()
    {
        log.info("Открытие окна ContractWindow");

        window = new JFrame("Список договоров завода");
        window.setSize(1000,500);
        window.setLocation(310,130);
        window.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        // Создание кнопок и прикрепление иконок
        log.info("Добавление кнопок к окну ContractWindow");
        add = new JButton("Добавить");
        delete = new JButton("Удалить");
        edit = new JButton("Редактировать");
        print = new JButton("Печать");
        description = new JButton("Описание");
        end = new JButton("Завершить");


        // Настройка подсказок
        add.setToolTipText("Добавить контракт");
        delete.setToolTipText("Удалить контракт");
        edit.setToolTipText("Изменить контракт");
        print.setToolTipText("Распечатать контракты");
        description.setToolTipText("Показать описание контракта");
        end.setToolTipText("Завершить выполнение контракта");


        // Добавление кнопок на панель инструментов
        toolBar = new JToolBar("Панель инструментов");
        toolBar.add(add);
        toolBar.add(delete);
        toolBar.add(edit);
        toolBar.add(print);
        toolBar.add(description);
        toolBar.add(end);
        // Размещение панели инструментов
        window.setLayout(new BorderLayout());
        window.add(toolBar,BorderLayout.NORTH);

        // Создание таблицы с данными
        String[] columns = {"ID", "Описание", "Прибыль", "Клиент", "Менеджер", "Дата подписания", "Дата окончания работ", "Состояние"};
        log.info("Добавление таблицы с данными к окну ContractWindow");
        List<Contract> contractsList;

        if (clientID == -1)
            contractsList = contractService.findAll();
        else
            contractsList = contractService.findByClientId(clientID);

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
            edit.setVisible(check);
            delete.setVisible(check);
            end.setVisible(check);
            description.setVisible(check);
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
            addDialogContract = new AddDialogContract(window, ContractWindow.this, "Добавление записи");
            addDialogContract.setVisible(true);
        });

        add.setMnemonic(KeyEvent.VK_A);
        delete.addActionListener((e) -> {
            log.info("Старт Delete listener");
            if (dataContracts.getRowCount() > 0) {
                if (dataContracts.getSelectedRow() != -1) {
                    try {
                        contractService.delete(Integer.parseInt(dataContracts.getValueAt(dataContracts.getSelectedRow(), 0).toString()));
                        model.removeRow(dataContracts.convertRowIndexToModel(dataContracts.getSelectedRow()));
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
                if (dataContracts.getSelectedRow() != -1) {
                    t1 = new Thread(() -> {
                        editDialogContract = new EditDialogContract(window, ContractWindow.this, "Редактирование");
                        editDialogContract.setVisible(true);
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

        end.addActionListener((e) -> {
            log.info("Старт End listener");
            if (dataContracts.getRowCount() > 0) {
                if (dataContracts.getSelectedRow() != -1) {
                    try
                    {
                        contractService.setEnd(Integer.parseInt(dataContracts.getValueAt(dataContracts.getSelectedRow(), 0).toString()));
                        dataContracts.setValueAt("Выполнено" ,dataContracts.getSelectedRow(), 7);
                        JOptionPane.showMessageDialog(window, "Вы отметили договор завершеным");
                    } catch (Exception ex)
                    {
                        JOptionPane.showMessageDialog(null, "Ошибка");
                        log.log(Level.SEVERE, "Исключение: ", ex);
                    }
                } else {
                    JOptionPane.showMessageDialog(null, "Вы не выбрали договор для завершения");
                    log.log(Level.WARNING, "Исключение: не выбрана строка для завершения");
                }
            } else {
                JOptionPane.showMessageDialog(null, "В данном окне нет записей. Нечего завершать");
                log.log(Level.WARNING, "Исключение: нет записей. нечего завершать");
            }
        });

        end.setMnemonic(KeyEvent.VK_D);

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

        print.setMnemonic(KeyEvent.VK_D);

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
                    List<Contract> contractList = contractService.findOutdated();
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
                    dialogTimePeriodSelection = new DialogTimePeriodSelection(window, ContractWindow.this, "Установка периода времени");
                    dialogTimePeriodSelection.setVisible(true);

                    if (dateBegin != null && dateEnd != null)
                    {
                        if (dateBegin.getTime() > dateEnd.getTime())
                            throw new UnacceptableTimePeriod();

                        List<Contract> contractList = contractService.findTimePeriod(dateBegin, dateEnd);
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
                    List<Contract> contractList = contractService.findAll();
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
                java.io.FileWriter fw = new FileWriter("dataContracts.xml");
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
     * @param arr - данные, полученные от пользователя
     */
    public void addR(String[] arr, Date begin, Date end)
    {
        String[] r = arr[2].split(" ");
        int client_id = Integer.parseInt(r[0]);
        r = arr[3].split(" ");
        int manager_id = Integer.parseInt(r[0]);

        Contract newM = new Contract(arr[0],
                Double.parseDouble(arr[1]),
                clientService.findById(client_id),
                managerService.findById(manager_id),
                null,
                begin,
                end,
                false);
        contractService.persist(newM);
        model.addRow(newM.toTableFormat());
    }

    /**
     * Вспомогательный метод изменения данных в таблице
     * @param arr - данные, полученные от пользователя
     */
    public void editR(String[] arr, Date begin, Date end)
    {
        Contract C = contractService.findById(Integer.parseInt(arr[0]));
        C.setDescription(arr[1]);
        C.setPrice(Double.parseDouble(arr[2]));
        String[] r = arr[3].split(" ");
        int client_id = Integer.parseInt(r[0]);
        C.setClient(clientService.findById(client_id));
        r = arr[4].split(" ");
        int manager_id = Integer.parseInt(r[0]);
        C.setManager(managerService.findById(manager_id));
        C.setDateBegin(begin);
        C.setDateEnd(end);
        contractService.update(C);
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

    public String[] getManagers()
    {
        List<Manager> managers = managerService.findAll();
        String [] result = new String[managers.size()];
        for (int i = 0; i < managers.size(); i++)
            result[i] = managers.get(i).getId() + " " +  managers.get(i).getName() + " " + managers.get(i).getSurname();
        return result;
    }

    public String[] getClients()
    {
        List<Client> clients = clientService.findAll();
        String [] result = new String[clients.size()];
        for (int i = 0; i < clients.size(); i++)
            result[i] = clients.get(i).getId() + " " + clients.get(i).getName() + " " + clients.get(i).getSurname();
        return result;
    }

}
