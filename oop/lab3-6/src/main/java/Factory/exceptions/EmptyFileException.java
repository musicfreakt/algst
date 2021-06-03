package Factory.exceptions;

/**
 * Класс исключения.
 * Запрет на запись в файл пустого списка
 */
public class EmptyFileException extends Exception {

    /** Конструктор ошибки */
    public EmptyFileException ()
    {
        super("Запись в файл невозможна! Список пуст");
    }
}

