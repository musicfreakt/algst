package Factory;

import java.util.*;
import javax.persistence.*;

/**
 * Класс клиента завода по производству металлических изделий.
 * Наследник класса {@link Person}
 * @author Яловега Никита 9308
 * @version 0.1
 */
@Entity
@Table(name = "persons")
@DiscriminatorValue(value = "C")
public class Client extends Person
{
    /** Название компании клиента */
    @Column(name="company")
    private String company;

    /** Контракты клиента */
    @OneToMany(fetch = FetchType.EAGER, cascade = CascadeType.ALL, orphanRemoval = true)
    private ArrayList<Contract> contracts;

    /**
     * Конструктор - создание нового объекта {@link Client}
     * @param id - идентификатор
     * @param name - имя
     * @param lastName - фамилия
     * @param company - компания
     */
    public Client(int id, String name, String lastName, String company)
    {
        super(id, name, lastName);
        this.company = company;
    }

    /**
     * Функция получения значения поля {@link Client#company}
     * @return возвращает название компании клиента
     */
    public String getCompany()
    {
        return company;
    }

    /**
     * Процедура определения значения поля {@link Client#company}
     * @param newCompany - новая название компании клиенита
     */
    public void setCompany(String newCompany)
    {
        company = newCompany;
    }

    /**
     * Процедура добавления новых контрактов клиента
     * @param newContract - новый контракт
     */
    public void addContract(Contract newContract)
    {
        contracts.add(newContract);
        newContract.setClient(this); // связываем контракт с этим клиентом
    }

    /**
     * Функция получения значения поля {@link Client#contracts}
     * @return возвращает контракты клиента
     */
    public ArrayList<Contract> getContracts()
    {
        return contracts;
    }

    /**
     * Процедура удаления контрактов
     * @param c - контракт
     */
    public void removeContract(Contract c)
    {
        contracts.remove(c);
        c.setClient(null);
    }
}
