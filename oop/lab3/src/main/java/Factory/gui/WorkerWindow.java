package Factory.gui;

import Factory.model.*;
import Factory.service.*;
import java.util.List;

//import net.sf.jasperreports.engine.*;
//import net.sf.jasperreports.engine.data.JRXmlDataSource;
//import net.sf.jasperreports.engine.export.JRPdfExporter;
//import org.apache.log4j.Logger;
//import org.w3c.dom.*;
//import org.xml.sax.SAXException;

import javax.swing.*;
import javax.swing.table.*;
import java.awt.*;
import java.awt.event.KeyEvent;

public class WorkerWindow
{

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

    /** Панель инструментов */
    private JToolBar toolBar;

    /** Таблица */
    protected JTable dataWorkers;

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
//    private static final Logger log = Logger.getLogger(ManagerWindow.class);

    private AddDialogWorker addDialogWorker;
    private EditDialogWorker editDialogWorker;

    public void show(){
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
        String[] columns = {"ID", "Имя", "Фамилия", "Опыт работы", "Специальность"};

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


        add.addActionListener((e) ->
        {
            addDialogWorker = new AddDialogWorker(window, WorkerWindow.this, "Добавление записи");
            addDialogWorker.setVisible(true);
        });

        add.setMnemonic(KeyEvent.VK_A);
        delete.addActionListener((e) -> {
            if (dataWorkers.getRowCount() > 0) {
                if (dataWorkers.getSelectedRow() != -1) {
                    try {
                        employeeService.delete(Integer.parseInt(dataWorkers.getValueAt(dataWorkers.getSelectedRow(), 0).toString()));
                        model.removeRow(dataWorkers.convertRowIndexToModel(dataWorkers.getSelectedRow()));
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
                if (dataWorkers.getSelectedRow() != -1) {
                    t1 = new Thread(() -> {
                        JOptionPane.showMessageDialog(null,"1 поток запущен");
                        editDialogWorker = new EditDialogWorker(window, WorkerWindow.this, "Редактирование");
                        editDialogWorker.setVisible(true);
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
//            if (model.getRowCount() != 0) {
//                employs.print("dataWorkers.xml", "window/dataWorkers", "prod.jrxml", "otchetProd.pdf");
//            }
        });

        // Если не выделена строка, то прячем кнопки
        dataWorkers.getSelectionModel().addListSelectionListener((e) -> {
            boolean check = !dataWorkers.getSelectionModel().isSelectionEmpty();
            edit.setVisible(check);
            delete.setVisible(check);
        });

        window.setVisible(true);
    }

    public String[] getSpecs()
    {
        List<Specialisation> specs = specialisationService.findAll();
        String [] result = new String[specs.size()];
        for (int i = 0; i < specs.size(); i++)
            result[i] = specs.get(i).getName();
        return result;
    }

    public void addR(String[] arr)
    {
        Employee newW = new Employee(arr[0], arr[1], Integer.parseInt(arr[2]), specialisationService.findByName(arr[3]));
        employeeService.persist(newW);
        model.addRow(newW.toTableFormat());
    }

    public void editR(String[] arr)
    {
        Employee W = employeeService.findById(Integer.parseInt(arr[0]));
        W.setName(arr[1]);
        W.setSurname(arr[2]);
        W.setWorkExp(Integer.parseInt(arr[3]));
        W.setSpecialisation(specialisationService.findByName(arr[4]));
        employeeService.update(W);
    }
}
