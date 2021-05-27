package Factory.gui;

import Factory.service.DateLabelFormatter;
import org.jdatepicker.impl.JDatePanelImpl;
import org.jdatepicker.impl.JDatePickerImpl;
import org.jdatepicker.impl.UtilDateModel;

import javax.swing.*;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Properties;

/**
 * Класс окна Изменения данных
 */
public class EditDialogContract extends DialogContract
{
    @Override
    public void progress(ContractWindow parent)
    {
        setVisible(false);
        int row = parent.dataContracts.getSelectedRow();
        String[] arr = {parent.dataContracts.getValueAt(row, 0).toString(),
                description.getText(),
                price.getText(),
                (String) clients.getSelectedItem(),
                (String) managers.getSelectedItem()
        };
        parent.dataContracts.setValueAt(description.getText(), row, 1);
        parent.dataContracts.setValueAt(price.getText(), row, 2);
        parent.dataContracts.setValueAt(clients.getSelectedItem(), row, 3);
        parent.dataContracts.setValueAt(managers.getSelectedItem(), row, 4);
        parent.dataContracts.setValueAt(new SimpleDateFormat("yyyy-MM-dd").format((Date) dataBegin.getModel().getValue()), row, 5);
        parent.dataContracts.setValueAt(new SimpleDateFormat("yyyy-MM-dd").format((Date) dataEnd.getModel().getValue()), row, 6);

        parent.editR(arr,(Date) dataBegin.getModel().getValue(), (Date) dataEnd.getModel().getValue());
    }

    @Override
    public void init(ContractWindow parent)
    {
        int row = parent.dataContracts.getSelectedRow();
        description = new JTextField(parent.dataContracts.getValueAt(row, 1).toString(), 20);
        price = new JTextField(parent.dataContracts.getValueAt(row, 2).toString(), 20);
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
        checkerFloat(0, price);
    }

    public EditDialogContract(JFrame owner, ContractWindow parent, String title)
    {
        super(owner, parent, title);
    };
}


