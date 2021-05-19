package Factory.model;

import java.util.List;
import javax.persistence.*;

/**
 * Класс менеджера завода по производству металлических изделий.
 * Наследник класса {@link Person}
 * @author Яловега Никита 9308
 * @version 0.1
 */
@Entity(name = "managers")
@Table(name = "managers")
public class Manager extends Person
{
    /** Контракты, в которые подписывал менеджер */
    @OneToMany(mappedBy = "manager")
    private List<Contract> manager_contracts;

    public Manager() {}

    /**
     * Конструктор - создание нового объекта Manager
     * @param name - имя
     * @param lastName - фамилия
     */
    public Manager(String name, String lastName)
    {
        super(name, lastName);
    }

    /**
     * Процедура добавления новых контрактов рабочему.
     * @param newContract - новый контракт, который выполняет рабочий
     */
    public void addContract(Contract newContract)
    {
        manager_contracts.add(newContract);
        newContract.setManager(this); // добавляем в контракт рабочего
    }

    /**
     * Функция получения значения поля {@link Manager#manager_contracts}
     * @return возвращает контракты, который выполняет рабочий
     */
    public List<Contract> getContracts()
    {
        return manager_contracts;
    }

    /**
     * Процедура удаления контрактов, которые выполняет рабочий
     * @param c - контракт
     */
    public void removeContract(Contract c)
    {
        manager_contracts.remove(c);
        c.setManager(null);
    }

    public String[] toTableFormat()
    {
        return new String[] {String.valueOf(id), name, surname};
    }

    @Override
    public String toString()
    {
        return name + " " + surname;
    }
}
