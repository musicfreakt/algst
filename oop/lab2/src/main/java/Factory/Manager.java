package Factory;

import java.util.List;
import javax.persistence.*;

/**
 * Класс менеджера завода по производству металлических изделий.
 * Наследник класса {@link Person}
 * @author Яловега Никита 9308
 * @version 0.1
 */
@Entity
@Table(name = "persons")
@DiscriminatorValue(value = "M")
public class Manager extends Person
{
    /** Контракты, в которые подписывал менеджер */
    @OneToMany(fetch = FetchType.EAGER, cascade = CascadeType.ALL, orphanRemoval = true)
    private List<Contract> contracts;

    /**
     * Конструктор - создание нового объекта Employee
     * @param id - идентификатор
     * @param name - имя
     * @param lastName - фамилия
     */
    public Manager(int id, String name, String lastName)
    {
        super(id, name, lastName);
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

}
