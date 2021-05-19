package Factory.entity;

import javax.persistence.*;

/**
 * Абстрактный класс человека
 * @author Яловега Никита 9308
 * @version 0.1
 */
@MappedSuperclass
public abstract class Person
{
    /** Поле уникального идентификатора */
    @Id
    @Column(name="id")
    @GeneratedValue(strategy=GenerationType.IDENTITY)
    protected int id;

    /** Поле имени человека */
    @Column(name="name")
    protected String name;

    /** Поле фимилии человека */
    @Column(name="surname")
    protected String surname;

    public Person() {}

    /**
     * Конструктор - создание нового объекта Person
     * @param name - имя
     * @param surname - фамилия
     */
    public Person(String name, String surname)
    {
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
     * Функция определения значения поля {@link Person#id}
     * @param newID - новый идентификатор пользователя
     */
    public void setID(int newID)
    {
        id = newID;
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
     * @param newSurname - новая фамилия человека
     */
    public void setSurname(String newSurname)
    {
        surname = newSurname;
    }

    public abstract String[] toTableFormat();
}
