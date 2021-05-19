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

public class ClientWindow
{
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

    /** Панель инструментов */
    private JToolBar toolBar;

    /** Таблица */
    protected JTable dataClients;

    /** Скролл */
    private JScrollPane scroll;

    /** Сервис Менеджера */
    private ClientService clientService = new ClientService();

    /** Поток 1 отвечает за редактирование данных */
    Thread t1 = new Thread();
    /** Поток 3 отвечает за формирование отчет */
    Thread t2 = new Thread();

    /** Логгер класса */
//    private static final Logger log = Logger.getLogger(ClientWindow.class);

    private AddDialogClient addDialogClient;
    private EditDialogClient editDialogClient;

    public void show()
    {
        window = new JFrame("Список клиентов завода");
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
        String[] columns = {"ID", "Имя", "Фамилия", "Компания"};

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

        scroll = new JScrollPane(this.dataClients);

        // Размещение таблицы с данными
        window.add(scroll,BorderLayout.CENTER);


        add.addActionListener((e) -> {
            addDialogClient = new AddDialogClient(window, ClientWindow.this, "Добавление записи");
            addDialogClient.setVisible(true);
        });

        add.setMnemonic(KeyEvent.VK_A);
        delete.addActionListener((e) -> {
            if (dataClients.getRowCount() > 0) {
                if (dataClients.getSelectedRow() != -1) {
                    try {
                        clientService.delete(Integer.parseInt(dataClients.getValueAt(dataClients.getSelectedRow(), 0).toString()));
                        model.removeRow(dataClients.convertRowIndexToModel(dataClients.getSelectedRow()));
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
                if (dataClients.getSelectedRow() != -1) {
                    t1 = new Thread(() -> {
                        JOptionPane.showMessageDialog(null,"1 поток запущен");
                        editDialogClient = new EditDialogClient(window, ClientWindow.this, "Редактирование");
                        editDialogClient.setVisible(true);
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
//                employs.print("dataClients.xml", "window/dataClients", "prod.jrxml", "otchetProd.pdf");
//            }
        });

        // Если не выделена строка, то прячем кнопки
        dataClients.getSelectionModel().addListSelectionListener((e) -> {
            boolean check = !dataClients.getSelectionModel().isSelectionEmpty();
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
//            JasperReport report = JasperCompileClient.compileReport(template);
//            // Заполнение отчета данными
//            JasperPrint print = JasperFillClient.fillReport(report, null, jr);
//            //JasperExportClient.exportReportToHtmlFile(print,resultpath);
//            if(resultpath.toLowerCase().endsWith("pdf")) {
//                JRExporter exporter;
//                exporter = new JRPdfExporter();
//                exporter.setParameter(JRExporterParameter.OUTPUT_FILE_NAME,resultpath);
//                exporter.setParameter(JRExporterParameter.JASPER_PRINT,print);
//                exporter.exportReport();
//            }
//            else
//                JasperExportClient.exportReportToHtmlFile(print,resultpath);
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
        Client newC = new Client(arr[0], arr[1], arr[2]);
        clientService.persist(newC);
        model.addRow(newC.toTableFormat());
    }

    public void editR(String[] arr)
    {
        Client C = clientService.findById(Integer.parseInt(arr[0]));
        C.setName(arr[1]);
        C.setSurname(arr[2]);
        C.setCompany(arr[3]);
        clientService.update(C);
    }
}
