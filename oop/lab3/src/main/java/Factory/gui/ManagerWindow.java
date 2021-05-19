package Factory.gui;

import Factory.entity.*;
import javax.persistence.*;
import org.hibernate.Session;
import org.hibernate.SessionFactory;

import javax.swing.*;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.util.List;


public class ManagerWindow {

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

    private AddDialogManager addDialogManager;
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
        String[] columns = {"ID", "Имя", "Фамилия"};

//        Session session = sessionFactory.getCurrentSession();

//        List<Manager> managersList = em.createQuery("SELECT m FROM managers m").getResultList();
//        String [][] data = new String[managersList.size()][5];
        String [][] data = new String[0][5];
//        for (int i = 0; i < managersList.size(); i++)
//        {
//            data[i] = managersList.get(i).toTableFormat();
//        }

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
//            if (dataManagers.getRowCount() > 0) {
//                if (dataManagers.getSelectedRow() != -1) {
//                    try {
//                        model.removeRow(dataManagers.convertRowIndexToModel(dataManagers.getSelectedRow()));
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
//                if (dataManagers.getSelectedRow() != -1) {
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
//                employs.print("dataManagers.xml", "window/dataManagers", "prod.jrxml", "otchetProd.pdf");
//            }
        });

        // Если не выделена строка, то прячем кнопки
        dataManagers.getSelectionModel().addListSelectionListener((e) -> {
//            Boolean check = true;
//            if (dataManagers.getSelectionModel().isSelectionEmpty()) {
//                check = false;
//            }
//            edit.setVisible(check);
//            delete.setVisible(check);
        });

        window.setVisible(true);
    }
    public void addR(String[] arr)
    {
        model.addRow(arr);
    }

}
