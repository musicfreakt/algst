package Factory.model;

import javax.persistence.*;
import java.util.List;

/**
 * Класс специализации работника
 * @author Яловега Никита 9308
 * @version 0.1
 */
@Entity(name = "specialisations")
@Table(name = "specialisations")
public class Specialisation
{

    /** Уникальный идентификатор профессии */
    @Id
    @Column(name="id")
    @GeneratedValue(strategy=GenerationType.IDENTITY)
    private int id;

    /** Название профессии */
    @Column(name="name")
    private String name;

    /** Рабочие данной профессии */
    @OneToMany(mappedBy = "specialisation", cascade = CascadeType.ALL, orphanRemoval = true)
    private List<Employee> employees;

    public Specialisation(){}

    public Specialisation(String name)
    {
        this.name = name;
    }

    /**
     * Метод получения значения поля {@link Specialisation#id}
     * @return возвращает уникальный идентификатор
     */
    public int getID()
    {
        return id;
    }

    /**
     * Метод определения значения поля {@link Specialisation#id}
     * @param newID - новый идентификатор
     */
    public void setID(int newID)
    {
        id = newID;
    }

    /**
     * Метод получения значения поля {@link Specialisation#name}
     * @return возвращает название специализации
     */
    public String getName()
    {
        return name;
    }

    /**
     * Метод определения значения поля {@link Specialisation#name}
     * @param newName - новое название специализации
     */
    public void setName(String newName)
    {
        name = newName;
    }

    /**
     * Метод добавления новых работников {@link Specialisation#employees}
     * @param newEmployee - новый работник
     */
    public void addEmployee(Employee newEmployee)
    {
        employees.add(newEmployee);
        // связываем сотрудника с этим отделом
        newEmployee.setSpecialisation(this);
    }

    /**
     * Метод получения значения поля {@link Specialisation#employees}
     * @return Работники данной профессии
     */
    public List<Employee> getEmployees()
    {
        return employees;
    }

    /**
     * Метод удаления работников из профессии {@link Specialisation#employees}
     * @param e - работник, которого нужно убрать
     */
    public void removeEmployee(Employee e)
    {
        employees.remove(e);
    }

    /**
     * Функция получения всей информации об объекте
     * @return  - массив строк с данными
     */
    public String[] toTableFormat()
    {
        return new String[] {String.valueOf(getID()), getName()};
    }
}
