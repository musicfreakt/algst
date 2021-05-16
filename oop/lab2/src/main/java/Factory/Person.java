package Factory;

//import javax.persistence.*;

//@Entity
//@Table(name = "kurs.persons")
//@Inheritance(strategy = InheritanceType.SINGLE_TABLE)
//@DiscriminatorColumn(name = "discriminator", discriminatorType = DiscriminatorType.STRING)
//@DiscriminatorValue(value = "P")
/**
 * Абстрактный класс человека
 * @author Яловега Никита 9308
 * @version 0.1
*/
public abstract class Person
{
//    @Id
//    @Column(name="id")
//    @GeneratedValue(strategy=GenerationType.IDENTITY)
    /** Поле уникального идентификатора */
    protected int id;

//    @Column(name="name")
    /** Поле имени человека */
    protected String name;

//    @Column(name="surname")
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
}
