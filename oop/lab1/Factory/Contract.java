package Factory;

import java.sql.Date;
import java.util.List;

/**
 * Класс контракта завода по производству металлических изделий.
 * @author Яловега Никита 9308
 * @version 0.1
*/
public class Contract
{
    /** Уникальный идентификатор контракта */
    private int id;

    /** Описание условий контракта */
    private String description;

    /** Цена контракта */
    private double price;

    /** Клиент, подписавший контракт */
    private Client client;

    /** Менеджер, подписавший контракт */
    private Employee manager;

    /** Рабочие, выполняющие условия контракта */
    private List<Employee> workers;

    /** Дата начала действия контракта */
    private Date dateBegin;

    /** Дата окончания действия контракта */
    private Date dateEnd;

    /**
     * Конструктор - создание нового объекта {@link Contract}
     * @param id - идентификатор
     * @param d - описание
     * @param p - цена
     * @param c -
     */
    public Contract(int id, String d, double p, List<Employee> w, Date b, Date e)
    {
        this.id = id;
        this.description = d;
        this.price = p;
        this.workers = w;
        this.dateBegin = b;
        this.dateEnd = e;
    }

    /**
     * Метод получения значения поля {@link Contract#id}
     * @return возвращает уникальный идентификатор контракта
     */
    public int getID()
    {
        return id;
    }

    /**
     * Метод определения значения поля {@link Contract#description}
     * @param newDescription - описание контракта
     */
    public void setDescription(String newDescription)
    {
        description = newDescription;
    }

    /**
     * Метод получения значения поля {@link Contract#description}
     * @return возвращает описание контракта
     */
    public String getDescription()
    {
        return description;
    }

    /**
     * Метод определения значения поля {@link Contract#price}
     * @param newPrice - цена контракта
     */
    public void setPrice(double newPrice)
    {
        price = newPrice;
    }

    /**
     * Метод получения значения поля {@link Contract#price}
     * @return возвращает цену контракта
     */
    public double getPrice()
    {
        return price;
    }

    /**
     * Метод определения значения поля {@link Contract#client}
     * @param newClient - новый клиент контракта
     */
    public void setClient(Client newClient)
    {
        client = newClient;
    }

    /**
     * Метод получения значения поля {@link Contract#client}
     * @return возвращает клиента
     */
    public Client getClient()
    {
        return client;
    }

    /**
     * Метод определения значения поля {@link Contract#manager}
     * @param newManager - новый менеджен контракта
     */
    public void setManager(Employee newManager)
    {
        manager = newManager;
    }

    /**
     * Метод получения значения поля {@link Contract#manager}
     * @return возвращает менеджера
     */
    public Employee getManager()
    {
        return manager;
    }

    /**
     * Метод определения значения поля {@link Contract#dateBegin}
     * @param newDate - новая дата
     */
    public void setDateBegin(Date newDate)
    {
        dateBegin = newDate;
    }

    /**
     * Метод получения значения поля {@link Contract#dateBegin}
     * @return возвращает дату начала
     */
    public Date getDateBegin()
    {
        return dateBegin;
    }

    /**
     * Метод определения значения поля {@link Contract#dateEnd}
     * @param newDate - новая дата
     */
    public void setDateEnd(Date newDate)
    {
        dateEnd = newDate;
    }

    /**
     * Метод получения значения поля {@link Contract#dateEnd}
     * @return возвращает дату начала
     */
    public Date getDateEnd()
    {
        return dateEnd;
    }

    /**
     * Метод добавления рабочего, который выполняет контракт {@link Contract#worker}
     * @param newWorker - новый рабочий
     */
    public void addWorker(Employee newWorker)
    {
        workers.add(newWorker);
    }

    /**
     * Метод удаления рабочего, который выполняет контракт {@link Contract#worker}
     * @param worker - рабочий
     */
    public void removeWorker(Employee worker)
    {
        workers.remove(worker);
    }

    /**
     * Метод получения значения поля {@link Contract#worker}
     * @return возвращает всех рабочих, выполняющих контракт
     */
    public List<Employee> getWorkers()
    {
        return workers;
    }

}
