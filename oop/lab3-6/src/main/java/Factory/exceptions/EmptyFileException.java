package Factory.exceptions;

/**
 * Класс исключения.
 * Запрет на запись в файл пустого списка
 * @author Яловега Никита 9308
 * @version 0.6
 */
public class EmptyFileException extends Exception {

    /** Конструктор ошибки */
    public EmptyFileException ()
    {
        super("Запись в файл невозможна! Список пуст");
    }
}

