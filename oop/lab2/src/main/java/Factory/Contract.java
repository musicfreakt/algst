package Factory;

import java.sql.Date;
import java.util.List;
import javax.persistence.*;

/**
 * Класс контракта завода по производству металлических изделий.
 * @author Яловега Никита 9308
 * @version 0.1
*/
@Entity
@Table(name="contract")
public class Contract
{
    /** Уникальный идентификатор контракта */
    @Id
    @Column(name="id")
    @GeneratedValue(strategy=GenerationType.IDENTITY)
    private int id;

    /** Описание условий контракта */
    @Column(name="description")
    private String description;

    /** Цена контракта */
    @Column(name="price")
    private double price;

    /** Клиент, подписавший контракт */
    @ManyToOne (optional=false)
    @JoinColumn (name = "client_id")
    private Client client;

    /** Менеджер, подписавший контракт */
    @ManyToOne (optional=false)
    @JoinColumn (name = "manager_id")
    private Manager manager;

    /** Рабочие, выполняющие условия контракта */
    @ManyToMany(mappedBy = "contract")
    private List<Employee> workers;

    /** Дата начала действия контракта */
    @Column(name="begin")
    private Date dateBegin;

    /** Дата окончания действия контракта */
    @Column(name="end")
    private Date dateEnd;

    /**
     * Конструктор - создание нового объекта {@link Contract}
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
     * Функция определения значения поля {@link Contract#id}
     * @param newID - новый идентификатор
     */
    public void setID(int newID)
    {
        id = newID;
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
    public void setManager(Manager newManager)
    {
        manager = newManager;
    }

    /**
     * Метод получения значения поля {@link Contract#manager}
     * @return возвращает менеджера
     */
    public Manager getManager()
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
     * Метод добавления рабочего, который выполняет контракт {@link Contract#workers}
     * @param newWorker - новый рабочий
     */
    public void addWorker(Employee newWorker)
    {
        workers.add(newWorker);
    }

    /**
     * Метод удаления рабочего, который выполняет контракт {@link Contract#workers}
     * @param worker - рабочий
     */
    public void removeWorker(Employee worker)
    {
        workers.remove(worker);
    }

    /**
     * Метод получения значения поля {@link Contract#workers}
     * @return возвращает всех рабочих, выполняющих контракт
     */
    public List<Employee> getWorkers()
    {
        return workers;
    }

}
