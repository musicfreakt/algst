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
    private List<Contract> contracts;

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
        contracts.add(newContract);
        newContract.setManager(this); // добавляем в контракт рабочего
    }

    /**
     * Функция получения значения поля {@link Manager#contracts}
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
        c.setManager(null);
    }

    /**
     * Процедура установки контрактов, которые выполняет рабочий
     * @param c - список контракт
     */
    public void setContract(List<Contract> c)
    {
        contracts = c;
    }

    /**
     * Функция получения всей информации об объекте
     * @return  - массив строк с данными
     */
    public String[] toTableFormat()
    {
        return new String[] {String.valueOf(id), name, surname};
    }
}
