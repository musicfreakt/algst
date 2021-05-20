package Factory.gui;

import Factory.model.*;
import Factory.service.*;

import javax.swing.*;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.util.List;

//import javax.xml.parsers.DocumentBuilderFactory;
//import javax.xml.parsers.ParserConfigurationException;
//import javax.xml.transform.Transformer;
//import javax.xml.transform.TransformerConfigurationException;
//import javax.xml.transform.TransformerException;
//import javax.xml.transform.TransformerFactory;
//import javax.xml.transform.dom.DOMSource;
//import javax.xml.transform.stream.StreamResult;
//import net.sf.jasperreports.engine.*;
//import net.sf.jasperreports.engine.data.JRXmlDataSource;
//import net.sf.jasperreports.engine.export.JRPdfExporter;

public class ManagerWindow
{
    ManagerWindow()
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

    /** Панель инструментов */
    private JToolBar toolBar;

    /** Таблица */
    protected JTable dataManagers;

    /** Скролл */
    private JScrollPane scroll;

    /** Сервис Менеджера */
    private ManagerService managerService = new ManagerService();
    
    /** Поток 1 отвечает за редактирование данных */
    Thread t1 = new Thread();
    /** Поток 3 отвечает за формирование отчет */
    Thread t2 = new Thread();

    /** Логгер класса */
//    private static final Logger log = Logger.getLogger(ManagerWindow.class);


    private AddDialogManager addDialogManager;
    private EditDialogManager editDialogProd;

    public void show()
    {
        window = new JFrame("Список менеджеров завода");
        window.setSize(1000,500);
        window.setLocation(310,130);
        window.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        // Создание кнопок и прикрепление иконок
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
        String[] columns = {"ID", "Имя", "Фамилия"};

        List<Manager> managersList = managerService.findAll();
        String [][] data = new String[managersList.size()][5];
        for (int i = 0; i < managersList.size(); i++)
        {
            data[i] = managersList.get(i).toTableFormat();
        }

        // Настройка таблицы
        model = new DefaultTableModel(data, columns)
        {
            public boolean isCellEditable(int rowIndex, int columnIndex)
            {
                return false;
            }
        };
        this.dataManagers = new JTable(model);
        dataManagers.setFont(new Font(Font.SERIF,Font.BOLD,14));
        dataManagers.setIntercellSpacing(new Dimension(0,1));
        dataManagers.setRowHeight(dataManagers.getRowHeight()+10);
        dataManagers.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);

        dataManagers.setDefaultRenderer(dataManagers.getColumnClass(1), new DefaultTableCellRenderer(){
            public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
                super.setHorizontalAlignment(SwingConstants.CENTER);
                super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
                return this;
            }

        });

        scroll = new JScrollPane(this.dataManagers);

        // Размещение таблицы с данными
        window.add(scroll,BorderLayout.CENTER);

        // Если не выделена строка, то прячем кнопки
        dataManagers.getSelectionModel().addListSelectionListener((e) -> {
            boolean check = !dataManagers.getSelectionModel().isSelectionEmpty();
            edit.setVisible(check);
            delete.setVisible(check);
        });

        add.addActionListener((e) -> {
            addDialogManager = new AddDialogManager(window, ManagerWindow.this, "Добавление записи");
            addDialogManager.setVisible(true);
        });

        add.setMnemonic(KeyEvent.VK_A);
        delete.addActionListener((e) -> {
            if (dataManagers.getRowCount() > 0) {
                if (dataManagers.getSelectedRow() != -1) {
                    try {
                        managerService.delete(Integer.parseInt(dataManagers.getValueAt(dataManagers.getSelectedRow(), 0).toString()));
                        model.removeRow(dataManagers.convertRowIndexToModel(dataManagers.getSelectedRow()));
                        JOptionPane.showMessageDialog(window, "Вы удалили строку");
                    } catch (Exception ex) {
                        JOptionPane.showMessageDialog(null, "Ошибка");
                    }
                } else {
                    JOptionPane.showMessageDialog(null, "Вы не выбрали строку для удаления");
                }
            } else {
                JOptionPane.showMessageDialog(null, "В данном окне нет записей. Нечего удалять");
            }
        });

        delete.setMnemonic(KeyEvent.VK_D);

        edit.addActionListener((e)-> {
            if (model.getRowCount() != 0) {
                if (dataManagers.getSelectedRow() != -1) {
                    t1 = new Thread(() -> {
                        JOptionPane.showMessageDialog(null,"1 поток запущен");
                        editDialogProd = new EditDialogManager(window, ManagerWindow.this, "Редактирование");
                        editDialogProd.setVisible(true);
                    });
                    t1.start();
                } else {
                    JOptionPane.showMessageDialog(null, "Не выбрана строка. Нечего редактировать");
                }
            } else {
                JOptionPane.showMessageDialog(null, "В данном окне нет записей. Нечего редактировать");
            }
        });
        edit.setMnemonic(KeyEvent.VK_E);

        print.addActionListener((e)->{
            if (model.getRowCount() != 0)
            {
//                ManagerWindow.print();
            }
        });

        window.setVisible(true);
    }


//    /**
//     * Метод загрузки данных в XML файл
//     */
//    public void makeXml() {
//        try {
//            // Создание парсера документа
//            DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
//            // Создание пустого документа
//            Document doc = builder.newDocument();
//            // Создание корневого элемента window и добавление его в документ
//            Node window = doc.createElement("window");
//            doc.appendChild(window);
//            // Создание дочерних элементов dataEmploy и присвоение значений атрибутам
//            for (int i = 0; i < model.getRowCount(); i++) {
//                Element dataManager = doc.createElement("dataManager");
//                window.appendChild(dataManager);
//                dataManager.setAttribute("name", (String) model.getValueAt(i, 0));
//                dataManager.setAttribute("surname", (String) model.getValueAt(i, 1));
//            }
//            try {
//                // Создание преобразователя документа
//                Transformer trans = TransformerFactory.newInstance().newTransformer();
//                // Создание файла с именем dataEmploy.xml для записи документа
//                java.io.FileWriter fw = new FileWriter("dataManager.xml");
//                // Запись документа в файл
//                trans.transform(new DOMSource(doc), new StreamResult(fw));
//            } catch (TransformerConfigurationException e) {
//                e.printStackTrace();
//            } catch (TransformerException e) {
//                e.printStackTrace();
//            } catch (IOException e) {
//                e.printStackTrace();
//            }
//
//
//        } catch (ParserConfigurationException e) {
//            e.printStackTrace();
//        }
//    }
//    /**
//     * Метод генерации отчетов в форматах DOCX и HTML.
//     * P.S генерация в формате PDF возможна, но символы кириллицы отображаться не будут.
//     * @param datasource Имя файла XML с данными
//     * @param xpath Директория до полей с данными. Ex.: "BookList/Books" - Fields
//     * @param template Имя файла шаблона .jrxml
//     * @param resultpath Имя файла, в который будет помещен отчет
//     */
//    public static void print(String datasource, String xpath, String template, String resultpath)
//    {
//        try
//        {
//            // Указание источника XML-данных
//            JRDataSource jr = new JRXmlDataSource(datasource, xpath);
//            // Создание отчета на базе шаблона
//            JasperReport report = JasperCompileManager.compileReport(template);
//            // Заполнение отчета данными
//            JasperPrint print = JasperFillManager.fillReport(report, null, jr);
//
//            if(resultpath.toLowerCase().endsWith("pdf")) {
//                JRExporter exporter;
//                exporter = new JRPdfExporter();
//                exporter.setParameter(JRExporterParameter.OUTPUT_FILE_NAME,resultpath);
//                exporter.setParameter(JRExporterParameter.JASPER_PRINT,print);
//                exporter.exportReport();
//            }
//            else
//                JasperExportManager.exportReportToHtmlFile(print,resultpath);
//            JOptionPane.showMessageDialog(null,"2 поток закончил работу. Отчет создан");
//        }
//        catch (JRException e)
//        {
//            e.printStackTrace();
//        }
//    }

    public void addR(String[] arr)
    {
        Manager newM = new Manager(arr[0], arr[1]);
        managerService.persist(newM);
        model.addRow(newM.toTableFormat());
    }

    public void editR(String[] arr)
    {
        Manager M = managerService.findById(Integer.parseInt(arr[0]));
        M.setName(arr[1]);
        M.setSurname(arr[2]);
        managerService.update(M);
    }

}
