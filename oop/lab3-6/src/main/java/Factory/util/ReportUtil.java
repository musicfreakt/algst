package Factory.util;

import net.sf.jasperreports.engine.*;
import net.sf.jasperreports.engine.data.JRXmlDataSource;
import net.sf.jasperreports.engine.export.JRPdfExporter;

/**
 * Класс для генерации отчетов
 */
public class ReportUtil
{
    /**
     * Метод генерации отчетов в форматах DOCX и HTML.
     * @param datasource Имя файла XML с данными
     * @param xpath Директория до полей с данными
     * @param template Имя файла шаблона .jrxml
     * @param resultpath Имя файла, в который будет помещен отчет
     */
    public static void print(String datasource, String xpath, String template, String resultpath)
    {
        try
        {
            // Указание источника XML-данных
            JRDataSource jr = new JRXmlDataSource(datasource, xpath);
            // Создание отчета на базе шаблона
            JasperReport report = JasperCompileManager.compileReport(template);
            // Заполнение отчета данными
            JasperPrint print = JasperFillManager.fillReport(report, null, jr);

            if(resultpath.toLowerCase().endsWith("pdf")) {
                JRExporter exporter;
                exporter = new JRPdfExporter();
                exporter.setParameter(JRExporterParameter.OUTPUT_FILE_NAME,resultpath);
                exporter.setParameter(JRExporterParameter.JASPER_PRINT,print);
                exporter.exportReport();
            }
            else
                JasperExportManager.exportReportToHtmlFile(print,resultpath);
        }
        catch (JRException e)
        {
            e.printStackTrace();
        }
    }
}


