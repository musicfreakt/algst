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

    /** Панель инструментов */
    private JToolBar toolBar;

    /** Таблица */
    protected JTable dataSpecialisations;

    /** Скролл */
    private JScrollPane scroll;

    /** Сервис Менеджера */
    private SpecialisationService specialisationService = new SpecialisationService();

    /** Поток 1 отвечает за редактирование данных */
    Thread t1 = new Thread();
    /** Поток 2 отвечает за формирование отчет */
    Thread t2 = new Thread();

    /** Логгер класса */
//    private static final Logger log = Logger.getLogger(SpecialisationWindow.class);


    private AddDialogSpecialisation addDialogSpecialisation;
    private EditDialogSpecialisation editDialogSpecialisation;

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

        // Если не выделена строка, то прячем кнопки
        dataSpecialisations.getSelectionModel().addListSelectionListener((e) -> {
            boolean check = !dataSpecialisations.getSelectionModel().isSelectionEmpty();
            edit.setVisible(check);
            delete.setVisible(check);
        });

        add.addActionListener((e) -> {
            addDialogSpecialisation = new AddDialogSpecialisation(window, SpecialisationWindow.this, "Добавление записи");
            addDialogSpecialisation.setVisible(true);
        });

        add.setMnemonic(KeyEvent.VK_A);
        delete.addActionListener((e) -> {
            if (dataSpecialisations.getRowCount() > 0) {
                if (dataSpecialisations.getSelectedRow() != -1) {
                    try {
                        specialisationService.delete(Integer.parseInt(dataSpecialisations.getValueAt(dataSpecialisations.getSelectedRow(), 0).toString()));
                        model.removeRow(dataSpecialisations.convertRowIndexToModel(dataSpecialisations.getSelectedRow()));
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
                if (dataSpecialisations.getSelectedRow() != -1) {
                    t1 = new Thread(() -> {
                        JOptionPane.showMessageDialog(null,"1 поток запущен");
                        editDialogSpecialisation = new EditDialogSpecialisation(window, SpecialisationWindow.this, "Редактирование");
                        editDialogSpecialisation.setVisible(true);
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
//            if (model.getRowCount() != 0)
//            {
//                SpecialisationWindow.print();
//            }
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
//                Element dataSpecialisation = doc.createElement("dataSpecialisation");
//                window.appendChild(dataSpecialisation);
//                dataSpecialisation.setAttribute("name", (String) model.getValueAt(i, 0));
//                dataSpecialisation.setAttribute("surname", (String) model.getValueAt(i, 1));
//            }
//            try {
//                // Создание преобразователя документа
//                Transformer trans = TransformerFactory.newInstance().newTransformer();
//                // Создание файла с именем dataEmploy.xml для записи документа
//                java.io.FileWriter fw = new FileWriter("dataSpecialisation.xml");
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
//            JasperReport report = JasperCompileSpecialisation.compileReport(template);
//            // Заполнение отчета данными
//            JasperPrint print = JasperFillSpecialisation.fillReport(report, null, jr);
//
//            if(resultpath.toLowerCase().endsWith("pdf")) {
//                JRExporter exporter;
//                exporter = new JRPdfExporter();
//                exporter.setParameter(JRExporterParameter.OUTPUT_FILE_NAME,resultpath);
//                exporter.setParameter(JRExporterParameter.JASPER_PRINT,print);
//                exporter.exportReport();
//            }
//            else
//                JasperExportSpecialisation.exportReportToHtmlFile(print,resultpath);
//            JOptionPane.showMessageDialog(null,"2 поток закончил работу. Отчет создан");
//        }
//        catch (JRException e)
//        {
//            e.printStackTrace();
//        }
//    }

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
