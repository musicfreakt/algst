package Factory.gui;

import Factory.util.DateLabelFormatter;
import org.jdatepicker.impl.JDatePanelImpl;
import org.jdatepicker.impl.JDatePickerImpl;
import org.jdatepicker.impl.UtilDateModel;

import javax.swing.*;
import java.text.ParseException;
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
        String dateValue = parent.dataContracts.getValueAt(row, 5).toString();
        Date date1 = null;
        Date date2 = null;
        try {
            date1 = new SimpleDateFormat("yyyy-mm-dd").parse(dateValue);
            dateValue = parent.dataContracts.getValueAt(row, 6).toString();
            date2 = new SimpleDateFormat("yyyy-mm-dd").parse(dateValue);
        } catch (ParseException e) {
            e.printStackTrace();
        }
        UtilDateModel model1 = new UtilDateModel(date1);
        UtilDateModel model2 = new UtilDateModel(date2);
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


