package Factory.gui;

import Factory.model.*;
import Factory.service.*;

import javax.swing.*;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.util.List;
//import net.sf.jasperreports.engine.*;
//import net.sf.jasperreports.engine.data.JRXmlDataSource;
//import net.sf.jasperreports.engine.export.JRPdfExporter;

public class ManagerWindow
{
    ManagerWindow()
    {
        show();
    }

    /**
     * Окно приложения
     */
    private JFrame window;
    /**
     * Модель таблицы
     */
    private DefaultTableModel model;
    /**
     * Добавить
     */
    private JButton add;
    /**
     * Удалить
     */
    private JButton delete;
    /**
     * Изменить
     */
    private JButton edit;
    /**
     * Печать
     */
    private JButton print;
    /**
     * Панель инструментов
     */
    private JToolBar toolBar;
    /**
     * Таблица
     */
    protected JTable dataManagers;
    /**
     * Скролл
     */
    private JScrollPane scroll;

    private ManagerService managerService = new ManagerService();

    /**
     * Поток 1 отвечает за загрузку данных из XML-файла в экранную форму
     */
    Thread t1 = new Thread();
    /**
     * Поток 2 отвечает за редактирование данных и сохранение XML-файла
     */
    Thread t2 = new Thread();
    /**
     * Поток 3 отвечает за формирование отчета
     */
    Thread t3 = new Thread();


    final static public Object shared=new Object();

    /**
     * Логгер класса employs
     */
//    private static final Logger log = Logger.getLogger(employs.class);


    private AddDialogManager addDialogManager;
    private EditDialogManager editDialogProd;

    public void show()
    {
        window = new JFrame("Список рабочих завода");
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
            if (t1.isAlive()) {
                try {
                    JOptionPane.showMessageDialog(window, "Ждем, пока отработает 1 поток");
                    t1.join();
                    JOptionPane.showMessageDialog(window, "1 поток отработал, пробуем запустить 2 поток");
                } catch (InterruptedException ex) {
                    ex.printStackTrace();
                }
            }
            if (model.getRowCount() != 0) {
                if (dataManagers.getSelectedRow() != -1) {
                    t2 = new Thread(() -> {
                        JOptionPane.showMessageDialog(null,"2 поток запущен");
                        editDialogProd = new EditDialogManager(window, ManagerWindow.this, "Редактирование");
                        editDialogProd.setVisible(true);
                    });
                    t2.start();
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
//                employs.print("dataManagers.xml", "window/dataManagers", "prod.jrxml", "otchetProd.pdf");
//            }
        });

        // Если не выделена строка, то прячем кнопки
        dataManagers.getSelectionModel().addListSelectionListener((e) -> {
            Boolean check = true;
            if (dataManagers.getSelectionModel().isSelectionEmpty()) {
                check = false;
            }
            edit.setVisible(check);
            delete.setVisible(check);
        });

        window.setVisible(true);
    }

//    /**
//     * Метод генерации отчетов в форматах DOCX и HTML.
//     * P.S генерация в формате PDF возможна, но символы кириллицы отображаться не будут.
//     * @param datasource Имя файла XML с данными
//     * @param xpath Директория до полей с данными. Ex.: "BookList/Books" - Fields
//     * @param template Имя файла шаблона .jrxml
//     * @param resultpath Имя файла, в который будет помещен отчет
//     */
//    public static void print(String datasource, String xpath, String template, String resultpath){
//        try {
//            // Указание источника XML-данных
//            JRDataSource jr = new JRXmlDataSource(datasource, xpath);
//            // Создание отчета на базе шаблона
//            JasperReport report = JasperCompileManager.compileReport(template);
//            // Заполнение отчета данными
//            JasperPrint print = JasperFillManager.fillReport(report, null, jr);
//            //JasperExportManager.exportReportToHtmlFile(print,resultpath);
//            if(resultpath.toLowerCase().endsWith("pdf")) {
//                JRExporter exporter;
//                exporter = new JRPdfExporter();
//                exporter.setParameter(JRExporterParameter.OUTPUT_FILE_NAME,resultpath);
//                exporter.setParameter(JRExporterParameter.JASPER_PRINT,print);
//                exporter.exportReport();
//            }
//            else
//                JasperExportManager.exportReportToHtmlFile(print,resultpath);
//            JOptionPane.showMessageDialog(null,"3 поток закончил работу. Отчет создан");
//        }
//        catch (JRException e){
//            e.printStackTrace();
//        }
//    }
//
//    /**
//     * Метод чтения данных из файла
//     * @param filename Имя файла
//     */
//    public  void read(String filename){
//        try{
//            BufferedReader reader = new BufferedReader(new FileReader(filename));
//            for(int i = 0; i<model.getRowCount();i++)
//                model.removeRow(0);
//            String temp;
//            do{
//                temp = reader.readLine();
//                if(temp!=null){
//                    String[] temp2 = temp.split(";");
//                    model.addRow(temp2);
//                }
//            }while(temp!=null);
//            reader.close();
//
//        }
//        catch (FileNotFoundException e){
//            e.printStackTrace();
//        }
//        catch (IOException e){
//            e.printStackTrace();
//        }
//    }
//
//    /**
//     * Метод записи данных в файл
//     * @param filename Имя файла
//     */
//    public void write(String filename){
//        try{
//            BufferedWriter writer = new BufferedWriter(new FileWriter(filename));
//            for(int i = 0; i<model.getRowCount();i++) {
//                for (int j = 0; j < model.getColumnCount(); j++) {
//                    writer.write((String) model.getValueAt(i, j));
//                    if(j!=model.getColumnCount()-1)
//                        writer.write(";");
//                }
//                if(i!=model.getRowCount()-1)
//                    writer.write("\r\n");
//            }
//            writer.close();
//        }
//        catch (IOException e){
//            e.printStackTrace();
//        }
//    }


    public void addR(String[] arr)
    {
        Manager newM = new Manager(arr[0], arr[1]);
        managerService.persist(newM);
    }

    public void editR(String[] arr)
    {
        Manager M = managerService.findById(Integer.parseInt(arr[0]));
        M.setName(arr[1]);
        M.setSurname(arr[2]);
        managerService.update(M);
    }

}
