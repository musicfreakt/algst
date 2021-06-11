package Factory.exceptions;

/**
 * Класс исключения.
 * Невозможный временной отрезок
 */
public class UnacceptableTimePeriod extends Exception {

    /** Конструктор ошибки */
    public UnacceptableTimePeriod ()
    {
        super("Некорректный временной промежуток. Попробуйте заного");
    }
}

