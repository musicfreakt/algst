package Factory.dao;
import Factory.model.*;

import Factory.util.HibernateSessionFactoryUtil;
import org.hibernate.Session;
import org.hibernate.Transaction;
import org.hibernate.query.Query;

import java.util.Date;
import java.util.List;

public class ContractDAO
{
    private Session currentSession;
    private Transaction currentTransaction;

    public ContractDAO() { }

    public Session openCurrentSession()
    {
        currentSession = HibernateSessionFactoryUtil.getSessionFactory().openSession();
        return currentSession;
    }

    public Session openCurrentSessionwithTransaction()
    {
        currentSession = HibernateSessionFactoryUtil.getSessionFactory().openSession();
        currentTransaction = currentSession.beginTransaction();
        return currentSession;
    }

    public void closeCurrentSession()
    {
        currentSession.close();
    }

    public void closeCurrentSessionwithTransaction()
    {
        currentTransaction.commit();
        currentSession.close();
    }

    public Session getCurrentSession() {
        return currentSession;
    }

    public void setCurrentSession(Session currentSession) {
        this.currentSession = currentSession;
    }

    public Transaction getCurrentTransaction() {
        return currentTransaction;
    }

    public void setCurrentTransaction(Transaction currentTransaction) {
        this.currentTransaction = currentTransaction;
    }

    public void persist(Contract entity) {
        getCurrentSession().save(entity);
    }

    public void update(Contract entity) {
        getCurrentSession().update(entity);
    }

    public Contract findById(int id)
    {
        return getCurrentSession().get(Contract.class, id);
    }

    public void delete(Contract entity) {
        getCurrentSession().delete(entity);
    }

    @SuppressWarnings("unchecked")
    public List<Contract> findAll() {
        return (List<Contract>) getCurrentSession().createQuery("SELECT c FROM contracts c").list();
    }

    @SuppressWarnings("unchecked")
    public List<Contract> findOutdated()
    {
        return (List<Contract>) getCurrentSession().createQuery("FROM contracts c WHERE c.isEnd = 0 and c.dateEnd <= current_date").list();
    }

    public int findNew(Date date)
    {
        Query<Contract> q = getCurrentSession().createQuery("SELECT c FROM contracts c WHERE month(c.dateBegin) = month(:date) and year(c.dateBegin) = year(:date)");
        q.setParameter("date", date);
        return q.list().size();
    }

    public List<Contract> findFinish(Date date)
    {
        Query<Contract> q = getCurrentSession().createQuery("SELECT c FROM contracts c WHERE month(c.dateEnd) = month(:date) and year(c.dateEnd) = year(:date)");
        q.setParameter("date", date);
        return q.list();
    }

    public List<Contract> findOutdated(int id)
    {
        Query<Contract> q = getCurrentSession().createQuery("SELECT c FROM contracts c JOIN c.workers w WHERE w.id = :id and c.isEnd = 0 and c.dateEnd <= current_date");
        q.setParameter("id", id);
        return q.list();
    }

    public List<Contract> findByClientId(int id)
    {
        Query<Contract> q = getCurrentSession().createQuery("SELECT c FROM contracts c WHERE c.client.id = :id");
        q.setParameter("id", id);
        return q.list();
    }

    @SuppressWarnings("unchecked")
    public List<Contract> findTimePeriod(Date btime, Date etime)
    {
        Query<Contract> q = getCurrentSession().createQuery("FROM contracts c WHERE c.dateBegin BETWEEN :btime AND :etime");
        q.setParameter("btime", btime);
        q.setParameter("etime", etime);
        return q.list();
    }

    @SuppressWarnings("unchecked")
    public List<Contract> findTimePeriod(Date btime, Date etime, int id)
    {
        Query<Contract> q = getCurrentSession().createQuery("SELECT c FROM contracts c JOIN c.workers w WHERE w.id = :id AND c.dateBegin BETWEEN :btime AND :etime");
        q.setParameter("id", id);
        q.setParameter("btime", btime);
        q.setParameter("etime", etime);
        return q.list();
    }

    public void deleteAll() {
        List<Contract> entityList = findAll();
        for (Contract entity : entityList)
        {
            delete(entity);
        }
    }

}

