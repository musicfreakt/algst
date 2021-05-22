package Factory.gui;

import org.jdatepicker.impl.*;

import javax.swing.*;
import java.util.Date;
import java.util.Properties;

/**
 * Класс окна Добавления данных
 */
public class AddDialogContract extends DialogContract {

    @Override
    public void progress(ContractWindow parent) {
        setVisible(false);
        String[] arr = {description.getText(),
                        price.getText(),
                        (String) clients.getSelectedItem(),
                        (String) managers.getSelectedItem()};
        parent.addR(arr, (Date) dataBegin.getModel().getValue(), (Date) dataEnd.getModel().getValue());

        JOptionPane.showMessageDialog(null, "Вы добавили новый договор");
    }

    @Override
    public void init(ContractWindow parent)
    {
        description = new JTextField(20);
        price = new JTextField(20);
        clients = new JComboBox(parent.getClients());
        managers = new JComboBox(parent.getManagers());

        UtilDateModel model1 = new UtilDateModel();
        UtilDateModel model2 = new UtilDateModel();
        Properties p = new Properties();
        p.put("text.today", "Today");
        p.put("text.month", "Month");
        p.put("text.year", "Year");
        JDatePanelImpl datePanel1 = new JDatePanelImpl(model1, p);
        JDatePanelImpl datePanel2 = new JDatePanelImpl(model2, p);
        dataBegin = new JDatePickerImpl(datePanel1, null);
        dataEnd = new JDatePickerImpl(datePanel2, null);
    }

    public AddDialogContract(JFrame owner, ContractWindow parent, String title)
    {
        super(owner,parent,title);
    }
}


