package Factory.gui;

import javax.swing.*;
import java.awt.*;

/** Класс главного меню */
public class Menu
{
    /** Метод показа главного меню */
    public void show()
    {
        JFrame window = new JFrame("factory: Главное окно");
        JButton specialisationWindow = new JButton("Список доступных профессий");
        JButton workerWindow = new JButton("Список работников завода"); // , new ImageIcon("./img/prodList.png")
        JButton managerWindow = new JButton("Список менеджеров завода");
        JButton clientWindow = new JButton("Список клиентов завода");
        JButton contractWindow = new JButton("Список договоров завода");
        JButton refWindow = new JButton("Открыть справку о программе");

        specialisationWindow.addActionListener((e) -> {
            try {
                new SpecialisationWindow();
            } catch (Exception ex) {
                JOptionPane.showMessageDialog(null, "Ошибка: " + ex);
            }
        });

        workerWindow.addActionListener((e) -> {
            try {
                new WorkerWindow();
            } catch (Exception ex) {
                JOptionPane.showMessageDialog(null, "Ошибка: " + ex);
            }
        });

        managerWindow.addActionListener((e) -> {
            try {
                new ManagerWindow();
            } catch (Exception ex) {
                JOptionPane.showMessageDialog(null, "Ошибка: " + ex);
            }
        });
        clientWindow.addActionListener((e) -> {
            try {
                new ClientWindow();
            } catch (Exception ex) {
                JOptionPane.showMessageDialog(null, "Ошибка: " + ex);
            }
        });
        contractWindow.addActionListener((e) -> {
            try {
                new ContractWindow();
            } catch (Exception ex) {
                JOptionPane.showMessageDialog(null, "Ошибка: " + ex);
            }
        });

        refWindow.addActionListener((e) -> new Ref(window, "Справка"));

        window.setLayout(new GridBagLayout());
        JPanel mainp = new JPanel();

        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(7, 1));

        panel.setSize(350, 100);

        panel.add(specialisationWindow);
        panel.add(workerWindow);
        panel.add(managerWindow);
        panel.add(clientWindow);
        panel.add(contractWindow);
        panel.add(refWindow);
        mainp.add(panel);
        window.add(mainp);
        window.setSize(400, 380);
        window.setLocation(500, 200);
        window.setVisible(true);
        window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }
}
