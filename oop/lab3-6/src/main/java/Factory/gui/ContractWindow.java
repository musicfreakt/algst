package Factory.gui;

import Factory.model.*;
import Factory.service.*;

import javax.swing.*;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.util.Date;
import java.util.List;
//import net.sf.jasperreports.engine.*;
//import net.sf.jasperreports.engine.data.JRXmlDataSource;
//import net.sf.jasperreports.engine.export.JRPdfExporter;

public class ContractWindow
{
    ContractWindow()
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

    /** Завершить */
    private JButton end;

    /** Панель инструментов */
    private JToolBar toolBar;

    /** Таблица */
    protected JTable dataContracts;

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
//    private static final Logger log = Logger.getLogger(ContractWindow.class);

    private AddDialogContract addDialogContract;
    private EditDialogContract editDialogContract;

    public void show()
    {
        window = new JFrame("Список договоров завода");
        window.setSize(1000,500);
        window.setLocation(310,130);
        window.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        // Создание кнопок и прикрепление иконок
        add = new JButton("Добавить");
        delete = new JButton("Удалить");
        edit = new JButton("Редактировать");
        print = new JButton("Печать");
        end = new JButton("Завершить");

        // Настройка подсказок
        add.setToolTipText("Добавить контракт");
        delete.setToolTipText("Удалить контракт");
        edit.setToolTipText("Изменить контракт");
        print.setToolTipText("Распечатать контракты");
        // Добавление кнопок на панель инструментов
        toolBar = new JToolBar("Панель инструментов");
        toolBar.add(add);
        toolBar.add(delete);
        toolBar.add(edit);
        toolBar.add(print);
        toolBar.add(end);
        // Размещение панели инструментов
        window.setLayout(new BorderLayout());
        window.add(toolBar,BorderLayout.NORTH);
        // Создание таблицы с данными
        String[] columns = {"ID", "Описание", "Цена", "Клиент", "Менеджер", "Дата подписания", "Дата окончания работ", "Состояние"};

        List<Contract> contractsList = contractService.findAll();
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

        scroll = new JScrollPane(this.dataContracts);

        // Размещение таблицы с данными
        window.add(scroll,BorderLayout.CENTER);

        // Если не выделена строка, то прячем кнопки
        dataContracts.getSelectionModel().addListSelectionListener((e) -> {
            boolean check = !dataContracts.getSelectionModel().isSelectionEmpty();
            edit.setVisible(check);
            delete.setVisible(check);
            end.setVisible(check);
        });

        add.addActionListener((e) -> {
            addDialogContract = new AddDialogContract(window, ContractWindow.this, "Добавление записи");
            addDialogContract.setVisible(true);
        });

        add.setMnemonic(KeyEvent.VK_A);
        delete.addActionListener((e) -> {
            if (dataContracts.getRowCount() > 0) {
                if (dataContracts.getSelectedRow() != -1) {
                    try {
                        contractService.delete(Integer.parseInt(dataContracts.getValueAt(dataContracts.getSelectedRow(), 0).toString()));
                        model.removeRow(dataContracts.convertRowIndexToModel(dataContracts.getSelectedRow()));
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
                if (dataContracts.getSelectedRow() != -1) {
                    t1 = new Thread(() -> {
                        JOptionPane.showMessageDialog(null,"1 поток запущен");
                        editDialogContract = new EditDialogContract(window, ContractWindow.this, "Редактирование");
                        editDialogContract.setVisible(true);
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

        end.addActionListener((e) -> {
            if (dataContracts.getRowCount() > 0) {
                if (dataContracts.getSelectedRow() != -1) {
                    try
                    {
                        contractService.setEnd(Integer.parseInt(dataContracts.getValueAt(dataContracts.getSelectedRow(), 0).toString()));
//                        model.removeRow(dataContracts.convertRowIndexToModel(dataContracts.getSelectedRow())); // todo: сделать изменение
                        JOptionPane.showMessageDialog(window, "Вы отметили договор завершеным");
                    } catch (Exception ex)
                    {
                        JOptionPane.showMessageDialog(null, "Ошибка");
                    }
                } else
                    JOptionPane.showMessageDialog(null, "Вы не выбрали строку для удаления");
            } else
                JOptionPane.showMessageDialog(null, "В данном окне нет записей. Нечего удалять");
        });

        end.setMnemonic(KeyEvent.VK_D);

        print.addActionListener((e)->{
//            if (model.getRowCount() != 0)
//            {
//                employs.print("dataContracts.xml", "window/dataContracts", "prod.jrxml", "otchetProd.pdf");
//            }
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
//            JasperReport report = JasperCompileContract.compileReport(template);
//            // Заполнение отчета данными
//            JasperPrint print = JasperFillContract.fillReport(report, null, jr);
//            //JasperExportContract.exportReportToHtmlFile(print,resultpath);
//            if(resultpath.toLowerCase().endsWith("pdf")) {
//                JRExporter exporter;
//                exporter = new JRPdfExporter();
//                exporter.setParameter(JRExporterParameter.OUTPUT_FILE_NAME,resultpath);
//                exporter.setParameter(JRExporterParameter.JASPER_PRINT,print);
//                exporter.exportReport();
//            }
//            else
//                JasperExportContract.exportReportToHtmlFile(print,resultpath);
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

    public void editR(String[] arr)
    {
        Contract C = contractService.findById(Integer.parseInt(arr[0]));
        C.setDescription(arr[1]);
        C.setPrice(Double.parseDouble(arr[2]));
        String[] r = arr[2].split(" ");
        int client_id = Integer.parseInt(r[0]);
        C.setClient(clientService.findById(client_id));
        r = arr[3].split(" ");
        int manager_id = Integer.parseInt(r[0]);
        C.setManager(managerService.findById(manager_id));
        contractService.update(C);
    }

    public String[] getManagers()
    {
        List<Manager> managers = managerService.findAll();
        String [] result = new String[managers.size()];
        for (int i = 0; i < managers.size(); i++)
            result[i] = managers.get(i).getID() + " " +  managers.get(i).getName() + " " + managers.get(i).getSurname();
        return result;
    }

    public String[] getClients()
    {
        List<Client> clients = clientService.findAll();
        String [] result = new String[clients.size()];
        for (int i = 0; i < clients.size(); i++)
            result[i] = clients.get(i).getID() + " " + clients.get(i).getName() + " " + clients.get(i).getSurname();
        return result;
    }

}
