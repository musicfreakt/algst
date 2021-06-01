package Factory.exceptions;

/**
 * Класс исключения. Запрет на запись в файл пустого списка
 */
public class UnacceptableTimePeriod extends Exception {
    public UnacceptableTimePeriod ()
    {
        super("Некорректный временной промежуток. Попробуйте заного");
    }
}

