package Factory;

/**
 * Абстрактный класс человека
 * @author Яловега Никита 9308
 * @version 0.1
*/
public abstract class Person
{
    /** Поле уникального идентификатора */
    protected int id;

    /** Поле имени человека */
    protected String name;

    /** Поле фимилии человека */
    protected String surname;

    /**
     * Конструктор - создание нового объекта Person
     * @param id - идентификатор
     * @param name - имя
     * @param surname - фамилия
     */
    public Person(int id, String name, String surname)
    {
        this.id = id;
        this.name = name;
        this.surname = surname;
    }

    /**
     * Функция получения значения поля {@link Person#id}
     * @return возвращает уникальный идентификатор человека
     */
    public int getID()
    {
        return id;
    }

    /**
     * Функция получения значения поля {@link Person#name}
     * @return возвращает имя человека
     */
    public String getName()
    {
        return name;
    }

    /**
     * Процедура определения значения поля {@link Person#name}
     * @param newName - новое имя человека
     */
    public void setName(String newName)
    {
        name = newName;
    }

    /**
     * Функция получения значения поля {@link Person#surname}
     * @return возвращает фамилию человека
     */
    public String getSurname()
    {
        return surname;
    }

    /**
     * Процедура определения значения поля {@link Person#name}
     * @param newName - новая фамилия человека
     */
    public void setSurname(String newSurname)
    {
        surname = newSurname;
    }
}
