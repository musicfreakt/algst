package kz.leti;

import java.util.ArrayList;

/**
 * Класс сотрудника завода по производству металлических изделий.
 * Наследник класса {@link Person}
 * @author Яловега Никита 9308
 * @version 0.1
*/
public class Employee extends Person
{
    /** Поле опыта работы */
    private int wexp;

    /** Поле профессии рабочего */
    private int specialisation;

    /** Контракты, в которых участвует рабочий */
    private ArrayList<Contract> contracts;

    /**
     * Конструктор - создание нового объекта Employee
     * @param id - идентификатор
     * @param name - имя
     * @param surname - фамилия
     * @param wexp - опыт работы
     */
    public Employee(int id, String name, String lastName, int wexp)
    {
        super(id, name, lastName);
        this.wexp = wexp;
    }

    /**
     * Функция получения значения поля {@link Employee#wexp}
     * @return возвращает опыт работы рабочего
     */
    public int getWorkExp()
    {
        return wexp;
    }

    /**
     * Процедура определения значения поля {@link Employee#wexp}
     * @param newWexp - новая фамилия человека
     */
    public void setWorkExp(int newWexp)
    {
        wexp = newWexp;
    }

    /**
     * Процедура определения значения поля {@link Employee#specialisation}
     * @param d - профессия сотрудника
     */
    public void setSpecialisation(int d)
    {
        specialisation = d;
    }

    /**
     * Функция получения значения поля {@link Employee#specialisation}
     * @return возвращает профессию сотрудника
     */
    public int getSpecialisation()
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
        if (specialisation == 1) // Если рабочий - менеджер
            newContract.setManager(this); // добавляем в контракт менеджера
        else
            newContract.addWorker(this); // добавляем в контракт рабочего
    }

    /**
     * Функция получения значения поля {@link Client#contracts}
     * @return возвращает контракты, который выполняет рабочий
     */
    public ArrayList<Contract> getContracts()
    {
        return contracts;
    }

    /**
     * Процедура удаления контрактов, которые выполняет рабочий
     * @param с - контракт
     */
    public void removeContract(Contract c)
    {
        contracts.remove(c);
        if (specialisation == 1) // Если рабочий - менеджер
            c.setManager(null);
        else
            c.removeWorker(this);
    }

}
