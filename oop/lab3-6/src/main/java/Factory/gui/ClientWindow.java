package Factory.gui;

import Factory.exceptions.EmptyFileException;
import Factory.model.*;
import Factory.service.*;

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
import java.util.logging.*;


/** Класс приложения, визуализирующий экранную форму с менеджерами */
public class ClientWindow
{
    /** Стандартный конструктор */
    ClientWindow()
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

    /** Кнопка контрактов */
    private JButton contract;

    /** Панель инструментов */
    private JToolBar toolBar;

    /** Таблица */
    protected JTable dataClients;

    /** Поле поискового запроса */
    private JTextField textSearch;

    /** Поиск */
    private JButton search;

    /** Скролл */
    private JScrollPane scroll;

    /** Сервис Менеджера */
    private ClientService clientService = new ClientService();

    /** Поток 1 отвечает за редактирование данных */
    Thread t1 = new Thread();

    /** Поток 3 отвечает за формирование отчет */
    Thread t2 = new Thread();

    /** Логгер класса */
    private static final Logger log = Logger.getLogger(ClientWindow.class.getName());

    /** Диалог добавления данных */
    private AddDialogClient addDialogClient;

    /** Диалог измения данных */
    private EditDialogClient editDialogClient;

    /** Метод отображения окна */
    public void show()
    {
        log.info("Открытие окна ManagerWindow");
        window = new JFrame("Список клиентов завода");
        window.setSize(1000,500);
        window.setLocation(310,130);
        window.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        log.info("Добавление кнопок к окну ClientWindow");
        // Создание кнопок и прикрепление иконок
        add = new JButton("Добавить");
        delete = new JButton("Удалить");
        edit = new JButton("Редактировать");
        print = new JButton("Печать");
        contract = new JButton("Контракты");

        // Настройка подсказок
        add.setToolTipText("Добавить информацию о клиентах");
        delete.setToolTipText("Удалить информацию о клиентах");
        edit.setToolTipText("Изменить информацию о клиентах");
        print.setToolTipText("Распечатать информацию о клиентах");
        contract.setToolTipText("Показать контракты клиентов");
        // Добавление кнопок на панель инструментов
        toolBar = new JToolBar("Панель инструментов");
        toolBar.add(add);
        toolBar.add(delete);
        toolBar.add(edit);
        toolBar.add(print);
        toolBar.add(contract);
        // Размещение панели инструментов
        window.setLayout(new BorderLayout());
        window.add(toolBar,BorderLayout.NORTH);
        // Создание таблицы с данными
        String[] columns = {"ID", "Имя", "Фамилия", "Компания"};
        log.info("Добавление таблицы с данными к окну ClientWindow");
        List<Client> clientsList = clientService.findAll();
        String [][] data = new String[clientsList.size()][4];
        for (int i = 0; i < clientsList.size(); i++)
        {
            data[i] = clientsList.get(i).toTableFormat();
        }

        // Настройка таблицы
        model = new DefaultTableModel(data, columns)
        {
            public boolean isCellEditable(int rowIndex, int columnIndex)
            {
                return false;
            }
        };
        this.dataClients = new JTable(model);
        dataClients.setFont(new Font(Font.SERIF,Font.BOLD,14));
        dataClients.setIntercellSpacing(new Dimension(0,1));
        dataClients.setRowHeight(dataClients.getRowHeight()+10);
        dataClients.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);

        dataClients.setDefaultRenderer(dataClients.getColumnClass(1), new DefaultTableCellRenderer(){
            public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
                super.setHorizontalAlignment(SwingConstants.CENTER);
                super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
                return this;
            }

        });

        // Если не выделена строка, то прячем кнопки
        dataClients.getSelectionModel().addListSelectionListener((e) -> {
            boolean check = !dataClients.getSelectionModel().isSelectionEmpty();
            edit.setVisible(check);
            delete.setVisible(check);
            contract.setVisible(check);
        });

        scroll = new JScrollPane(this.dataClients);

        // Размещение таблицы с данными
        window.add(scroll,BorderLayout.CENTER);

        // Подготовка компонентов поиска
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

        add.addActionListener((e) -> {
            log.info("Старт Add listener");
            addDialogClient = new AddDialogClient(window, ClientWindow.this, "Добавление записи");
            addDialogClient.setVisible(true);
        });

        add.setMnemonic(KeyEvent.VK_A);
        delete.addActionListener((e) -> {
            log.info("Старт Delete listener");
            if (dataClients.getRowCount() > 0) {
                if (dataClients.getSelectedRow() != -1) {
                    try {
                        clientService.delete(Integer.parseInt(dataClients.getValueAt(dataClients.getSelectedRow(), 0).toString()));
                        model.removeRow(dataClients.convertRowIndexToModel(dataClients.getSelectedRow()));
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
                if (dataClients.getSelectedRow() != -1) {
                    t1 = new Thread(() -> {
                        JOptionPane.showMessageDialog(null,"1 поток запущен");
                        editDialogClient = new EditDialogClient(window, ClientWindow.this, "Редактирование");
                        editDialogClient.setVisible(true);
                    });
                    t1.start();
                } else {
                    JOptionPane.showMessageDialog(null, "Не выбрана строка. Нечего редактировать");
                    log.log(Level.WARNING, "Исключение: не выбрана строка для удаление");
                }
            } else {
                JOptionPane.showMessageDialog(null, "В данном окне нет записей. Нечего редактировать");
                log.log(Level.WARNING, "Исключение: нет записей. нечего удалять");
            }
        });
        edit.setMnemonic(KeyEvent.VK_E);

        print.addActionListener((e)->{
            log.info("Старт Print listener");
            try {
                checkList();
                makeXml();
                ReportUtil.print("dataClients.xml", "window/dataClients", "clients.jrxml", "reportClients.pdf");
                JOptionPane.showMessageDialog(null,"2 поток закончил работу. Отчет создан");
            }
            catch (Exception ex)
            {
                JOptionPane.showMessageDialog(null, "Ошибка: " + ex.toString());
                log.log(Level.SEVERE, "Исключение: ", ex);
            }
        });

        print.setMnemonic(KeyEvent.VK_E);

        contract.addActionListener((e)-> {
            log.info("Старт contract listener");
            if (model.getRowCount() != 0) {
                if (dataClients.getSelectedRow() != -1)
                {
                    try {
                        
                        JOptionPane.showMessageDialog(window, "Вы удалили строку");
                        log.info("Была удалена строка данных");
                    } catch (Exception ex) {
                        JOptionPane.showMessageDialog(null, "Ошибка:" + ex.toString());
                        log.log(Level.SEVERE, "Исключение: ", ex);
                    }
                }
                else
                {
                    JOptionPane.showMessageDialog(null, "Не выбрана строка. Нечего показывать");
                    log.log(Level.WARNING, "Исключение: не выбрана строка");
                }
            } else {
                JOptionPane.showMessageDialog(null, "В данном окне нет записей. Нечего показывать");
                log.log(Level.WARNING, "Исключение: нет записей. нечего удалять");
            }
        });
        contract.setMnemonic(KeyEvent.VK_E);

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
                dataClients.setRowSorter(sorter);
            }
        });

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
                Element dataManager = doc.createElement("dataClients");
                window.appendChild(dataManager);
                dataManager.setAttribute("name", (String) model.getValueAt(i, 1));
                dataManager.setAttribute("surname", (String) model.getValueAt(i, 2));
                dataManager.setAttribute("company", (String) model.getValueAt(i, 3));
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
     * @param arr - данные, полученные от пользователя
     */
    public void addR(String[] arr)
    {
        Client newC = new Client(arr[0], arr[1], arr[2]);
        clientService.persist(newC);
        model.addRow(newC.toTableFormat());
    }

    /**
     * Вспомогательный метод изменения данных в таблице
     * @param arr - данные, полученные от пользователя
     */
    public void editR(String[] arr)
    {
        Client C = clientService.findById(Integer.parseInt(arr[0]));
        C.setName(arr[1]);
        C.setSurname(arr[2]);
        C.setCompany(arr[3]);
        clientService.update(C);
    }
}
