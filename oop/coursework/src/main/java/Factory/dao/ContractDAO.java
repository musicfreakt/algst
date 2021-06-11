package Factory.dao;
import Factory.model.*;

import Factory.util.HibernateSessionFactoryUtil;
import org.hibernate.Session;
import org.hibernate.Transaction;
import org.hibernate.query.Query;

import java.util.Date;
import java.util.List;

/** Data Access Object для класса Client */
public class ContractDAO
{
    /** Текущая сессия */
    private Session currentSession;
    /** Текущая транзакция */
    private Transaction currentTransaction;

    /** Стандартный конструктор */
    public ContractDAO() { }

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

    /** Добавить контракт в БД
     *
     * @param entity клиент
     */
    public void persist(Contract entity) {
        getCurrentSession().save(entity);
    }

    /** Обновить контракт в БД
     *
     * @param entity клиент
     */
    public void update(Contract entity) {
        getCurrentSession().update(entity);
    }

    /** Найти контракт в БД по уникальному идентификатору
     *
     * @param id уникальный идентификатор
     * @return найденные контракты
     */
    public Contract findById(int id)
    {
        return getCurrentSession().get(Contract.class, id);
    }

    /** Удалить контракт из БД
     *
     * @param entity клиент
     */
    public void delete(Contract entity) {
        getCurrentSession().delete(entity);
    }

    /** Найти всех контракты в БД
     * @return найденные контракты
     */
    @SuppressWarnings("unchecked")
    public List<Contract> findAll() {
        return (List<Contract>) getCurrentSession().createQuery("SELECT c FROM contracts c").list();
    }

    /** Найти все устаревшие контракты в БД
     * @return найденные контракты
     */
    @SuppressWarnings("unchecked")
    public List<Contract> findOutdated()
    {
        return (List<Contract>) getCurrentSession().createQuery("FROM contracts c WHERE c.isEnd = 0 and c.dateEnd <= current_date").list();
    }

    /** Найти все новые контракты в БД 
     * @param date дата, по которой ищем в бд
     * @return найденные контракты
     */
    public int findNew(Date date)
    {
        Query<Contract> q = getCurrentSession().createQuery("SELECT c FROM contracts c WHERE month(c.dateBegin) = month(:date) and year(c.dateBegin) = year(:date)");
        q.setParameter("date", date);
        return q.list().size();
    }

    /** Найти все завершенные контракты в БД 
     * @param date дата, по которой ищем в бд
     * @return найденные контракты
     */
    public List<Contract> findFinish(Date date)
    {
        Query<Contract> q = getCurrentSession().createQuery("SELECT c FROM contracts c WHERE month(c.dateEnd) = month(:date) and year(c.dateEnd) = year(:date) and c.isEnd = 1");
        q.setParameter("date", date);
        return q.list();
    }

    /** Найти все устаревшие контракты в БД 
     * @param id уникальный идентификатор рабочего
     * @return найденные контракты
     */
    public List<Contract> findOutdated(int id)
    {
        Query<Contract> q = getCurrentSession().createQuery("SELECT c FROM contracts c JOIN c.workers w WHERE w.id = :id and c.isEnd = 0 and c.dateEnd <= current_date");
        q.setParameter("id", id);
        return q.list();
    }

    /** Найти все контракты указанного контракт в БД 
     * @param id уникальный идентификатор контракт
     * @return найденные контракты
     */
    public List<Contract> findByClientId(int id)
    {
        Query<Contract> q = getCurrentSession().createQuery("SELECT c FROM contracts c WHERE c.client.id = :id");
        q.setParameter("id", id);
        return q.list();
    }

    /** Найти период времени
     *
     * @param btime начало промежутка
     * @param etime конец промежутка
     * @return найденные контракты
     */
    @SuppressWarnings("unchecked")
    public List<Contract> findTimePeriod(Date btime, Date etime)
    {
        Query<Contract> q = getCurrentSession().createQuery("FROM contracts c WHERE c.dateBegin BETWEEN :btime AND :etime");
        q.setParameter("btime", btime);
        q.setParameter("etime", etime);
        return q.list();
    }

    /** Найти период времени
     *
     * @param btime начало промежутка
     * @param etime конец промежутка
     * @param id уникальный идентификатор
     * @return найденные контракты
     */
    @SuppressWarnings("unchecked")
    public List<Contract> findTimePeriod(Date btime, Date etime, int id)
    {
        Query<Contract> q = getCurrentSession().createQuery("SELECT c FROM contracts c JOIN c.workers w WHERE w.id = :id AND c.dateBegin BETWEEN :btime AND :etime");
        q.setParameter("id", id);
        q.setParameter("btime", btime);
        q.setParameter("etime", etime);
        return q.list();
    }
    
    /**Удалить всех контракты из БД */
    public void deleteAll() {
        List<Contract> entityList = findAll();
        for (Contract entity : entityList)
        {
            delete(entity);
        }
    }

}

