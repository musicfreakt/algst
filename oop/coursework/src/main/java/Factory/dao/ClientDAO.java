package Factory.dao;
import Factory.model.*;

import Factory.util.HibernateSessionFactoryUtil;
import org.hibernate.Session;
import org.hibernate.Transaction;
import org.hibernate.query.Query;

import java.util.List;

/** Data Access Object для класса Client */
public class ClientDAO
{
    /** Текущая сессия */
    private Session currentSession;
    /** Текущая транзакция */
    private Transaction currentTransaction;

    /** Стандартный конструктор */
    public ClientDAO() { }

    /** Открыть текущую сессию */
    public Session openCurrentSession()
    {
        currentSession = HibernateSessionFactoryUtil.getSessionFactory().openSession();
        return currentSession;
    }

    /** Открыть текущую сессию с транзакцией */
    public Session openCurrentSessionwithTransaction()
    {
        currentSession = HibernateSessionFactoryUtil.getSessionFactory().openSession();
        currentTransaction = currentSession.beginTransaction();
        return currentSession;
    }

    /** Закрыть текущую сессию */
    public void closeCurrentSession()
    {
        currentSession.close();
    }

    /** Закрыть текущую сессию с транзакцией */
    public void closeCurrentSessionwithTransaction()
    {
        currentTransaction.commit();
        currentSession.close();
    }

    /** Получить текущую сессию */
    public Session getCurrentSession() {
        return currentSession;
    }

    /** Установить текущую сессию */
    public void setCurrentSession(Session currentSession) {
        this.currentSession = currentSession;
    }

    /** Получить текущую транзакцию */
    public Transaction getCurrentTransaction() {
        return currentTransaction;
    }

    /** Установить текущую сессию */
    public void setCurrentTransaction(Transaction currentTransaction) {
        this.currentTransaction = currentTransaction;
    }

    /** Добавить клиента в БД
     *
     * @param entity клиент
     */
    public void persist(Client entity) {
        getCurrentSession().save(entity);
    }

    /** Обновить клиента в БД
     *
     * @param entity клиент
     */
    public void update(Client entity) {
        getCurrentSession().update(entity);
    }

    /** Найти клиента в БД по уникальному идентификатору
     *
     * @param id уникальный идентификатор
     */
    public Client findById(int id)
    {
        return getCurrentSession().get(Client.class, id);
    }

    /** Удалить клиента из БД
     *
     * @param entity клиент
     */
    public void delete(Client entity) {
        getCurrentSession().delete(entity);
    }

    /** Найти всех клиентов в БД */
    @SuppressWarnings("unchecked")
    public List<Client> findAll() {
        return (List<Client>) getCurrentSession().createQuery("SELECT c FROM clients c").list();
    }

    /** Найти всех клиентов указанного менеджера в БД
     * @param id - уникальный идентификатор менеджера
     */
    @SuppressWarnings("unchecked")
    public List<Client> findByManagerId(int id)
    {
        Query<Client> q = getCurrentSession().createQuery("SELECT c FROM clients c JOIN c.client_contracts s WHERE s.manager.id = :id");
        q.setParameter("id", id);
        return q.list();
    }

    /**Удалить всех клиентов из БД */
    public void deleteAll() {
        List<Client> entityList = findAll();
        for (Client entity : entityList)
        {
            delete(entity);
        }
    }

}

