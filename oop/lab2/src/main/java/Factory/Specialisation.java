package Factory;

import javax.persistence.*;

//@Entity
//@Table(name = "kurs.specialisations")
/**
 * Класс специализации работника
 * @author Яловега Никита 9308
 * @version 0.1
 */
public class Specialisation
{
    private int id;
    private String name;

//    @Id
//    @Column(name="id")
//    @GeneratedValue(strategy=GenerationType.IDENTITY)

    /**
     * Функция получения значения поля {@link Specialisation#id}
     * @return возвращает уникальный идентификатор
     */
    public int getID()
    {
        return id;
    }

    /**
     * Функция определения значения поля {@link Specialisation#id}
     * @param newID - новый идентификатор
     */
    public void setID(int newID)
    {
        id = newID;
    }

    /**
     * Функция получения значения поля {@link Specialisation#name}
     * @return возвращает название специализации
     */
    public String getName()
    {
        return name;
    }

    /**
     * Процедура определения значения поля {@link Specialisation#name}
     * @param newName - новое название специализации
     */
    public void setName(String newName)
    {
        name = newName;
    }
}
