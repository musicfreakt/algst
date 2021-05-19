package Factory.entity;

import java.util.List;
import javax.persistence.*;

/**
 * Класс сотрудника завода по производству металлических изделий.
 * Наследник класса {@link Person}
 * @author Яловега Никита 9308
 * @version 0.1
 */
@Entity(name = "employees")
@Table(name = "employees")
public class Employee extends Person
{
    /** Поле опыта работы */
    @Column(name="exp")
    private int exp;

    /** Поле профессии рабочего */
    @OneToOne (optional = false)
    @JoinColumn (name="specialisation_id")
    private Specialisation specialisation;

    /** Контракты, в которых участвует рабочий */
    @ManyToMany(cascade = { CascadeType.ALL })
    @JoinTable(
            name = "employee_contract",
            joinColumns = { @JoinColumn(name = "employees_id") },
            inverseJoinColumns = { @JoinColumn(name = "contract_id") }
    )
    private List<Contract> contracts;

    public Employee(){}

    /**
     * Конструктор - создание нового объекта Employee
     * @param name - имя
     * @param lastName - фамилия
     * @param exp - опыт работы
     */
    public Employee(String name, String lastName, int exp)
    {
        super(name, lastName);
        this.exp = exp;
    }

    /**
     * Функция получения значения поля {@link Employee#exp}
     * @return возвращает опыт работы рабочего
     */
    public int getWorkExp()
    {
        return exp;
    }

    /**
     * Процедура определения значения поля {@link Employee#exp}
     * @param newexp - новая фамилия человека
     */
    public void setWorkExp(int newexp)
    {
        exp = newexp;
    }

    /**
     * Процедура определения значения поля {@link Employee#specialisation}
     * @param d - профессия сотрудника
     */
    public void setSpecialisation(Specialisation d)
    {
        specialisation = d;
    }

    /**
     * Функция получения значения поля {@link Employee#specialisation}
     * @return возвращает профессию сотрудника
     */
    public Specialisation getSpecialisation()
    {
        return specialisation;
    }

    /**
     * Процедура добавления новых контрактов рабочему.
     * @param newContract - новый контракт, который выполняет рабочий
     */
    public void addContract(Contract newContract)
    {
        contracts.add(newContract);
        newContract.addWorker(this); // добавляем в контракт рабочего
    }

    /**
     * Функция получения значения поля {@link Employee#contracts}
     * @return возвращает контракты, который выполняет рабочий
     */
    public List<Contract> getContracts()
    {
        return contracts;
    }

    /**
     * Процедура удаления контрактов, которые выполняет рабочий
     * @param c - контракт
     */
    public void removeContract(Contract c)
    {
        contracts.remove(c);
        c.removeWorker(this);
    }

    public String[] toTableFormat()
    {
        return new String[] {String.valueOf(id), name, surname, String.valueOf(exp), specialisation.getName()};
    }

}
