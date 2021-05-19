package Factory.entity;

import java.util.*;
import javax.persistence.*;

/**
 * Класс клиента завода по производству металлических изделий.
 * Наследник класса {@link Person}
 * @author Яловега Никита 9308
 * @version 0.1
 */
@Entity(name = "clients")
@Table(name = "clients")
public class Client extends Person
{
    /** Название компании клиента */
    @Column(name="company")
    private String company;

    /** Контракты клиента */
    @OneToMany(fetch = FetchType.EAGER, orphanRemoval = false)
    private List<Contract> client_contracts;

    public Client() { }

    /**
     * Конструктор - создание нового объекта {@link Client}
     * @param name - имя
     * @param lastName - фамилия
     * @param company - компания
     * @param contract - контракты
     */
    public Client(String name, String lastName, String company, List<Contract> contract)
    {
        super(name, lastName);
        this.company = company;
        this.client_contracts = contract;
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
        client_contracts.add(newContract);
        newContract.setClient(this); // связываем контракт с этим клиентом
    }

    /**
     * Функция получения значения поля {@link Client#client_contracts}
     * @return возвращает контракты клиента
     */
    public List<Contract> getContracts()
    {
        return client_contracts;
    }

    /**
     * Процедура удаления контрактов
     * @param c - контракт
     */
    public void removeContract(Contract c)
    {
        client_contracts.remove(c);
        c.setClient(null);
    }

    public String[] toTableFormat()
    {
        return new String[] {String.valueOf(id), name, surname, company};
    }
}
