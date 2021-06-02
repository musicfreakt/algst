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

/** Класс приложения, визуализирующий экранную форму с рабочими */
public class WorkerWindow
{
    /** Стандартный конструктор */
    WorkerWindow()
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

    /** Работа */
    private JButton work;

    /** Панель инструментов */
    private JToolBar toolBar;

    /** Таблица */
    protected JTable dataWorkers;

    /** Поле поискового запроса */
    private JTextField textSearch;

    /** Поиск */
    private JButton search;

    /** Скролл */
    private JScrollPane scroll;

    /** Сервис Рабочего */
    private EmployeeService employeeService = new EmployeeService();

    /** Сервис Профессий */
    private SpecialisationService specialisationService = new SpecialisationService();

    /** Поток 1 отвечает за редактирование данных */
    Thread t1 = new Thread();

    /** Поток 2 отвечает за формирование отчет */
    Thread t2 = new Thread();

    /** Логгер класса */
    private static final Logger log = Logger.getLogger(ManagerWindow.class.getName());

    /** Диалог добавления данных */
    private AddDialogWorker addDialogWorker;

    /** Диалог измения данных */
    private EditDialogWorker editDialogWorker;

    /** Метод отображения окна */
    public void show(){
        log.info("Открытие окна WorkerWindow");
        window = new JFrame("Список рабочих завода");
        window.setSize(1000,500);
        window.setLocation(310,130);
        window.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        // Создание кнопок и прикрепление иконок
        log.info("Добавление кнопок к окну WorkerWindow");
        add = new JButton("Добавить");
        delete = new JButton("Удалить");
        edit = new JButton("Редактировать");
        print = new JButton("Печать");
        work = new JButton("Работа");

        // Настройка подсказок
        add.setToolTipText("Добавить информацию о рабочих");
        delete.setToolTipText("Удалить информацию о рабочих");
        edit.setToolTipText("Изменить информацию о рабочих");
        print.setToolTipText("Распечатать информацию о рабочих");
        work.setToolTipText("Показать договоры, которые выполняют рабочие");
        // Добавление кнопок на панель инструментов
        toolBar = new JToolBar("Панель инструментов");
        toolBar.add(add);
        toolBar.add(delete);
        toolBar.add(edit);
        toolBar.add(print);
        toolBar.add(work);
        // Размещение панели инструментов
        window.setLayout(new BorderLayout());
        window.add(toolBar,BorderLayout.NORTH);
        // Создание таблицы с данными
        log.info("Добавление таблицы с данными к окну WorkerWindow");
        String[] columns = {"ID", "Имя", "Фамилия", "Опыт работы", "Должность"};

        List<Employee> workersList = employeeService.findAll();
        String [][] data = new String[workersList.size()][5];
        for (int i = 0; i < workersList.size(); i++)
        {
            data[i] = workersList.get(i).toTableFormat();
        }

        // Настройка таблицы
        model = new DefaultTableModel(data, columns)
        {
            public boolean isCellEditable(int rowIndex, int columnIndex)
            {
                return false;
            }
        };
        this.dataWorkers = new JTable(model);
        dataWorkers.setFont(new Font(Font.SERIF,Font.BOLD,14));
        dataWorkers.setIntercellSpacing(new Dimension(0,1));
        dataWorkers.setRowHeight(dataWorkers.getRowHeight()+10);
        dataWorkers.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);

        dataWorkers.setDefaultRenderer(dataWorkers.getColumnClass(1), new DefaultTableCellRenderer(){
            public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
                super.setHorizontalAlignment(SwingConstants.CENTER);
                super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
                return this;
            }

        });

        scroll = new JScrollPane(this.dataWorkers);

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

        // Если не выделена строка, то прячем кнопки
        dataWorkers.getSelectionModel().addListSelectionListener((e) -> {
            boolean check = !dataWorkers.getSelectionModel().isSelectionEmpty();
            edit.setVisible(check);
            delete.setVisible(check);
            work.setVisible(check);
        });

        add.addActionListener((e) ->
        {
            log.info("Старт Add listener");
            addDialogWorker = new AddDialogWorker(window, WorkerWindow.this, "Добавление записи");
            addDialogWorker.setVisible(true);
        });

        add.setMnemonic(KeyEvent.VK_A);
        delete.addActionListener((e) -> {
            log.info("Старт Delete listener");
            if (dataWorkers.getRowCount() > 0) {
                if (dataWorkers.getSelectedRow() != -1) {
                    try {
                        employeeService.delete(Integer.parseInt(dataWorkers.getValueAt(dataWorkers.getSelectedRow(), 0).toString()));
                        model.removeRow(dataWorkers.convertRowIndexToModel(dataWorkers.getSelectedRow()));
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
                if (dataWorkers.getSelectedRow() != -1) {
                    t1 = new Thread(() -> {
                        JOptionPane.showMessageDialog(null,"1 поток запущен");
                        editDialogWorker = new EditDialogWorker(window, WorkerWindow.this, "Редактирование");
                        editDialogWorker.setVisible(true);
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
            try
            {
                checkList();
                makeXml();
                ReportUtil.print("dataWorkers.xml", "window/dataWorkers", "workers.jrxml", "reportWorkers.pdf");
                JOptionPane.showMessageDialog(null,"2 поток закончил работу. Отчет создан");
            } catch (Exception ex) {
                JOptionPane.showMessageDialog(null, "Ошибка: " + ex.toString());
                log.log(Level.SEVERE, "Исключение: ", ex);
            }
        });

        work.addActionListener((e) -> {
            log.info("Старт work listener");
            if (dataWorkers.getRowCount() > 0) {
                if (dataWorkers.getSelectedRow() != -1) {
                    try
                    {
                        new WorkerContractWindow(Integer.parseInt(dataWorkers.getValueAt(dataWorkers.getSelectedRow(), 0).toString()));
                    }
                    catch (Exception ex)
                    {
                        JOptionPane.showMessageDialog(null, "Ошибка");
                        log.log(Level.SEVERE, "Исключение: ", ex);
                    }
                }
                else
                {
                    JOptionPane.showMessageDialog(null, "Вы не выбрали строку");
                    log.log(Level.WARNING, "Исключение: не выбрана строка");
                }
            }
            else
            {
                JOptionPane.showMessageDialog(null, "В данном окне нет записей");
                log.log(Level.WARNING, "Исключение: нет записей");
            }
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
                dataWorkers.setRowSorter(sorter);
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
    public void makeXml()
    {
        try
        {
            // Создание парсера документа
            DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
            // Создание пустого документа
            Document doc = builder.newDocument();
            // Создание корневого элемента window и добавление его в документ
            Node window = doc.createElement("window");
            doc.appendChild(window);
            // Создание дочерних элементов dataEmploy и присвоение значений атрибутам
            for (int i = 0; i < model.getRowCount(); i++) {
                Element dataEmploy = doc.createElement("dataWorkers");
                window.appendChild(dataEmploy);
                dataEmploy.setAttribute("name", (String) model.getValueAt(i, 1));
                dataEmploy.setAttribute("surname", (String) model.getValueAt(i, 2));
                dataEmploy.setAttribute("exp", (String) model.getValueAt(i, 3));
                dataEmploy.setAttribute("specialisation", (String) model.getValueAt(i, 4));
            }
            try
            {
                // Создание преобразователя документа
                Transformer trans = TransformerFactory.newInstance().newTransformer();
                // Создание файла с именем dataEmploy.xml для записи документа
                java.io.FileWriter fw = new FileWriter("dataWorkers.xml");
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


    /** Вспомогательный метод получения строк всех профессий */
    public String[] getSpecs()
    {
        List<Specialisation> specs = specialisationService.findAll();
        String [] result = new String[specs.size()];
        for (int i = 0; i < specs.size(); i++)
            result[i] = specs.get(i).getName();
        return result;
    }

    /**
     * Вспомогательный метод добавления данных в таблицу
     * @param arr - данные, полученные от пользователя
     */
    public void addR(String[] arr)
    {
        Employee newW = new Employee(arr[0], arr[1], Integer.parseInt(arr[2]), specialisationService.findByName(arr[3]));
        employeeService.persist(newW);
        model.addRow(newW.toTableFormat());
    }

    /**
     * Вспомогательный метод изменения данных в таблице
     * @param arr - данные, полученные от пользователя
     */
    public void editR(String[] arr)
    {
        Employee W = employeeService.findById(Integer.parseInt(arr[0]));
        W.setName(arr[1]);
        W.setSurname(arr[2]);
        W.setExp(Integer.parseInt(arr[3]));
        W.setSpecialisation(specialisationService.findByName(arr[4]));
        employeeService.update(W);
    }
}
