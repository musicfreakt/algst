package Factory.model;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import javax.persistence.*;

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
    private Manager manager;

    /** Рабочие, выполняющие условия контракта */
    private List<Employee> workers;

    /** Дата начала действия контракта */
    private Date dateBegin;

    /** Дата окончания действия контракта */
    private Date dateEnd;

    /** Завершили ли выполнение договора */
    private boolean isEnd;

    /** Стандатный конструктор контракта {@link Contract} */
    public Contract() {}

    /**
     * Конструктор - создание нового объекта {@link Contract}
     */
    public Contract(String d, double p, Client c, Manager m, List<Employee> w, Date b, Date e, boolean i)
    {
        this.description = d;
        this.price = p;
        this.client = c;
        this.manager = m;
        this.workers = w;
        this.dateBegin = b;
        this.dateEnd = e;
        this.isEnd = i;
    }

    /**
     * Метод получения значения поля {@link Contract#id}
     * @return возвращает уникальный идентификатор контракта
     */
    public int getId()
    {
        return id;
    }

    /**
     * Функция определения значения поля {@link Contract#id}
     * @param newID - новый идентификатор
     */
    public void setId(int newID)
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
     * Метод установки рабочих, которые выполнят контракт {@link Contract#workers}
     * @param w - список рабочих
     */
    public void setWorkers(List<Employee> w)
    {
        workers = w;
    }

    /**
     * Метод получения значения поля {@link Contract#workers}
     * @return возвращает всех рабочих, выполняющих контракт
     */
    public List<Employee> getWorkers()
    {
        return workers;
    }

    /**
     * Метод получения значения поля {@link Contract#isEnd}
     * @return возвращает состояние договора
     */
    public boolean getIsEnd()
    {
        return isEnd;
    }

    /**
     * Функция определения значения поля {@link Contract#id}
     * @param i - новое состояние
     */
    public void setIsEnd(boolean i)
    {
        isEnd = i;
    }

    /**
     * Функция получения всей информации об объекте
     * @return  - массив строк с данными
     */
    public String[] toTableFormat()
    {
        DateFormat df = new SimpleDateFormat("yyyy-MM-dd");

        return new String[] {
                String.valueOf(id),
                description,
                String.valueOf(price),
                String.valueOf(client.getId()) + " " + client.getName() + " " + client.getSurname(),
                String.valueOf(manager.getId()) + " " + manager.getName() + " " + manager.getSurname(),
                df.format(dateBegin),
                df.format(dateEnd),
                isEnd ? "Выполнено" : "В процессе"
        };
    }

}
