package Factory.gui;

import javax.swing.*;
import java.awt.*;

public class Menu
{
    public void show()
    {
        JFrame window = new JFrame("Главное окно");
        JButton specialisationWindow = new JButton("Список доступных профессий");
        JButton workerWindow = new JButton("Список работников завода"); // , new ImageIcon("./img/prodList.png")
        JButton managerWindow = new JButton("Список менеджеров завода");
        JButton clientWindow = new JButton("Список клиентов завода");
        JButton contractWindow = new JButton("Список договоров завода");
        JButton reportWindow = new JButton("Отчеты по работе завода");
        JButton refWindow = new JButton("Открыть справку о программе");

        specialisationWindow.addActionListener((e) -> new SpecialisationWindow());
        workerWindow.addActionListener((e) -> new WorkerWindow());
        managerWindow.addActionListener((e) -> new ManagerWindow());
        clientWindow.addActionListener((e) -> new ClientWindow());
        contractWindow.addActionListener((e) -> new ContractWindow());
//            reportWindow.addActionListener((e)-> new report());
        refWindow.addActionListener((e) -> new Ref(window, "Справка"));

        window.setLayout(new FlowLayout());
        JPanel mainp = new JPanel();

        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(7, 1));

        panel.setSize(350, 100);

        // adds to the GridLayout
        panel.add(specialisationWindow);
        panel.add(workerWindow);
        panel.add(managerWindow);
        panel.add(clientWindow);
        panel.add(contractWindow);
        panel.add(reportWindow);
        panel.add(refWindow);
        mainp.add(panel);
        window.add(BorderLayout.CENTER, mainp);
        window.setSize(400, 380);
        window.setLocation(500, 200);
        window.setVisible(true);
        window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }
}
