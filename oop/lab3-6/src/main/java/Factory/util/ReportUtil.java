package Factory.util;

import net.sf.jasperreports.engine.*;
import net.sf.jasperreports.engine.data.JRXmlDataSource;
import net.sf.jasperreports.engine.design.JasperDesign;
import net.sf.jasperreports.engine.xml.JRXmlLoader;

import java.io.File;

/**
 * Класс для генерации отчетов
 */
public class ReportUtil
{
    /**
     * Метод генерации отчетов в формате PDF.
     * @param datasource Имя файла XML с данными
     * @param xpath Директория до полей с данными
     * @param template Имя файла шаблона .jrxml
     * @param resultpath Имя файла, в который будет помещен отчет
     */
    public static void print(String datasource, String xpath, String template, String resultpath) throws JRException
    {
        File reportPattern = new File(template);

        JasperDesign jasperDesign = JRXmlLoader.load(reportPattern);
        JasperReport jasperReport = JasperCompileManager.compileReport(jasperDesign);
        JRDataSource jr = new JRXmlDataSource(datasource, xpath);
        JasperPrint jasperPrint = JasperFillManager.fillReport(jasperReport, null, jr);
        JasperExportManager.exportReportToPdfFile(jasperPrint, resultpath);
    }
}


