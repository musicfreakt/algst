package Factory.gui;

import Factory.model.*;

import javax.persistence.*;
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

public class WorkerWindow {

    WorkerWindow()
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
    protected JTable dataWorkers;
    /**
     * Скролл
     */
    private JScrollPane scroll;

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

//    private SessionFactory sessionFactory;

    final static public Object shared=new Object();

    /**
     * Логгер класса employs
     */
//    private static final Logger log = Logger.getLogger(employs.class);

//    private EntityManagerFactory emf = Persistence.createEntityManagerFactory("test_persistence");
//    private EntityManager em = emf.createEntityManager();

//    private AddDialogProd addDialogProd;
//    private EditDialogProd editDialogProd;

//    public void setSessionFactory(SessionFactory sessionFactory)
//    {
//        this.sessionFactory = sessionFactory;
//    }

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


//        List<Employee> workersList = em.createQuery("SELECT e FROM employees e").getResultList();
//        String [][] data = new String[workersList.size()][5];
        String [][] data = new String[0][5];
//        for (int i = 0; i < workersList.size(); i++)
//        {
//            data[i] = workersList.get(i).toTableFormat();
//        }

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


        add.addActionListener((e) -> {
//            addDialogProd = new AddDialogProd(window, products.this, "Добавление записи");
//            addDialogProd.setVisible(true);
        });

        add.setMnemonic(KeyEvent.VK_A);
        delete.addActionListener((e) -> {
//            if (dataWorkers.getRowCount() > 0) {
//                if (dataWorkers.getSelectedRow() != -1) {
//                    try {
//                        model.removeRow(dataWorkers.convertRowIndexToModel(dataWorkers.getSelectedRow()));
//                        JOptionPane.showMessageDialog(window, "Вы удалили строку");
//                    } catch (Exception ex) {
//                        JOptionPane.showMessageDialog(null, "Ошибка");
//                    }
//                } else {
//                    JOptionPane.showMessageDialog(null, "Вы не выбрали строку для удаления");
//                }
//            } else {
//                JOptionPane.showMessageDialog(null, "В данном окне нет записей. Нечего удалять");
//            }
        });

        delete.setMnemonic(KeyEvent.VK_D);

        edit.addActionListener((e)-> {
//            if (t1.isAlive()) {
//                try {
//                    JOptionPane.showMessageDialog(window, "Ждем, пока отработает 1 поток");
//                    t1.join();
//                    JOptionPane.showMessageDialog(window, "1 поток отработал, пробуем запустить 2 поток");
//                } catch (InterruptedException ex) {
//                    ex.printStackTrace();
//                }
//            }
//            if (model.getRowCount() != 0) {
//                if (dataWorkers.getSelectedRow() != -1) {
//                    t2 = new Thread(() -> {
//                        JOptionPane.showMessageDialog(null,"2 поток запущен");
//                        editDialogProd = new EditDialogProd(window, products.this, "Редактирование");
//                        editDialogProd.setVisible(true);
//                    });
//                    t2.start();
//                } else {
//                    JOptionPane.showMessageDialog(null, "Не выбрана строка. Нечего редактировать");
//                }
//            } else {
//                JOptionPane.showMessageDialog(null, "В данном окне нет записей. Нечего редактировать");
//            }
        });
        edit.setMnemonic(KeyEvent.VK_E);
        
        print.addActionListener((e)->{
//            if (model.getRowCount() != 0) {
//                employs.print("dataWorkers.xml", "window/dataWorkers", "prod.jrxml", "otchetProd.pdf");
//            }
        });

        // Если не выделена строка, то прячем кнопки
        dataWorkers.getSelectionModel().addListSelectionListener((e) -> {
//            Boolean check = true;
//            if (dataWorkers.getSelectionModel().isSelectionEmpty()) {
//                check = false;
//            }
//            edit.setVisible(check);
//            delete.setVisible(check);
        });

        window.setVisible(true);
    }
    public void addR(String[] arr){
        model.addRow(arr);
    }

}
