package Factory.gui;

import Factory.util.DateLabelFormatter;

import org.jdatepicker.impl.*;
import java.util.Date;
import java.util.Properties;
import javax.swing.*;

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
        parent.addR(arr,(Date) dataBegin.getModel().getValue(), (Date) dataEnd.getModel().getValue());

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
        dataBegin = new JDatePickerImpl(datePanel1, new DateLabelFormatter());
        dataEnd = new JDatePickerImpl(datePanel2, new DateLabelFormatter());
    }

    public AddDialogContract(JFrame owner, ContractWindow parent, String title)
    {
        super(owner,parent,title);
    }
}


